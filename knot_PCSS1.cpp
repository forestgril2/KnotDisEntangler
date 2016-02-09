#include <string.h>
#include "Wezel3D.h"


static int par = 6;
double R;
int ile=2;
int nr=0;
int naj;
int licznik=0;
Wektor3D A(20,0,0);
int razyile=100;

double x=0,y=0, 
       Energia=0, 
       EnergiaNowa=0,
       EnergiaPocz=0, 
       WagaEnergiiOdpych=0;

short int m=0;
short int n=0;

Wezel3D W(40),Pomoc;

//---------!!!!!!!!!!!!!!!!!!! potrzebne do SONO
CORECT ZZ;

static void 
obliczenia(void)
{
    ile=100000;

    if (m) {for(int i=0;i<2000;i++) ZZ.Zaciskaj();};
    
    if(n)
      {
       for(int i=0;i<ile;i++) 
          {
           W.UstawPunktGiecia1(W.Losownik.LosujInt(W.IloscSegm-1));
           W.UstawPunktGiecia2(W.Losownik.LosujInt(W.IloscSegm-1));
           
           //wyznaczyc przedzia
        
           W.KatGieciaMax=0.9*W.WyznaczKatGieciaMax();
          
           double KatGiecia=
                 W.Losownik.LosujLongDouble(-W.KatGieciaMax,W.KatGieciaMax);
           KatGiecia=W.Zegnij(KatGiecia);
           //W.DodajGiecie(GiecieWezla(W.PunktGiecia1,W.PunktGiecia2,KatGiecia,true));
           W.Przesun(Wektor3D()-W.WyznaczSrodekMasy());
                      
           EnergiaNowa=W.WyznaczEnergieKrzyw()+0.7*WagaEnergiiOdpych*W.WyznaczEnergieOdpych();
           
           if(W.SprawdzCzySaPrzekrycia()
              || !W.SprawdzKrzywProm(W.PunktGiecia1,
                                    W.PunktGiecia1+1==W.IloscSegm?
                                    W.IloscSegm-2:W.PunktGiecia1+1)
              || !W.SprawdzKrzywProm(W.PunktGiecia2,
                                    W.PunktGiecia2+1==W.IloscSegm?
                                    W.IloscSegm-2:W.PunktGiecia2+1)) 
             {
              W.Zegnij(-KatGiecia);
              W.Przesun(Wektor3D()-W.WyznaczSrodekMasy());
             }
           else
             {
              if(Energia<EnergiaNowa)
                {
                 double StosunekGestosciStanow=pow(M_E,(Energia-EnergiaNowa)/(0.01*EnergiaPocz));
                 if(W.Losownik.LosujLongDouble()<=StosunekGestosciStanow) 
                   {
                    W.Zegnij(-KatGiecia);
                   } 
                 else
                   {
                    W.UsunMomentor(&Pomoc);
                    W.UsunMomentor(&Pomoc);
                   };
                }
              else
                {
                 Energia=EnergiaNowa;
                 W.UsunMomentor(&Pomoc);
                 W.UsunMomentor(&Pomoc);
                };
             };
           
          };
      };
    
    W.Zapisz("wynik.txt");   

}


int 
main(int argc, char *argv[])
{
    W.Wczytaj("newgordian.txt");   
    W.Skaluj(2.2);
    W.WyznaczWszystko();
    W.Przesun(Wektor3D()-W.WyznaczSrodekMasy());
    Pomoc=W;    
    W.SprawdzCzySaPrzekrycia();
    W.SprawdzKrzywSrodk();  
    W.DlugoscSegmSrednia=W.WyznaczDlugoscSegmSrednia();
    WagaEnergiiOdpych=W.WyznaczEnergieKrzyw()/W.WyznaczEnergieOdpych()/2;
    EnergiaPocz=Energia= 
           W.WyznaczEnergieKrzyw()+WagaEnergiiOdpych*W.WyznaczEnergieOdpych();  

    //DLA SONO:
    ZZ.Wprowadz((WK3D*)W.wskTablicySegm, W.IloscSegm);
    ZZ.Szybkosc(2.0);
    
    n=1;
    
    obliczenia();

    return EXIT_SUCCESS;
}

 

