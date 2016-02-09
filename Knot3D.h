#ifndef KNOT3D_H
#define KNOT3D_H

#include <set>
#include <deque>

//------------------  ***********KLASA WEZLA ZAMKNIETEGO Z NIESKONCZENIE 
//------------------             TWARDYCH KULEK o stalych
//------------------            *srodku masy
//------------------            *momentorze masy

/* ----------  START pisania 22 marca okolo 12:00 ----------------- */
// SumaCzasu: (+)  OstatniOdcinekCzasowy 
//   Godz:Min (+)  Godz:min
//    119:20  (+) +++++++..... 


using namespace std;

//------------ GLOBALNE (DO PRZENIESIENIA) -------------------------------------
	                                
enum WidokStand 
{
     XY,XZ, //podstawowe widoki we wlasnym ukladzie na okreslona plaszczyzne 
     YX,YZ, //np XY oznacza widok rownolegly do glebokosci Z ekranu;
     ZX,ZY  //stosuje sie tu uklad wspolrzednych, gdzie Z wzrasta w dol ekranu
};
    
enum KonfiguracjaStandWezla     //standardowe konfiguracja wezla
{
     Pierscien,
     Koniczynka
};    

#ifndef ZBIOR_H
#include "zbior.h"
#endif

#ifndef LOSOWANIA_H
#include "Losowania.h"
#endif


#ifndef PRZEKRYCIE_H
#include "Przekrycie.h"
#endif

#ifndef GIECIEWEZLA_H
#include "GiecieWezla.h"
#endif

#ifndef WEKTOR3D_H
#include "Wektor3D.h"
#endif

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------ DEFINICJA CIA�A KLASY -------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

class Knot3D                                    
{
public:
	//DANE:	        
    bool KrzywiznaOK;               //krzywizna mniejsza od KrzywiznaMax?             
    bool PrzekryciaSa;              //brak przekryc?
    int IloscPrzekryc;
    int IlePrzekrycSprawdzac;       //ilu szukamy przekryc (domyslnie 1)
    int IlePunktowKrzywSprawdzac;   //ilu szukamy punktow zbyt duzej krzywizny
                                    //(domyslnie IloscSegm)
    int PrzekryciaPewne;
                                    
    double DlugoscSegmSrednia;    //srednia z odleglosci elementow 
                                      //----dla kulek odleglosc miedzy srodkami
                                    
    Losowania Losownik;             //jeden obiekt losujacy       
    
	int IloscSegm;                  //ilosc punktow wezla
		
    //vector<Wektor3D> Segmenty;       //tablica elementow wezla
                                      //--------- zszablonowac
    Wektor3D* wskTablicySegm;


    set<int> PrzekryciaPojedynczo;
    set<Przekrycie,Porownanie> Przekrycia;

    set<int> SegmentyPrzekrKrzyw; //zbior kojarzacy punktow o za duzej krzywiznie
    //iteratorZbioru<int> itSegmentyPrzekrKrzyw(SegmentyPrzekrKrzyw); //iterator



    double PromienPrzekroju;     //promien przekroju kazdego elementu
                                      //****************************************
                                      //W PRZYSZLOSCI ZMIENNY DLA KAZDEGO ELEM
                                      //****************************************
	
    Zbior<int> SegmentyWybrane;        //zbior kojarzacy wybranych elementow
    
    int PunktGiecia1,PunktGiecia2;      //numery punktow giecia wezla
 
    deque<GiecieWezla> OstatnieGiecia;      //dwustronna kolejka ostatnich 
                                            //(udanych!) giec                                        
    long int DlugoscListyOstUdanychGiecMax;   //dlugosc tejze listy 
    double KatGieciaListyMax;                 //sposob okreslenia dlugosci 
                                              //kolejki giec 
                                              
                               
    double PromienMax;         //maks. odleglosc od srodka ukladu
                                   
    long int IloscKrokow;        //ile bylo prob giecia
    long int IloscGiec;          //ile udalo sie giec
    double WydajnoscGiec;        //wydajnosc giecia

    //FUNKCJE:
     
     //-----------------------KONSTRUKTORY 
      //konstruktor podstawowy        
      Knot3D(int IloscSegm1=4, 
              double PromienPW=1.0,
              const KonfiguracjaStandWezla& Konf1=Pierscien,
              double DlugoscES=0.05, 
              const WidokStand& Widok1=XY, 
              int Numer3D1=1);
                     
      //konstruktor kopiujacy 
      Knot3D(const Knot3D&); 
      
      //konstruktor kopiujacy 
      Knot3D& operator=(const Knot3D& W); 

