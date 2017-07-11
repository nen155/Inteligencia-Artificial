#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;
int jugadas = 0;

// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & estado){
    actual_=estado;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}


// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Puntuacion(jugador,estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar

double valorar(const Environment & estado,int jugador)
{
    double res=0;
    for(int y=0; y<7; ++y){
        for(int x=0; x<7; ++x){
            if(estado.See_Casilla(y,x)==jugador){


                //////////////VERTICALES------------------------->
                /////////////////JUGADAS DE 2////////////////////
                //Vertical de 2
                if( y>0 && estado.See_Casilla(y-1,x) == jugador )
                    res += 4;
                //Vertical de 2 central
                if((y>0 && x==3) && estado.See_Casilla(y-1,x) == jugador)
                    res += 1;
                //Vertical de 2 izquierda
                if((y>0 && x==2) && estado.See_Casilla(y-1,x) == jugador )
                    res += 1;
                 //Vertical de 2 derecha
                if((y>0 && x==4) && estado.See_Casilla(y-1,x) == jugador )
                  res += 1;
                ////////////////JUGADAS DE 3/////////////////////
                //Vertical de 3
                if((y>1 && y<6)  && estado.See_Casilla(y-1,x) == jugador && estado.See_Casilla(y-2,x) == jugador)
                {
                    res += 3;
                    //Vertical de 3 con espacio arriba
                    if(y > 2 && estado.See_Casilla(y-3,x)==0)
                        res+=6;
                }
                //Vertical de 3 central
                if(y>1 && x==3   && estado.See_Casilla(y-1,x) == jugador && estado.See_Casilla(y-2,x) == jugador)
                    res += 1;


                //////////////HORIZONTALES------------------------->
                /////////////////JUGADAS DE 2//////////////////////
                //Horizontal de 2
                if(x<6 && estado.See_Casilla(y,x+1) == jugador )
                    res += 2;
                //Horizontal de 2 hacia la izquierda cerca del centro
                if(x>1 && x<4 && estado.See_Casilla(y,x+1) == jugador )
                    res += 2;
                /////////////////JUGADAS DE 3////////////////////
                //Horizontal de 3
                if(x<5 && estado.See_Casilla(y,x+1) == jugador && estado.See_Casilla(y,x+2) == jugador)
                    res += 5;
                //Horizontal de 3 hacia la izquierda cerca del centro
                if(x>0 && x<4  && estado.See_Casilla(y,x+1) == jugador && estado.See_Casilla(y,x+2) == jugador)
                {
                    res += 5;
                    //Horizontal de 3 con espacios laterales
                    if(estado.See_Casilla(y,x+3)==0 && estado.See_Casilla(y,x-1)==0)
                        res += 10;
                }



                //////////////DIAGONALES------------------------->
                /////////////////JUGADAS DE 2////////////////////
                //Diagonal derecha de 2
                if((y>0 && x<6) && estado.See_Casilla(y-1,x+1) == jugador )
                    res += 2;
                //Diagonal derecha de 2 más a la izquierda para poder terminarla
                if( (y>0 && x<4) && estado.See_Casilla(y-1,x+1) == jugador )
                    res += 3;
                //Diagonal izquierda de 2
                if((y>0 && x>0) && estado.See_Casilla(y-1,x-1) == jugador )
                    res += 5;
                //Diagonal izquierda de 2 más a la derecha para poder terminarla
                if((y>2 && x>2) && estado.See_Casilla(y-1,x-1) == jugador )
                    res += 3;
                /////////////////JUGADAS DE 3////////////////////
                //Diagonal derecha de 3
                if((y>1 && x<5)  && estado.See_Casilla(y-1,x+1) == jugador && estado.See_Casilla(y-2,x+2) == jugador)
                    res += 6;
                //Diagonal derecha de 3 más a la izquierda para poder terminarla
                if((y>2 && x<4)  && estado.See_Casilla(y-1,x+1) == jugador && estado.See_Casilla(y-2,x+2) == jugador)
                {
                    res += 6;
                    //Diagonal derecha de 3 con espacios laterales
                    if(y<6 && x>0 && estado.See_Casilla(y-3,x+3)==0 && estado.See_Casilla(y+1,x-1)==0)
                        res += 10;
                }
                //Diagonal izquierda de 3
                if((y>1 && x>1)  && estado.See_Casilla(y-1,x-1) == jugador && estado.See_Casilla(y-2,x-2) == jugador)
                    res += 5;
                //Diagonal izquierda de 3 más a la derecha para poder terminarla
                if((y>2 && x>2)  && estado.See_Casilla(y-1,x-1) == jugador && estado.See_Casilla(y-2,x-2) == jugador)
                {
                    res += 5;
                    //Diagonal izquierda de 3 con espacios laterales
                    if(y<6 && x<6 && estado.See_Casilla(y+1,x+1)==0 && estado.See_Casilla(y-3,x-3)==0)
                        res += 12;
                }


                //COMBINACIONES CON ESPACIO EN MEDIO
                if(estado.See_Casilla(y,x+1) == 0  && estado.See_Casilla(y,x+2) == jugador && estado.See_Casilla(y,x+3) == jugador)
                    res+=10;
                if( estado.See_Casilla(y,x+1) == jugador  && estado.See_Casilla(y,x+2) == 0 && estado.See_Casilla(y,x+3) == jugador)
                    res+=10;

                //Prioridad inicial
                if(x==3 && jugador ==2)
                    res+=2;
                if(x==5 && jugador ==1)
                    res+=2;
                 if(x==3)
                   res+=2;
            }
        }
    }
    return res;
}



