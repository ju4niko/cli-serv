#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <puerto>\n", argv[0]);
        exit(1);
    }

    int puerto = atoi(argv[1]);

    // Crear un socket
    int servidor_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor_socket == -1) {
        perror("Error al crear el socket");
        exit(1);
    }

    // Enlace a una dirección y puerto
    struct sockaddr_in servidor_addr;
    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_port = htons(puerto);
    servidor_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(servidor_socket, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) == -1) {
        perror("Error al enlazar el socket");
        exit(1);
    }

    // Escuchar por conexiones entrantes
    if (listen(servidor_socket, 5) == -1) {
        perror("Error al escuchar");
        exit(1);
    }

    printf("Esperando conexiones en el puerto %d...\n", puerto);

    // Aceptar una conexión entrante
    struct sockaddr_in cliente_addr;
    socklen_t cliente_len = sizeof(cliente_addr);
    int cliente_socket = accept(servidor_socket, (struct sockaddr *)&cliente_addr, &cliente_len);

    if (cliente_socket == -1) {
        perror("Error al aceptar la conexión");
        exit(1);
    }

    printf("Cliente conectado.\n");

    // Procesar la comunicación
    char buffer[1024];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_recv = recv(cliente_socket, buffer, sizeof(buffer), 0);

        if (bytes_recv <= 0) {
            perror("Error en la recepción o el cliente ha cerrado la conexión.");
            break;
        }

        for (int i = 0; buffer[i] != '\0'; i++) {
            if (islower(buffer[i])) {
                buffer[i] = toupper(buffer[i]); // Convertir a mayúscula
            }
        }
        send(cliente_socket, buffer, strlen(buffer), 0);

        if (strlen(buffer) == 1 && buffer[0] == '\n') {
            printf("Cliente ingresó una cadena vacía. Terminando la conexión.\n");
            break;
        }
    }

    close(cliente_socket);
    close(servidor_socket);

    return 0;
}

