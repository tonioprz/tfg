#Se importa el módulo
import socket
 
#instanciamos un objeto para trabajar con el socket
ser = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

host = "192.168.50.200"
port = 4012

#Puerto y servidor que debe escuchar
ser.connect((host, port))
 
#Aceptamos conexiones entrantes con el metodo listen. Por parámetro las conexiones simutáneas.
#mensaje = "GET / HTTP/1.1\r\nData=1\r\n\r\n"
#ser.send(mensaje.encode())

#response = ser.recvfrom(16)
#http_response = repr(response)
#http_response_len = len(http_response)
MSGLEN = 4
chunks = []
bytes_recd = 0
while bytes_recd < MSGLEN:
    chunk = ser.recv(min(MSGLEN - bytes_recd, 2048))
    if chunk == b'':
        raise RuntimeError("socket connection broken")
    chunks.append(chunk)
    bytes_recd = bytes_recd + len(chunk)

print(b''.join(chunks))


#Cerramos la instancia del socket cliente y servidor

ser.close()

print("Conexiones cerradas")