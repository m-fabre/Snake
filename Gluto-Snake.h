// Gluto-Snake.h - TDA

#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<stdbool.h>
#include <conio.h> // Permite usar kbhit y getch, uno detecta qué tecla se usa, y el otro permite guardar esa tecla, para poder usar las flechas

//  ---------- Declaración de Constantes  ---------- 
// Velocidad de movimiento de la Serpiente
#define FRAMERATE 	16 // Ratio de Actualizacion de fotogramas: 16 milisegundos (60 fps aprox)
#define SPEED 		24 // Velocidad a de actualización de un cuadro cada 24 milisegundos.
                     // Si estos valores son iguales se logra mayor fluidez (60 fps) pero la velocidad de la serpienta es muy alta. Para hacerlo ligeramente más lenta, reducimos su velocidad ajustando SPEED
//teclas 
#define ESC 		27
#define ARRIBA 		72
#define IZQUIERDA 	75
#define DERECHA 	77
#define ABAJO 		80

//Caracteres ASCII del escenario:
#define ESQ_SI 201 // Esquina Superior Izquierda
#define ESQ_SD 187 // Esquina Superior Derecha
#define ESQ_II 200 // Esquina Inferior Izquierda
#define ESQ_ID 188 // Esquina Inferior Derecha
#define BAR_H 205  // Barra Horizontal
#define BAR_V 186  // Barra Vertical

#define NX 50    // Espacio de juego del eje X (Horizontal)
#define NY 50    // Espacio de juego del eje Y (Vertical)
#define XBOUND 5 // Posición de la esquina del tablero en el eje X 
#define YBOUND 5 // Posición de la esquina del tablero en el eje Y (Para que la esquina del tablero tenga un espacio respecto a la esquina del ejecutable)

// Códigos ASCII de los Caracteres que conforman la Serpiente
#define LENGUA_ARRIBA 118
#define LENGUA_ABAJO 94
#define LENGUA_IZQ 62
#define LENGUA_DER 60
#define CUERPO 'o'

// Puntos por Manzana:
#define MANZANA 227
#define GOLD_APPLE 5
#define APPLE 1
#define PROBABILIDAD_GOLD 25


// ---------- Declaración de Tipos Estructuras----------

typedef struct snake{
	int X, Y, AP, COLOR; //Posición en el eje X, posición en el eje Y, apariencia (caracter), color.
	struct snake *next;  // Puntero al siguiente elemento.
}tSnake;                 // Tipo de dato compuesto: "tipoSnake"

typedef struct {
	int x, Y, AP, COLOR, PUNTOS;
}tManzana;

// ---------- Declaración de Funciones ---------- 

// Funciones de Impresión.
void gotoxy(int pX, int pY); // Función de Código de Escape, Para Reposicionar en donde queremos imprimir algo en Pantalla.
void imprimir_Escenario();
void imprimir_Serpiente(tSnake *pBody);
void imprimirManzana(tManzana *);

// Funciones de la Serpiente.
void crecer_Serpiente(int pX, int pY, tSnake **pBody);
void crear_Serpiente(tSnake **pSnake);
void mover_Serpiente (int pX, int pY, tSnake **pSnake); 
bool colisionTablero(tSnake *);
bool colisionPropia(tSnake *, tSnake **);

// Funciones de la Manzana.
void crearManzana(tSnake *, tSnake **, tManzana **);
bool colisionManzana(tSnake *, tSnake **, tManzana *);
void resetManzana(tSnake *, tSnake **, tManzana **);


// ---------- Declaración de Variables ---------- 

int cola_Anterior[2];


// ---------- Desarrollo De Funciones  ---------- 

void gotoxy(int pX, int pY){
    printf("%c[%d;%dH", ESC, pY, pX);
}


