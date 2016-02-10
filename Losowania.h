#ifndef LOSOWANIA_H
#define LOSOWANIA_H

//-------------PLIK Losowania.h-----------------------------------------------

#include <math.h>

class Losowania //w programie nie powinien byc wiecej niz jeden
//(z zawsze identycznym ciagiem stanow zaleznym od
//calkowitej ilosci losowan)
{

private:
  //STALE
  long int seed;
  long int a;
  long int m;
  long int q;
  long int r;
  long double conv;
  long double PI;

public:

  //ZMIENNE
  long int b;    //=seed; zaninicjalizowac ziarnem
  long int IloscLosowan; //=0;
  long int IleRazyObroconoCalkowicieLicznikLosowan; //=0;
  short int CzyPozostGauss; //=0;
  long double PozostalyGauss;

  //FUNKCJE
  //--------------------------------------------------domyslny kontruktor
  Losowania();
  //--------------------------------------------------KOPIATOR :)
  Losowania& operator=(const Losowania& L)
  {
    b = L.b;
    IloscLosowan = L.IloscLosowan;
    IleRazyObroconoCalkowicieLicznikLosowan =
        L.IleRazyObroconoCalkowicieLicznikLosowan;
    CzyPozostGauss = L.CzyPozostGauss;
    PozostalyGauss = L.PozostalyGauss;
    return *this;
  }
  ;

  //-------------------------------funkcja losujaca z rozkladem normalnym
  double LosujLongDouble(double DO = 1.0, double OD = 0.0)
  {
    long int l;
    l = b / q;
    b = a * (b - q * l) - r * l;
    if (b < 0) b += m;

    if ((++IloscLosowan) == 0)
    {
      IleRazyObroconoCalkowicieLicznikLosowan++;
    }

    return (DO - OD) * (b - 1) * conv + OD;

  }
  ;

  //---------------------funkcja losujaca z rozkladem rownomiernym na int
  long int LosujInt(long int DO = 1, long int OD = 0)
  {
    return (long int) LosujLongDouble(DO + 1, OD);
  }
  ;

  //----------------------------------funkcja losujaca z rozkladem Gaussa
  long double LosujGaussa(long double OdchStand = 1.0,
      long double Srednia = 0.0)
  {
    if (CzyPozostGauss)
    {
      CzyPozostGauss--;
      return PozostalyGauss * OdchStand + Srednia;
    }
    else
    {
      long double c = LosujLongDouble();
      long double d = LosujLongDouble();

      PozostalyGauss = cosl(2 * PI * c) * sqrtl(-2 * logl(d));
      //ten niech bedzie zapasowy
      CzyPozostGauss++;

      return Srednia + OdchStand * sinl(2 * PI * c) * sqrtl(-2 * logl(d)); //ten teraz uzyjemy
    };

  }
  ;

};

#endif
