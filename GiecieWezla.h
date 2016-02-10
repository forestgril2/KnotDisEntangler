#ifndef GIECIEWEZLA_H
#define GIECIEWEZLA_H

#include <iostream>
using namespace std;

//------------------------------------------------------------------------------
struct GiecieWezla
{
  bool CzyUdane;
  double Kat;
  int Segment1, odlSegment12;

  GiecieWezla()
  {
  }

  GiecieWezla(int P1, int P2, double Kat1, bool Udane = true) :
      CzyUdane(Udane), Kat(Kat1), Segment1(P1), odlSegment12(P2)
  {
  }

  friend ostream& operator<<(ostream& str, GiecieWezla& Giecie);
  friend istream& operator>>(istream& str, GiecieWezla& Giecie);
};

#endif
