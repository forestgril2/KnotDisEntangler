#include <cstdlib>
#include <iostream>
#include <fstream>
#include <deque>
#include <set>
#include <vector>
#include <math.h>

using namespace std;

#include "GiecieWezla.cpp"
#include "funkcjezew.cpp"
#include "Losowania.cpp"
#include "Knot3D.cpp"
#include "KnotPliker.cpp"
#include "Histogram.cpp"
#include "KnotSimData.cpp"
#include "KnotSimulator.cpp"

char wskNazwyDomyslna[64]={"newgordian.txt"};
char wskNazwyWynikAuto[64]={"wynik.knb"};
char wskNazwyWynikMoja[64]={"ciekawe.knb"};
char wskNazwyWynikAutotxt[64]={"wynik.knt"};
char wskNazwyWynikMojatxt[64]={"ciekawe.knt"};

KnotSimulator SymulatorGlowny(wskNazwyDomyslna,
                              wskNazwyWynikAuto,
                              wskNazwyWynikMoja,
                              wskNazwyWynikAutotxt, 
                              wskNazwyWynikMojatxt);

int main(int argc, char *argv[]) 
{

 //SymulatorGlowny.WczytajPlik(wskNazwyWynikAuto);
 //SymulatorGlowny.WczytajKnot(ostatni);
 //SymulatorGlowny.UstawParametry();

 int Ilosc=SymulatorGlowny.Symulowany.IloscSegm;

 Wektor3D* wskWektor3D=SymulatorGlowny.Symulowany.wskTablicySegm;
 Wektor3D Roznica;

 double a=0;
 long int prevTime,b=100000000;

 prevTime=time(0);

 //for (int i=0; i<b; i++)  {Roznica=wskWektor3D[nrP(b,Ilosc)]-wskWektor3D[nrP(b+1,Ilosc)]; a=Roznica.length();};

 cout<<static_cast<int>(M_PI/asinl((0.5*106.6/584.0)/0.5))<<endl;


    system("PAUSE");
    return EXIT_SUCCESS;
}
