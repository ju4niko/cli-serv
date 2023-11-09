#!/usr/bin/env python3
import socket,sys
if len(sys.argv) != 2:
    print("Uso: server.py <puerto>")
    sys.exit(1)
port = int(sys.argv[1])
host = "0.0.0.0"  # Escucha en todas las interfaces de red
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((host, port))
server_socket.listen(5)
print(f"Esperando conexiones en el puerto {port}...")
client_socket, client_address = server_socket.accept()
print(f"Cliente conectado desde {client_address}")
while True:
    data = client_socket.recv(1024).decode()
    if not data:
        break
    data = data.upper()
    client_socket.send(data.encode())
    if data.strip() == "":
        print("Cliente ingreso una cadena vacia. Terminando la conexion.")
        break
client_socket.close()
server_socket.close()
