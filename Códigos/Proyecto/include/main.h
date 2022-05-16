#define faseA 2
#define faseB 3

byte MAC[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // Dirección MAC del dispositivo
IPAddress IP(192,168,50,200); // IP estática del dispositivo
EthernetServer servidor(4012); // Puerto donde se transmite la información

#define BUTTON_ESC 22
#define BUTTON_ENTER 23
#define BUTTON_UP 24
#define BUTTON_DOWN 25

#define BUTTON_LOCAL 28
#define BUTTON_MICRO 29
#define BUTTON_EMERGENCIA 30

#define SENSOR_FOTO 31

#define CAL_CLK 32
#define CAL_DATA 33

#define ENB 5
#define IN3 40
#define IN4 41

bool enter;
bool esc;
bool up;
bool down;
bool esc_ant = 0;
bool enter_ant = 0;
bool up_ant = 0;
bool down_ant = 0;

bool emergencia;
bool local;
bool micro;
bool fotoelectrico;

bool emergencia_ant;
bool local_ant;
bool micro_ant;

long posicion = 0;
long * pposicion = &posicion;

float posy = 0;

char estado = 'i';
bool discreto = 0;
long desplazamiento = 1000;
long objetivo = 0;

float kp = 0.5;
float Ti = 2;
float Td = 0.1;
float T = 0.01;