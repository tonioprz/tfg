#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,16,2);  //

#define BUTTON_UP 30
#define BUTTON_DOWN 31



bool up;
bool down;
bool up_ant = 0;
bool down_ant = 0;

int cont = 0;
int cont_ant = 0;
char state = 'i';

int boton(bool up, bool down, bool up_ant, bool down_ant, int cont);

void setup() {
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_UP, INPUT_PULLUP);
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
  cont = boton(up,down,up_ant,down_ant,cont);

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

  if(cont > cont_ant){
    state = 'u';
    lcd.setCursor(0, 1);
    lcd.print("Modo up          ");
    lcd.setCursor(14, 1);
    lcd.print(cont);
  }

  if(cont < cont_ant){
    state = 'd';
    lcd.setCursor(0, 1);
    lcd.print("Modo down          ");
    lcd.setCursor(14, 1);
    lcd.print(cont);
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