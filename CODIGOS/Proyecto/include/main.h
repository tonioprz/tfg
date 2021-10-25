#define faseA 2
#define faseB 3


#define BUTTON_ESC 22
#define BUTTON_ENTER 23
#define BUTTON_UP 24
#define BUTTON_DOWN 25

#define BUTTON_EMERGENCIA 18

#define ENB 5
#define IN3 27
#define IN4 26

bool enter;
bool esc;
bool up;
bool down;
bool esc_ant = 0;
bool enter_ant = 0;
bool up_ant = 0;
bool down_ant = 0;

long posicion = 0;
long * pposicion = &posicion;

char state = 'i';
bool discreto = 0;
long desplazamiento = 10000;
long objetivo = 0;
