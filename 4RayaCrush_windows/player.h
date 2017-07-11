#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

class Player{
    public:
      Player(int jug);
      Environment::ActionType Think();
      void Perceive(const Environment &estado);
     // double Poda_AlfaBeta(const Environment &actual_,int jugador_,int PROFUNDIDAD_ALFABETA, double alpha,double beta);
      //int miniMax(Environment estado, bool* aplicables, int niveles, int jugador_);
      double miniMax(const Environment &estado,int jugador, int PROFUNDIDAD_ALFABETA,bool minymax,double alpha,double beta);
      void PodaAlfaBeta(Environment  actual_,int &n_act,Environment::ActionType &accion, int jugador,double alp,double bet,int profundidad);
      int getIndiceAleatorio(int tam);
    private:
      int jugador_;
      Environment actual_;
};
#endif
