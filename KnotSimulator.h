#ifndef KNOTSIMULATOR_H
#define KNOTSIMULATOR_H

#include <iostream>

#ifndef WEZEL3D_H
#include "Knot3D.h"
#endif

#ifndef WEZELPLIKER_H
#include "KnotPliker.h"
#endif

#ifndef KNOTSIMDATA_H
#include "KnotSimData.h"
#endif

enum MetropolisModes {off,DistRep,DistAttr,CloseRep,CloseAttr};

enum MiejsceNaLiscie 
{
pierwszy,
wybrany,
ostatni
};

class KnotSimulator
{
 public:
 //--------------FUNCTIONS-------------------------
 KnotSimulator(char*,char*,char*,char*,char*);
 ~KnotSimulator(){wskstrPlikuBin->close();}

 void ZastosujSONO();
 void ZastosujBONO(double (*wskFunZmiennaMetropolisa)(Knot3D*)=0,double (*wskFunRozkladZmiennejMetropolisa)(double,double,double)=0);
 void StworzHistogramy();
 void WczytajPlik(char* nazwapliku);
 void ZamknijPlik();
 void WczytajKnot(MiejsceNaLiscie=ostatni,long int=0);
 void ZapiszKnot();
 void ZastosujAlgorytmy(long int ileBONO=0, long int ileSONO=0);
 void InfoKonsolowe();
 void UstawCoIleUdanychGiecZapis(long int coile);
 void UstawCoIleSONOZapis(long int coile);
 Knot3D& WskazKnotSymulowany();

 //--------------PROPERTIES------------------------

 ///private: //-------------------------------------------------------------------------
 //--------------FUNCTIONS-------------------------
 void UstawParametry();

 //--------------PROPERTIES------------------------
 //--------------glowne parametry symulatora-------------------------------------------------
 int nrmojebin, 		//nr wezla/pozycja w moim pliku binarnym
     nrmojetxt,  	        //nr wezla/pozycja w moim pliku txt
     nrtxt;			//nr wezla/pozycja w pliku txt

 //wskazniki domyslnych nazw
 char* wskNazwyDomyslna;
 char* wskNazwyWynikAuto;
 char* wskNazwyWynikMoja;
 char* wskNazwyWynikAutotxt;
 char* wskNazwyWynikMojatxt;

 //strumienie plikowe do odczytu
 ifstream 	strPlikuBin; 
 ifstream*	wskstrPlikuBin;
 ifstream 	strPlikuMojaBin;
 ifstream* 	wskstrPlikuMojaBin;

 //--------------glowne parametry symulacji---------------------------------------
 KnotPliker PlikerWynikow;

 KnotSimData* wskHistogramySymulacji;
 KnotSimData* wskprevHistogramySymulacji;

 //jaka czesc maksymalnej wartosci w przedzialach zliczania uwzzlgedniac 
 double Temperatura;
 double ParametrPunktuA;
 double ParametrOdleglosciAB;
 double OdchStandParametruZmianyWezla;

 MetropolisModes Metropolis;
 Losowania Losownik;

 int defaultIleRazySONO,		//ile wykonywac SONO miedzy wizualizacjami 
     defaultIleRazyBONO,		//ile wykonywac giec miedzy wizualizacja eliminujac przekrycia
     CoIleUdanychNaturalnychGiecHistogram;

 ///private:
 int CoIleUdanychGiecZapis,	
     CoIleSONOZapis;
 
 public:
 double StosunekEnTermDoEnPocz,  	//tutaj zawarta jest temperatura ukladu
        StosunekKataDoMax,		//dla bezpieczenstwa<1
        WagaZmiennejMetropolisaOdpych, //energiom przypisane sa wagi
        StosunekGiecNaturalnych;       //w jakim stosunku do histogramowych uzywac giec z naturalnym rozkladem

  bool 	SONO, 				//czy wlaczony algorytm
	BONO;  				//czy wlaczony algorytm

 //-------------------------zmienne-------------------------------------------------
 Knot3D Symulowany,Pomoc,Poczatkowy;

 bool PierwszyRazHistogram;

 int A,B;          			//przedzialy sprawdzane po gieciach
 
 //liczniki
 long int LicznikStanowWiekszegoPrawdopodobienstwa,	//ile razy w czasie BONO-metropolis uzyskano stan bardziej prawdopodobny
          LicznikStanowMniejszegoPrawdopodobienstwa,	//ile razy w czasie BONO-metropolis uzyskano stan mniej prawdopodobny
          LicznikNieudanychGiec,	//ile giec spowodowalo przekrycia
          LicznikUdanychGiec,		//ile bylo giec bez przekryc
          LicznikProbGiec,		//ile bylo wszystkich prob giec
          LicznikSONO,			//ile razy suma SONO
          LicznikBONO,			//ile razy w sumie gieto BONO
          LicznikUdanychNaturalnychGiec,	

	  // wartosci z poprzedniej konsoli lub zapisu
          prevLicznikUdanychNaturalnychGiec,
	  prevLicznikStanowWiekszegoPrawdopodobienstwa,	
          prevLicznikStanowMniejszegoPrawdopodobienstwa,
          prevLicznikNieudanychGiec,	
          prevLicznikUdanychGiecZapisu,
          prevLicznikUdanychGiec,		
          prevLicznikProbGiec,
          LicznikNieudanychShrink,
          LicznikUdanychShrink,		
          prevLicznikSONO,			
          prevLicznikBONO,
          prevLicznikBONOHistogramow,
          prevTime;

long int  diffTime;	

clock_t prevClock;
double CzasMiniety;
	

 double Epsilon,			//dopuszczalna odchylka
        StosunekPomocniczy,		//stosunek ZmiennejMetropolisaOdpychania do ZmiennejMetropolisaKrzywizny???
        ZmiennaMetropolisa,
        ZmiennaMetropolisaNowa,
        ZmiennaMetropolisaPocz,			//Poczatkowa energia wezla ???????????????????? kiedy?
        StosunekZmiennejMetropolisaDoPocz,		//iloraz energii do poczatkowej
        StosunekZmiennejMetropolisaDoPoczMin,	//minimalny odnotowany iloraz energii do poczatkowej
        SumaNizszychPrawdpodobienstwPrzejsc,		//ilosc stanow wynikajaca z energii
        prevSumaNizszychPrawdpodobienstwPrzejsc,
        DlugoscWezlaPoczatkowa,
        DlugoscWezla,
        PromienMaksObiektu,		//promien wezla od srodka do brzegu uwzgledniajac promien przekroju
 	ZmianaKatrazyOdl,
        ZmianaNaSek,
        prevZmianaKatrazyOdl,
        prevZmianaNaSek;
		
 //tablice danych dla histogramow 
 GiecieWezla* wskTablicyUdanychGiec;

 //----------------------------INNE--------------------------------------------------

 //pomocnicze miejsca w pamieci dla szybkosci
 Wektor3D* wskTablicyPomocniczejWektorow;
 Wektor3D* wskPomocniczyWektora1;
 Wektor3D* wskPomocniczyWektora2;


 //TODO:
 //zmiana czestosci zapisu w trakcie symulacji
 ///--------historia giec
 //tworzenie tablicy danych z pliku
 //tworzenie histogramow
 //badanie czasu symulacji poprzez probkowanie udawalnosci giec (statystyczna calka)
};

#endif