      //------------------------DESTRUKTORY
      //destruktor podstawowy 
      ~Knot3D()
      {
                delete[] wskTablicySegm;
      };
    void WypiszInfo();
 
    double WyznaczEnergieKrzyw();
    double WyznaczEnergieOdpych();
    double WyznaczEnergieOdSrod();

    int WyznaczPrzekryciaPewne(double=0.0);   //wyznacza ilosc pewnych przekryc


    bool SprawdzCzySaPrzekrycia(int=0,int=0,bool Szybciej=false);
                                //zwraca 'prawda' jesli sa
                                //flaga Szybciej=true zwieksza  szybkosc (13% dla 300 elem)
                                //i nie przeoczy pojedynczego przekrycia
                                //ale nie wpisuje wszystkiego do zbioru przekryc
 

    bool SprawdzCzySaPrzekryciaPoGieciu(int=0,int=0); //zwraca 'prawda' jesli sa

    void WyczyscPrzekrycia(); //czysci zbiory przekrywajacych sie punktow
    void WyczyscSegmentyPrzekrKrzyw(); //czysci zbior elem. przekraczajacych krzywizne

    void WczytajTxt(char* nazwa, int Numer=0);//wczytuje z pliku sekwencyjnego 
    void WczytajBin(ifstream*, int Numer=0);
    bool WczytajBinOdtad(ifstream* wskstrPliku);//wczytuje od zadanej w strumieniu pozycji, nie wraca na miejsce
    void WczytajTxtStary(char* Nazwa); //wczytuje ze starego pliku 
    void WczytajTxtSzkielet(char* Nazwa,int Numer);
    void ZapiszTxt(char* Nazwa);//zapisuje do pliku sekwencyjnego w trybie .txt
    void ZapiszBin(char* Nazwa);//zapisuje do pliku sekwencyjnego w trybie bin.
	
    void Skaluj(double);    //zmienia propocjonalnie odleglosci miedzy punktami
    void SkalujMin();       //skaluje w dol do konformacji o 0.000001 wiekszej
                            //niz ta, w ktorej nastepuje pierwsze przekrycie
	
    void Przesun(const Wektor3D& W);    //przesuwa o dany wektor     
	
    void ObracajX(double Kat);             //proste obroty wokol osi ekranu X
    void ObracajY(double Kat);             //proste obroty wokol osi ekranu Y
    void ObracajZ(double Kat);             //proste obroty wokol osi ekranu Z
    void ObracajWektor(const Wektor3D W);  //obraca o kat dany wektorem
    
    void Transformuj3D(const Macierz3D& M);       //transfomacja3D (mnozenie)
    void Transformuj4D(const Macierz4D& M);       //transfomacja4D (mnozenie)

    void CenterMirror(); //multiplies all coords by -1

    void WyrownajSegmentyPoLamanej(int SegmStartowy,int NaIleRozlozycBlad=0);
    void RozsunPrzekrycia(double Epsilon=0.00000001);
    void ZmienIloscSegm(double IleRazy=2.0, double Poczatek=0.0);
                                            //zwieksza ilosc elementow wezla 
                                            //double razy, zaczynajac od punktu
                                            //najblizszego double w odleglosci
                                            //od niego okreslonej po przecinku

    void UsunPed(Knot3D* const);
    void UsunMomentor(Knot3D* const);
    void UsunMomentorW(Knot3D* const wskW, const Wektor3D& W); //usuwa mom 
                                                    //wzgledem punktu danego 'W'
    
	
    int ZnajdzNajblSegm(int* const wskNr=NULL,
                             int* const wskIle=NULL,
                             Wektor3D* const wskW=NULL);  //znajduje numer elementu o 
                                                    //srodku masy najbli�szym 
                                                    //wektorowi3D, zaczynajac od
                                                    
    int ZnajdzNajdalSegm(int* const wskNr=NULL,
                              int* const wskIle=NULL,
                              Wektor3D* const wskW=NULL);  //znajduje numer elementu o 
                                                    //srodku masy najbli�szym 
                                                    //wektorowi3D, zaczynajac od   
                                                  
    void WybierzSegm(int numer);    //wpisuje element do listy wybr.
                                    //i ustawia jako ostatni lub 
                                    //usuwa z listy jak byl juz 
                                    //wybrany   
                                    
    void UstawPunktGiecia1(int Nr); //ustawia punkt giecia nr1 :D  
    void UstawPunktGiecia2(int Nr); //ustawia punkt giecia nr2 :D 
    void UstawPunktGiecia(int Numer);                                      
                                                                            
    double Zegnij(double Kat);     //zgina wzgledem 2 ostatnio wybranych punktow; 
                                 //zwraca kat giecia; udane giecie
                                 //wpisuje na liste
                                 //--------------tu powinien byc szablon
                                 //--------------uwzgledniajacy
                                 //--------------rodzaj elementu podst
	
