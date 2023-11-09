#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

int is_numeric(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <IP> <puerto>\n", argv[0]);
        exit(1);
    }

    char *ip = argv[1];
    int puerto = atoi(argv[2]);

    // Crear un socket
    int cliente_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (cliente_socket == -1) {
        perror("Error al crear el socket");
        exit(1);
    }

    // Conectar al servidor
    struct sockaddr_in servidor_addr;
    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_port = htons(puerto);
    inet_pton(AF_INET, ip, &servidor_addr.sin_addr);

    if (connect(cliente_socket, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) == -1) {
        perror("Error al conectar al servidor");
        exit(1);
    }

    printf("Conectado al servidor %s en el puerto %d.\n", ip, puerto);

    // Enviar y recibir datos
    char buffer[1024];
    while (1) {
        printf("Ingrese una cadena en minúsculas (o ENTER para salir): ");
        fgets(buffer, sizeof(buffer), stdin);
        send(cliente_socket, buffer, strlen(buffer), 0);

        int bytes_recv = recv(cliente_socket, buffer, sizeof(buffer), 0);
        if (bytes_recv <= 0) {
            perror("Error en la recepción o el servidor ha cerrado la conexión.");
            break;
        }

        printf("Respuesta del servidor: %s", buffer);

    }

    close(cliente_socket);

    return 0;
}

