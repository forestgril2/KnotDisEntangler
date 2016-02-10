#ifndef SONO_H
#define SONO_H

#include <math.h>
// ==========================================================================
class WK3D
{
protected:
  double x, y, z;
public:
  WK3D(void) :
      x(0.), y(0.), z(0.)
  {
  }
  WK3D(double px, double py, double pz) :
      x(px), y(py), z(pz)
  {
  }
  WK3D(const WK3D &P) :
      x(P.x), y(P.y), z(P.z)
  {
  }

  friend inline WK3D operator+(WK3D &A, WK3D &B)
  {
    return WK3D(A.x + B.x, A.y + B.y, A.z + B.z);
  }
  friend inline WK3D operator-(WK3D &A, WK3D &B)
  {
    return WK3D(A.x - B.x, A.y - B.y, A.z - B.z);
  }
  friend inline WK3D operator*(WK3D &A, double b)
  {
    return WK3D(A.x * b, A.y * b, A.z * b);
  }
  friend inline WK3D operator*(double a, WK3D &B)
  {
    return WK3D(a * B.x, a * B.y, a * B.z);
  }
  friend inline double modul(WK3D &A)
  {
    return sqrt(A.x * A.x + A.y * A.y + A.z * A.z);
  }
  friend inline WK3D wersor(WK3D &A)
  {
    double m = modul(A);
    return WK3D(A * (1.0 / m));
  }

  WK3D& operator+=(const WK3D& A)
  {
    x += A.x;
    y += A.y;
    z += A.z;
    return *this;
  }
  WK3D& operator-=(const WK3D& A)
  {
    x -= A.x;
    y -= A.y;
    z -= A.z;
    return *this;
  }
  WK3D& operator=(const WK3D& A)
  {
    x = A.x;
    y = A.y;
    z = A.z;
    return *this;
  }
  WK3D& operator*=(double a)
  {
    x *= a;
    y *= a;
    z *= a;
    return *this;
  }
  WK3D operator+()
  {
    return WK3D(*this);
  }
  WK3D operator-()
  {
    return WK3D(-x, -y, -z);
  }
};

class CORECT
{
public:
  CORECT(void);
  ~CORECT();
  void Wprowadz(WK3D *W, int ip, double = 0.03);
  void Zaciskaj(void);
  double DlugoscW(void)
  {
    return DlugoscWezla;
  }
  void Szybkosc(double TT)
  {
    DlugoscTrwania = TT;
  }

private:
  int Si, *TablicaNumerowElementowA, *TablicaNumerowElementowB, IloscSegm;

  double DlugoscWezla, SredniaDlugosc, Sb, Ot, Pp, DlugoscTrwania,
      ParametrZaciskania;

  WK3D *TablicaElementow, *Wezel;

  int RezSt(double Nd, int **ta, int **tb);
  void Odswiez(double);
  void Sprawdz(void);
  void FStykPR(void);
  void FSegmentPR(void);
};

#endif
