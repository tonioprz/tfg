#include <motor.h>

Motor::Motor(int pinIN3, int pinIN4, int pinENB, int pinEMERGENCIA){
    IN3 = pinIN3;
    IN4 = pinIN4;
    ENB = pinENB;
    EMERGENCIA = pinEMERGENCIA;
}

void Motor::pararMotor(){
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 255);
}

void Motor::moverMotor(int u){
    if(u >= 0){ // Avance positivo
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    } else{ // Avance negativo
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      u = -u;
    }

    if(u > 200){
        u = 200; // La salida máxima es de 255
    }

    analogWrite(ENB, u);
}

void Motor::movimientoMotor(long objetivo, long* posicion, LiquidCrystal_I2C lcd){
  lcd.setCursor(0, 0);
  lcd.print("MOVIENDO...     ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(*posicion);
  lcd.print("/");
  lcd.print(objetivo);

  long u = 0;
  long ek = 0;
  long ek1 = 0;
  long ik = 0;
  long D = 0;
  int aux = 0;
  bool emer = 0;
  while(((*posicion < objetivo - 50) || (*posicion > objetivo + 50) || (u > 55)) && !emer){
    
    emer = digitalRead(EMERGENCIA);

    // Se actualiza la posición actual en el LCD cada 120 ms
    aux++;
    if(aux > 3){
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(*posicion);
      lcd.print("/");
      lcd.print(objetivo);        
      aux = 0;
    }

    //Actualización de error integral
    ek1 = ek;
    ek = objetivo - *posicion;
    ik = ik + ek;
    D = ek - ek1;
    //Cálculo de la señal de control
    u = kp * (ek + (T/Ti) * ik + (Td/T) * D);
    
    

    if((abs(u) > 255) && (abs(ek) > 200)){
      ik = 0;
    }

    if(u > 255){
      u = 255;
    }
    if(u < -255){
      u = -255;
    }
    
    if(!emer){
      moverMotor(u);
      delay(1000 * T);
    }
  }
  
  pararMotor();
  
  if(emer){
    lcd.clear();
    lcd.print("EMERGENCIA");
    delay(1000*T);
  }
}