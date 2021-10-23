#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,16,2);  //

#define faseA 2
#define faseB 3

#define ENB 5
#define IN3 27
#define IN4 26
#define BUTTON_ESC 22
#define BUTTON_ENTER 23
#define BUTTON_UP 24
#define BUTTON_DOWN 25

bool enter;
bool esc;
bool up;
bool down;
bool esc_ant = 0;
bool enter_ant = 0;
bool up_ant = 0;
bool down_ant = 0;

long posicion = 0;
char state = 'i';
bool discreto = 0;
long desplazamiento = 2000;
long objetivo = 0;

double kp = 1;
double Ti = 1;

void cambiofaseA(void);
void cambiofaseB(void);

void setup() {
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_ENTER, INPUT_PULLUP);
  pinMode(BUTTON_ESC, INPUT_PULLUP);

  pinMode(faseA, INPUT_PULLUP);
  pinMode(faseB, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(faseA), cambiofaseA, RISING);
  attachInterrupt(digitalPinToInterrupt(faseB), cambiofaseB, RISING);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();
  
  // Escribimos el Mensaje en el LCD.
  lcd.print("BIENVENIDO");
}

void loop() {
   // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)

   // Escribimos el número de segundos trascurridos
  up = digitalRead(BUTTON_UP);
  down = digitalRead(BUTTON_DOWN);
  enter = digitalRead(BUTTON_ENTER);
  esc = digitalRead(BUTTON_ESC);

  // Máquina de estados
  switch(state){
    case 'i':
      lcd.setCursor(0, 0);
      lcd.print("BIENVENIDO      ");
      lcd.setCursor(0, 1);
      lcd.print("PULSE ENTER     ");
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
      lcd.setCursor(0, 0);
      if(discreto){
        lcd.print("SEL. A. DISCRETO");
      } else{
        lcd.print("SEL. A. ABSOLUTO");
      }
      break;

    case 'm':
      lcd.setCursor(0, 0);
      lcd.print("MOVIENDO...     ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(posicion);
      lcd.print("/");
      lcd.print(objetivo);
      int u = 0;
      while((posicion < objetivo - 100) || (posicion > objetivo + 100)){
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print(posicion);
        lcd.print("/");
        lcd.print(objetivo);        
        u = (objetivo - posicion) * kp;
        if(u >= 0){
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
        } else{
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          u = -u;
        }
        if(u > 255){
          u = 255;
        }
        analogWrite(ENB, u);
        delay(1);
      }

      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, 255);
      lcd.setCursor(0, 0);
      lcd.print("POSICION FINAL  ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(posicion);
      
      
      delay(2000);
      state = 'i';
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
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
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
        lcd.print(desplazamiento);
        lcd.print(" pulsos");        
      }
      if(!down && down_ant){
        desplazamiento = desplazamiento - 1000;
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print(desplazamiento);
        lcd.print(" pulsos");        
      }
      break;
  }



  esc_ant = esc;
  enter_ant = enter;
  up_ant = up;
  down_ant = down;
  
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