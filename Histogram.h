#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <iostream>
#include <fstream>

//klasa histogram przechowuje i zarzadza histogramem typu TYP
#ifndef DOMYSLNA_ILOSC_PRZEDZIALOW_HISTOGRAMU
#define DOMYSLNA_ILOSC_PRZEDZIALOW_HISTOGRAMU 100
#endif

using namespace std;

enum KierunekLiczenia {OdPoczTablicy,OdKoncaTablicy};

template<class TYP> class Histogram;

template <class TYP> ostream& operator<< (ostream & stru , Histogram<TYP> & x);

template<class TYP>
class Histogram
{
 public:
 	///funkcje
	//konstruktory
        Histogram();
	Histogram(TYP* wskNowejTablicyDanych,//wersja dynamicznie modyfikujaca poczatek i koniec
		  long int NowaIloscDanych=1,
		  int IloscPrzedzialow=0,
		  KierunekLiczenia NowyKierunek=OdPoczTablicy);

	Histogram(TYP* wskNowejTablicyDanych,	//wersja o stalych przedzialach
		  TYP poczatek,
		  TYP koniec,
		  long int NowaIloscDanych=1,
		  int IloscPrzedzialow=0,
		  KierunekLiczenia NowyKierunek=OdPoczTablicy);


		//i destruktory
		~Histogram();
		
 	void Odswiez();

	void DodajDana(TYP NowaDana);
	void OdejmijDana(TYP NowaDana);

	void DodajDane(long int IloscNowychDanych); 	//dodaje ilosc danych z tablicy
	void DodajDaneInnaTablica(long int IloscNowychDanych, TYP* wskInnaTablicaDanych);
	
	void UstawTabliceDanych(TYP* wskNowejTablicy);
	void UstawKierunekLiczenia(KierunekLiczenia NowyKierunek);
 	void UstawIloscDanych(long int NowaIloscDanych);
 	void UstawIloscPrzedz(int NowaIloscPrzedz);
 	void UstawSzerokoscPrzedz(TYP NowaSzerokoscPrzedz);
 	void UstawPoczatekPrzedz(TYP NowyPoczatekPrzedz);
 	void UstawKoniecPrzedz(TYP NowyKoniecPrzedz);

        void ZapiszTxt(char* Nazwa);
        void ZapiszBin(char* Nazwa);
        void WczytajBinStr(ifstream* wskstrPliku);

 	///zmienne
 	long int 	IloscDanych; 			//z ilu danych robimy histogram
 	int 		IloscPrzedz;			//na ile przedzialow dzielimy histogram 
 	TYP 		SzerokoscPrzedz;		//jakiej szerokosci przedzial
 	TYP 		PoczatekPrzedz;			//poczatek pierwszego przedzialu <PoczatekPrzedzialow,
 	TYP 		KoniecPrzedz;			// KoniecPrzedzialow)	

 ///private:
        //funkcje
	//zmienne
        bool StalePrzedzialy;

 	long int* 	wskTablicyPrzedzialow;		//wskazuje poczatek tablicy zapelnienia przedzialow 
	long int 	IloscDanychPrzedzMax;		//przedzial o maxymalnej ilosci danych
	//int		DomyslnaIloscPrzedzialow;	//na poczatku tyle przedzialow
	TYP		DanaMax;			//maxymalna zarejestrowana wartość
	TYP		DanaMin;			//minimalna zarejestrowana wartość
 	
	union
	{
	 TYP* 		wskKoniecTablicyDanych;		//wskazuje koniec tablicy danych
 	 TYP* 		wskPoczatekTablicyDanych;	//wskazuje poczatek tablicy danych
	};

        KierunekLiczenia KierunekLiczeniaDanych;	//czy dany jest wskaznik konca tablicy, czy poczatek

        friend ostream& operator<< <>(ostream & stru , Histogram<TYP> & x);
                               //wypisywanie wezla
        //friend istream& operator>> (istream& stru, Histogram<TYP> & x);
};
//--------------------------------------------------------------------------------
template <class TYP> 
ostream& operator<< (ostream & stru , Histogram<TYP> & x)
{
 stru<<"danej rozmiar w bajtach: "<<sizeof(TYP)<<endl;
 stru<<"ilosc danych: "<<x.IloscDanych<<endl; 
 stru<<"ilosc przedzialow: "<<x.IloscPrzedz<<endl;
 stru<<"szerokosc przedzialow: "<<x.SzerokoscPrzedz<<endl; 
 stru<<"poczatek przedzialow: "<<x.PoczatekPrzedz<<endl; 
 stru<<"koniec przedzialow: "<<x.KoniecPrzedz<<endl; 
 stru<<"czy stale przedzialy: "<<x.StalePrzedzialy<<endl; 	
 stru<<"max zliczen w przedziale: "<<x.IloscDanychPrzedzMax<<endl; 
 stru<<"max zarejestrowana dana: "<<x.DanaMax<<endl; 
 stru<<"min zarejestrowana dana: "<<x.DanaMin<<endl; 
 stru<<"Kierunek Liczenia od poczatku(0)/konca(1): "<<x.KierunekLiczeniaDanych<<endl<<endl;
 stru<<"ILOSCI ZLICZEN W PRZEDZIALACH:"<<endl;
 for (int i=0;i<x.IloscPrzedz;i++) stru<<x.wskTablicyPrzedzialow[i]<<endl;
 stru<<endl;
}
#endif
//-----------------------------------------------------------------------------------------------------------
