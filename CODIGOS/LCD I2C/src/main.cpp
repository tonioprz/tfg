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

long cont = 0;
char state = 'i';
bool discreto = 0;

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
      }
      if(!esc && esc_ant){
        state = 'i';
      }
      if((!up && up_ant) || (!down && down_ant)){
        discreto = !discreto;
      }
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
    cont++;
  } else{
    cont--;
  
  }
}

void cambiofaseB(void){
  bool fA = digitalRead(faseA);
  if(fA){
    cont--;
  } else{
    cont++;
  }
}