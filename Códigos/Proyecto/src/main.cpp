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
bool comprobarRobot(void);
bool enviarRobot(char dato);
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

  pinMode(BUTTON_DOWN, INPUT);
  digitalWrite(BUTTON_DOWN, HIGH);
  pinMode(BUTTON_UP, INPUT);
  digitalWrite(BUTTON_UP, HIGH);
  pinMode(BUTTON_ENTER, INPUT);
  digitalWrite(BUTTON_ENTER, HIGH);
  pinMode(BUTTON_ESC, INPUT);
  digitalWrite(BUTTON_ESC, HIGH);

  pinMode(BUTTON_EMERGENCIA, INPUT);
  pinMode(BUTTON_LOCAL, INPUT);
  pinMode(BUTTON_MICRO, INPUT);

  pinMode(SENSOR_FOTO, INPUT);

  if(!digitalRead(BUTTON_MICRO)){
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENB, OUTPUT);
    if(!digitalRead(BUTTON_EMERGENCIA)){
      estado = 'i';
    } else{
      estado = 'e';
    }
    if(digitalRead(BUTTON_LOCAL)){
      estado = 'r';
    }
  } else{
    pinMode(IN3, INPUT);
    pinMode(IN4, INPUT);
    pinMode(ENB, INPUT);
    estado = 'f';
  }

  posy = medidaCalibre();

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

  Serial.println(enter);
  Serial.println(esc);
  // Se lee el estado de la configuración
  emergencia = digitalRead(BUTTON_EMERGENCIA);
  local = digitalRead(BUTTON_LOCAL);
  micro = digitalRead(BUTTON_MICRO);

  if(micro && (estado != 'f') && (estado != 'e')){
    setup();
  }

  if(local != local_ant){
    setup();
  }

  if(emergencia && (estado != 'e')){
    estado = 'e';
    lcd.clear();
  }

  // Máquina de estados
  switch(estado){
    case 'i':
      lcd.setCursor(0, 0);
      lcd.print("MODO LOCAL      ");
      lcd.setCursor(0, 1);
      lcd.print("PULSE ENTER     ");
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, 255);
      if(comprobarRobot()){
        enviarRobot(estado);
      }
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
      if(comprobarRobot()){
        enviarRobot(estado);
      }
      break;

    case 'd':
      if(comprobarRobot()){
        enviarRobot(estado);
      }
      break;

    case 'm':
      if(comprobarRobot()){
        enviarRobot(estado);
      }
      mot.movimientoMotor(objetivo, pposicion, lcd);
      if(comprobarRobot()){
        enviarRobot(estado);
      }
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
      estado = 'i';
      if(digitalRead(BUTTON_LOCAL)){
        estado ='r';
      }
      break;

    case 'f':
      lcd.setCursor(0, 0);
      lcd.print("SIN MICRO");
      break;
    
    case 'e':
      lcd.setCursor(0, 0);
      lcd.print("EMERGENCIA");
      break;

    case 'r':
      lcd.setCursor(0, 0);
      lcd.print("MODO REMOTO     ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, 255);
      if(comprobarRobot()){
        enviarRobot(estado);
      }
      break;
  }


  // Transiciones entre estados
  switch(estado){

    case 'i':
      if(!enter && enter_ant){
        estado = 's';
      }
      break;

    case 's':
      if(!enter && enter_ant){
        estado = 'd';
        
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
        estado = 'i';
      }
      if((!up && up_ant) || (!down && down_ant)){
        discreto = !discreto;
      }
      break;

    case 'd':
      if(!enter && enter_ant){
        estado = 'm';
        if(discreto){
          objetivo = posicion + desplazamiento;
        } else{
          objetivo = desplazamiento;
        }
      }
      if(!esc && esc_ant){
        estado = 's';
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

    //while (digitalRead(CAL_CLK)==HIGH) {
    while (digitalRead(CAL_CLK)==LOW) {
      delayMicroseconds(1);
      
    }

    tempmicros2 = micros();

    if ((tempmicros2-tempmicros)>10000) {
      for (int i=0; i<24; i++) {
        //while (digitalRead(CAL_CLK)==LOW) {
        while (digitalRead(CAL_CLK)==HIGH) {
          delayMicroseconds(1);
        }

        data = !digitalRead(CAL_DATA);

        if(i<16){
          value |= data << i;
        }else{
          signo |= (data << (i-16));
        }

        //while (digitalRead(CAL_CLK)==HIGH) {
        while (digitalRead(CAL_CLK)==LOW) {
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

bool comprobarRobot(void){
  EthernetClient cliente = servidor.available();
  if (cliente) {
    //Serial.print("CONECTADO");
    //cliente.stop();
    return 1;
  } else{
    cliente.stop();
    return 0;
  }
}

bool enviarRobot(char dato){
  EthernetClient cliente = servidor.available();
  String recepcion;
  if (cliente) {
    while (cliente.connected()) {
      if (cliente.available()) {
        recepcion = cliente.readString();
        Serial.println(recepcion);
        if(recepcion == "STATUS"){
          String envio = (String) dato + ";X=" + String(posicion, 2) + ";Y=" + String(posy, 2);
          String fotoele = ";F=" + String(digitalRead(SENSOR_FOTO));
          envio = envio + fotoele;
          cliente.println(envio);
          Serial.println(recepcion);
        }
        if(recepcion.indexOf('M') > -1){
          int igual_pos = recepcion.indexOf("=");
          int fin_pos = recepcion.indexOf(";",igual_pos);
          String movimiento = recepcion.substring(igual_pos+1, fin_pos);
          objetivo = movimiento.toInt();
          estado = 'm';
        }
        if(recepcion.indexOf('R') > -1){
          int igual_pos = recepcion.indexOf("=");
          int fin_pos = recepcion.indexOf(";",igual_pos);
          String movimiento = recepcion.substring(igual_pos+1, fin_pos);
          objetivo = movimiento.toInt() + posicion;
          estado = 'm';
        }
      }
    }
    cliente.stop();
  }
  return 0;
}