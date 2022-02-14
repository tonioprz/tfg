#include <Arduino.h>

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };//Ponemos la dirección MAC de la Ethernet Shield
IPAddress ip(192,168,50,200); //Asignamos  la IP al Arduino
EthernetServer server(4012); //Creamos un servidor Web con el puerto 80 que es el puerto HTTP por defecto
 
int LED1=2; //Pin del LED 1
int LED2=3; //Pin del LED 2
String estado1="OFF"; //Estado del Led 1 inicialmente "OFF"
String estado2="OFF"; //Estado del Led 2 inicialmente "OFF" 

void setup()
{
  Serial.begin(9600);
 
  // Inicializamos la comunicación Ethernet y el servidor
  Ethernet.begin(mac, ip);
  server.begin();
  
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
}
 
void loop()
{
  EthernetClient client = server.available(); //Creamos un cliente Web
  //Verificamos si se detecte un cliente a través de una petición HTTP
  if (client) {
    Serial.println("\nnew client");
    boolean currentLineIsBlank = true; //Una petición HTTP acaba con una línea en blanco
    String cadena=""; //Creamos una cadena de caracteres vacía
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();//Leemos la petición HTTP carácter por carácter
        Serial.write(c);//Visualizamos la petición HTTP por el Monitor Serial
        if(cadena.length()<50)
        {
                  cadena.concat(c);//concatenmos el String 'cadena' con la petición HTTP (c). De esta manera convertimos la petición HTTP a un String
          
                  //Ya que hemos convertido la petición HTTP a una cadena de caracteres, ahora podremos buscar partes del texto.
                  int posicion=cadena.indexOf("Data="); //Guardamos la posición de la Palabra "Data=" a la variable 'posicion'
          
                  if(cadena.substring(posicion)=="Data=1")//Si en la posición hay "Data=1"
                  {
                    digitalWrite(LED1,LOW);
                    estado1="ON";
                    client.println("RECIBIDO");
                  }
                  else if(cadena.substring(posicion)=="Data=2")//Si en posición hay "Data=2"
                  {
                    digitalWrite(LED1,HIGH);
                    estado1="OFF";
                  }
                  else if(cadena.substring(posicion)=="Data=3")//Si en la posición hay "Data=3"
                  {
                    digitalWrite(LED2,LOW);
                    estado2="ON";
                  }
                  else if(cadena.substring(posicion)=="Data=4")//Si en la posición hay "Data=4"
                  {
                    digitalWrite(LED2,HIGH);
                    estado2="OFF";
                  } 
                          
        }
        //Cuando reciba una línea en blanco, quiere decir que la petición HTTP ha acabado y el servidor Web está listo para enviar una respuesta
        if (c == 'n' && currentLineIsBlank) {
 
            // Enviamos al cliente una respuesta HTTP
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
 
            //Página web en formato HTML
            client.println("<html>");                 
            client.println("<head><title>Naylamp Mechatronics</title>");
            client.println("</head>");
            client.println("<body>");
            client.println("<div style='text-align:center;'>");
            client.println("<h1>NAYLAMP MECHATRONICS</h1>");
            client.println("<h2>Entradas Analogicas</h2>");
            client.print("AN0="); client.println(analogRead(0));
            client.print("<br />AN1=");client.println(analogRead(1)); 
            client.println("<h2>Entradas Digitales</h2>");
            client.print("D4=");client.println(digitalRead(4));
            client.println("<br />D5=");client.print(digitalRead(5));
            client.println("<br /><br />");
            client.println("<a href='http://192.168.50.200'>Actualizar entradas</a>");
            client.println("<h2>Salidas Digitales </h2>");        
            client.println("Estado del LED 1 = ");client.print(estado1);            
            client.println("<br />");            
            client.print("<button onClick=location.href='./?Data=1'>ON</button>");           
            client.print("<button onClick=location.href='./?Data=2'>OFF</button>");
            client.println("<br /><br />");
            client.println("Estado del LED 2 = ");client.print(estado2);            
            client.println("<br />");            
            client.print("<button onClick=location.href='./?Data=3'>ON</button>");           
            client.print("<button onClick=location.href='./?Data=4'>OFF</button>");
            client.println("<br /><br />");
            client.println("<a href='https://naylampmechatronics.com/'>naylampmechatronics.com</a>");
            client.println("<br /><br />");             
            client.println("</b></body>");
            client.println("</html>");
            break;
        }
        if (c == 'n') {
          currentLineIsBlank = true;
        }
        else if (c != 'r') {
          currentLineIsBlank = false;
        }
      }
    }
    //Dar tiempo al navegador para recibir los datos
    delay(1);
    client.stop();// Cierra la conexión
  }
}
