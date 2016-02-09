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

//domyslne nazwy plikow:
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

KnotSimulator& S=SymulatorGlowny;
Knot3D& W=SymulatorGlowny.Symulowany;


int 
main(int argc, char *argv[])
{
    //strPlikuMojaTxt.open(wskNazwyWynikMojaTxt,ios_base::in);

    {//ustawienia konsolowego strumienia wyjsciowego
     cout.setf(ios::fixed);
     cout.precision(3);
    }

    //S=SymulatorGlowny;
    W=S.WskazKnotSymulowany();
    SymulatorGlowny.WczytajPlik(wskNazwyWynikAuto);
    SymulatorGlowny.WczytajKnot(ostatni);
    SymulatorGlowny.UstawParametry();
    SymulatorGlowny.InfoKonsolowe();

    W.PromienPrzekroju=0.5;
    W.WyznaczWszystko();
    S.Pomoc=W;

    S.BONO=S.BONO?0:1;

    long int prevLicznik1,prevLicznik2;

    S.CoIleUdanychGiecZapis=500000;
    prevLicznik1=S.CoIleUdanychGiecZapis;

    int Ile=8;

    double* ZmianyNaSek=new double[Ile];
    double* Stosunki=new double[Ile];

    S.prevTime=time(0);
    SymulatorGlowny.PierwszyRazHistogram=true;

    while(S.LicznikUdanychGiec<S.CoIleUdanychGiecZapis){
       S.ZastosujAlgorytmy(25000);
       if(S.LicznikUdanychGiec<S.CoIleUdanychGiecZapis){
          cout<<"LicznikGiec:\t"<<S.LicznikUdanychGiec<<" |";
          cout<<"Zmiana/sek:\t"<<S.ZmianaNaSek<<" |"<<endl<<flush;
          }
       }
    
    ZmianyNaSek[0]=S.prevZmianaNaSek;

    S.CoIleUdanychGiecZapis=500000;
    SymulatorGlowny.UstawParametry();
    double Stosunek;
    long int* pointer;

    //SymulatorGlowny.wskHistogramySymulacji->WczytajBin("KnotSimData1.bin");
    //SymulatorGlowny.wskHistogramySymulacji->ZapiszTxt("KnotSimData1.txt");
    //SymulatorGlowny.wskHistogramySymulacji->WczytajBin("KnotSimData2.bin");
    //SymulatorGlowny.wskHistogramySymulacji->ZapiszTxt("KnotSimData2.txt");

    SymulatorGlowny.PierwszyRazHistogram=true;


    pointer=SymulatorGlowny.wskHistogramySymulacji->wskHistPunktyA->wskTablicyPrzedzialow;
    SymulatorGlowny.wskHistogramySymulacji->wskHistPunktyA->IloscDanychPrzedzMax=
	ZnajdzMax(pointer,SymulatorGlowny.wskHistogramySymulacji->wskHistPunktyA->IloscPrzedz);

    pointer=SymulatorGlowny.wskHistogramySymulacji->wskHistOdlAB->wskTablicyPrzedzialow;
    SymulatorGlowny.wskHistogramySymulacji->wskHistOdlAB->IloscDanychPrzedzMax=
	ZnajdzMax(pointer,SymulatorGlowny.wskHistogramySymulacji->wskHistOdlAB->IloscPrzedz);

    pointer=SymulatorGlowny.wskHistogramySymulacji->wskHistKatyGiec->wskTablicyPrzedzialow;
    SymulatorGlowny.wskHistogramySymulacji->wskHistKatyGiec->IloscDanychPrzedzMax=
	ZnajdzMax(pointer,SymulatorGlowny.wskHistogramySymulacji->wskHistKatyGiec->IloscPrzedz);

    pointer=SymulatorGlowny.wskHistogramySymulacji->wskHistOdlABrazyKatyGiec->wskTablicyPrzedzialow;
    SymulatorGlowny.wskHistogramySymulacji->wskHistOdlABrazyKatyGiec->IloscDanychPrzedzMax=
	ZnajdzMax(pointer,SymulatorGlowny.wskHistogramySymulacji->wskHistOdlABrazyKatyGiec->IloscPrzedz);

    SymulatorGlowny.wskHistogramySymulacji->ZapiszBin("KnotSimData1.bin");
    

    for(int i=0;i<Ile;i++) {
 
       //Stosunek=1.03*powl(1.01,i-1)*powl(1.001,powl(i,2.5));
       Stosunek=0.1+i*0.9/(Ile-1);
       Stosunki[i]=Stosunek;
       S.ParametrOdleglosciAB=Stosunek;

       while((S.LicznikUdanychGiec)/S.CoIleUdanychGiecZapis<i){
             S.ZastosujAlgorytmy(50000);
             if ((S.LicznikUdanychGiec)/S.CoIleUdanychGiecZapis<i){
                cout<<"LicznikGiec:\t"<<S.LicznikUdanychGiec<<" |";
                cout<<"Zmiana/sek:\t"<<S.ZmianaNaSek<<" |";
                cout<<"Symulacja nr.:\t"<<i<<" |"<<endl<<flush;
                }
            }
        ZmianyNaSek[i]=S.prevZmianaNaSek;
       }

    cout<<endl<<endl;
    cout<<"Stosunki:"<<endl;
    for(int i=0;i<Ile;i++) cout<<Stosunki[i]<<"\t";
    cout<<endl<<endl;
    cout<<"Zmiany na sekunde:"<<endl;
    for(int i=0;i<Ile;i++) cout<<ZmianyNaSek[i]<<"\t";
    cout<<endl;

    return EXIT_SUCCESS;
}
