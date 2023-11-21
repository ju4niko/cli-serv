#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <pthread.h>

// Estructura para almacenar datos que se pasarán al hilo
struct HiloDatos {
    int cliente_socket;
    int numero_hilo;
};

void *manejar_cliente(void *arg) {
    struct HiloDatos *datos = (struct HiloDatos *)arg;
    int cliente_socket = datos->cliente_socket;
    int numero_hilo = datos->numero_hilo;
    free(datos);

    printf("Hilo %d: Cliente conectado.\n", numero_hilo);

    char buffer[1024];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_recv = recv(cliente_socket, buffer, sizeof(buffer), 0);

        if (bytes_recv <= 0) {
            printf("Hilo %d: Cliente ha cerrado la conexión.\n", numero_hilo);
            break;
        }

        printf("Hilo %d: Mensaje del cliente: %s", numero_hilo, buffer);

        for (int i = 0; buffer[i] != '\0'; i++) {
            if (islower(buffer[i])) {
                buffer[i] = toupper(buffer[i]); // Convertir a mayúscula
            }
        }
        send(cliente_socket, buffer, strlen(buffer), 0);

        if (strlen(buffer) == 1 && buffer[0] == '\n') {
            printf("Hilo %d: Cliente ingresó una cadena vacía. Terminando la conexión.\n", numero_hilo);
            break;
        }
    }

    close(cliente_socket);
    printf("Hilo %d: Conexión cerrada.\n", numero_hilo);
    pthread_exit(NULL);
}

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

    int numero_hilo = 0;
    while (1) {
        // Aceptar una conexión entrante
        struct sockaddr_in cliente_addr;
        socklen_t cliente_len = sizeof(cliente_addr);
        int *cliente_socket = malloc(sizeof(int));
        *cliente_socket = accept(servidor_socket, (struct sockaddr *)&cliente_addr, &cliente_len);

        if (*cliente_socket == -1) {
            perror("Error al aceptar la conexión");
            free(cliente_socket);
            continue;
        }

        // Crear un hilo para manejar la comunicación con el cliente
        pthread_t hilo;
        
        // Estructura para pasar datos al hilo
        struct HiloDatos *datos = malloc(sizeof(struct HiloDatos));
        datos->cliente_socket = *cliente_socket;
        datos->numero_hilo = numero_hilo;
        numero_hilo++;

        if (pthread_create(&hilo, NULL, manejar_cliente, datos) != 0) {
            perror("Error al crear el hilo");
            free(cliente_socket);
            close(*cliente_socket);
            free(datos);
            continue;
        }

        // Hilo creado, detápalo para que no retenga recursos después de terminar
        pthread_detach(hilo);
    }

    close(servidor_socket);

    return 0;
}
