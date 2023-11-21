#include "Gluto-Snake.h"


// ------------------ Declaración de Constantes ------------------
// Ejes del arreglo Dirección del Registro "tDatosJuego" (Sería la posición del arreglo)
const int X = 0;
const int Y = 1;

// Sentido del arreglo Dirección del registro "tDatosJuego" (Serían los posibles valores del arreglo)
const int SENTIDO_DER = 1;  // Sentido Derecha.
const int SENTIDO_IZQ = -1; // Sentido Izquierda.
const int SENTIDO_ABJ = 1;  // Sentido Abajo.
const int SENTIDO_ARR = -1; // Sentido Arriba.
const int QUIETO = 0;     


// ------------------ Declaración de Tipos ------------------
typedef char tString[10];


// Declaración de Estructuras
typedef struct{
	tSnake *snake;
	bool gameOver;
	bool digestion;
	int tiempo;
	int direcc[2];
	char tecla;
	int puntuacion;
	int longitudSerpiente;
	int tiempoDigestion;
	tManzana *manzana;
	
}tDatosJuego; // Datos del funcionamiento del Juego.

typedef struct{
	tString nombreJugador;
	int codigoJugador;
	int puntaje;
	int ranking;
}tDatosJugador; // Datos del Jugador.

typedef struct{
	FILE *archivo;
	tString nombreArchivo;
	int codCampoControl;
	
	int cantidadJugadores;
	tDatosJugador rankJugadores[10]; // Rank de los 10 jugadores con más puntaje;
}tDatosCorteControl; // Datos de Fin de Juego.


// ------------------ Prototipos de Funciones ------------------
// Inicialización del Programa.
bool inicializarJuego(tDatosCorteControl *, tDatosJugador *);
void inicializarDatosJuego(tDatosJuego *);

// Ejecución del Juego.
void bucleDeJuego(tDatosJuego *);

// Finalización del Juego.
void escribirJugador(tDatosCorteControl *, tDatosJugador *); // Escribir en el archivo los datos del jugador.

// Funciones de Corte de Control de final de juego, para mostrar ranking de los jugadores en el archivo.
// Corte de Control.
void iniciarFinJuego(tDatosCorteControl *, tDatosJugador *); // Inicio Corte
void procesoRanking(tDatosCorteControl *, tDatosJugador *);  // Proceso Corte
void mostrarRanking(tDatosCorteControl *, tDatosJugador *);  // Fin Corte




// ------------------ Función Principal ------------------
int main(){
	tDatosJuego datosJuego;
	tDatosJugador datosJugador, datosLectura;
	tDatosCorteControl datosFinDeJuego;
	
	inicializarDatosJuego(&datosJuego);
	datosJuego.gameOver = !(inicializarJuego(&datosFinDeJuego, &datosJugador)); // Si no se inicializa el juego, entonces es Game Over. Caso contrario, no lo es.
	
    crear_Serpiente(&(datosJuego.snake));
    crearManzana(datosJuego.snake, &(datosJuego.snake), &(datosJuego.manzana));
    imprimir_Serpiente(datosJuego.snake);
	imprimirManzana(datosJuego.manzana);
	
    bucleDeJuego(&datosJuego);
    datosJugador.puntaje = datosJuego.puntuacion*10;
    
    escribirJugador(&datosFinDeJuego, &datosJugador);
    
    iniciarFinJuego(&datosFinDeJuego, &datosLectura);
    procesoRanking(&datosFinDeJuego, &datosLectura);
    mostrarRanking(&datosFinDeJuego, &datosLectura);
    
    return 0;
}


// ------------------ Desarrollo de Funciones ------------------
void inicializarDatosJuego(tDatosJuego *pDatos){
	pDatos->gameOver = false;
	pDatos->tiempo = 0;              // Contador de Tiempo (Para establecer la velocidad de movimiento de la serpiente)
	pDatos->tecla = ARRIBA;          // Empieza yendo hacia arriba.
	pDatos->direcc[X] = QUIETO;      // EJE X
	pDatos->direcc[Y] = SENTIDO_ARR; // EJE Y, ecomo empieza yendo arriba, el sentido de la dirección del eje Y es hacia arriba.
	pDatos->puntuacion = 0;          // Puntuación del Jugador.
	pDatos->longitudSerpiente = 0;
	pDatos->digestion = false;
	pDatos->tiempoDigestion = 0;
	srand(time(NULL));               // Senilla de la función rand basada en la hora actual.
}

