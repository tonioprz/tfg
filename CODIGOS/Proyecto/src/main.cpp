#include <Arduino.h>
#include <Ethernet.h>
#include <LiquidCrystal_I2C.h>
#include <motor.h>
#include <main.h>


//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,16,2);

Motor mot(IN3, IN4, ENB, BUTTON_EMERGENCIA);

void cambiofaseA(void);
void cambiofaseB(void);
float medidaCalibre(void);
void ethconex(void);

void setup() {
  // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();
    
  pinMode(faseA, INPUT_PULLUP);
  pinMode(faseB, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(faseA), cambiofaseA, RISING);
  attachInterrupt(digitalPinToInterrupt(faseB), cambiofaseB, RISING);

  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_ENTER, INPUT_PULLUP);
  pinMode(BUTTON_ESC, INPUT_PULLUP);

  pinMode(BUTTON_EMERGENCIA, INPUT);
  pinMode(BUTTON_LOCAL, INPUT);
  pinMode(BUTTON_MICRO, INPUT);

  if(!digitalRead(BUTTON_MICRO)){
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENB, OUTPUT);
    if(!digitalRead(BUTTON_EMERGENCIA)){
      state = 'i';
    } else{
      state = 'e';
    }
    if(digitalRead(BUTTON_LOCAL)){
      state = 'r';
    }
  } else{
    pinMode(IN3, INPUT);
    pinMode(IN4, INPUT);
    pinMode(ENB, INPUT);
    state = 'f';
  }

  // Se inicia la comunicación Ethernet y la serie para depuración
  Serial.begin(9600);
  Ethernet.begin(MAC, IP);

  Serial.print("Servidor en IP ");
  Serial.println(Ethernet.localIP());
}