    void DodajGiecie(const GiecieWezla& G);  
                            
    bool UzyjSONO(double Ile);   //skraca wezel algorytmem SONO o 'Ile' 
                                 //poczatkowej dlugosci
    
    bool UstawPromienPrzekroju(double Ile);   //ustawia nowy promien przekroju 
                                              //dla calego wezla na 'Ile';
                                              // jesli sie uda zwraca 1,
                                              //nie - 0

    void UstawMomentorWzglDeltaMax(double MomentorWzglDeltaMax1);  
                                          //ustawia dopuszczalny powstaly 
                                          //wzgledny momentor calkowity

    bool OkreslCzyRozplat();     //okresla z duzym prawdopodobienstwem, czy 
                                 //wezel da sie rozplatac algorytmem SONO

protected:
	//DANE:
	//FUNKCJE:
	
private:
    //DANE:
    static const int MaxIloscSegm=30000;

    WidokStand Widok;

    bool NieSprawdzacPrzekryc;
    bool NieSprawdzacKrzywizny;

    KonfiguracjaStandWezla KonfStand;     //standardowe konfiguracja wezla   

    int Numer3D;                    //nr podprzestrzeni przestrzeni 3D 	
	double Masa;                  //ta oczywiscie nie moze sie zmieniac
	double MasaDelta;             //odchylenie standardowe masy
    double MasaDeltaMax;          //max. dopuszcz. odchylenie 
                                       //standardowe masy        


    //int* wskListySegmPrzekr;         //po co mi te numery??? chyba zapomnialem
                                       //o tym, ze lepszy jest set<Przekrycie>

        
    double DlugoscSegmDelta;     //odch. stand. odl.  elementow     
                                      
    double DlugoscSegmDeltaMax;   //max dopuszcz. odch. stand. dlug. element                           
                                      
    double PromienKrzywiznyMin;       //okresla maksymalna krzywizna wezla
    
    public:
	double KatGieciaMax;       //wyznacza sie go z promienia przekroju i 
    private:                           //max rozmiarow wezla


    Wektor3D WektorPunktGiecia1,WektorPunktGiecia2;     //wybrane punkty giecia wezla
    
    public://-----------------------------------------------------

   
    Wektor3D SrodekMasy;        //srodek masy w ukladzie odniesienia  

    Macierz3D MacierzBezwl;     //macierz bezwladnosci w ukladzie odniesienia
    
    double MomentorPI2;              //modul momentora wyznaczony obrotem o PI/2
    double MomentorChwilMax;         //dopuszczalny modul momentora chwilowego     
    double MomentorWzglMax;     // dopuszczalny modul momentora chwilowego 
                                     //wzgledem MomentoraPI2 
    Wektor3D MomentorChwil;     //jesli chwilowy przekracza warto�� maksymaln�  
                                //(MomentorMax) wykonuje sie obrot powrotny 
    Wektor3D MomentorSum;       //zapamietuje caly momentor wszystkich ruchow

	
    //FUNKCJE 
    void WyznaczWszystko();    //wyznacza i inicjuje wszystkie dane wezla
                               //----SPRAWDZIC, CZY CZEGOS NIE BRAK!!!
    void WyznaczWszystkoLog();    //wyznacza i inicjuje wszystkie dane wezla
                                //wyswietla info konsolowe
    
    bool SprawdzCzySaPrzekryciaPrzedzialu(int Poczatek,
                                               int Koniec,
                                               int Wiersz);
     
    double WyznaczDlugoscSegmSrednia(int Segment1=0, int Segment2=0); 
                                       //wyznacza ja w podanym przedziale 
 
    double WyznaczPromienMax(const Wektor3D& Punkt=
                                        Wektor3D(0.0,0.0,0.0));   
                                        //maksymalna odleglosc punktu wezla 
                                        //w domyslnym ukladzie wspolrz od Punkt
                                                            
    double WyznaczKrzywizneMax();          //wyznacz max krzywizne 
                                           //z rozmiaru promienia przekroju
                                    
    bool SprawdzKrzywProm(int Punkt1=0, int Punkt2=0);
                                    //sprawdza krzywizne wybranego 
                                    //przedzialu tablicy elementow wezla 
                                    //do pierwszego elementu, ale nie sprawdza,
                                    //czy dziala sensownie
                                    
    bool SprawdzKrzywSrodk(int Punkt1=0, int Punkt2=0);
                                //sprawdza krzywizne wybranego przedzialu 
                                //tablicy elementow wezla, czysci 
                                //zbior "nrPunktowKrzyw" i wpisuje don elementy 
                                //srodkowe; nie sprawdza, czy dziala sensownie                                 

