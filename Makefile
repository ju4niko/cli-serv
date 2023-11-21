all:
	gcc -o server_multi server_multi.c -lpthread
	gcc -o cliente cliente.c 
	gcc -o servidor servidor.c
	gcc -o libcurl-ej libcurl-ej.c -lcurl
clean:
	rm -f server_multi cliente servidor libcurl-ej

