#include <Arduino.h>

int sign;

long value;

float result;

int clockpin = 4;  

int datapin = 5;

unsigned long tempmicros;
unsigned long tempmicros2;

bool res[24];
void decode();

 

void setup() {

  Serial.begin(9600);

  pinMode(clockpin, INPUT);

  pinMode(datapin, INPUT);

}



 void loop () {

/*bool clk = digitalRead(clockpin);
bool data = digitalRead(datapin);

 if(!clk){
   Serial.print("CLK = ");
   Serial.print(clk);
   Serial.print(", DATA = ");
   Serial.println(data);
 }*/

  for(int i = 0; i<24; i++){
    res[i] = 0;
  }

  tempmicros = micros();
  
  while (digitalRead(clockpin)==HIGH) {
    //delayMicroseconds(1);
    
  } //if clock is LOW wait until it turns to HIGH

  tempmicros2 = micros();

  if ((tempmicros2-tempmicros)>10000) { //if the HIGH pulse was longer than 500 micros we are at the start of a new bit sequence
    

   // while (digitalRead(clockpin)==LOW) {
   //   delayMicroseconds(1);
   // } //wait for the end of the HIGH pulse
      //tempmicros = micros();
      decode(); //decode the bit sequence
  }
}

 

void decode() {
  bool data;
  // = digitalRead(datapin);
  //tempmicros2 = micros();
  //Serial.println(tempmicros2-tempmicros);
  //res[0] = data;
  int value = 0;
  int signo = 0;
  for (int i=0; i<24; i++) {
    while (digitalRead(clockpin)==LOW) {
      //delayMicroseconds(1);
      //tempmicros = micros();
    }

    data = digitalRead(datapin);
    res[i] = data;
    if(i<16){
      value |= data << i;
    }else{
      signo |= (data << (i-16));
    }

    while (digitalRead(clockpin)==HIGH) {
      //delayMicroseconds(1);
    }
    //tempmicros2 = micros();
    //Serial.println(tempmicros2-tempmicros);
  }
  Serial.print("INI  ");
  int j = 0;
  for(int i=0; i<24; i++){
    if(res[23-i]){
        Serial.print("1");
    }else{
        Serial.print("0");
    }
    j++;
    if(j == 4){
      Serial.print(" ");
      j=0;
    }
  }
  Serial.println(" FIN");
    
  if(signo & 0x10){
    value = -value;
  }
  if(signo & 0x80){
    Serial.print("inches");
    value = value/2;
  }
  Serial.println(value);
  Serial.println(signo);
  delay(1000);
  return;
}