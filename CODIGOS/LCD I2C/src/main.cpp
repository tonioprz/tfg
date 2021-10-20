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
bool up;
bool down;
bool enter_ant = 0;
bool up_ant = 0;
bool down_ant = 0;

int cont = 0;
int cont_ant = 0;
char state = 'i';

void cambiofaseA(void);
void cambiofaseB(void);
int boton(bool up, bool down, bool up_ant, bool down_ant, int cont);

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
  lcd.print("PRUEBAS");
}

void loop() {
   // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)

   // Escribimos el número de segundos trascurridos
  up = digitalRead(BUTTON_UP);
  down = digitalRead(BUTTON_DOWN);
  enter = digitalRead(BUTTON_ENTER);

  switch(state){
    case 'i':
      lcd.setCursor(0, 1);
      lcd.print("INICIO");
      break;
    
    case 'u':
      lcd.setCursor(0, 0);
      lcd.print("ARRIBA  ");
      break;

    case 'd':
      lcd.setCursor(0, 0);
      lcd.print("ABAJO  ");
      break;
  }
  lcd.setCursor(10, 1);
  lcd.print("      ");
  lcd.setCursor(10, 1);
  lcd.print(cont);
  
  if(!enter){
    state = 'e';
    lcd.setCursor(0, 1);
    lcd.print("Enter       ");
  }
  cont_ant = cont;
  up_ant = up;
  down_ant = down;
  
  delay(100);
}

int boton(bool up, bool down, bool up_ant, bool down_ant, int cont){
  if(!down && down_ant){
    cont--;
  }
  if(!up && up_ant){
    cont++;
  }
  return cont;
}

void cambiofaseA(void){
  bool fA = digitalRead(faseA);
  bool fB = digitalRead(faseB);

  if(fA){
    if(fB){
      cont++;
    } else{
      cont--;
    }
  } else{
    if(fB){
      cont--;
    } else{
      cont++;
    }
  }
}

void cambiofaseB(void){
  bool fA = digitalRead(faseA);
  bool fB = digitalRead(faseB);

  if(fB){
    if(fA){
      cont--;
    } else{
      cont++;
    }
  } else{
    if(fA){
      cont++;
    } else{
      cont--;
    }
  }  
}