double Valoracion(const Environment &estado, int jugador)
{
    int ganador = estado.RevisarTablero();
    if (ganador==jugador)
        return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
        return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
        return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else{
        //Si no he ganado ni perdido ni empatado aplicamos la heurística
        double total=0;
        total += valorar(estado,jugador);
        return total;
    }
}





// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<8; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}
/* FUNCIÓN QUE PROBÉ PARA QUITAR LOS POSIBLES MOVIMIENTOS SIMÉTRICOS EN EL ARRAY DE MOVIMIENTOS
void quitarMovSimetricos(Environment * movs,int & n_act){
    int cont=0;
    for(int c=0,f=n_act-1;c<n_act && f>=0;++c,--f){
        for(int i=0,k=6;i<7 && k>=0;++i,--k){
            for(int j =0,h=7;j<7 && h>0 && movs[c].See_Casilla(i,j) == movs[f].See_Casilla(k,h);++j,--h){
                    cont++;
            }
        }
        if(cont==49){
            for(int i=f;i<n_act-1;i++)
            {
                movs[i]=movs[i+1];
            }
            n_act--;
        }
        cont=0;
    }
}
*/
double Player::miniMax(const Environment &estado,int jugador, int profundidad,bool minymax,double alp,double bet)
{
    if(estado.JuegoTerminado() || profundidad==0){
        jugadas++;
        return Valoracion(estado,jugador);
    }
    int numAct=-1;
    Environment siguient;
    siguient = estado.GenerateNextMove(numAct);
    if(minymax){
        while(!(siguient==estado)){
            alp = max(alp,miniMax(siguient,jugador,profundidad-1,!minymax,alp,bet));
            if(alp >= bet)
                return bet;
            siguient = estado.GenerateNextMove(numAct);
        }

        return alp;
    }
    else{
        while(!(siguient==estado)){
            bet = min(bet,miniMax(siguient,jugador,profundidad-1,!minymax,alp,bet));
            if(alp >= bet)
                return alp;
            siguient = estado.GenerateNextMove(numAct);
        }
        return bet;
    }

}

void Player::PodaAlfaBeta(Environment  actual_,int &n_act,Environment::ActionType &accion, int jugador,double alp,double bet,int profundidad){
    double valor;
    int act=-1;
    Environment tableros[8];
    n_act = actual_.GenerateAllMoves(tableros);
    for(int i=0; i<n_act; ++i){
        valor = miniMax(tableros[i],jugador,profundidad-1,false,alp,bet);
            if(valor > alp){
                alp = valor;
                accion = static_cast< Environment::ActionType > (tableros[i].Last_Action(jugador));
                cout << "Valor>: " << valor << "  Accion: "<<actual_.ActionStr(accion)<<endl;
            }else
            if(alp == menosinf){
                Environment tablero = actual_.GenerateNextMove(act);
                accion = static_cast< Environment::ActionType > (tablero.Last_Action(jugador));
                cout << "Valor=: " << valor << "  Accion: "<<actual_.ActionStr(accion)<<endl;
            }
   }

     cout << "Jugadas:" << jugadas << endl;
}

// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){

    clock_t comienzo,fin;//para tiempo de ejecución

    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // acción que se va a devolver
    bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable
                        // aplicables[7]==true si BOOM es aplicable



    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha=menosinf, beta=masinf; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual


    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;


    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------

    alpha = menosinf;
    beta = masinf;
    comienzo=clock();
    PodaAlfaBeta(actual_,n_act,accion,jugador_,alpha,beta,PROFUNDIDAD_ALFABETA);
    fin=clock();
    double res=(double)(fin-comienzo)/1000;
	cout<<"Tiempo pensado: "<< res <<" seg"<<endl;

    return accion;
}