void imprimir_Escenario() {
    int i, j; // i = filas - j = columnas.
    char c;   // Caracter que al asignar valores imprime alguna parte del tablero.
    
    gotoxy(XBOUND, YBOUND); // Separación de la esquina del tablero, respecto de la esquina del ejecutable.
    for(i=0; i<NX+2; ++i){  // NY * NX es el espacio de juego, hay que imprimir el tablero fuera de ese espacio, por lo tanto sumamos 2 a  NY y NX.
        for(j=0; j<NY+2; ++j){
             //Imprimir Esquinas y Columnas {
           if(j == 0 || j == NX+1){ // Si estamos en alguno de los extremos de las columnas {
                switch(i) {                      //
                    case 0:                      // Si estamos al principio de las filas i {
					 if(j == 0){                 // Si estamos al principio de las columnas j: Estamos en la Esquina Superior Izquierda, por lo tanto la imprimimos.
					 c = ESQ_SI;                 //
					 }					         //
					 else{                       // Si no, estamos al final de las columnas j: Estamos en la Esquina Superior Derecha, por lo tanto la imprimimos.
					 c = ESQ_SD;                 //
					 }                           // }
                    break;                       //
                    case NY+1:                   // Si estamos al final de las filas i: {
					 if(j == 0){                 // Si estamos al principio de las columnas j: Estamos en la Esquina Inferior Izquierda, por lo tanto la imprimimos.
					 c = ESQ_II;                 //
					 }                           //
					 else{  	                 // Si no, estamos al final de las columnas j: Estamos en la Esquina Inferior Derecha, por lo tanto la imprimimos.
					 c = ESQ_ID;                 //
					 }                           // }
                    break;                       //
                    default: c = BAR_V;          // Si estamos en alguno de los extremos de las columnas que no sean esquinas (O sea, las filas no son ningún extremo), entonces imprimimos las Barras Verticales que unen cada esquina.
                    break;          // }
                }
           } // }
             // Imprimir Filas y el tablero vacío entre medio {
           else{ // Si no estamos en alguno de los extremos columnas {
                if(i == 0 || i == NY+1) { // Si estamos en alguno de los extremos filas que no sean esquinas (O sea, que las columnas no son ningún extremo), entonces imprimimos las Barras Horizontales que unen cada esquina.
                    c = BAR_H;            //
                }                         //
                else{                     //
                    c = ' ';              // Si no estamos en ningún extremo, es espacio de juego, por lo tanto imprimimos un espacio vacío.
                } // }
           } // }
           printf("%c", c); // Imprimimos el caracter guardado
           if(j == NY+1){   // Si es el final de la columna {
                gotoxy(XBOUND, YBOUND+i+1); // Salto de línea a la siguiente vertical, respetando el margen X Y definido, respecto del ejecutable.
           }                // }
        } 
    }
}

void imprimir_Serpiente(tSnake *pBody){    // Es una función recursiva que se llama a si misma.
	printf("%c[1;%dm", ESC, pBody->COLOR); // Color de la impresión del cuerpo.
	gotoxy(pBody->X, pBody->Y);            // Posicionamos donde queremos el cursor.
	printf("%c", pBody->AP);               // Imprimimos el caracter de la apariencia del cuerpo.
	printf("%c[0m", ESC);
	if(pBody->next != NULL){
		 imprimir_Serpiente(pBody->next);
	}
	else{ // AL mover la serpiente, hay que borrar su cola anterior
		gotoxy(cola_Anterior[0], cola_Anterior[1]);
			printf(" ");
	}
}

// GESTION DE SERPIENTE (como va creciendo)
void crecer_Serpiente(int pX, int pY, tSnake **pSnake){
	if(((*pSnake)->next) == NULL){
		tSnake *body;
	
	 	body = (tSnake *)malloc(sizeof(tSnake));
	 	body->X = pX;
		body->Y = pY;
		body->AP = CUERPO;
		body->COLOR = 32; // 32 es el codigo del color verde
		body->next = NULL;
		
		(*pSnake)->next = body;
	}
	else{
		crecer_Serpiente(pX, pY, &((*pSnake)->next));
	}
}

void crear_Serpiente(tSnake **pSnake){
	tSnake *newSnake;
	
	newSnake = (tSnake *)malloc(sizeof(tSnake));
	newSnake->X = XBOUND + NX/2; 
	newSnake->Y = YBOUND + NY/2;  // Posicionamiento de la Serpiente en el centro del tablero.
	newSnake->AP = LENGUA_ARRIBA; // Empezará mirando hacia arriba.
	newSnake->COLOR = 31;         // Color de la Lengua: 31 = Rojo.
	newSnake->next = NULL;
	(*pSnake) = newSnake;
	
	crecer_Serpiente(XBOUND+NX/2, YBOUND+(NY/2)+1, pSnake); // Se agregará el cuerpo de la serpiente, conectada 1 lugar por detrás de la cara.
}

