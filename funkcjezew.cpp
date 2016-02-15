#ifndef FUNKCJEZEW_CPP
#define FUNKCJEZEW_CPP

#ifndef FUNKCJEZEW_H
#include "funkcjezew.h"
#endif

#ifndef WEKTOR3D_H
#include "Wektor3D.h"
#endif

#include <limits>
#include <cstddef>

//------------------------------------------------------------------------------
double WyznaczPromienMax(const Wektor3D& Punkt,             //funkcja nie dba
    const Wektor3D* const wskTablicySegm, //czy nie dziala
    int IloscSegm)                        //poza tablica!
{
  if (wskTablicySegm == NULL) return 0;

  Wektor3D Punkt1 = Punkt, Pomoc;
  Wektor3D* wskPomoc;

  double R, RMax2 = 0;
  if (IloscSegm < 0)
  {
    wskPomoc = const_cast<Wektor3D*>(wskTablicySegm);
    for (int i = 0; i > IloscSegm; i--)
    {
      Pomoc = (*wskPomoc--) - Punkt1;
      R = Pomoc.lengthSq();
      if (R > RMax2) RMax2 = R;
    };
  }
  else
  {
    wskPomoc = const_cast<Wektor3D*>(wskTablicySegm);
    for (int i = 0; i < IloscSegm; i++)
    {
      Pomoc = (*wskPomoc++) - Punkt1;
      R = Pomoc.lengthSq();
      if (R > RMax2) RMax2 = R;
    };
  };

  return sqrt(RMax2);
}
//------------------------------------------------------------------------------
double WyznaczPromienMin(const Wektor3D& Punkt,             //funkcja nie dba
    const Wektor3D* const wskTablicySegm, //czy nie dziala
    int IloscSegm)                        //poza tablica!
{
  if (wskTablicySegm == NULL) return 0;

  Wektor3D Punkt1 = Punkt, Pomoc;
  Wektor3D* wskPomoc;

  double R, RMin2 = std::numeric_limits<double>::max();
  if (IloscSegm < 0)
  {
    wskPomoc = const_cast<Wektor3D*>(wskTablicySegm);
    for (int i = 0; i > IloscSegm; i--)
    {
      Pomoc = (*wskPomoc--) - Punkt1;
      R = Pomoc.lengthSq();
      if (R < RMin2) RMin2 = R;
    };
  }
  else
  {
    wskPomoc = const_cast<Wektor3D*>(wskTablicySegm);
    for (int i = 0; i < IloscSegm; i++)
    {
      Pomoc = (*wskPomoc++) - Punkt1;
      R = Pomoc.lengthSq();
      if (R < RMin2) RMin2 = R;
    };
  };

  return sqrt(RMin2);
}
//------------------------------------------------------------------------------
Wektor3D WyznaczSrodekMasy(const Wektor3D* wskTablicy1, int Ile, int Punkt1,
    int Punkt2)
//wyznaczaja srodek masy wybranego
//przedzialu tablicy elem;
//nie uwzglednia drugiego punktu;
//-----zmienic na dowolne elem----
//-----zszablonowac------------
{
  Wektor3D Suma(0, 0, 0);
  Wektor3D *wskPomoc;

  if (Ile <= 0 || Punkt1 < 0 || Punkt2 < 0 || Punkt1 > Ile || Punkt2 > Ile)
    return Suma / 0.0;

  int Roznica = Punkt2 - Punkt1;

  double odwrRozn;

  if (!Roznica)
  {
    wskPomoc = const_cast<Wektor3D*>(wskTablicy1);
    for (int i = 0; i < Ile; i++)
    {
      Suma += *wskTablicy1++;
    };
    odwrRozn = 1.0 / Ile;
    return Suma * odwrRozn;
  };

  odwrRozn = (Roznica > 0 ? 1.0 / Roznica : -1.0 / Roznica);

  wskPomoc = const_cast<Wektor3D*>(&wskTablicy1[Punkt1]);
  for (int i = Punkt1; i != Punkt2; Roznica > 0 ? i++ : i--)
  {
    Suma += Roznica > 0 ? *wskPomoc++ : *wskPomoc--;
  };
  return Suma * odwrRozn;
}
//------------------------------------------------------------------------------
Wektor3D WyznaczSrodekMasy(const Wektor3D* wskSegm, int IleSegm,
    const int* wskNumerow, int Ile)