bool inicializarJuego(tDatosCorteControl *pDatosCorte, tDatosJugador *pDatosJugador){
	bool iniciarJuego = true;
	char caracter;
	int codigoJugador = 0;
	tString nombre;
	
	system("cls");       // Borra todo lo que hay en la consola y pone el cursor en la pos inicial.
	gotoxy(XBOUND + NX/2 -5, 4);
	printf("GLUTO-SNAKE"); // Título.
    imprimir_Escenario();
    
    gotoxy(XBOUND+(NX/2) - 20, YBOUND+NY/2);
    printf(" Ingrese Nombre Jugador: ");
    scanf("%[^\n]", &nombre);
    
	gotoxy(XBOUND+(NX/2) - 20, YBOUND+NY/2);
	printf("                                            ");
	
	gotoxy(XBOUND+(NX/2) - 20, YBOUND+NY/2);
	while (getchar() != '\n');
    printf(" Pulse Cualquier tecla (ESC para Salir): ");
    caracter = getch();
    
	gotoxy(XBOUND+(NX/2) - 20, YBOUND+NY/2);
	printf("                                            ");
	
	strcpy(pDatosCorte->nombreArchivo, "snake.dat"); 				// Establecemos nombre del archivo.
	pDatosCorte->archivo = fopen(pDatosCorte->nombreArchivo, "rb"); // Abrimos el archivo para lectura.
	fread(pDatosJugador, sizeof(tDatosJugador), 1, pDatosCorte->archivo);
	if(pDatosCorte->archivo != NULL){
		while(!feof(pDatosCorte->archivo)){
			fread(pDatosJugador, sizeof(tDatosJugador), 1, pDatosCorte->archivo);
			codigoJugador++; // Contamos cuántos jugadores hay registrados.
		}
	}
	pDatosJugador->codigoJugador = codigoJugador;
	pDatosCorte->cantidadJugadores = codigoJugador+1;
	
	fclose(pDatosCorte->archivo);
	
	
	
	if(caracter == ESC){
		iniciarJuego = false;
	}
	
	strcpy(pDatosJugador->nombreJugador, nombre);
	return iniciarJuego;
}

void escribirJugador(tDatosCorteControl *pDatosCorte, tDatosJugador *pDatosJugador){
	strcpy(pDatosCorte->nombreArchivo, "snake.dat");
	pDatosCorte->archivo = fopen(pDatosCorte->nombreArchivo, "ab");
	
	fwrite(pDatosJugador, sizeof(tDatosJugador), 1, pDatosCorte->archivo);
	fclose(pDatosCorte->archivo);
	
}

void iniciarFinJuego(tDatosCorteControl *pDatosCorte, tDatosJugador *pDatosJugador){
	
	pDatosCorte->archivo = fopen(pDatosCorte->nombreArchivo, "rb");
	fread(pDatosJugador, sizeof(tDatosJugador), 1, pDatosCorte->archivo);
	pDatosCorte->codCampoControl = pDatosJugador->codigoJugador;
	
	gotoxy(XBOUND+(NX/2) - 5, YBOUND+1);
    printf("GAME OVER");
    gotoxy(XBOUND+(NX/2) - 16, YBOUND+5);
    printf("| Ranking |  Jugador  | Puntaje |");
    
    int i;
    for(i=0; i<10; i++){
    	pDatosCorte->rankJugadores[i].codigoJugador = 0;
    	strcpy(pDatosCorte->rankJugadores[i].nombreJugador, "NN");
    	pDatosCorte->rankJugadores[i].puntaje = 0;
    	pDatosCorte->rankJugadores[i].ranking = 0;
	}
}

