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

void setup() {
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();
  
  // Escribimos el Mensaje en el LCD.
  lcd.print("PRUEBA BOTON");
}

void loop() {
   // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  lcd.setCursor(0, 1);
   // Escribimos el número de segundos trascurridos
  down = digitalRead(BUTTON_DOWN);
  up = digitalRead(BUTTON_UP);

  if(!down && down_ant){
    cont--;
  }
  if(!up && up_ant){
    cont++;
  }
  
  if(cont != cont_ant){
    lcd.print("       ");
    lcd.setCursor(0, 1);
    lcd.print(cont);
  }

  up_ant = up;
  down_ant = down;
  cont_ant = cont;
  
  delay(100);
}