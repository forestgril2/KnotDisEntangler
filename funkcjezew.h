#ifndef FUNKCJEZEW_H
#define FUNKCJEZEW_H

#ifndef WEKTOR3D_H
#include "Wektor3D.h"
#endif

#ifndef KNOT3D_H
#include "Knot3D.h"
#endif

inline int nrP(int i, int N) //numer 'i' zamieniony na numer w pierscieniu o
// 'N' elementach ("numer w Pierscieniu")
{
  return i < 0 ? (N + (-i < N ? i : i % N)) : (i < N ? i : i % N);
}
;
//------------------------------------------------------------------------------
inline int odlP(int A, int B, int N) //odleglosc elementow 'A' i 'B' w 
//piercieniu o 'N' elementach
{
  A = nrP(A, N);
  B = nrP(B, N);
  return A <= B ? B - A : B + N - A;
}
;
//------------------------------------------------------------------------------
inline bool delta(int i, int j) //delta Kroneckera
{
  return i == j ? 1 : 0;
}
;
//------------------------------------------------------------------------------
double WyznaczPromienMax(const Wektor3D& Punkt,             //funkcja nie dba
    const Wektor3D* const wskTablicySegm, //czy nie dziala
    int IloscSegm);                        //poza tablica!

//------------------------------------------------------------------------------
double WyznaczPromienMin(const Wektor3D& Punkt,             //funkcja nie dba
    const Wektor3D* const wskTablicySegm, //czy nie dziala
    int IloscSegm);                        //poza tablica!

//------------------------------------------------------------------------------
Wektor3D WyznaczSrodekMasy(const Wektor3D* wskTablicy1, int Ile, int Punkt1,
    int Punkt2);
//wyznaczaja srodek masy wybranego
//przedzialu tablicy elem;
//nie uwzglednia drugiego punktu;
//-----zmienic na dowolne elem----
//-----zszablonowac------------

//------------------------------------------------------------------------------
Wektor3D WyznaczSrodekMasy(const Wektor3D* wskSegm, int IleSegm,
    const int* wskNumerow, int Ile);
//wyznacza srodek masy dla elementow w tablicy
// 'wskSegm' danych numerami w 'wskNumerow'

//------------------------------------------------------------------------------
Macierz3D WyznaczMacierzBezwl(const Wektor3D* wskTablicy1, int Ile, int Punkt1,
    int Punkt2);

//------------------------------------------------------------------------------
Macierz3D WyznaczMacierzBezwl(const Wektor3D* wskTablicy1, int IleSegm,
    const int* wskNumerow, int Ile);
//wyznacza macierz bezwladnosci dla elementow
//w tablicy 'wskSegm' o numerach w 'wskNumerow'

//------------------------------------------------------------------------------
Wektor3D WyznaczSrodekKrzywizny(const Wektor3D* wskSegmenty);
//dostaje wskaznik do srodkoewgo punktu trojki
//niezbednej do wyznaczenia srodka
//krzywizny, zakladajac, ze sa na okregu
//***********UWAGA!!!!***********************
//funkcja nie sprawdza (bo niby jak?), czy
//'wskSegmenty-1' oraz 'wskSegmenty+1'
//maj� sensowne wartosci!!!!!

//------------------------------------------------------------------------------
double WyznaczSredniaOdlegloscSegm(const Wektor3D* wskTablicy1, int Ile,
    int Segment1, int Segment2);
//nie uwzglednia ostatniego

//------------------------------------------------------------------------------
Wektor3D WyznaczMomentor(const Wektor3D* const wsk1, //wyznacza momentor wzglpomiedzy
    const Wektor3D* const wsk2,         //dwiema tablicami wektorow;
    int IleLiczyc,                      //sprawdza 'IleLiczyc' wektorow;
    const Wektor3D Srodek = Wektor3D());   //nie kontroluje zakresu tablic!

//------------------------------------------------------------------------------
double WyznaczMaxKatGieciaPlus(Wektor3D P1,          //po sprowadzeniu do 
    Wektor3D P2,          //odpowiedniego ukladu:
    double KwadratMinOdl); //"maxkatgiecia.nb"

//--------------------------------------------------------------------------------
inline double StosunekGestosciStanowBoltzmanna(double EnergiaNowa,
    double EnergiaStara, double Temperatura)
{
  return powl(2.7182818284590452, ((EnergiaStara - EnergiaNowa) / Temperatura));
}
//--------------------------------------------------------------------------------
double CoulombKnotEnergy(Knot3D *Knot);
double CloseAttractiveKnotEnergy(Knot3D* wskKnot);
double CloseRepulsiveKnotEnergy(Knot3D* wskKnot);
double DistantRepulsiveKnotEnergy(Knot3D* wskKnot);
double DistantAttractiveKnotEnergy(Knot3D* wskKnot);

//--------------------------------------------------------------------------------
template<class TYP>
TYP Srednia(TYP* wskTablicy, int Ilosc)
{
  TYP Suma;
  for (int i = Ilosc; i--;)
  {
    Suma += *(wskTablicy++);
  }
  return Suma / Ilosc;
}
//--------------------------------------------------------------------------------
template<class TYP>
TYP OdchylenieStand(TYP* wskTablicy, int Ilosc)
{
  TYP Suma, SredniaWTablicy = Srednia(wskTablicy, Ilosc), Pomocnik;

  for (int i = Ilosc; i--;)
  {
    Pomocnik = (SredniaWTablicy - *(wskTablicy++));
    Suma += Pomocnik * Pomocnik;
  }
  return Suma / Ilosc;
}
//--------------------------------------------------------------------------------
template<class TYP>
TYP OdchylenieStand(TYP* wskTablicy, int Ilosc, TYP SredniaDana)
{
  TYP Suma = 0, Pomocnik;

  for (int i = Ilosc; i--;)
  {
    Pomocnik = (SredniaDana - *(wskTablicy++));
    Suma += Pomocnik * Pomocnik;
  }
  return Suma / Ilosc;
}
//--------------------------------------------------------------------------------
template<class TYP>
TYP ZnajdzMin(TYP* wskTablicy, long int IloscDanych)
{
  if (IloscDanych == 1) return *wskTablicy;
  TYP min = *wskTablicy;
  if (IloscDanych > 0)
  {
    for (; IloscDanych--; ++wskTablicy)
      if (*wskTablicy < min) min = *wskTablicy;
  }
  else
  {
    for (; IloscDanych++; --wskTablicy)
      if (*wskTablicy < min) min = *wskTablicy;
  }
  return min;
}
//--------------------------------------------------------------------------------
template<class TYP>
TYP ZnajdzMax(TYP* wskTablicy, long int IloscDanych)
{
  if (IloscDanych == 1) return *wskTablicy;
  TYP max = *wskTablicy;
  if (IloscDanych > 0)
  {
    for (; IloscDanych--; ++wskTablicy)
      if (*wskTablicy > max) max = *wskTablicy;
  }
  else
  {
    for (; IloscDanych++; --wskTablicy)
      if (*wskTablicy > max) max = *wskTablicy;
  }
  return max;
}
//--------------------------------------------------------------------------------
/*template<class TYP>
 TYP* ZnajdzMaxMin(TYP* wskTablicy, long int IloscDanych)
 {
 TYP maxmin[2],max,min;
 if(IloscDanych=>0) for (;IloscDanych--;) max=(*(wskTablicy++)>max)?*wskTablicy:max;
 else for (;IloscDanych++;) max=(*(wskTablicy--)>max)?*wskTablicy:max;
 return maxmin;
 }*/
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
