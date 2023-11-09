#!/usr/bin/env python3
import socket,sys
if len(sys.argv) != 3:
    print("Uso: client.py <IP> <puerto>")
    sys.exit(1)
host = sys.argv[1]
port = int(sys.argv[2])
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((host, port))
print(f"Conectado al servidor {host} en el puerto {port}.")
while True:
    data = input("Ingrese una cadena en minusculas (o una cadena vacia para salir): ")
    client_socket.send(data.encode())
    if data.strip() == "":
        print("Cliente ingreso una cadena vacía. Terminando la conexion.")
        break
    response = client_socket.recv(1024).decode()
    print(f"Respuesta del servidor: {response}")
client_socket.close()
