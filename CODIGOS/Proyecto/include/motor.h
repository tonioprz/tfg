#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

class Motor
{
    int IN3;
    int IN4;
    int ENB;
    int EMERGENCIA;
    float kp = 1;
    float Ti = 2;
    float Td = 0.3;
    float T = 0.03;
    
    public:
        Motor(int pinIN3, int pinIN4, int pinENB, int pinEMERGENCIA);
        void pararMotor();
        void moverMotor(int u);
        void movimientoMotor(long objetivo, long* posicion, LiquidCrystal_I2C lcd);
};