//wyznacza srodek masy dla elementow w tablicy
// 'wskSegm' danych numerami w 'wskNumerow'
{
  Wektor3D Suma(0, 0, 0);

  if (IleSegm <= 0 || Ile <= 0 || Ile > IleSegm) return Suma / 0.0;

  double odwrIle = 1.0 / Ile;

  for (int i = 0; i < Ile; i++)
  {
    Suma += wskSegm[wskNumerow[i]];
  };
  return Suma * odwrIle;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Macierz3D WyznaczMacierzBezwl(const Wektor3D* wskTablicy1, int Ile, int Punkt1,
    int Punkt2)
{
  double zer9[] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; //pusta macierz

  Macierz3D MacierzBezwl(zer9);

  Wektor3D* wskPomoc;

  if (!Ile) return MacierzBezwl;

  if (Ile < 0 || Punkt1 < 0 || Punkt2 < 0 || Punkt1 > Ile || Punkt2 > Ile)
    return MacierzBezwl / 0.0;

  double x, y, z; //pomocnicze wspolrzedne punktu

  double xyz[3]; //-------------CZY TO MUSI BYC??? (patrz do srodka petli for)

  int Roznica = Punkt2 - Punkt1;

  //macierz bezwladnosci dla calej tablicy wektorow
  if (!Roznica)
  {
    wskPomoc = const_cast<Wektor3D*>(wskTablicy1);
    for (int p = 0; p < Ile; p++) //dla kazdego punktu
    {
      x = wskPomoc->x;    //znajdujemy jego x,y,z
      y = wskPomoc->y;
      z = wskPomoc->z;
      wskPomoc++;
      xyz[0] = x;
      xyz[1] = y;
      xyz[2] = z;

      //zwiekszamy albo zmniejszamy skladowe macierzy;
      // ale tylko prawy gorny rog i przekatna (macierz symetryczna)
      for (int i = 0; i < 3; i++)
      {
        for (int j = i; j < 3; j++)
        {
          //czynimy to zgodnie z maksymalnie uproszczona definicja
          // (u Skalmierskiego, str 193)
          MacierzBezwl.at(i, j) += (x * x + y * y + z * z) * delta(i, j)
              - xyz[i] * xyz[j];
        };
      };

      MacierzBezwl.at(1, 0) = MacierzBezwl.at(0, 1);
      MacierzBezwl.at(2, 0) = MacierzBezwl.at(0, 2);
      MacierzBezwl.at(2, 1) = MacierzBezwl.at(1, 2);
    };
    return MacierzBezwl;
  };

  wskPomoc = const_cast<Wektor3D*>(&wskTablicy1[Punkt1]);
  for (int p = Punkt1; p != Punkt2; Roznica > 0 ? p++ : p--)
  {

    x = wskPomoc->x;    //znajdujemy jego x,y,z
    y = wskPomoc->y;
    z = wskPomoc->z;
    Roznica > 0 ? wskPomoc++ : wskPomoc--;
    xyz[0] = x;
    xyz[1] = y;
    xyz[2] = z;

    //zwiekszamy albo zmniejszamy skladowe macierzy;
    // ale tylko prawy gorny rog i przekatna (macierz symetryczna)
    for (int i = 0; i < 3; i++)
    {
      for (int j = i; j < 3; j++)
      {
        //czynimy to zgodnie z maksymalnie uproszczona definicja
        // (u Skalmierskiego, str 193)
        MacierzBezwl.at(i, j) += (x * x + y * y + z * z) * delta(i, j)
            - xyz[i] * xyz[j];
      };
    };

    MacierzBezwl.at(1, 0) = MacierzBezwl.at(0, 1);
    MacierzBezwl.at(2, 0) = MacierzBezwl.at(0, 2);
    MacierzBezwl.at(2, 1) = MacierzBezwl.at(1, 2);
  };
  return MacierzBezwl;
}
//------------------------------------------------------------------------------
Macierz3D WyznaczMacierzBezwl(const Wektor3D* wskTablicy1, int IleSegm,
    const int* wskNumerow, int Ile)
//wyznacza macierz bezwladnosci dla elementow
//w tablicy 'wskSegm' o numerach w 'wskNumerow'
{
  double zer9[] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; //pusta macierz

  Macierz3D MacierzBezwl(zer9);

  if (!(Ile && IleSegm)) return MacierzBezwl;

  double x, y, z; //pomocnicze wspolrzedne punktu

  double xyz[3]; //-------------CZY TO MUSI BYC??? (patrz do srodka petli for)

  if (IleSegm < 0 || Ile < 0 || Ile > IleSegm) return MacierzBezwl / 0.0;

  for (int p = 0; p < Ile; p++)
  {
    x = wskTablicy1[wskNumerow[p]].x;    //znajdujemy jego x,y,z
    y = wskTablicy1[wskNumerow[p]].y;
    z = wskTablicy1[wskNumerow[p]].z;
    xyz[0] = x;
    xyz[1] = y;
    xyz[2] = z;

    //zwiekszamy albo zmniejszamy skladowe macierzy;
    // ale tylko prawy gorny rog i przekatna (macierz symetryczna)
    for (int i = 0; i < 3; i++)
    {
      for (int j = i; j < 3; j++)
      {
        //czynimy to zgodnie z maksymalnie uproszczona definicja
        // (u Skalmierskiego, str 193)
        MacierzBezwl.at(i, j) += (x * x + y * y + z * z) * delta(i, j)
            - xyz[i] * xyz[j];
      };
    };

    MacierzBezwl.at(1, 0) = MacierzBezwl.at(0, 1);
    MacierzBezwl.at(2, 0) = MacierzBezwl.at(0, 2);
    MacierzBezwl.at(2, 1) = MacierzBezwl.at(1, 2);
  };
  return MacierzBezwl;
}
//------------------------------------------------------------------------------
Wektor3D WyznaczSrodekKrzywizny(const Wektor3D* wskSegmenty)
//dostaje wskaznik do srodkoewgo punktu trojki
//niezbednej do wyznaczenia srodka
//krzywizny, zakladajac, ze sa na okregu
//***********UWAGA!!!!***********************
//funkcja nie sprawdza (bo niby jak?), czy
//'wskSegmenty-1' oraz 'wskSegmenty+1'
//maj� sensowne wartosci!!!!!
{
  Wektor3D A = wskSegmenty[0] - wskSegmenty[-1], B = wskSegmenty[1]
      - wskSegmenty[0], Wektor = A.crossProduct(B); //chwilowo iloczyn wektorowy

  double ax = A.x, ay = A.y, az = A.z, //dla szybkosci
      bx = B.x, by = B.y, bz = B.z, f;

  if (Wektor.length() < 1E-14) //jesli jest dobra rownoleglosc
                               //-------PRZEMYSLEC SPRAWE :/
                               //-------czy to dobre rozwiazanie jest???
  {
    if (!ax && !ay && !az) return Wektor / 0.0;    //ten sam punkt!
    if (!ax) return Wektor3D(1E+12, 0.0, 0.0);
    if (!ay) return Wektor3D(0.0, 1E+12, 0.0);
    if (!az) return Wektor3D(0.0, 0.0, 1E+12);

    Wektor.x = (ay + az) / ax;
    Wektor.y = 1;
    Wektor.z = 1;
    Wektor.normalize();

    return Wektor * (1E+12); //zwracamy wektor prostopadly mocno odlegly
  };

  //to co nastepuje mam gdzies w archiwach na dysku w pliku "srodek krzywizny.nb"
  //zadziwiajaco krotkie, jak to uzyskalem?.. :/ (to jest dobre! :D)

  f = (bx * (ax + bx) + by * (ay + by) + bz * (az + bz))
      / (az * az * (bx * bx + by * by) + ay * ay * (bx * bx + bz * bz)
          + ax * ax * (by * by + bz * bz) - 2 * ay * by * (ax * bx + az * bz)
          - 2 * ax * az * bx * bz);

  Wektor.x = wskSegmenty[-1].x
      + 0.5 * (ax + f * ((ay * ay + az * az) * bx - (ay * by + az * bz) * ax));
  Wektor.y = wskSegmenty[-1].y
      + 0.5 * (ay + f * ((ax * ax + az * az) * by - (ax * bx + az * bz) * ay));
  Wektor.z = wskSegmenty[-1].z
      + 0.5 * (az + f * ((ax * ax + ay * ay) * bz - (ax * bx + ay * by) * az));

  return Wektor;
}
//------------------------------------------------------------------------------
double WyznaczSredniaOdlegloscSegm(const Wektor3D* wskTablicy1, int Ile,
    int Segment1, int Segment2)
//nie uwzglednia ostatniego
{
  Wektor3D A;
  Wektor3D* wskPomoc;

  double Dlugosc = 0;

  if (Ile <= 0 || Segment1 < 0 || Segment2 < 0 || Segment1 > Ile
      || Segment2 > Ile)
  {
    return 0.0;   //UWAGA!!! co tutaj?
  };

  int Roznica = Segment2 - Segment1;

  double odwrRozn;

  if (!Roznica)
  {
    wskPomoc = const_cast<Wektor3D*>(wskTablicy1);
    for (int i = 0; i < Ile - 1; i++)
    {
      A = *wskPomoc++ - *wskPomoc;
      Dlugosc += A.length();
      //wskPomoc++;
    };
    A = *wskPomoc - wskTablicy1[0];
    Dlugosc += A.length();
    return Dlugosc / Ile;
  };

  wskPomoc = const_cast<Wektor3D*>(&wskTablicy1[Segment1]);
  for (int i = Segment1; i != Segment2; Roznica > 0 ? i++ : i--)
  {
    A = *(Roznica > 0 ? wskPomoc++ : wskPomoc--) - *wskPomoc; //UWAGA!!!!!!!!!!! czy tak mozna?
    Dlugosc += A.length();
  };

  return Dlugosc / Roznica > 0 ? Roznica : -Roznica;
}
//------------------------------------------------------------------------------
Wektor3D WyznaczMomentor(const Wektor3D* const wsk1, //wyznacza momentor wzglpomiedzy
    const Wektor3D* const wsk2,         //dwiema tablicami wektorow;
    int IleLiczyc,                      //sprawdza 'IleLiczyc' wektorow;
    const Wektor3D Srodek)   //nie kontroluje zakresu tablic!
{
  Wektor3D Momentor, Pomoc1, Pomoc2;

  Wektor3D* wskPomoc1 = const_cast<Wektor3D*>(wsk1);
  Wektor3D* wskPomoc2 = const_cast<Wektor3D*>(wsk2);

  if (!IleLiczyc) return Momentor;

  if (Srodek == Wektor3D())            //dzialamy troszke krocej
  {
    for (int i = 0; i != IleLiczyc; IleLiczyc > 0 ? i++ : i--)
    {
      Pomoc1 = *wskPomoc1 + *wskPomoc2;
      Pomoc1 *= 0.5;
      Pomoc2 = *wskPomoc2 - *wskPomoc1;
      IleLiczyc > 0 ? wskPomoc1++ : wskPomoc1--;
      IleLiczyc > 0 ? wskPomoc2++ : wskPomoc2--;
      Momentor += Pomoc1.crossProduct(Pomoc2);
    };
    return Momentor;
  };

  for (int i = 0; i != IleLiczyc; IleLiczyc > 0 ? i++ : i--)
  {
    Pomoc1 = *wskPomoc1 + *wskPomoc2;
    Pomoc1 *= 0.5;
    Pomoc1 -= Srodek;
    Pomoc2 = *wskPomoc2 - *wskPomoc1;
    IleLiczyc > 0 ? wskPomoc1++ : wskPomoc1--;
    IleLiczyc > 0 ? wskPomoc2++ : wskPomoc2--;
    Momentor += Pomoc1.crossProduct(Pomoc2);
  };
  return Momentor;
}
//------------------------------------------------------------------------------
double WyznaczMaxKatGieciaPlus(Wektor3D P1,          //po sprowadzeniu do 
    Wektor3D P2,          //odpowiedniego ukladu:
    double KwadratMinOdl) //"maxkatgiecia.nb"
{
  double odlz2 = P1.z - P2.z;

  odlz2 *= odlz2;

  if (odlz2 >= KwadratMinOdl) return 2 * M_PI;

  double p1 = P1.x * P1.x + P1.y * P1.y;
  double p2 = P2.x * P2.x + P2.y * P2.y;

  double StalyZnak = (P1.x * P2.x + P1.y * P2.y)
      * (odlz2 + p1 + p2 - KwadratMinOdl);
  double ZmiennyZnak = P2.x * P1.y - P1.x * P2.y;
  ZmiennyZnak *= sqrtl(
      -ZmiennyZnak
          * (odlz2 * odlz2 + p1 * p1
              + (p2 - KwadratMinOdl) * (p2 - KwadratMinOdl)
              + 2 * odlz2 * (p1 + p2 - KwadratMinOdl)
              - 2 * p1 * (p2 + KwadratMinOdl)));
  return acosl((StalyZnak + ZmiennyZnak) * powl(p1 * p2, -1));
}
;
//--------------------------------------------------------------------------------
double CloseRepulsiveKnotEnergy(Knot3D* wskKnot)
{
  double Energia = 0.0;

  int IleLiczyc = 100, IleSkakac = wskKnot->IloscSegm / IleLiczyc, i = 0;

  Wektor3D Roznica;

  double odlkwadrat;

  Wektor3D* wskPomoc1 = wskKnot->wskTablicySegm;
  Wektor3D* wskPomoc2;

  for (; i < wskKnot->IloscSegm - IleSkakac; i += IleSkakac)
  {
    wskPomoc2 = wskPomoc1 + IleSkakac;

    for (int j = i + IleSkakac; j < wskKnot->IloscSegm; j += IleSkakac)
    {
      Roznica = *wskPomoc1 - *wskPomoc2;
      odlkwadrat = Roznica.length() - 2 * wskKnot->PromienPrzekroju;
      odlkwadrat *= odlkwadrat;
      odlkwadrat *= odlkwadrat;
      Energia += 1.0 / odlkwadrat;
      wskPomoc2 += IleSkakac;
    };

    wskPomoc1 += IleSkakac;
  };

  Roznica = *wskPomoc1
      - wskKnot->wskTablicySegm[nrP(wskKnot->IloscSegm, i + IleSkakac)];
  Energia += 1.0 / Roznica.length();

  return Energia;
}
//--------------------------------------------------------------------------------
double CloseAttractiveKnotEnergy(Knot3D* wskKnot)
{
  double Energia = 0.0;

  int IleLiczyc = 100, IleSkakac = wskKnot->IloscSegm / IleLiczyc, i = 0;

  Wektor3D Roznica;

  double odlkwadrat;

  Wektor3D* wskPomoc1 = wskKnot->wskTablicySegm;
  Wektor3D* wskPomoc2;

  for (; i < wskKnot->IloscSegm - IleSkakac; i += IleSkakac)
  {
    wskPomoc2 = wskPomoc1 + IleSkakac;

    for (int j = i + IleSkakac; j < wskKnot->IloscSegm; j += IleSkakac)
    {
      Roznica = *wskPomoc1 - *wskPomoc2;
      odlkwadrat = Roznica.length() - 2 * wskKnot->PromienPrzekroju;
      odlkwadrat *= odlkwadrat;
      odlkwadrat *= odlkwadrat;
      Energia -= 1.0 / odlkwadrat;
      wskPomoc2 += IleSkakac;
    };

    wskPomoc1 += IleSkakac;
  };

  Roznica = *wskPomoc1
      - wskKnot->wskTablicySegm[nrP(wskKnot->IloscSegm, i + IleSkakac)];
  Energia -= 1.0 / Roznica.length();

  return Energia;
}
//--------------------------------------------------------------------------------
double DistantRepulsiveKnotEnergy(Knot3D* wskKnot)
{
  double Energia = 0.0;

  int i = 0, Ilosc = wskKnot->IloscSegm;

  Wektor3D Roznica;

  double odlkwadrat;

  for (; i < Ilosc; i++)
  {
    Roznica = wskKnot->wskTablicySegm[i]
        - wskKnot->wskTablicySegm[nrP(i + Ilosc / 2, Ilosc)];
    odlkwadrat = Roznica.length() - 2 * wskKnot->PromienPrzekroju;
    odlkwadrat = sqrtl(odlkwadrat);
    Energia += 1.0 / odlkwadrat;
  }

  return Energia;
}
//--------------------------------------------------------------------------------
double DistantAttractiveKnotEnergy(Knot3D* wskKnot)
{
  double Energia = 0.0;

  int i = 0, Ilosc = wskKnot->IloscSegm;

  Wektor3D Roznica;

  double odlkwadrat;

  for (; i < Ilosc; i++)
  {
    Roznica = wskKnot->wskTablicySegm[i]
        - wskKnot->wskTablicySegm[nrP(i + Ilosc / 2, Ilosc)];
    odlkwadrat = Roznica.length() - 2 * wskKnot->PromienPrzekroju;
    odlkwadrat = sqrtl(odlkwadrat);
    Energia -= 1.0 / odlkwadrat;
  }

  return Energia;
}
//--------------------------------------------------------------------------------

/*bool CzyLancuchyRowne(const char* Gdzie,const char* Co)
 {
 //bool Zwrot=true;
 //for(int i=0; Gdzie[i]!=&&
 //            Co[i]!='\n'&&
 //	      Gdzie[i]!=' '&&
 //            Co[i]!=' '; i++) {    Zwrot=((Gdzie[i]==Co[i])?true:false);    };
 return strcmp(Gdzie,Co);//Zwrot;
 }
 //--------------------------------------------------------------------------------
 char* PrzepiszLancuch(char* Gdzie,char* Co)
 {
 for(int i=0; Co[i]!='\n'&&
 Co[i]!=' '; i++) {    Gdzie[i]=Co[i];	};
 return Gdzie;
 }
 */
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/************ UMIESZCZONO W DEFINICJACH KLAS W "vmath.h"************************
 friend std::istream& operator >>(std::istream& lhs, Matrix3<T>& rhs)
 {
 char znak;
 for (int i = 0; i < 3; i++)
 {
 lhs >> znak;
 for (int j = 0; j < 3; j++)
 {
 lhs >> rhs.at(j,i);
 }
 lhs >> znak;
 }
 return lhs;
 }
 //------------------------------------------------------------------------------
 friend std::istream& operator >>(std::istream& lhs, Matrix4<T>& rhs)
 {
 char znak;
 for (int i = 0; i < 4; i++)
 {
 lhs >> znak;
 for (int j = 0; j < 4; j++)
 {
 lhs >> rhs.at(j,i);
 }
 lhs >> znak;
 }
 return lhs;
 }
 //------------------------------------------------------------------------------
 friend std::istream& operator>>(std::istream& lhs, Vector3<T>& rhs)
 {
 char znak;
 lhs >> znak >> rhs.x >> znak >> rhs.y >> znak >> rhs.z  >> znak;
 return lhs;
 };
 //------------------------------------------------------------------------------
 friend std::istream& operator>>(std::istream& lhs, Vector4<T>& rhs)
 {
 char znak;
 lhs >> znak >> rhs.x >> znak >> rhs.y >> znak >> rhs.z  >> rhs.w >>ak;
 return lhs;
 };

 //------------------------------------------------------------------------------
 //
 // returns determinant of matrix
 //
 T det()
 {
 return (at(0,1)*at(1,2)-at(0,2)*at(1,1))*at(2,0)+
 (at(0,2)*at(1,0)-at(0,0)*at(1,2))*at(2,1)+
 (at(0,0)*at(1,1)-at(0,1)*at(1,0))*at(2,2);
 };


 //
 // returns inverse matrix if determinant is not 0
 // and if it is returns with very big matrix :/
 //
 Matrix3<T> inverse()
 {
 Matrix3<T> ret;
 T determinant=det();
 if(determinant==0.0) return ret*1E+4920;
 ret.at(0,0)=(at(1,1)*at(2,2)-at(1,2)*at(2,1))/determinant;
 ret.at(0,1)=(at(0,2)*at(2,1)-at(0,1)*at(2,2))/determinant;
 ret.at(0,2)=(at(0,1)*at(1,2)-at(0,2)*at(1,1))/determinant;
 ret.at(1,0)=(at(1,2)*at(2,0)-at(1,0)*at(2,0))/determinant;
 ret.at(1,1)=(at(0,0)*at(2,2)-at(0,2)*at(2,0))/determinant;
 ret.at(1,2)=(at(0,2)*at(1,0)-at(0,0)*at(1,2))/determinant;
 ret.at(2,0)=(at(1,0)*at(2,1)-at(1,1)*at(2,0))/determinant;
 ret.at(2,1)=(at(0,1)*at(2,0)-at(0,0)*at(2,1))/determinant;
 ret.at(2,2)=(at(0,0)*at(1,1)-at(0,1)*at(1,0))/determinant;
 return ret;
 }

 //
 // returns transformation matrix to system with OX-wersor parallel to 'W'
 //
 static Matrix3<T> parallelOX(Vector3<T> W)
 {
 Matrix3<T> ret;
 //niezbedne wektory
 Vector3<T> w2,                   //drugi wiersz macierzy transformacji
 w3,                   //trzeci wiersz macierzy transformacji
 A, Pomocnik;          //pomocniki

 W.normalize();
 w2=W;
 A=w2;

 for(int i=1, rownolegle=1; rownolegle; i++)
 {
 A.rotate(i*0.1234543534534534,2*i*0.15678435654656,3*i*0.098762346546546);
 Pomocnik=W.crossProduct(A);
 rownolegle=Pomocnik.length()<1E-14?true:false;
 };

 w2=w2.crossProduct(A);
 w2.normalize();

 w3=W.crossProduct(w2);
 w3.normalize();

 double tab[]={W.x,w2.x,w3.x,W.y,w2.y,w3.y,W.z,w2.z,w3.z};
 ret=Matrix3<T>(tab);

 return ret;
 };


 //
 // returns transformation matrix to system with OY-wersor parallel to 'W'
 //
 static Matrix3<T> parallelOY(Vector3<T> W)
 {
 Matrix3<T> ret;
 //niezbedne wektory
 Vector3<T> w1,                   //pierwszy wiersz macierzy transformacji
 w3,                   //trzeci wiersz macierzy transformacji
 A, Pomocnik;          //pomocniki

 W.normalize();
 w3=W;
 A=w3;

 for(int i=1, rownolegle=1; rownolegle; i++)
 {
 A.rotate(i*0.1234543534534534,2*i*0.15678435654656,3*i*0.098762346546546);
 Pomocnik=W.crossProduct(A);
 rownolegle=Pomocnik.length()<1E-14?true:false;
 };

 w3=w3.crossProduct(A);
 w3.normalize();

 w1=W.crossProduct(w2);
 w1.normalize();

 double tab[]={w1.x,W.x,w3.x,w1.y,W.y,w3.y,w1.z,W.z,w3.z};
 ret=Matrix3<T>(tab);

 return ret;
 };

 //
 // returns transformation matrix to system with OZ-wersor parallel to 'W'
 //
 static Matrix3<T> parallelOZ(Vector3<T> W)
 {
 Matrix3<T> ret;
 //niezbedne wektory
 Vector3<T> w1,                   //pierwszy wiersz macierzy transformacji
 w2,                   //drugi wiersz macierzy transformacji
 A, Pomocnik;          //pomocniki

 W.normalize();
 w1=W;
 A=w1;

 for(int i=1, rownolegle=1; rownolegle; i++)
 {
 A.rotate(i*0.1234543534534534,2*i*0.15678435654656,3*i*0.098762346546546);
 Pomocnik=W.crossProduct(A);
 rownolegle=Pomocnik.length()<1E-14?true:false;
 };

 w1=w1.crossProduct(A);
 w1.normalize();

 w2=W.crossProduct(w1);
 w2.normalize();

 double tab[]={w1.x,w2.x,W.x,w1.y,w2.y,W.y,w1.z,w2.z,W.z};
 ret=Matrix3<T>(tab);

 return ret;
 };

 //
 // fast type 'T' operators:
 //
 Matrix3<T>& operator+=(Matrix3<T> rhs)
 {
 at(0,0)+=rhs.at(0,0);
 at(0,1)+=rhs.at(0,1);
 at(0,2)+=rhs.at(0,2);
 at(1,0)+=rhs.at(1,0);
 at(1,1)+=rhs.at(1,1);
 at(1,2)+=rhs.at(1,2);
 at(2,0)+=rhs.at(2,0);
 at(2,1)+=rhs.at(2,1);
 at(2,2)+=rhs.at(2,2);
 return * this;
 }


 Matrix3<T>& operator-=(Matrix3<T> rhs)
 {
 at(0,0)-=rhs.at(0,0);
 at(0,1)-=rhs.at(0,1);
 at(0,2)-=rhs.at(0,2);
 at(1,0)-=rhs.at(1,0);
 at(1,1)-=rhs.at(1,1);
 at(1,2)-=rhs.at(1,2);
 at(2,0)-=rhs.at(2,0);
 at(2,1)-=rhs.at(2,1);
 at(2,2)-=rhs.at(2,2);
 return * this;
 }


 Matrix3<T>& operator*=(T rhs)
 {
 at(0,0)*=rhs;
 at(0,1)*=rhs;
 at(0,2)*=rhs;
 at(1,0)*=rhs;
 at(1,1)*=rhs;
 at(1,2)*=rhs;
 at(2,0)*=rhs;
 at(2,1)*=rhs;
 at(2,2)*=rhs;
 return * this;
 }


 Matrix3<T>& operator/=(T rhs)
 {
 at(0,0)/=rhs;
 at(0,1)/=rhs;
 at(0,2)/=rhs;
 at(1,0)/=rhs;
 at(1,1)/=rhs;
 at(1,2)/=rhs;
 at(2,0)/=rhs;
 at(2,1)/=rhs;
 at(2,2)/=rhs;
 return * this;
 }

 */

#endif
