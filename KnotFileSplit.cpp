#include <cstdlib>
#include <iostream>
#include <fstream>
#include <deque>
#include <set>
#include <vector>
#include <math.h>

using namespace std;

#include "SONO.cpp"
#include "GiecieWezla.cpp"
#include "funkcjezew.cpp"
#include "Losowania.cpp"
#include "Knot3D.cpp"
#include "KnotPliker.cpp"
#include "KnotSimulator.cpp"
#include "KnotSimData.cpp"
#include "Histogram.cpp"

KnotPliker PlikerWynikow;

int defaultCoIle=5;
long int defaultPoczatek=0;
long int defaultKoniec=-1;
int CoIle;
long int Poczatek=0,Koniec=-1;

ifstream Zrodlowy;
ifstream* wskstrPlikuZrodl=&Zrodlowy;

ifstream Wynikowy;
ifstream* wskstrPlikuWynik=&Wynikowy;

char wsknazwywczyt[64]={"wynik.knb"};
char wsknazwyzapis[64]={"wyniksplit.knb"};

Knot3D W,Poczatkowy;

int main(int argc, char *argv[])
{
 cout<<endl;

 char Pomocnik[64]="";
 int nrParametruFunkcjiMain=1;
 int nrPrzekazanegoLancucha=0;
 double ParametrDouble=0;

 if (argc!=6) {
    cout<<"Niepoprawne parametry!"<<endl;
    cout<<"Powinno byc: <plik wczytywany> <plik do zapisu> <co ile wezlow> <nr poczatek> <nr koniec>"<<endl;
    cout<<"Stosuje domyslne parametry..."<<endl<<endl;
    CoIle=defaultCoIle;
    }
 else {
    strcpy(wsknazwywczyt,argv[1]);

    strcpy(wsknazwyzapis,argv[2]);

    strcpy(Pomocnik,argv[3]);
		CoIle=atoi(Pomocnik);

    strcpy(Pomocnik,argv[4]);
		Poczatek=atoi(Pomocnik);

    strcpy(Pomocnik,argv[5]);
		Koniec=atoi(Pomocnik);

    if(CoIle<=0)
      {
       cout<<"Wartosc 'CoIle' nieprawidlowa: \t"<<CoIle<<endl;

       cout<<"\t(Zapisze domyslnie  co"<<defaultCoIle<<" wezel"<<CoIle<<endl;
      };

    if(Poczatek<0)
      {
       Poczatek=defaultPoczatek;
       cout<<"Numer poczatkowego wezla nieprawidlowy: \t"<<CoIle<<endl;
       cout<<"\t(Wczytam domyslnie od nr:"<<Poczatek<<" wezel"<<CoIle<<endl;
      };

    if(Koniec<=0)
      {
       Koniec=defaultKoniec;
       cout<<"Numer koncowego wezla nieprawidlowy: \t"<<CoIle<<endl;
       cout<<"\t(Wczytam domyslnie do nr:"<<Koniec<<" wezel"<<CoIle<<endl;
      };
    };

 cout<<"Nazwa pliku-zrodla: \t\t\t"<<wsknazwywczyt<<endl;
 cout<<"Nazwa pliku zapisu: \t"<<wsknazwyzapis<<endl;
 cout<<"Co ile zapisuje: \t \t"<<CoIle<<endl;
 cout<<"Zaczynam na wezle nr: \t \t"<<Poczatek<<endl;
 cout<<"Koncze na wezle nr (ujemna wartosc oznacza ostatni wezel): \t \t"<<Koniec<<endl;
 cout<<endl;

 PlikerWynikow.WczytajPlik(wsknazwywczyt);

 cout<<"Plik wczytany: \t \t \t"<<wsknazwywczyt<<endl<<endl;
 cout<<"Ilosc wezlow w pliku \t \t \t"<<PlikerWynikow.IloscWezlow<<endl;
 cout<<endl;

 PlikerWynikow.ZapiszCoIle(wsknazwyzapis,CoIle,Poczatek,Koniec);

 cout<<"Zakonczono z powodzeniem."<<endl<<endl;

 return EXIT_SUCCESS;
}