void procesoRanking(tDatosCorteControl *pDatosCorte, tDatosJugador *pDatosJugador){
	typedef tDatosJugador vectorJugadores[pDatosCorte->cantidadJugadores];
	vectorJugadores totalJugadores;
	int inicio = 0;
	int cantidad = 0;
	
	while(!feof(pDatosCorte->archivo)){
		while(!feof(pDatosCorte->archivo) && pDatosCorte->codCampoControl == pDatosJugador->codigoJugador){
			
			totalJugadores[pDatosJugador->codigoJugador].codigoJugador = pDatosJugador->codigoJugador;
			strcpy(totalJugadores[pDatosJugador->codigoJugador].nombreJugador, pDatosJugador->nombreJugador);
			totalJugadores[pDatosJugador->codigoJugador].puntaje = pDatosJugador->puntaje;
			
			fread(pDatosJugador, sizeof(tDatosJugador), 1, pDatosCorte->archivo);
		}
		
		pDatosCorte->codCampoControl = pDatosJugador->codigoJugador;
	}
	
	tDatosJugador aux;
	int i, j;
	for(i=1; i<pDatosCorte->cantidadJugadores; i++){
		for(j=0; j<pDatosCorte->cantidadJugadores-i; j++){
			if((totalJugadores[j]).puntaje > totalJugadores[j+1].puntaje){
				aux.codigoJugador = totalJugadores[j].codigoJugador;
				strcpy(aux.nombreJugador, totalJugadores[j].nombreJugador);
				aux.puntaje = totalJugadores[j].puntaje;
				aux.ranking = totalJugadores[j].ranking;
				
				totalJugadores[j].codigoJugador = totalJugadores[j+1].codigoJugador;
				strcpy(totalJugadores[j].nombreJugador, totalJugadores[j+1].nombreJugador);
				totalJugadores[j].puntaje = totalJugadores[j+1].puntaje;
				totalJugadores[j].ranking = totalJugadores[j+1].ranking;
				
				totalJugadores[j+1].codigoJugador = aux.codigoJugador;
				strcpy(totalJugadores[j+1].nombreJugador, aux.nombreJugador );
				totalJugadores[j+1].puntaje = aux.puntaje;
				totalJugadores[j+1].ranking = aux.ranking;
			}
		}
	}
	
	if(pDatosCorte->cantidadJugadores > 10){
		inicio = pDatosCorte->cantidadJugadores % 10;
	}
	
	for(i=inicio; i<pDatosCorte->cantidadJugadores; i++){
		pDatosCorte->rankJugadores[i-inicio].codigoJugador = totalJugadores[i].codigoJugador;
		strcpy(pDatosCorte->rankJugadores[i-inicio].nombreJugador, totalJugadores[i].nombreJugador);
		pDatosCorte->rankJugadores[i-inicio].puntaje = totalJugadores[i].puntaje;
		pDatosCorte->rankJugadores[i-inicio].ranking = totalJugadores[i].ranking;
		
	}
	for(i=inicio; i<pDatosCorte->cantidadJugadores; i++){
		pDatosCorte->rankJugadores[i-inicio].ranking = pDatosCorte->cantidadJugadores-i-1;
	}
}


void mostrarRanking(tDatosCorteControl *pDatosCorte, tDatosJugador *pDatosJugador){
	int i;
	int cantidad = 10;
	if(pDatosCorte->cantidadJugadores <= 10){
		cantidad = pDatosCorte->cantidadJugadores;
	}
	for(i=0; i<cantidad; i++){
		gotoxy(XBOUND+(NX/2) - 16, YBOUND+5+i+1);
		printf("         |               %d  ", pDatosCorte->rankJugadores[cantidad-1-i].puntaje);
		gotoxy(XBOUND+(NX/2) - 16, YBOUND+5+i+1);
		printf("   %d       %s", pDatosCorte->rankJugadores[cantidad-1-i].ranking+1, pDatosCorte->rankJugadores[cantidad-1-i].nombreJugador);
	}
	gotoxy(XBOUND+(NX/2) - 10, YBOUND+5+15);
	printf(" Gracias Por Jugar!");
	gotoxy(XBOUND + NX/2, YBOUND+NY+5);
	fclose(pDatosCorte->archivo);
}

