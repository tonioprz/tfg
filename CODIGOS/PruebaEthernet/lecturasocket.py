#Se importa el módulo
import socket
 
#instanciamos un objeto para trabajar con el socket
ser = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

host = "192.168.50.200"
port = 4012

#Puerto y servidor que debe escuchar
ser.connect((host, port))
 
#Aceptamos conexiones entrantes con el metodo listen. Por parámetro las conexiones simutáneas.
mensaje = "GET / HTTP/1.1\r\nData=1\r\n\r\n"
ser.send(mensaje.encode())

response = ser.recv(1024)  
http_response = str(response)
http_response_len = len(http_response)

#Cerramos la instancia del socket cliente y servidor

print(http_response)

ser.close()

print("Conexiones cerradas")