void mover_Serpiente (int pX, int pY, tSnake **pSnake){
	//lista enlazada y recursiva, pone un elemento en la posicion anterior
	tSnake *body;
	body = (*pSnake);
	
	if(body->next != NULL){ //si estamos en el primer elemento body apunta next es distinto de null
		mover_Serpiente(body->X, body->Y, &body->next); //entonces, el siguiente elemento se va a mover a la posicion del elemento actual
	} //como esto es una funcion recursiva cuadno salgamos de imprimir la cola, vamos a ir a la funcion anterior
	else {
		cola_Anterior[0] = body->X;
		cola_Anterior[1] = body->Y; // Se almacena la posicion de la cola actual de la serpiente, para poder borrarla y que no queden impresas en el recorrido de la serpiente
	}

	(*pSnake)->X = pX; //me salgo del bucle y actualizo la nueva posicion 
	(*pSnake)->Y = pY; 
}

void crearManzana(tSnake *pBody, tSnake **pSnake, tManzana **pManzana){
	tManzana *nuevaManzana;
	nuevaManzana = (tManzana *) malloc(sizeof(tManzana));
	nuevaManzana->AP = MANZANA;
	
	(*pManzana) = nuevaManzana;
	resetManzana(pBody, pSnake, pManzana);
}

bool colisionManzana(tSnake *pBody, tSnake **pSnake, tManzana *pManz){
	static bool colision;
	if(pBody == (*pSnake)){ // Si es la cabeza de la serpiente, entonces asignar valor 0 a colision.
		colision = false;
	}
	if((pBody->X == pManz->x) && (pBody->Y == pManz->Y)){// Si coincide con la posición de la manzana, colisión cambiará su valor, si no hay colisión, seguirá siendo 0.
		colision = true;
	}
	if(colision == false){
		if(pBody->next != NULL){
			colisionManzana(pBody->next, pSnake, pManz);
		}
	}
	return colision;
}

void resetManzana(tSnake *pBody, tSnake **pSnake, tManzana **pManzana){
	tManzana *nuevaManzana;
	nuevaManzana = (tManzana *) malloc(sizeof(tManzana));
	nuevaManzana->AP = MANZANA;
	// Asignación de Posición
	nuevaManzana->x = XBOUND + rand() % NX + 1;
	nuevaManzana->Y = YBOUND + rand() % NY + 1;
	if(colisionManzana(pBody, pSnake, *pManzana) == true){ // Si hay colision en el posicionamiento de la manzana nueva, entonces llamamos a que se vuelva a posicionar con otro valor.
		resetManzana(pBody, pSnake, pManzana);
	}
	
	// Asignación de Puntos.
	nuevaManzana->PUNTOS = APPLE + (GOLD_APPLE - APPLE) * (PROBABILIDAD_GOLD >= rand() % 101); // Si 25 es mayor o igual a un numero aleatorio entre 0 y 100, entocnes será una manzana dorada. Caso contrario será una manzana común.
	if(nuevaManzana->PUNTOS == APPLE){
		nuevaManzana->COLOR = 31;
	}
	if(nuevaManzana->PUNTOS == GOLD_APPLE){
		nuevaManzana->COLOR = 33;
	}
	
	(*pManzana) = nuevaManzana;
}

void imprimirManzana(tManzana *pManzana){
	printf("%c[1;%dm", ESC, pManzana->COLOR); // Color de la impresión
	gotoxy(pManzana->x, pManzana->Y);         // Posicionamos donde queremos el cursor.
	printf("%c", pManzana->AP);               // Imprimimos el caracter de la apariencia del cuerpo.
	printf("%c[0m", ESC);
}

bool colisionTablero(tSnake *pSnake){
	int B = false;
	if((pSnake->X <= XBOUND || pSnake->X > XBOUND + NX) || (pSnake->Y <= YBOUND || pSnake->Y > YBOUND + NY)){
		B = true;
	}
	return B;
}

bool colisionPropia(tSnake *pBody, tSnake **pSnake){
	tSnake *snake;
	
	snake = *pSnake;
	if((snake->X == pBody->X) && (snake->Y == pBody->Y)){
		return true;
	}
	else{
		if(pBody->next != NULL){
			colisionPropia(pBody->next, pSnake);
		}
		else{
			return false;
		}
	}
}
