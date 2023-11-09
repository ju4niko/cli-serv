#!/bin/bash
while true
do 
	{ 
		echo -e 'HTTP/1.1 200 OK\r\n';  
		echo -e '<html><body>'
		echo -e '<meta http-equiv="refresh" content="5">'
		echo -e '<h2>HOLA</h2>'
		echo -e '<h3>Server HTTP argento</h3>'
		echo -e '<h3>La hora actual es:</h3>'
		
		date
		
		echo -e '</body></html>'
		
 
	} | nc -l 8080

done 