void bucleDeJuego(tDatosJuego *pDatos){
	while(pDatos->gameOver == false){ // Mientras no sea el Fin del Juego
     	if(kbhit()){                		 // Si se detecta que pulsa una tecla. 
     		pDatos->tecla = getch(); 		 // Getch nos devuelve la tecla pulsada y guardamos la información.
			
			if ((pDatos->tecla == DERECHA && pDatos->direcc[X] != SENTIDO_IZQ) || (pDatos->tecla == IZQUIERDA && pDatos->direcc[X] != SENTIDO_DER)) { // Si se pulsó derecha, y en el eje X, el sentido es distinto del izquierdo, y viceversa.
        		pDatos->direcc[X] = (pDatos->tecla == DERECHA) - (pDatos->tecla == IZQUIERDA);														  //
        		pDatos->direcc[Y] = 0;      																										  // 	No permitir moverse hacia la derecha, cuando se está yendo a la izquierda, y viceversa.
    		} 																																		  //
			else {																																	  //
				if ((pDatos->tecla == ABAJO && pDatos->direcc[Y] != SENTIDO_ARR) || (pDatos->tecla == ARRIBA && pDatos->direcc[Y] != SENTIDO_ABJ)) {  // Si se pulsó abajo, y en el eje Y, el sentido es distinto de arriba, y viceversa.
        			pDatos->direcc[Y] = (pDatos->tecla == ABAJO) - (pDatos->tecla == ARRIBA);														  //
        			pDatos->direcc[X] = 0;   																										  // 	No permitir moverse hacia arriba, cuando se está yendo hacia abajo, y viceversa.
				}
			}
			
	 		switch (pDatos->tecla){
	 	 		case ARRIBA: 														// Se modifica la lengua dependiendo de hacia donde está se está moviendo.
				  	if(pDatos->tecla == ARRIBA && pDatos->direcc[Y] != SENTIDO_ABJ){ // No permitir que la lengua apunte hacia detrás, estando yendo hacia delante. (Que la serpiente no se desnuque)
				  	pDatos->snake->AP = LENGUA_ARRIBA;
				  	} 
	 	 			break;
	 	 		case ABAJO: 
	 	 			if(pDatos->tecla == ABAJO && pDatos->direcc[Y] != SENTIDO_ARR){
				  	pDatos->snake->AP = LENGUA_ABAJO;
				  	}
	 	 			break;
	 	 		case DERECHA: 
	 	 			if(pDatos->tecla == DERECHA && pDatos->direcc[X] != SENTIDO_IZQ){
				  	pDatos->snake->AP = LENGUA_DER;
				  	}
	 	 			break;
	 	 		case IZQUIERDA: 
	 	 			if(pDatos->tecla == IZQUIERDA && pDatos->direcc[X] != SENTIDO_DER){
				  	pDatos->snake->AP = LENGUA_IZQ;
				  	}
	 	 			break;		
		 	}
		}
	 	
		Sleep(FRAMERATE);
		pDatos->tiempo = pDatos->tiempo + FRAMERATE; // Tiempo refiere a los cuadros que se muestran por segundo. Y framerate es el tiempo en el que se ejecutan todas las acciones.
		
		if(pDatos->tiempo >= SPEED){                 // Cuando el contador de tiempo llegue a un valor mayor o igual que la velocidad de la serpiente para moverse entonces.
			imprimir_Serpiente(pDatos->snake);
			imprimirManzana(pDatos->manzana);
			gotoxy(XBOUND, YBOUND-1);
			printf(" Puntuaci%cn: %d", 162, pDatos->puntuacion*10);
			mover_Serpiente(pDatos->snake->X + pDatos->direcc[X], pDatos->snake->Y + pDatos->direcc[Y], &(pDatos->snake));
			
			if(colisionTablero(pDatos->snake) == true || (pDatos->tecla == ESC) == true || colisionPropia(pDatos->snake->next, &(pDatos->snake)) == true ){ // Si alguna de las condiciones de fin de juego es verdadera, se termina el juego.
				pDatos->gameOver = true;
			}
		 	pDatos->tiempo = 0;             // se resetea el contador de tiempo
			if(pDatos->digestion == false){ 													// Si no hemos comido ninguna manzana
				if(colisionManzana(pDatos->snake, &(pDatos->snake), pDatos->manzana) == true){  // Si llegamos a comer una manzana
					pDatos->puntuacion += pDatos->manzana->PUNTOS;							    // Aumentamos la puntuación dependiendo del tipo de manzana.
					pDatos->digestion = true;													// Iniciamos digestión
					pDatos->tiempoDigestion += pDatos->longitudSerpiente; 						// Tiempo en digerir la manzana, mientras más larga la serpiente, más tiempo tardará en digerirse.
					crearManzana(pDatos->snake, &(pDatos->snake), &(pDatos->manzana));			// Creamos otra manzana aleatoria en el escenario.		  
				}																				
			}																				 	
			else{
				if(colisionManzana(pDatos->snake, &(pDatos->snake), pDatos->manzana) == true){  // Volvemos a comprobar si se comió una manzana antes de haber digerido por completo la anterior.
					pDatos->puntuacion += pDatos->manzana->PUNTOS;							    
					pDatos->digestion = true;											    
					pDatos->tiempoDigestion += pDatos->longitudSerpiente;
					crearManzana(pDatos->snake, &(pDatos->snake), &(pDatos->manzana));			
				}
				else{
					if(pDatos->longitudSerpiente < pDatos->puntuacion){ 						// La puntuacion refiere a la cantidad de manzanas y los tipos especiales de manzanas, la longitud se equipara a este valor. Por lo tanto, si la longitud es menor, hay que hacer crecer a la serpiente.
					if(pDatos->tiempoDigestion == 0){ 											// Si ya se digirió completamente, hacemos crecer la serpiente.
						crecer_Serpiente(pDatos->manzana->x, pDatos->manzana->Y, &(pDatos->snake));
						pDatos->longitudSerpiente++;  											// Aumenta la longitud de la serpiente.
					}																			//
					else{																		//
						pDatos->tiempoDigestion--;    											// Si aún no fue digerida completamente, reducimos su tiempo de digestión.
					}																			//
					}																			//
					else{ 																		// Si la puntuación es igual a la longitud, podemos generar otra manzana y finalizar la digestión de la serpiente.
					pDatos->digestion = false; 													// Digestión finalizada.
					}
				}																				
				
				
			}
			
		}
    }
}