    double WyznaczMase();
    double WyznaczMaseDelta();
    
    bool UstawIloscSegm2();		//Po prostu podwaja ilosc elem
    bool UstawIloscSegm(int Ilosc, int nrSegmP0cz=0);     //ilosc punktow wezla
    
    Wektor3D WyznaczMomentor(Knot3D* const wskKnot,
                             const Wektor3D& Srodek=Wektor3D(0.0,0.0,0.0)); 
                                   //wyznacza momentor 
                                   //wzgledem drugiego wezla danego wektora
                                                            
    double WyznaczKatGieciaMax();      //wyznacza sie go z promienia kulki i 
                                       //max rozmiarow wezla;
                                       //wartosc przyblizona
    public: //--------------------------------------------------------------
    Wektor3D WyznaczSrodekMasy(int P1=0, int P2=0); //wyznaczajaca srodek masy wezla
                                          
    Wektor3D WyznaczSrodekMasy(const int* wskTablicyNumerowPunktow, 
                                    int Ile);
                                          //wyznaczajaca srodek masy punktow, 
                                          //gdy dane sa numery punktow w tablicy
                                          //oraz jej wielkosc
                                          
    Macierz3D WyznaczMacierzBezwl(int Punkt1=0, int Punkt2=0);  //wyznaczaja macierz bezwl calego 
                                          //wezla
      
    Macierz3D WyznaczMacierzBezwl(const int* wskTablicyNumerowPunktow,
                                       int Ile);     
                                       //Wyznacza Macierz Bezwladnosci elem
                                       //o numerach danych w tablicy
        
        
    //bool WyrownajOdlMiedzyPunktami();  //ustawia luki pomiedzy punktami na te 
                                       //sama dlugosc
                                       
    double WyznaczDlugoscSegmDelta();
    
    Wektor3D WyznaczMomentorObrotu(const Wektor3D& KatObrotu);
                                       //wyznacza momentor w ukladzie 
                                       //odniesienia powstaly obrotem 
                                       //o kat dany wektorem 'KatObrotu'
                                       //w ukladzie odniesienia
    
    bool UstawDlugoscListyUdanychGiec(long int Dlugosc);
    
    bool UstawKatGieciaListyMax(double KatMax);
    
    //Przekrycie* WyznaczPrzekrycia(int Ile=1, int NumerPunktu1=0);
                                 //szuka zadanej ilosci przekryc zaczynajac od 
                                 //punktu Listy wybranego numerem 'NumerPunktu1' 
                                 //i rekurencyjnie wpisuje do tablicy przekryc; 
                                 //ujemna wartosc 'Ile' oznacza szukanie 
                                 //numerami wstecz, korzysta ona z istniejacych 
                                 //przekryc; zwraca wskaznik pary numerow 
                                 //punktow ostatnich przekryc;

    void StworzKonfStand(Wektor3D* wskTablicySegm,
                                const KonfiguracjaStandWezla& Rodz=Pierscien,
                                double Promien=1.0,
                                double DlugoscSegm=0.05); 
                                //majac zmienna KonfStand i wskaznik do 
                                //tablicy elementow z gotowym miejscem, 
                                //robi wezel  
    
    //operatory matematyczne
    double Modul();
    Knot3D& operator+=(Knot3D& Plusowany);
    Knot3D& operator-=(Knot3D& Minusowany);
    Knot3D& operator*=(double);
    Knot3D& operator/=(double);
    Knot3D operator+(Knot3D& Plusowany);
    Knot3D operator-(Knot3D& Minusowany);
    Knot3D operator*(double);
    Knot3D operator/(double);

    //operatory strumieniowe
    friend ostream& operator<< (ostream & stru , Knot3D & x);
                               //wypisywanie wezla
    friend istream& operator>> (istream& stru, Knot3D & x);

};


//------------------------------------------------------------------------------
//TODO:
//      -cos nie tak z energia krzywizny
//      -intergracja SONO + dobor promienia
//      -nie korzystanie ze zbioru przekryc
//      -wyznaczanie max kata kiecia (funkcje zew.) zaleznosc od promienia kulki i odleglosci
//      -kazda zmiana parametru ma byc wywolana odpowiednia funkcja, zeby zapobiec
//            problemom, sprawdzic, czy wszystko tak sie zmienia
//	-zmiana ilosci elementow
//      -wyznaczanie minimalnej ilosci kulek miedzy punktami (od kata)
//      -wybor najdalszego i najblizszego elementu inkrementacyjnie
//      -aktualizacja listy giec
//      -cout,cin listy giec


#endif
