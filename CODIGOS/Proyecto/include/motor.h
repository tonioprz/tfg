#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

class Motor
{
    int IN3;
    int IN4;
    int ENB;
    int EMERGENCIA;
    float kp = 0.5;
    float Ti = 2;
    float Td = 0.1;
    float T = 0.01;
    
    public:
        Motor(int pinIN3, int pinIN4, int pinENB, int pinEMERGENCIA);
        void pararMotor();
        void moverMotor(int u);
        void movimientoMotor(long objetivo, long* posicion, LiquidCrystal_I2C lcd);
};