void loop() {

  // Se lee el estado de los botones
  up = digitalRead(BUTTON_UP);
  down = digitalRead(BUTTON_DOWN);
  enter = digitalRead(BUTTON_ENTER);
  esc = digitalRead(BUTTON_ESC);

  // Se lee el estado de la configuración
  emergencia = digitalRead(BUTTON_EMERGENCIA);
  local = digitalRead(BUTTON_LOCAL);
  micro = digitalRead(BUTTON_MICRO);

  if(micro && (state != 'f') && (state != 'e')){
    setup();
  }

  if(local != local_ant){
    setup();
  }

  if(emergencia && (state != 'e')){
    state = 'e';
    lcd.clear();
  }

  // Máquina de estados
  switch(state){
    case 'i':
      lcd.setCursor(0, 0);
      lcd.print("MODO LOCAL      ");
      lcd.setCursor(0, 1);
      lcd.print("PULSE ENTER     ");
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, 255);
      break;
    
    case 's':
      lcd.setCursor(0, 0);
      lcd.print("SELECCIONE MOV. ");
      if(discreto){
        lcd.setCursor(0, 1);
        lcd.print("AVANCE DISCRETO ");
      } else{
        lcd.setCursor(0, 1);
        lcd.print("AVANCE ABSOLUTO ");
      }
      break;

    case 'd':
      break;

    case 'm':
      mot.movimientoMotor(objetivo, pposicion, lcd);
      posy = medidaCalibre();
      lcd.setCursor(0, 0);
      lcd.print("POSICION FINAL  ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("X=");
      lcd.print(posicion);
      lcd.print(" Y=");
      lcd.print(posy);
      delay(2000);
      state = 'i';
      break;

    case 'f':
      lcd.setCursor(0, 0);
      lcd.print("SIN MICRO");
      break;
    
    case 'e':
      lcd.setCursor(0, 0);
      lcd.print("EMERGENCIA");
      break;
  }
  // Transiciones entre estados
  switch(state){

    case 'i':
      if(!enter && enter_ant){
        state = 's';
      }
      break;

    case 's':
      if(!enter && enter_ant){
        state = 'd';
        
        posy = medidaCalibre();
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("X=");
        lcd.print(posicion);
        lcd.print(" Y=");
        lcd.print(posy);

        lcd.setCursor(0, 1);
        if(desplazamiento > 0){
          lcd.print("+");
        }
        lcd.print(desplazamiento);
        lcd.print(" pulsos");                
      }
      if(!esc && esc_ant){
        state = 'i';
      }
      if((!up && up_ant) || (!down && down_ant)){
        discreto = !discreto;
      }
      break;

    case 'd':
      if(!enter && enter_ant){
        state = 'm';
        if(discreto){
          objetivo = posicion + desplazamiento;
        } else{
          objetivo = desplazamiento;
        }
      }
      if(!esc && esc_ant){
        state = 's';
      }
      if(!up && up_ant){
        desplazamiento = desplazamiento + 1000;
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        if(desplazamiento > 0){
          lcd.print("+");
        }
        lcd.print(desplazamiento);
        lcd.print(" pulsos");        
      }
      if(!down && down_ant){
        desplazamiento = desplazamiento - 1000;
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        if(desplazamiento > 0){
          lcd.print("+");
        }
        lcd.print(desplazamiento);
        lcd.print(" pulsos");        
      }
      break;
    
    case 'r':
      lcd.setCursor(0, 0);
      lcd.print("MODO REMOTO     ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, 255);
      break;

    case 'f':
      if(!micro){
        setup();
      }
      break;

    case 'e':
      if(!emergencia){
        setup();
      }
      break;
  }

  esc_ant = esc;
  enter_ant = enter;
  up_ant = up;
  down_ant = down;
  
  emergencia_ant = emergencia;
  local_ant = local;
  micro_ant = micro;
    
  delay(10);
}



void cambiofaseA(void){
  bool fB = digitalRead(faseB);
  if(fB){
    posicion++;
  } else{
    posicion--;
  }
}

void cambiofaseB(void){
  bool fA = digitalRead(faseA);
  if(fA){
    posicion--;
  } else{
    posicion++;
  }
}

float medidaCalibre(void){
  bool data;
  float medida;
  int value = 0;
  int signo = 0;

  unsigned long tempmicros;
  unsigned long tempmicros2;

  for(int j=0; j<10 && (value == 0); j++){
    tempmicros = micros();

    while (digitalRead(CAL_CLK)==HIGH) {
      delayMicroseconds(1);
      
    }

    tempmicros2 = micros();

    if ((tempmicros2-tempmicros)>10000) {
      for (int i=0; i<24; i++) {
        while (digitalRead(CAL_CLK)==LOW) {
          delayMicroseconds(1);
        }

        data = digitalRead(CAL_DATA);

        if(i<16){
          value |= data << i;
        }else{
          signo |= (data << (i-16));
        }

        while (digitalRead(CAL_CLK)==HIGH) {
          delayMicroseconds(1);
        }
      }
        
      
      if(signo & 0x80){
        medida = 25.4*float(value)/(2*1000);
      } else{
        medida = float(value)/100;
      } 

      if(signo & 0x10){
        medida = -medida;
      }
    }
  }
  return medida;
}


void ethconex(void){
  EthernetClient cliente = servidor.available();
  if (cliente) {
    Serial.println("\nNuevo Cliente");
    boolean currentLineIsBlank = true; //Una petición HTTP acaba con una línea en blanco
    String cadena=""; //Creamos una cadena de caracteres vacía
    while (cliente.connected()) {
      if (cliente.available()) {
        char c = cliente.read();//Leemos la petición HTTP carácter por carácter
        Serial.write(c);//Visualizamos la petición HTTP por el Monitor Serial
        cliente.println("HOLITA ROBOTITO");
        if(cadena.length()<50)
        {
                  cadena.concat(c);//concatenmos el String 'cadena' con la petición HTTP (c). De esta manera convertimos la petición HTTP a un String          
                  //Ya que hemos convertido la petición HTTP a una cadena de caracteres, ahora podremos buscar partes del texto.                          
        }
        //Cuando reciba una línea en blanco, quiere decir que la petición HTTP ha acabado y el servidor Web está listo para enviar una respuesta
        if (c == 'n' && currentLineIsBlank) {
 
            // Enviamos al cliente una respuesta HTTP
            cliente.println("HTTP/1.1 200 OK");
            cliente.println("Content-Type: text/html");
            cliente.println();

            cliente.println("<html>"); 
            cliente.println("HOLI WORLD");
            cliente.println("</html>");
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
    cliente.stop();// Cierra la conexión
  }

}