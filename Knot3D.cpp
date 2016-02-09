#ifndef KNOT3D_CPP
#define KNOT3D_CPP

#include <fstream>

#ifndef KNOT3D_H
#include "Knot3D.h"
#endif

#ifndef FUNKCJEZEW_H
#include "funkcjezew.h"
#endif

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------klasa     Knot3D      DEFINICJE FUNKCJI SKLADOWYCH --------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Knot3D::WyznaczWszystko()
{
 Wektor3D A;

 PromienKrzywiznyMin=PromienPrzekroju;
 WyznaczDlugoscSegmSrednia();
 WyznaczPromienMax(); 
 WyznaczKatGieciaMax();
 WyznaczSrodekMasy();
 WyznaczMacierzBezwl();
 A=WyznaczMomentorObrotu(Wektor3D(0,0,M_PI/2.0));
 MomentorPI2=A.length();
 MomentorChwilMax=MomentorWzglMax*MomentorPI2;
 int PamietanaIlosc=IlePrzekrycSprawdzac;
 IlePrzekrycSprawdzac=IloscSegm;
 WyznaczPrzekryciaPewne();
 WyczyscPrzekrycia();
 SprawdzCzySaPrzekrycia();
 IlePrzekrycSprawdzac=PamietanaIlosc;
 PamietanaIlosc=IlePunktowKrzywSprawdzac;
 IlePunktowKrzywSprawdzac=IloscSegm;
 WyczyscSegmentyPrzekrKrzyw();
 SprawdzKrzywProm(); 
 IlePunktowKrzywSprawdzac=PamietanaIlosc;
}
//-------------------------------------------------------------
void Knot3D::WyznaczWszystkoLog()
{
 Wektor3D A;
 cout<<"============ Wezel o "<<IloscSegm<<" segmentach. ==========="<<endl<<endl<<flush;
 PromienKrzywiznyMin=PromienPrzekroju;
 cout<<"============ PromienPrzekroju:\t "<<PromienPrzekroju<<" ==========="<<endl<<endl<<flush;
 WyznaczDlugoscSegmSrednia();
 cout<<"============ Dlugosc:\t"<<IloscSegm*DlugoscSegmSrednia<<"==========="<<endl<<endl<<flush;
 WyznaczPromienMax(); 
 cout<<"============ PromienMax:\t"<<PromienMax<<"==========="<<endl<<endl<<flush;
 WyznaczKatGieciaMax();
 cout<<"============ KatGieciaMax:\t"<<KatGieciaMax<<"==========="<<endl<<endl<<flush;
 A = WyznaczSrodekMasy();
 cout<<"============ SrodekMasy:\t"<<A<<"==========="<<endl<<endl<<flush;
 cout<<"============ MacierzBezwl:\t"<<WyznaczMacierzBezwl()<<"==========="<<endl<<endl<<flush;
 A=WyznaczMomentorObrotu(Wektor3D(0,0,M_PI/2.0));
 MomentorPI2=A.length();
 MomentorChwilMax=MomentorWzglMax*MomentorPI2;
 cout<<"============ MomentorChwilMax:\t"<<MomentorChwilMax<<"==========="<<endl<<endl<<flush;
 int PamietanaIlosc=IlePrzekrycSprawdzac;
 IlePrzekrycSprawdzac=IloscSegm;
 WyznaczPrzekryciaPewne();
 cout<<"============ SprawdzonoPrzekryciaPewne:\t"<<"==========="<<endl<<endl<<flush;
 WyczyscPrzekrycia();
 cout<<"============ WyczyszczonoPrzekrycia:\t"<<"==========="<<endl<<endl<<flush;
 cout<<"============ SprawdzonoPrzekrycia:\t"<<SprawdzCzySaPrzekrycia() <<"==========="<<endl<<endl<<flush;
 IlePrzekrycSprawdzac=PamietanaIlosc;
 PamietanaIlosc=IlePunktowKrzywSprawdzac;
 IlePunktowKrzywSprawdzac=IloscSegm;
 WyczyscSegmentyPrzekrKrzyw();
 cout<<"============ WyczyszczonoKrzywizny:\t"<<"==========="<<endl<<endl<<flush;
 SprawdzKrzywProm(); 
 cout<<"============ SprawdzonowKrzywizny:\t"<<SprawdzKrzywProm()<<"==========="<<endl<<endl<<flush;
 IlePunktowKrzywSprawdzac=PamietanaIlosc;
}
//------------------------------------------------------------------------------
Knot3D::Knot3D(int IloscSegm1, 
                 double PromienPW,
                 const KonfiguracjaStandWezla& Konf1,
                 double DlugoscES, 
                 const WidokStand& Widok1, 
                 int Numer3D1) : 

                       IloscSegm(IloscSegm1>=7?IloscSegm1:7),
                       Masa(IloscSegm1>=7?IloscSegm1:7),
                       IlePrzekrycSprawdzac(IloscSegm1>=7?IloscSegm1:7),
                       IlePunktowKrzywSprawdzac(IloscSegm1>=7?IloscSegm1:7),
                       MasaDelta(0.0),
                       MasaDeltaMax(0.0),
                       PromienPrzekroju(PromienPW>0.0?PromienPW:1.0),
                       DlugoscSegmSrednia(DlugoscES>0.0?DlugoscES:0.05),
                       DlugoscSegmDelta(0.0),  
                       DlugoscSegmDeltaMax(1.0E-15),
                       Widok(Widok1),
                       KonfStand(Konf1),
                       Numer3D(Numer3D1),
                       MomentorWzglMax(0.005),
                       MomentorChwil(Wektor3D(0.0,0.0,0.0)),
                       MomentorSum(Wektor3D(0.0,0.0,0.0)),
                       KatGieciaMax(0.0),
                       KatGieciaListyMax(0.17453292519943295769236907),//10deg
                       IloscKrokow(0),
                       IloscGiec(0),
                       WydajnoscGiec(0),
                       NieSprawdzacPrzekryc(false),
                       NieSprawdzacKrzywizny(false)

{
    PromienKrzywiznyMin=PromienPrzekroju;

    wskTablicySegm=new Wektor3D[IloscSegm];

    StworzKonfStand(wskTablicySegm,KonfStand,PromienPrzekroju,
                      DlugoscSegmSrednia); 

    WyznaczWszystko();
 
    if(IloscSegm) 
      {
       SprawdzKrzywSrodk();
       SprawdzCzySaPrzekrycia();
       IlePrzekrycSprawdzac=1;      
      };

    UstawPunktGiecia1(0); //ustawia punkt giecia nr1 :D  
    UstawPunktGiecia2(IloscSegm/2); //ustawia punkt giecia nr2 :D  

}
//------------------------------------------------------------------------------
Knot3D::Knot3D(const Knot3D& W)
{
 IloscSegm=W.IloscSegm;
 Masa=W.Masa;
 MasaDelta=W.MasaDelta;
 MasaDeltaMax=W.MasaDeltaMax;
 PromienPrzekroju=W.PromienPrzekroju;
 DlugoscSegmSrednia=W.DlugoscSegmSrednia;
 DlugoscSegmDelta=W.DlugoscSegmDelta  ;
 DlugoscSegmDeltaMax=W.DlugoscSegmDeltaMax;
 Widok=W.Widok;
 KonfStand=W.KonfStand;  
 Numer3D=W.Numer3D;
 MomentorWzglMax=W.MomentorWzglMax; 
 MomentorChwil=W.MomentorChwil; 
 MomentorSum=W.MomentorSum;
 KatGieciaMax=W.KatGieciaMax;
 KatGieciaListyMax=W.KatGieciaListyMax;
 IloscKrokow=W.IloscKrokow;
 IloscGiec=W.IloscGiec;
 WydajnoscGiec=W.WydajnoscGiec;
 PromienKrzywiznyMin=W.PromienKrzywiznyMin; 

 delete[] wskTablicySegm; 
 wskTablicySegm=new Wektor3D[IloscSegm];
 for (int i=0; i<IloscSegm; i++) wskTablicySegm[i]=W.wskTablicySegm[i]; 
 
 PromienMax=W.PromienMax;
 KatGieciaMax=W.KatGieciaMax;
 SrodekMasy=W.SrodekMasy;
 MacierzBezwl=W.MacierzBezwl;
 MomentorPI2=W.MomentorPI2;
 MomentorChwilMax=W.MomentorChwilMax;
 if (IloscSegm) KrzywiznaOK=SprawdzKrzywSrodk(); 
 PunktGiecia1=W.PunktGiecia1;
 PunktGiecia2=W.PunktGiecia2;
 IlePrzekrycSprawdzac=W.IlePrzekrycSprawdzac;      
 IlePunktowKrzywSprawdzac=W.IlePunktowKrzywSprawdzac;
 PrzekryciaPewne=W.PrzekryciaPewne;
}  
//------------------------------------------------------------------------------
Knot3D& Knot3D::operator=(const Knot3D& W)
{
 IloscSegm=W.IloscSegm;
 Masa=W.Masa;
 MasaDelta=W.MasaDelta;
 MasaDeltaMax=W.MasaDeltaMax;
 PromienPrzekroju=W.PromienPrzekroju;
 DlugoscSegmSrednia=W.DlugoscSegmSrednia;
 DlugoscSegmDelta=W.DlugoscSegmDelta  ;
 DlugoscSegmDeltaMax=W.DlugoscSegmDeltaMax;
 Widok=W.Widok;
 KonfStand=W.KonfStand;  
 Numer3D=W.Numer3D;
 MomentorWzglMax=W.MomentorWzglMax; 
 MomentorChwil=W.MomentorChwil; 
 MomentorSum=W.MomentorSum;
 KatGieciaMax=W.KatGieciaMax;
 KatGieciaListyMax=W.KatGieciaListyMax;
 IloscKrokow=W.IloscKrokow;        
 IloscGiec=W.IloscGiec;          
 WydajnoscGiec=W.WydajnoscGiec;
 PromienKrzywiznyMin=W.PromienPrzekroju; 
 
 if(wskTablicySegm!=NULL) delete[] wskTablicySegm; 
 wskTablicySegm=new Wektor3D[IloscSegm];
 for (int i=0; i<IloscSegm; i++) wskTablicySegm[i]=W.wskTablicySegm[i]; 
 
 PromienMax=W.PromienMax;
 KatGieciaMax=W.KatGieciaMax;
 SrodekMasy=W.SrodekMasy;
 MacierzBezwl=W.MacierzBezwl;
 MomentorPI2=W.MomentorPI2;
 MomentorChwilMax=W.MomentorChwilMax;
 if (IloscSegm) KrzywiznaOK=SprawdzKrzywSrodk(); 
 PunktGiecia1=W.PunktGiecia1;
 PunktGiecia2=W.PunktGiecia2;
 Losownik=W.Losownik;
 IlePrzekrycSprawdzac=W.IlePrzekrycSprawdzac;      
 IlePunktowKrzywSprawdzac=W.IlePunktowKrzywSprawdzac;
 
return *this;
}
//------------------------------------------------------------------------------
void Knot3D::StworzKonfStand(Wektor3D* wskTablicySegm1,
                              const KonfiguracjaStandWezla& Rodz,
                              double Promien,
                              double DlugoscSegm)
{
 //jesli maja byc wogle jakies punkty
 if (IloscSegm)
    {
    double Kat=2*M_PI/IloscSegm;   //kat rozst. punktow pierscienia  
    double SumaKat=0.0;
    double R=0.5*DlugoscSegm/sinl(0.5*Kat); //promien piersc.
    switch(Rodz)
           {
           case Pierscien :
                //wypelnic tablice punktow wspolrzednymi:
                for (int i=0; i<IloscSegm;i++) 
                    {
                    *wskTablicySegm1++=Wektor3D(R*cosl(SumaKat),R*sinl(SumaKat),0);
                    SumaKat+=Kat;
                    };
                break;

           default :   
                break; 

           };
    }
 //jesli nie ma zadnych punktow
 else        
     {
     KrzywiznaOK=true;               
     PrzekryciaSa=false;
     };       
}
//------------------------------------------------------------------------------

double Knot3D::WyznaczPromienMax(const Wektor3D& Punkt)
{
       PromienMax=::WyznaczPromienMax(Punkt,wskTablicySegm,IloscSegm)+PromienPrzekroju; 
       return PromienMax;
}
//------------------------------------------------------------------------------
double Knot3D::WyznaczKatGieciaMax()
{
       KatGieciaMax=2*asinl(PromienPrzekroju/PromienMax); 
       return KatGieciaMax;
}
//------------------------------------------------------------------------------
Wektor3D Knot3D::WyznaczSrodekMasy(int P1, int P2)
{
if (P1<=P2) 
   {
   return ::WyznaczSrodekMasy(wskTablicySegm, IloscSegm, P1, P2);
   }
else
    {
    Wektor3D SM=::WyznaczSrodekMasy(wskTablicySegm, IloscSegm, P1, IloscSegm);
    SM+=::WyznaczSrodekMasy(wskTablicySegm, IloscSegm, 0, P2);
    };
}
//------------------------------------------------------------------------------
Wektor3D Knot3D::WyznaczSrodekMasy(const int* wskNumerow, int Ile)
{
         SrodekMasy=::WyznaczSrodekMasy(wskTablicySegm, IloscSegm, wskNumerow, Ile);
         return SrodekMasy;
}
//------------------------------------------------------------------------------
Macierz3D Knot3D::WyznaczMacierzBezwl(int P1, int P2)
{
 Macierz3D MB;
 
 if(P1<=P2){
    return ::WyznaczMacierzBezwl(wskTablicySegm, IloscSegm, P1, P2);
    }
 else{
    MB=::WyznaczMacierzBezwl(wskTablicySegm, IloscSegm, 0, P2);
    MB=MB+::WyznaczMacierzBezwl(wskTablicySegm, IloscSegm, P1, IloscSegm);
    return MB;
   };
}
//------------------------------------------------------------------------------
Macierz3D Knot3D::WyznaczMacierzBezwl(const int* wskNr, int Ile)
{
 MacierzBezwl=::WyznaczMacierzBezwl(wskTablicySegm, IloscSegm, wskNr, Ile);
 return MacierzBezwl;
}
//------------------------------------------------------------------------------
Wektor3D Knot3D::WyznaczMomentorObrotu(const Wektor3D& Kat)
{
 MacierzBezwl=WyznaczMacierzBezwl();
 return MacierzBezwl*Kat;
}
//------------------------------------------------------------------------------
bool Knot3D::SprawdzKrzywProm(int Punkt1, int Punkt2)
                                           //funkcja sprawdza wybrany przedzial 
                                           //do pierwszej za duzej krzywizny
                                           //jako ostatni punkt niezbednej 
                                           //trojki wybiera 'Punkt2-1'
                                           //nierownosc ma byc mocna
{
 int A=nrP(Punkt1,IloscSegm),
     B=nrP(Punkt2,IloscSegm),
     RoznicaInt=B-A;
 
 long double Rrazy2=2.0*PromienKrzywiznyMin,
             Rrazy2Bezp=2.00001*PromienKrzywiznyMin, //dla bezpieczenstwa obliczen num.
             Al,Bl,Odl;
 
 Wektor3Dd Roznica;
 Wektor3D* wskPomoc1;
 Wektor3D* wskPomoc2;

 //-----gdyby koniec i poczatek sie pokrywaly to jedziemy od poczatku tablicy
 if(!RoznicaInt)
   {
    //poczatek przedzialu:
    Roznica=wskTablicySegm[IloscSegm-1]-*wskTablicySegm[0];
    Al=Roznica.length();
    Roznica=wskTablicySegm[1]-wskTablicySegm[0];
    Bl=Roznica.length();
    Roznica=wskTablicySegm[IloscSegm-1]-wskTablicySegm[1];
    Odl=Roznica.length(); 
    if(Odl<=Rrazy2Bezp*sinl(asinl(Al/Rrazy2)+asinl(Bl/Rrazy2))) 
      {
       SegmentyPrzekrKrzyw.insert(0);
       if(SegmentyPrzekrKrzyw.size()>=IlePunktowKrzywSprawdzac) 
         {
          KrzywiznaOK=false;
          return false;
         };
      };


    //teraz ze srodka tego przedzialu
    wskPomoc1=wskTablicySegm;
    wskPomoc2=wskTablicySegm+1;

    for (int i=1;i<IloscSegm-1;i++)
        {
        Roznica=*wskPomoc2-*wskPomoc1;
        Al=Roznica.length();
        wskPomoc2++;
        Roznica=*wskPomoc2-*wskPomoc1;
        Odl=Roznica.length();
        wskPomoc1++;
        Roznica=*wskPomoc2-*wskPomoc1;
        Bl=Roznica.length();

        if(Odl<=Rrazy2Bezp*sinl(asinl(Al/Rrazy2)+asinl(Bl/Rrazy2)))
          {
           SegmentyPrzekrKrzyw.insert(i);
           if(SegmentyPrzekrKrzyw.size()>=IlePunktowKrzywSprawdzac) 
             {
              KrzywiznaOK=false;
              return false;
             };
          };
        };
        
    //na koniec trzeba rozpatrzyc te z konca tablicy
    Roznica=wskTablicySegm[IloscSegm-2]-wskTablicySegm[IloscSegm-1];
    Al=Roznica.length();
    Roznica=wskTablicySegm[0]-wskTablicySegm[IloscSegm-1];
    Bl=Roznica.length();
    Roznica=wskTablicySegm[0]-wskTablicySegm[IloscSegm-2];
    Odl=Roznica.length(); 
    if(Odl<=Rrazy2Bezp*sinl(asinl(Al/Rrazy2)+asinl(Bl/Rrazy2)))  
      {
       SegmentyPrzekrKrzyw.insert(IloscSegm-1);
       if(SegmentyPrzekrKrzyw.size()>=IlePunktowKrzywSprawdzac) 
         {
          KrzywiznaOK=false;
          return false;
         };
      };
    };
 
 //------------------------------a teraz, jesli to zwykly przedzial---
 //chyba (???) trzeba rozpatrzyc te dwa przypadki, dla roznicy > i < 0
 if (RoznicaInt>0)
    { 
    Roznica=wskTablicySegm[A]-wskTablicySegm[nrP(A-1,IloscSegm)];
    Al=Roznica.length();
    Roznica=wskTablicySegm[A+1]-wskTablicySegm[A];
    Bl=Roznica.length();
    Roznica=wskTablicySegm[A+1]-wskTablicySegm[nrP(A-1,IloscSegm)];
    Odl=Roznica.length(); 

    if(Odl<=Rrazy2Bezp*sinl(asinl(Al/Rrazy2)+asinl(Bl/Rrazy2)))  
      {
       SegmentyPrzekrKrzyw.insert(A);
       if(SegmentyPrzekrKrzyw.size()>=IlePunktowKrzywSprawdzac) 
         {
          KrzywiznaOK=false;
          return false;
         };
      };

    //teraz srodek przedzialu
    wskPomoc1=&wskTablicySegm[A+1];
    wskPomoc2=wskPomoc1+1;

    for(int i=A+1; i<B; i++)
       {
        Roznica=*wskPomoc2-*wskPomoc1;
        Al=Roznica.length();
        wskPomoc2++;
        Roznica=*wskPomoc2-*wskPomoc1;
        Odl=Roznica.length();
        wskPomoc1++;
        Roznica=*wskPomoc2-*wskPomoc1;
        Bl=Roznica.length();

         if(Odl<=Rrazy2Bezp*sinl(asinl(Al/Rrazy2)+asinl(Bl/Rrazy2)))
           {
            SegmentyPrzekrKrzyw.insert(i);
            if(SegmentyPrzekrKrzyw.size()>=IlePunktowKrzywSprawdzac)
              {
               KrzywiznaOK=false;
               return false;
              };
           };
        };
    };

 if(RoznicaInt<0)
   {
    Roznica=wskTablicySegm[A]-wskTablicySegm[nrP(A+1,IloscSegm)];
    Al=Roznica.length();
    Roznica=wskTablicySegm[A-1]-wskTablicySegm[A];
    Bl=Roznica.length();
    Roznica=wskTablicySegm[A-1]-wskTablicySegm[nrP(A+1,IloscSegm)];
    Odl=Roznica.length(); 

    if(Odl<=Rrazy2Bezp*sinl(asinl(Al/Rrazy2)+asinl(Bl/Rrazy2)))  
      {
       SegmentyPrzekrKrzyw.insert(A);
       if(SegmentyPrzekrKrzyw.size()>=IlePunktowKrzywSprawdzac) 
         {
          KrzywiznaOK=false;
          return false;
         };
      };
    //teraz srodek przedzialu
    wskPomoc1=&wskTablicySegm[A-1];
    wskPomoc2=wskPomoc1-1;

    for (int i=nrP(A-1,IloscSegm); i>B; i--)
        {
         Roznica=*wskPomoc2-*wskPomoc1;
         Al=Roznica.length();
         wskPomoc2--;
         Roznica=*wskPomoc2-*wskPomoc1;
         Odl=Roznica.length();
         wskPomoc1--;
         Roznica=*wskPomoc2-*wskPomoc1;
         Bl=Roznica.length();

         if(Odl<=Rrazy2Bezp*sinl(asinl(Al/Rrazy2)+asinl(Bl/Rrazy2)))  
           {
            SegmentyPrzekrKrzyw.insert(i);
            if(SegmentyPrzekrKrzyw.size()>=IlePunktowKrzywSprawdzac) 
              {
               KrzywiznaOK=false;
               return false;
              };
           };
        };
   };
 //jak juz przeszlim to wszystko i zbior krzywizny jest pusty... 
 if (!SegmentyPrzekrKrzyw.size()) return true;
 else                             return false;
}
//------------------------------------------------------------------------------
bool Knot3D::SprawdzKrzywSrodk(int Punkt1, int Punkt2)  // UWAGAUWAGAUWAGAUWAGAUWAGAUWAGA
                                                         // UWAGAUWAGAUWAGAUWAGAUWAGAUWAGA
                                                         // zrobic to inkrementacja wskaznikow
{ 
 Wektor3D WektorSrodka; //wektor srodka 
 
 int A=nrP(Punkt1,IloscSegm),
     B=nrP(Punkt2,IloscSegm),
     Roznica=B-A;
 Wektor3D Pomocnicza[3];
 
 double KwadratRmin=PromienKrzywiznyMin*PromienKrzywiznyMin;
     
 //if (0) return WektorSrodka/0.0;  //KIEDY TO SIE MOZE PRZYDAC?
 
 //************gdyby koniec i poczatek sie pokrywaly to jedziemy od poczatku tablicy
 if (!Roznica)
    {
    //trzeba rozpatrzyc te problemowe z poczatku tablicy elementow
    Pomocnicza[0]=wskTablicySegm[IloscSegm-1]; 
    Pomocnicza[1]=wskTablicySegm[0];
    Pomocnicza[2]=wskTablicySegm[1];
    
    WektorSrodka=WyznaczSrodekKrzywizny(&Pomocnicza[1])-Pomocnicza[1];
    
    if (WektorSrodka.lengthSq()<=KwadratRmin) SegmentyPrzekrKrzyw.insert(0);
    
    if(SegmentyPrzekrKrzyw.size()>=IlePunktowKrzywSprawdzac) 
      {
       KrzywiznaOK=false;
       return false;
      };
    
    //teraz ze srodka tego przedzialu
    for (int i=1;i<IloscSegm-1;i++)
        {
        WektorSrodka=WyznaczSrodekKrzywizny(&wskTablicySegm[i])-wskTablicySegm[i];
        if (WektorSrodka.lengthSq()<=KwadratRmin) SegmentyPrzekrKrzyw.insert(i);
        if(SegmentyPrzekrKrzyw.size()>=IlePunktowKrzywSprawdzac) 
          {
           KrzywiznaOK=false;
           return false;
          };
        };
        
    //na koniec trzeba rozpatrzyc te problemowe z konca tablicy
    Pomocnicza[0]=wskTablicySegm[IloscSegm-2]; 
    Pomocnicza[1]=wskTablicySegm[IloscSegm-1];
    Pomocnicza[2]=wskTablicySegm[0];
    
    WektorSrodka=WyznaczSrodekKrzywizny(&Pomocnicza[1])-Pomocnicza[1];
    
    if(WektorSrodka.lengthSq()<=KwadratRmin) SegmentyPrzekrKrzyw.insert(IloscSegm-1);
    if(SegmentyPrzekrKrzyw.size()>=IlePunktowKrzywSprawdzac) 
      {
       KrzywiznaOK=false;
       return false;
      };
    };
 
 //***********************8a teraz, jesli to zwykly przedzial---
 //chyba (???) trzeba rozpatrzyc te dwa przypadki, dla roznicy > i < 0 
 if (Roznica>0)
    {
    //sa problemy, jesli 'Punkt1' lub 'Punkt2' sa na koncach
    //wiec dzialamy "podstepnie" ;> i uzywamy pomocniczej trojki
    //(nie wolamy przy tym zbyt czesto funkcji 'nrP(int,int)')
    Pomocnicza[0]=wskTablicySegm[nrP(A-1,IloscSegm)]; 
    Pomocnicza[1]=wskTablicySegm[A];
    Pomocnicza[2]=wskTablicySegm[A+1];
    
    WektorSrodka=WyznaczSrodekKrzywizny(&Pomocnicza[1])-Pomocnicza[1];
    
    if (WektorSrodka.lengthSq()<=KwadratRmin) SegmentyPrzekrKrzyw.insert(A);
    
    if(SegmentyPrzekrKrzyw.size()>=IlePunktowKrzywSprawdzac) 
      {
       KrzywiznaOK=false;
       return false;
      };
       
    //teraz srodek przedzialu     
    for (int i=nrP(A+1,IloscSegm); i<B; i++)
        {
        WektorSrodka=WyznaczSrodekKrzywizny(&wskTablicySegm[i])-wskTablicySegm[i];
        if (WektorSrodka.lengthSq()<=KwadratRmin) SegmentyPrzekrKrzyw.insert(i);
        if(SegmentyPrzekrKrzyw.size()>=IlePunktowKrzywSprawdzac) 
          {
           KrzywiznaOK=false;
           return false;
          };
        };
    }
    
 else
     
    if (Roznica<0)
    {
    //sa problemy, jesli 'Punkt1' lub 'Punkt2' sa na koncach
    //wiec dzialamy "podstepnie" ;> i uzywamy pomocniczej trojki
    //(nie wolamy przy tym zbyt czesto funkcji 'nrP(int,int)')
    Pomocnicza[0]=wskTablicySegm[nrP(A+1,IloscSegm)]; 
    Pomocnicza[1]=wskTablicySegm[A];
    Pomocnicza[2]=wskTablicySegm[A-1];
    
    WektorSrodka=WyznaczSrodekKrzywizny(&Pomocnicza[1])-Pomocnicza[1];
    
    if (WektorSrodka.lengthSq()<=KwadratRmin) SegmentyPrzekrKrzyw.insert(A);
    
    if(SegmentyPrzekrKrzyw.size()>=IlePunktowKrzywSprawdzac) 
      {
       KrzywiznaOK=false;
       return false;
      };
       
    //teraz srodek przedzialu     
    for (int i=nrP(A-1,IloscSegm); i>B; i--)
        {
        WektorSrodka=WyznaczSrodekKrzywizny(&wskTablicySegm[i])-wskTablicySegm[i];
        if (WektorSrodka.lengthSq()<=KwadratRmin) SegmentyPrzekrKrzyw.insert(i);
        if(SegmentyPrzekrKrzyw.size()>=IlePunktowKrzywSprawdzac)
          {
           KrzywiznaOK=false;
           return false;
          };
        };
    };
 //jak juz przeszlim to wszystko i zbior krzywizny jest pusty... 
 if (!SegmentyPrzekrKrzyw.size()) return true;
};
//------------------------------------------------------------------------------
double Knot3D::WyznaczDlugoscSegmSrednia(int Segment1, int Segment2)
{
 DlugoscSegmSrednia=::WyznaczSredniaOdlegloscSegm(wskTablicySegm,IloscSegm,
                                          Segment1,Segment2); 
 return DlugoscSegmSrednia;   
}
//------------------------------------------------------------------------------
ostream& operator<<(ostream& stru, Knot3D& Knot)
{
 Wektor3D* wskPomoc=Knot.wskTablicySegm;
 stru<<Knot.IloscSegm<<endl;
 for (int i=0;i<Knot.IloscSegm;i++) stru<<*wskPomoc++<<endl;
 stru<<Knot.OstatnieGiecia.size()<<endl<<endl;
 for (int i=0;i<Knot.OstatnieGiecia.size();i++) 
          stru<<Knot.OstatnieGiecia[i]<<endl;
 return stru ;
}
//------------------------------------------------------------------------------
istream& operator>>(istream& stru, Knot3D& Knot)
{
 Wektor3D* wskPomoc;
 stru>>Knot.IloscSegm;
 if(Knot.IloscSegm) 
   {
    delete[] Knot.wskTablicySegm;
    Knot.wskTablicySegm = new Wektor3D[Knot.IloscSegm];
    wskPomoc=Knot.wskTablicySegm;
    for (int i=0;i<Knot.IloscSegm;i++) stru>>*wskPomoc++;
   };

 long int IloscGiec;
 stru>>IloscGiec;
 if (IloscGiec) 
    {
    for (int i=0;i<IloscGiec;i++) 
        {
        Knot.OstatnieGiecia.resize(IloscGiec);     
        stru>>Knot.OstatnieGiecia[i];
        };
    };

 return stru ;
}
//------------------------------------------------------------------------------
void Knot3D::Skaluj(double IleRazy)
{
 if(IloscSegm*DlugoscSegmSrednia*IleRazy>1.9*PromienPrzekroju*IloscSegm) IleRazy=(1.9*PromienPrzekroju*IloscSegm)/(IloscSegm*DlugoscSegmSrednia);
 Wektor3D A(0.0,0.0,0.0);
 Wektor3D* wskPomoc=wskTablicySegm;
 if (IloscSegm)
    {
    Wektor3D Srodek=WyznaczSrodekMasy();
    A=A-Srodek;
    Przesun(A);
    for(int i=0; i<IloscSegm; i++) 
       {
        (*wskPomoc)*=IleRazy;
        wskPomoc++;
       };
    Przesun(Srodek);
    };
    
   WyznaczWszystko();
}   
//------------------------------------------------------------------------------
void Knot3D::Przesun(const Wektor3D& W)
{
 double x=W.x,
        y=W.y,
        z=W.z;

 Wektor3D* wskPomoc=wskTablicySegm;
 for(int i=0; i<IloscSegm; i++) 
    {
     wskPomoc->x+=x;
     wskPomoc->y+=y;
     wskPomoc->z+=z;
     wskPomoc++;
    };
}
//------------------------------------------------------------------------------
void Knot3D::ObracajX(double Kat)
{
 Macierz3D Obrot=Macierz3D::createRotationAroundAxis(Kat,0,0);
 Wektor3D* wskPomoc=wskTablicySegm;
 for(int i=0; i<IloscSegm; i++) 
    {
     *wskPomoc=Obrot*(*wskPomoc);
     wskPomoc++;
    };
}
//------------------------------------------------------------------------------
void Knot3D::ObracajY(double Kat)
{
 Macierz3D Obrot=Macierz3D::createRotationAroundAxis(0,Kat,0);
 Wektor3D* wskPomoc=wskTablicySegm;
 for(int i=0; i<IloscSegm; i++) 
    {
     *wskPomoc=Obrot*(*wskPomoc);
     wskPomoc++;
    };
}
//------------------------------------------------------------------------------
void Knot3D::ObracajZ(double Kat)
{
 Macierz3D Obrot=Macierz3D::createRotationAroundAxis(0,0,Kat);
 Wektor3D* wskPomoc=wskTablicySegm;
 for(int i=0; i<IloscSegm; i++) 
    {
     *wskPomoc=Obrot*(*wskPomoc);
     wskPomoc++;
    };
}
//------------------------------------------------------------------------------
void Knot3D::Transformuj3D(const Macierz3D& Transform)
{
 Macierz3D Transformacja=Transform;
 Wektor3D* wskPomoc=wskTablicySegm;
 for(int i=0; i<IloscSegm; i++) 
    {
     *wskPomoc=Transformacja*(*wskPomoc);
     wskPomoc++;
    };
}
//------------------------------------------------------------------------------
void Knot3D::ObracajWektor(const Wektor3D W)
{
 Macierz3D Obrot=Macierz3D::createRotationAroundAxis(W.length(),0,0);
 Macierz3D Trans=Macierz3D::parallelOX(W);
 Obrot=Trans.transpose()*Obrot*Trans; 
 Wektor3D* wskPomoc=wskTablicySegm;
 for(int i=0; i<IloscSegm; i++) 
    {
     *wskPomoc=Obrot*(*wskPomoc);
     wskPomoc++;
    };
}
//------------------------------------------------------------------------------
void Knot3D::Transformuj4D(const Macierz4D& Transform)
{
 Macierz4Dd Transformacja=Transform;
 Wektor4Dd Pomoc;
 Wektor3D* wskPomoc=wskTablicySegm;

 for(int i=0; i<IloscSegm; i++) 
    {
     Pomoc=Transformacja*(Wektor4Dd(wskPomoc->x,wskPomoc->y,wskPomoc->z, 1.0));
     wskPomoc->x=Pomoc.x;
     wskPomoc->y=Pomoc.y;
     wskPomoc->z=Pomoc.z;
     wskPomoc++;
    }; 
}
//------------------------------------------------------------------------------
int Knot3D::ZnajdzNajblSegm(int* const wskNr,
                                  int* const wskIle,
                                  Wektor3D* const wskW)
                                        //TA FUNsKCJA MA BYC ZROBIONA 
                                        //PODOBNIE JAK PRZEKRYCIA !!!!!!!!!!!
                                        //************!!!!!!!!!!****************
                                        //zawsze sprawdza parzysta liczbe 
                                        //kolejnych sasiadow lub wszystkie 
                                        //punkty
{                                                                              
 Wektor3D Kopia, Roznica;
 
 int Najblizszy=0,
     Wskazany,
     Ile;
     
 double OdlegloscMin,
        Aktualna;
 
 if(wskW&&wskNr&&wskIle) //JESLI WIADOMO WSZYSTKO:
   {
    Ile=*wskIle;
    Wskazany=nrP(*wskNr,IloscSegm);
    Kopia=*wskW;
    
    Roznica=Kopia-wskTablicySegm[Wskazany];
    
    OdlegloscMin=Roznica.length();
    
    for(int i=1;i<(Ile+1)/2;i++)
       {
        Roznica=Kopia-wskTablicySegm[nrP(Wskazany+i,IloscSegm)];
        Aktualna=Roznica.length();
             
        if(Aktualna<OdlegloscMin) 
          {
           Najblizszy=nrP(Wskazany+i,IloscSegm);
           OdlegloscMin=Aktualna;
          }
        Roznica=Kopia-wskTablicySegm[nrP(Wskazany-i,IloscSegm)];  
        Aktualna=Roznica.length();
             
        if(Aktualna<OdlegloscMin) 
          {
           Najblizszy=nrP(Wskazany-i,IloscSegm);
           OdlegloscMin=Aktualna;
          } 
        }
    return Najblizszy;
   } 
   
 else
   {
    if(wskW==NULL&&wskNr==NULL&&wskIle==NULL) //JESLI NIE WIADOMO NIC
                                              //to wszystko wiadomo :D
      {
       int zero=0;
       return ZnajdzNajblSegm(&zero, &IloscSegm, &SrodekMasy);
      }
    
    
    if(wskW==NULL&&wskNr&&wskIle) //JESLI NIE ZNAMY WEKTORA, szukamy 
                                  //najblizszego elementowi
                                  // o numerze *wskNr w obie strony wezla razy
                                  //ile 
      {
       Wskazany=nrP(*wskNr,IloscSegm);
       Kopia=wskTablicySegm[Wskazany];
       Ile=*wskIle;
       
       if(Ile)return Wskazany; //najblizszy punktowi to ten sam
       
       Roznica=Kopia-wskTablicySegm[nrP(Wskazany+1,IloscSegm)];
       OdlegloscMin=Roznica.length(); 
       
       for(int i=1;i<(Ile+1)/2;i++)
          {
           Roznica=Kopia-wskTablicySegm[nrP(Wskazany+i,IloscSegm)];
           Aktualna=Roznica.length();
             
             if(Aktualna<OdlegloscMin) 
               {
                Najblizszy=nrP(Wskazany+i,IloscSegm);
                OdlegloscMin=Aktualna;
               }
           
           Roznica=(Kopia-wskTablicySegm[nrP(Wskazany-i,IloscSegm)]);
           Aktualna=Roznica.length();
             
             if(Aktualna<OdlegloscMin) 
               {
                Najblizszy=nrP(Wskazany-i,IloscSegm);
                OdlegloscMin=Aktualna;
               }
          }
       return Najblizszy;
      }
      
      if(wskNr==NULL&&wskW&&wskIle) //jesli nie wiemy od ktorego punktu zaczac
                                    //wyrozniamy zero i od niego zaczynamy
      {
       int zero=0;
       return ZnajdzNajblSegm(&zero, wskIle, wskW);
      }
      
      if(wskIle==NULL&&wskW&&wskNr) //jesli nie wiemy ile jechac od punktu
                                    //to jedziemu jeden i znajdujemy ten z 
                                    //trojki najblizszych sasiadow lacznie z 
                                    //*wskNr, co jest naj blizej wektora 
      {
       
       Wskazany=nrP(*wskNr,IloscSegm);
       Najblizszy=Wskazany;
       Kopia=*wskW;
       
       Roznica=Kopia-wskTablicySegm[Wskazany];
       OdlegloscMin=Roznica.length();
       
       Roznica=Kopia-wskTablicySegm[nrP(Wskazany+1,IloscSegm)];
       Aktualna=Roznica.length();
             
       if(Aktualna<OdlegloscMin) 
         {
          Najblizszy=nrP(Wskazany+1,IloscSegm);
          OdlegloscMin=Aktualna;
         }
       
       Roznica=Kopia-wskTablicySegm[nrP(Wskazany-1,IloscSegm)];    
       Aktualna=Roznica.length();
             
       if(Aktualna<OdlegloscMin) 
         {
          Najblizszy=nrP(Wskazany-1,IloscSegm);
          OdlegloscMin=Aktualna;
         }

       return Najblizszy;
      }
      
       if(wskW&&wskNr==NULL&&wskIle==NULL) //JESLI ZNAMY TYLKO WEKTOR, szukamy 
                                  //najblizszego wektorowi elem wsrod wszystkich 
                                  //zaczynajac od numeru zero w obie str. wezla
         {
          int zero=0; 
          return ZnajdzNajblSegm(&zero,&IloscSegm,wskW);
         }
      
      if(wskNr&&wskW==NULL&&wskIle==NULL)//jesli wiemy tylko to,  od ktorego elem 
                                         //zaczac, szukamy wezla najdalej 
                                         //wektora nieskonczonosci ;] czyli 
                                         //najblizj tego elementu
        {
         Kopia=wskTablicySegm[nrP(*wskNr,IloscSegm)];
         return ZnajdzNajblSegm(wskNr,&IloscSegm,&Kopia);
        }
      
      if(wskIle&&wskW==NULL&&wskNr==NULL)//jesli wiemy tylko ile jechac od punktu
                                        //to losujemy tyle i wybieramy majblizszy 
                                      //srodka masy
      {
       Kopia=SrodekMasy;
       OdlegloscMin=1E+400;
       
       for(int i=0;i<*wskIle;i++)
          {
           Wskazany=Losownik.LosujInt(0,IloscSegm-1);
           
           Roznica=Kopia-wskTablicySegm[Wskazany];    
           Aktualna=Roznica.length();
             
             if(Aktualna<OdlegloscMin) 
               {
                Najblizszy=Wskazany;
                OdlegloscMin=Aktualna;
               }
          }
          
       return Najblizszy;
      }
      
   }
       
}
//------------------------------------------------------------------------------
int Knot3D::ZnajdzNajdalSegm(int* const wskNr,
                                  int* const wskIle,
                                  Wektor3D* const wskW)
                                        //TA FUNsKCJA MA BYC ZROBIONA 
                                        //PODOBNIE JAK PRZEKRYCIA !!!!!!!!!!!
                                        //************!!!!!!!!!!****************
                                        //zawsze sprawdza parzysta liczbe 
                                        //kolejnych sasiadow lub wszystkie 
                                        //punkty
{                                                                              
 Wektor3D Kopia, Roznica;
 
 int Najdalszy=0,
     Wskazany,
     Ile;
     
 double OdlegloscMax,
        Aktualna;
 
 if(wskW&&wskNr&&wskIle) //JESLI WIADOMO WSZYSTKO:
   {
    Ile=*wskIle;
    Wskazany=nrP(*wskNr,IloscSegm);
    Kopia=*wskW;
    
    Roznica=Kopia-wskTablicySegm[Wskazany];
    OdlegloscMax=Roznica.length();
    
    for(int i=1;i<(Ile+1)/2;i++)
       {
        Roznica=Kopia-wskTablicySegm[nrP(Wskazany+i,IloscSegm)];
        Aktualna=Roznica.length();
             
        if(Aktualna>OdlegloscMax) 
          {
           Najdalszy=nrP(Wskazany+i,IloscSegm);
           OdlegloscMax=Aktualna;
          }
          
        Roznica=Kopia-wskTablicySegm[nrP(Wskazany-i,IloscSegm)];   
        Aktualna=Roznica.length();
             
        if(Aktualna>OdlegloscMax) 
          {
           Najdalszy=nrP(Wskazany-i,IloscSegm);
           OdlegloscMax=Aktualna;
          } 
        }
    return Najdalszy;
   } 
   
 else
   {
    if(wskW==NULL&&wskNr==NULL&&wskIle==NULL) //JESLI NIE WIADOMO NIC
                                              //to szukamy najdalszego 
                                              //od srodka masy
      {
       int zero=0;
       return ZnajdzNajdalSegm(&zero, &IloscSegm, &SrodekMasy);
      }
    
    
    if(wskW==NULL&&wskNr&&wskIle) //JESLI NIE ZNAMY WEKTORA, szukamy 
                                  //najdalszego elementowi
                                  // o numerze *wskNr w obie strony wezla razy
                                  //ile 
      {
       Wskazany=nrP(*wskNr,IloscSegm);
       Kopia=wskTablicySegm[Wskazany];
       Ile=*wskIle;
       
       if(Ile)return Wskazany; //Najdalszy punktowi to ten sam jesli nie szukac
       
       Roznica=Kopia-wskTablicySegm[nrP(Wskazany+1,IloscSegm)];
       OdlegloscMax=Roznica.length(); 
       
       for(int i=1;i<(Ile+1)/2;i++)
          {
           Roznica=Kopia-wskTablicySegm[nrP(Wskazany+i,IloscSegm)];
           Aktualna=Roznica.length();
             
             if(Aktualna>OdlegloscMax) 
               {
                Najdalszy=nrP(Wskazany+i,IloscSegm);
                OdlegloscMax=Aktualna;
               }
           
           Roznica=Kopia-wskTablicySegm[nrP(Wskazany-i,IloscSegm)];
           Aktualna=Roznica.length();
             
             if(Aktualna>OdlegloscMax) 
               {
                Najdalszy=nrP(Wskazany-i,IloscSegm);
                OdlegloscMax=Aktualna;
               }
          }
       return Najdalszy;
      }
      
      if(wskNr==NULL&&wskW&&wskIle) //jesli nie wiemy od ktorego punktu zaczac
                                    //wyrozniamy zero i od niego zaczynamy
      {
       int zero=0;
       return ZnajdzNajdalSegm(&zero, wskIle, wskW);
      }
      
      if(wskIle==NULL&&wskW&&wskNr) //jesli nie wiemy ile jechac od punktu
                                    //to jedziemu jeden i znajdujemy ten z 
                                    //trojki najblizszyh sasiadow *wskNr
                                    //co jest najdalej wektora 
      {
       
       Wskazany=nrP(*wskNr,IloscSegm);
       Najdalszy=Wskazany;
       Kopia=*wskW;
       
       Roznica=Kopia-wskTablicySegm[Wskazany];
       OdlegloscMax=Roznica.length();
       
       Roznica=Kopia-wskTablicySegm[nrP(Wskazany+1,IloscSegm)];
       Aktualna=Roznica.length();
             
       if(Aktualna>OdlegloscMax) 
         {
          Najdalszy=nrP(Wskazany+1,IloscSegm);
          OdlegloscMax=Aktualna;
         }
       
       Roznica=Kopia-wskTablicySegm[nrP(Wskazany-1,IloscSegm)];    
       Aktualna=Roznica.length();
             
       if(Aktualna>OdlegloscMax) 
         {
          Najdalszy=nrP(Wskazany-1,IloscSegm);
          OdlegloscMax=Aktualna;
         }

       return Najdalszy;
      }
      
       if(wskW&&wskNr==NULL&&wskIle==NULL) //JESLI ZNAMY TYLKO WEKTOR, szukamy 
                                  //najblizszego wektorowi elem wsrod wszystkich 
                                  //zaczynajac od numeru zero w obie str. wezla
         {
          int zero=0; 
          return ZnajdzNajdalSegm(&zero,&IloscSegm,wskW);
         }
      
      if(wskNr&&wskW==NULL&&wskIle==NULL)//jesli wiemy tylko to,  od ktorego elem 
                                         //zaczac, szukamy wezla najblizej
                                         //wektora nieskonczonosci ;] czyli 
                                         //najdalej tego elementu
        {
         Kopia=wskTablicySegm[nrP(*wskNr,IloscSegm)];
         return ZnajdzNajdalSegm(wskNr,&IloscSegm,&Kopia);
        }
      
      if(wskIle&&wskW==NULL&&wskNr==NULL)//jesli wiemy tylko ile jechac od punktu
                                        //to losujemy tyle i wybieramy najdalszy 
                                      //srodka masy
      {
       Kopia=SrodekMasy;
       OdlegloscMax=0;
       
       for(int i=0;i<*wskIle;i++)
          {
           Wskazany=Losownik.LosujInt(0,IloscSegm-1);
           
           Roznica=Kopia-wskTablicySegm[Wskazany];    
           Aktualna=Roznica.length();
             
             if(Aktualna>OdlegloscMax) 
               {
                Najdalszy=Wskazany;
                OdlegloscMax=Aktualna;
               }
          }
          
       return Najdalszy;
      }
      
   }
       
}
//------------------------------------------------------------------------------
void Knot3D::WybierzSegm(int Numer1)
{
 //jesli jest juz na liscie, to usuwamy, jesli nie ma to insertiamy
 int Numer=nrP(Numer1,IloscSegm);
 iteratorZbioru<int> it=SegmentyWybrane.znajdz(Numer);
 
 if (it) 
    {
         SegmentyWybrane.usun(*it);
    }
 else    
    {
         SegmentyWybrane.wstaw(Numer);
    }; 
}
//------------------------------------------------------------------------------
bool Knot3D::UstawDlugoscListyUdanychGiec(long int Nowa)
{
//DlugoscListyOstUdanychGiecMax=Nowa;     
}
//------------------------------------------------------------------------------
bool Knot3D::UstawIloscSegm(int NowaIloscSegm, int nrSegmPocz)
{
WyznaczDlugoscSegmSrednia();
if(1.9*NowaIloscSegm*PromienPrzekroju<IloscSegm*DlugoscSegmSrednia) NowaIloscSegm=IloscSegm*DlugoscSegmSrednia/(1.9*PromienPrzekroju);


 double OdlozonaIloscLiny;
 double OdlegloscStarychWierzch;
 double BrakujacaDlugoscSegm;
 double OdlegloscOdPoczStaregoSegm;
 double NowaDlugoscSegmSrednia=DlugoscSegmSrednia*double(IloscSegm)/double(NowaIloscSegm);

 Wektor3D* wskWektor3DStareSegm =new Wektor3D[IloscSegm];
 Wektor3D* wskWektor3DPomoc1;
 Wektor3D* wskWektor3DPomoc2;
 Wektor3D wekPomoc;
 {//zapamietanie starego wezla
 wskWektor3DPomoc1=wskTablicySegm;
 wskWektor3DPomoc2=wskWektor3DStareSegm;
 for(int i=IloscSegm; i--; *wskWektor3DPomoc2++=*wskWektor3DPomoc1++);
 }

 {//tablica dla nowej ilosci segm
 delete[] wskTablicySegm;
 wskTablicySegm=new Wektor3D[NowaIloscSegm]; 
 }

 //rosnace indeksy
 int nrStarySegm=nrSegmPocz;
 int nrNowySegm=nrP(nrStarySegm*NowaIloscSegm/IloscSegm,NowaIloscSegm);
 wskTablicySegm[nrNowySegm]=wskWektor3DStareSegm[nrStarySegm];
 nrNowySegm=nrP(nrNowySegm+1,NowaIloscSegm);
 OdlegloscOdPoczStaregoSegm=0;
 BrakujacaDlugoscSegm=NowaDlugoscSegmSrednia;
 OdlozonaIloscLiny=0;

 do{
     nrStarySegm=nrP(nrStarySegm,IloscSegm);
     wekPomoc=wskWektor3DStareSegm[nrP(nrStarySegm+1,IloscSegm)]-wskWektor3DStareSegm[nrStarySegm];
     OdlegloscStarychWierzch=wekPomoc.length();

    if(OdlegloscOdPoczStaregoSegm + BrakujacaDlugoscSegm >OdlegloscStarychWierzch) {
        OdlozonaIloscLiny+=OdlegloscStarychWierzch-OdlegloscOdPoczStaregoSegm;
        BrakujacaDlugoscSegm-=OdlegloscStarychWierzch-OdlegloscOdPoczStaregoSegm;
        OdlegloscOdPoczStaregoSegm=0;
        nrStarySegm++;
        }
    else {
        OdlegloscOdPoczStaregoSegm+=BrakujacaDlugoscSegm;
        OdlozonaIloscLiny+=BrakujacaDlugoscSegm;
        BrakujacaDlugoscSegm=NowaDlugoscSegmSrednia;
        wskTablicySegm[nrP(nrNowySegm++,NowaIloscSegm)]=wskWektor3DStareSegm[nrStarySegm]+
                                                        (wekPomoc/OdlegloscStarychWierzch)*OdlegloscOdPoczStaregoSegm;
        }
    } while (OdlozonaIloscLiny<=DlugoscSegmSrednia*IloscSegm);

/*
//malejace indeksy
 nrStarySegm=nrSegmPocz;
 nrNowySegm=nrP(nrStarySegm*NowaIloscSegm/IloscSegm,NowaIloscSegm);
 nrNowySegm=nrP(nrNowySegm-1,NowaIloscSegm);
 OdlegloscOdPoczStaregoSegm=0;
 BrakujacaDlugoscSegm=NowaDlugoscSegmSrednia;
 OdlozonaIloscLiny=0;

 do {
    nrStarySegm=nrP(nrStarySegm,IloscSegm);
     wekPomoc=wskWektor3DStareSegm[nrP(nrStarySegm-1,IloscSegm)]-wskWektor3DStareSegm[nrStarySegm];
     OdlegloscStarychWierzch=wekPomoc.length();

    if(OdlegloscOdPoczStaregoSegm + BrakujacaDlugoscSegm >OdlegloscStarychWierzch) {
        OdlozonaIloscLiny+=OdlegloscStarychWierzch-OdlegloscOdPoczStaregoSegm;
        BrakujacaDlugoscSegm-=OdlegloscStarychWierzch-OdlegloscOdPoczStaregoSegm;
        OdlegloscOdPoczStaregoSegm=0;
        nrStarySegm++;
        }
    else {
        OdlegloscOdPoczStaregoSegm=BrakujacaDlugoscSegm;
        OdlozonaIloscLiny+=BrakujacaDlugoscSegm;
        BrakujacaDlugoscSegm=NowaDlugoscSegmSrednia;
        wskTablicySegm[nrP(nrNowySegm--,NowaIloscSegm)]=wskWektor3DStareSegm[nrStarySegm]+
                                                        (wekPomoc/OdlegloscStarychWierzch)*OdlegloscOdPoczStaregoSegm;
        }
    } while (OdlozonaIloscLiny<=0.5*DlugoscSegmSrednia*IloscSegm);
*/
  delete[] wskWektor3DStareSegm;
  IloscSegm=NowaIloscSegm;
WyznaczWszystko();
 RozsunPrzekrycia();
WyznaczWszystko();
return true;
}
//------------------------------------------------------------------------------
bool Knot3D::UstawIloscSegm2()
{
 int NowaIloscSegm=2*IloscSegm;
 Wektor3D Srednia;

 Wektor3D* wskStarejTablicy=new Wektor3D[IloscSegm];
 Wektor3D* wskStaryPomocniczy0=wskStarejTablicy;
 Wektor3D* wskStaryPomocniczy=wskTablicySegm;
 
for(int i=0;i<IloscSegm;i++)
    {
     *wskStaryPomocniczy0++=*wskStaryPomocniczy++;
    };
 
 delete[] wskTablicySegm;
 wskTablicySegm=new Wektor3D[NowaIloscSegm];

 Wektor3D* wskPomocniczy=wskTablicySegm;
 wskStaryPomocniczy0=wskStarejTablicy;

 for(int i=0;i<IloscSegm-1;i++)
    {
     Srednia=*wskPomocniczy++=*wskStaryPomocniczy0++;
     *(wskPomocniczy++)=(Srednia+*wskStaryPomocniczy0)*0.5;
    };
 Srednia=*wskPomocniczy++=*wskStaryPomocniczy0++;
 *(wskPomocniczy++)=(Srednia+*wskStarejTablicy)*0.5;

 delete[] wskStarejTablicy;

 IloscSegm=NowaIloscSegm;
 WyznaczWszystko();
 RozsunPrzekrycia();
 return true;
}
//------------------------------------------------------------------------------
void Knot3D::WczytajTxtSzkielet(char* Nazwa, int Numer)   //wczytuje z pliku sekwencyjnego 
{
 char znak;
 ifstream strPliku; 
 strPliku.setf(ios::fixed);
 strPliku.precision(16);
 strPliku.open(Nazwa,ios_base::in);
 for(int i=0;i<Numer;i++) 
    {
     strPliku>>IloscSegm;
     for(int i=IloscSegm;i;i--) 
        {
         strPliku.seekg(45,ios_base::cur);
         do
           {
            strPliku.read(&znak,1);
           }
         while(znak!='\n');
        };

     strPliku>>IloscGiec;
     for(int i=0;i<IloscGiec;i--) 
        {
          strPliku.seekg(16,ios_base::cur);
         do
           {
            strPliku.read(&znak,1);
           }
         while(znak!='\n');
        };
     };
     
 strPliku>>*this;
 strPliku.close();
 NieSprawdzacPrzekryc=true;
 UstawIloscSegm(30);
 WyznaczWszystkoLog();
 Przesun(Wektor3D(0,0,0)-SrodekMasy);
 NieSprawdzacPrzekryc=false;
 SkalujMin();
}
//---------------------------------------------------------------------------------------------------------
void Knot3D::WczytajTxt(char* Nazwa, int Numer)   //wczytuje z pliku sekwencyjnego 
{
 char znak;
 ifstream strPliku; 
 strPliku.setf(ios::fixed);
 strPliku.precision(16);
 strPliku.open(Nazwa,ios_base::in);
 for(int i=0;i<Numer;i++) 
    {
     strPliku>>IloscSegm;
     for(int i=IloscSegm;i;i--) 
        {
         strPliku.seekg(45,ios_base::cur);
         do
           {
            strPliku.read(&znak,1);
           }
         while(znak!='\n');
        };

     strPliku>>IloscGiec;
     for(int i=0;i<IloscGiec;i--) 
        {
          strPliku.seekg(16,ios_base::cur);
         do
           {
            strPliku.read(&znak,1);
           }
         while(znak!='\n');
        };
     };
     
 strPliku>>*this;   
 strPliku.close(); 
}
//-------------------------------------------------------------------------------
void Knot3D::WczytajBin(ifstream* wskstrPliku, int Numer)   //wczytuje z pliku sekwencyjnego
							//od pozyzcji zadanej + "Numer"
{
 
 long int Roznica,
	  Aktualny=static_cast<long int>(wskstrPliku->tellg());
 
 wskstrPliku->seekg(0,ios_base::end);
 
 long int KoniecPliku=static_cast<long int>(wskstrPliku->tellg());

 wskstrPliku->seekg(Aktualny,ios_base::beg);
 
 int StaraIloscSegm=IloscSegm;

 if(Numer>=0){
   for(int i=0;i<Numer;i++)
      {
       wskstrPliku->read((char*)&IloscSegm,sizeof(IloscSegm));
       wskstrPliku->seekg(sizeof(Wektor3D)*IloscSegm,ios_base::cur);
       wskstrPliku->read((char*)&IloscGiec,sizeof(IloscGiec));
       wskstrPliku->seekg(sizeof(GiecieWezla)*IloscGiec,ios_base::cur);
      };
   }
 else
   {//to nalezy zmienic, tak zeby moc wczytac poprzedni wezel o innej ilosci elem i giec (class pliker)
    for(int i=0;i>Numer;i--)
	{
        //wskstrPliku->seekg(-(sizeof(GiecieWezla)*IloscGiec),ios_base::cur);
        //wskstrPliku->seekg(-(sizeof(IloscGiec)),ios_base::cur);
        //wskstrPliku->seekg(-(sizeof(Wektor3D)*IloscSegm),ios_base::cur);
        //wskstrPliku->seekg(-(sizeof(IloscSegm)),ios_base::cur);
	wskstrPliku->seekg(-7208,ios_base::cur);
       };
   };

 Roznica=static_cast<long int>(wskstrPliku->tellg()); //poczatkowa pozycja
 if(static_cast<long int>(wskstrPliku->tellg())>=KoniecPliku) return;
 wskstrPliku->read((char*)&IloscSegm,sizeof(IloscSegm));//wczyt ilosc elem
 if(IloscSegm<=0||static_cast<long int>(wskstrPliku->tellg())>=KoniecPliku)
   {
    IloscSegm=StaraIloscSegm;
     return;
   }
 delete[] wskTablicySegm;
 wskTablicySegm = new Wektor3D[IloscSegm];
 Wektor3D* wskPomoc=wskTablicySegm;
 for(int i=0;i<IloscSegm;i++)
    { 
     wskstrPliku->read((char*)wskPomoc,sizeof(Wektor3D));
     wskPomoc++;
    };
  
 wskstrPliku->read((char*)&IloscGiec,sizeof(IloscGiec));
 OstatnieGiecia.resize(IloscGiec);  
 for(int i=0;i<IloscGiec;i++)
    {
     wskstrPliku->read((char*)(&OstatnieGiecia[i]), sizeof(GiecieWezla));
    };

 Roznica=(long int)wskstrPliku->tellg()-Roznica; //roznica pozycji
    {//cofamy sie na poczatek przeczytanego wezla
     wskstrPliku->seekg(-Roznica,ios_base::cur);
    }
}
//-------------------------------------------------------------------------------
bool Knot3D::WczytajBinOdtad(ifstream* wskstrPW)   //wczytuje z pliku sekwencyjn
                                                                //od pozyzcji zadanej + "Numer"
{ 
 int StaraIloscSegm=IloscSegm;
 wskstrPW->read((char*)&IloscSegm,sizeof(IloscSegm));//wczyt ilosc elem
    if(IloscSegm<=0||IloscSegm>=MaxIloscSegm)
      {
       cout<<"============ Brak wezla lub za duzo sementow! (wiecej niz "<<
            MaxIloscSegm<<")!!========="<<endl<<endl<<flush;
       IloscSegm=StaraIloscSegm;
       return false;
      }
    delete[] wskTablicySegm;
    wskTablicySegm = new Wektor3D[IloscSegm];
    Wektor3D* wskPomoc=wskTablicySegm;
    for(int i=0;i<IloscSegm;i++)
       { 
        wskstrPW->read((char*)wskPomoc,sizeof(Wektor3D));
        wskPomoc++;
       }
  
    wskstrPW->read((char*)&IloscGiec,sizeof(IloscGiec));
    OstatnieGiecia.resize(IloscGiec);  
    for(int i=0;i<IloscGiec;i++)
       {
        wskstrPW->read((char*)(&OstatnieGiecia[i]), sizeof(GiecieWezla));
       }

 cout<<"============ Pomyslnie wczytano wezel o "<<IloscSegm<<" segmentach! ==========="<<endl<<endl<<flush;
 return true;
}
//------------------------------------------------------------------------------

void Knot3D::WczytajTxtStary(char* Nazwa)   //wczytuje z pliku sekwencyjnego 
{
 ifstream strPliku; 
 strPliku.setf(ios::fixed);
 strPliku.precision(16);
 strPliku.open(Nazwa,ios_base::in);
 char znak;
 strPliku>>IloscSegm;
 if (IloscSegm) 
    {
    delete[] wskTablicySegm;
    wskTablicySegm = new Wektor3D[IloscSegm];

    for (int i=0;i<IloscSegm;i++) 
        {
        strPliku>>wskTablicySegm[i].x;
        strPliku>>znak;
        strPliku>>wskTablicySegm[i].y;
        strPliku>>znak;
        strPliku>>wskTablicySegm[i].z;
        };
    };
 
 strPliku.close();
}



//------------------------------------------------------------------------------
void Knot3D::ZapiszTxt(char* Nazwa)   //zapisuje do pliku sekwencyjnego 
{
 ofstream strPliku;
 strPliku.setf(ios::fixed);
 strPliku.precision(16);
 strPliku.open(Nazwa,ios_base::app);
 strPliku<<*this;
 strPliku.close();
}
//------------------------------------------------------------------------------
void Knot3D::ZapiszBin(char* Nazwa)   //zapisuje do pliku sekwencyjnego 
{
 ofstream strPliku;
 strPliku.open(Nazwa,ios::binary|ios::out|ios::app);
 strPliku.write((char*)(&IloscSegm),sizeof(IloscSegm));
 Wektor3D* wskPomoc=wskTablicySegm;
 for(int i=0;i<IloscSegm;i++)
    {
     strPliku.write((char*)wskPomoc,sizeof(Wektor3D));
     wskPomoc++;
    };
 strPliku.write((char*)(&IloscGiec),sizeof(IloscGiec));
 for(int i=0;i<IloscGiec;i++)
    {
     strPliku.write((char*)(&OstatnieGiecia[i]), sizeof(GiecieWezla));
    };
 strPliku.close();

  cout<<"Zapisano wezel w pliku "<< Nazwa <<endl<<endl<<flush; 
}
//------------------------------------------------------------------------------
void Knot3D::DodajGiecie(const GiecieWezla& G)
{
OstatnieGiecia.push_back(G);
IloscGiec=OstatnieGiecia.size();
};
//------------------------------------------------------------------------------
double Knot3D::Zegnij(double Kat) 
{
//stworzenie macierzy:
Macierz3Dd Trans;

Macierz4Dd Trans1Czesc,Trans2Czesc,  //macierze transformacji calkowitej
          Przysun,TransIn,Obrot1Czesci,Obrot2Czesci,TransOut,Odsun; //czastkowe
         
//niezbedne wektory
Wektor3Dd A=wskTablicySegm[PunktGiecia1],
          AB=(wskTablicySegm[PunktGiecia2]-A),
          SrodekMasy1=WyznaczSrodekMasy(PunktGiecia1,PunktGiecia2),
          SrodekMasy2=WyznaczSrodekMasy(PunktGiecia2,PunktGiecia1);
         
Przysun.setTranslation(Wektor3Dd()-A);    //macierze translacji
Odsun.setTranslation(A);

Trans=Macierz3Dd::parallelOX(AB);

TransIn.setRotation(Trans);
TransOut.setRotation(Trans.transpose());

Macierz4Dd R;

Wektor4Dd Pomoc;

Obrot1Czesci=R.createRotationAroundAxis(Kat,0, 0);        //ma byc KAT1 !!!
//Obrot2Czesci=R.createRotationAroundAxis(Kat2,0, 0);

Trans1Czesc=Odsun*TransOut*Obrot1Czesci*TransIn*Przysun;
//Trans2Czesc=Odsun*TransOut*Obrot2Czesci*TransIn*Przysun;   

Wektor3D* wskPomoc;

if (PunktGiecia1<PunktGiecia2)
   {
    wskPomoc=&wskTablicySegm[PunktGiecia1+1];
    for (int i=PunktGiecia1+1; i<PunktGiecia2; i++) 
         {
         Pomoc=Trans1Czesc*(Wektor4D(wskPomoc->x,wskPomoc->y,wskPomoc->z,1.0));
         wskPomoc->x=Pomoc.x;
         wskPomoc->y=Pomoc.y;
         wskPomoc->z=Pomoc.z;
         wskPomoc++;
         }; 

    /*
    for (int i=PunktGiecia2+1; i<IloscSegm; i++) 
         {  
         Pomoc=Trans2Czesc*(Wektor4D(wskTablicySegm[i].x,
                                       wskTablicySegm[i].y,
                                       wskTablicySegm[i].z,  1));

         wskTablicySegm[i].x=Pomoc.x;
         wskTablicySegm[i].y=Pomoc.y;
         wskTablicySegm[i].z=Pomoc.z;
         }; 
         
     for (int i=0; i<PunktGiecia1; i++) 
         {  
         Pomoc=Trans2Czesc*(Wektor4D(wskTablicySegm[i].x,
                                       wskTablicySegm[i].y,
                                       wskTablicySegm[i].z,  1));

         wskTablicySegm[i].x=Pomoc.x;
         wskTablicySegm[i].y=Pomoc.y;
         wskTablicySegm[i].z=Pomoc.z;
         };
      */                           
   }
else
   {
    /*  
    for (int i=PunktGiecia2+1; i<PunktGiecia1; i++) 
         {  
         Pomoc=Trans1Czesc*(Wektor4D(wskTablicySegm[i].x,
                                       wskTablicySegm[i].y,
                                       wskTablicySegm[i].z,  1));

         wskTablicySegm[i].x=Pomoc.x;
         wskTablicySegm[i].y=Pomoc.y;
         wskTablicySegm[i].z=Pomoc.z;
         }; 
    */ 
    wskPomoc=&wskTablicySegm[PunktGiecia1+1];
    for(int i=PunktGiecia1+1; i<IloscSegm; i++) 
        {
         Pomoc=Trans1Czesc*(Wektor4D(wskPomoc->x,wskPomoc->y,wskPomoc->z,1.0));
         wskPomoc->x=Pomoc.x;
         wskPomoc->y=Pomoc.y;
         wskPomoc->z=Pomoc.z;
         wskPomoc++;
        };

     wskPomoc=wskTablicySegm;
     for (int i=0; i<PunktGiecia2; i++) 
         {
         Pomoc=Trans1Czesc*(Wektor4D(wskPomoc->x,wskPomoc->y,wskPomoc->z,1.0));
         wskPomoc->x=Pomoc.x;
         wskPomoc->y=Pomoc.y;
         wskPomoc->z=Pomoc.z;
         wskPomoc++;
         };
   };
return Kat;
}
//-----------------------------------------------------------------------------
void Knot3D::UstawPunktGiecia1(int Numer) //ustawia punkt giecia :D  
{
 Numer=nrP(Numer,IloscSegm);
 
 if (PunktGiecia2==Numer)                        //jesli wybrany wlasnie jest 
                                                   //drugim punktem giecia
    {
     PunktGiecia2=PunktGiecia1;                   //zamienimy miejscami: 
     PunktGiecia1=Numer;
    }
 else
     {
      PunktGiecia1=Numer;
     };
 int o=odlP(PunktGiecia1,PunktGiecia2,IloscSegm);
 if(o<2||o>IloscSegm-2){
   PunktGiecia1=nrP(PunktGiecia1-1,IloscSegm);
   PunktGiecia2=nrP(PunktGiecia1+2,IloscSegm);
   }
}
//-----------------------------------------------------------------------------
void Knot3D::UstawPunktGiecia2(int Numer) //ustawia punkt giecia :D  
{
 Numer=nrP(Numer,IloscSegm);
 
 if (PunktGiecia1==Numer)                        //jesli wybrany wlasnie jest 
                                                   //drugim punktem giecia
    {
     PunktGiecia1=PunktGiecia2;                   //zamienimy miejscami: 
     PunktGiecia2=Numer;
    }
 else
     {
      PunktGiecia2=Numer;
     };

 int o=odlP(PunktGiecia2,PunktGiecia1,IloscSegm);
 if(o<2||o>IloscSegm-2){
   PunktGiecia2=nrP(PunktGiecia2-1,IloscSegm);
   PunktGiecia1=nrP(PunktGiecia2+2,IloscSegm);
   }

}
//-----------------------------------------------------------------------------
void Knot3D::UstawPunktGiecia(int Numer) 
{
 UstawPunktGiecia1(PunktGiecia2);
 UstawPunktGiecia2(Numer);
}
//-----------------------------------------------------------------------------
void Knot3D::ZmienIloscSegm(double IleRazy, double Poczatek)
                            //inetrpolacja punktu nastepuje z 3 rownymi wagami:
                            //1) (1-odleglosc) od jednego punktu
                            //2) (1-odleglosc) od srodka
                            //3) (1-odleglosc) od drugiego punktu
                                    
{
 Wektor3D A,B; //pomocnicze

 if(IleRazy<=0.0) return; //jakby komus przyszlo do glowy cos glupiego, ale 
                          //kiedys mozna dodac ujemny kierunek dopasowan wiec: 
 bool DoPrzodu=true;
                          
 int PoczatekInt=static_cast<int>(Poczatek);   //numer punktu pocz
 double ResztkaPoczatku=Poczatek-PoczatekInt;  //okresla odleglosc od powyzszego
 PoczatekInt=nrP(PoczatekInt,IloscSegm);       //zawezenie do wezla
 
 int NowaIloscSegm=static_cast<int>(IloscSegm*IleRazy);
   
 if(NowaIloscSegm>MaxIloscSegm) 
   {
   NowaIloscSegm=MaxIloscSegm-1;
   IleRazy=(double)NowaIloscSegm/(double)IloscSegm;
   };
 
 NowaIloscSegm++; //lepiej, zeby bylo wiecej niz mniej (np. jakies luki)
 
 double NowaSredniaOdleglosc=DlugoscSegmSrednia/IleRazy;
 
 bool UdaloSieZmienic=false;  //czy proces sie udal, bo moze sie okazac, ze jest 
                              //za duza krzywizna gdzies, albo przekrycia
 
 Wektor3D* wskNowaTablicaSegm=new Wektor3D[NowaIloscSegm];
 
 Wektor3D StaryPoprzedni,         //stary najblizszy danemu
          Najblizszy,             //aktualny najblizszy danemu
          NowyNastepny;           //wektor nowy dopiero opracowywany
  
 Wektor3D Najblizsze[4]; //tu umiescimy kolejne stare najblizsze danemu nowemu
                         //punktowi
 
 NowyNastepny=wskTablicySegm[PoczatekInt];
 NowyNastepny=NowyNastepny.lerp(ResztkaPoczatku,wskTablicySegm[PoczatekInt+1]);
            //w ten sposob uzyskujemy przyblizone polozenie pocz. punktu
 
 for(int i=1;i<NowaIloscSegm;i++)
    {
     int ile=2;
 
     int NajblizszyNr=ZnajdzNajblSegm(&PoczatekInt,&ile,&NowyNastepny);
     
     A=wskTablicySegm[nrP(NajblizszyNr-1,IloscSegm)]-NowyNastepny;
     B=wskTablicySegm[nrP(NajblizszyNr,IloscSegm)]-NowyNastepny;
     
     if(A.length()<=B.length()) 
                                        //jesli poprzedni jest blizej
       {
        Najblizsze[0]=wskTablicySegm[nrP(NajblizszyNr-1,IloscSegm)];
        Najblizsze[1]=wskTablicySegm[nrP(NajblizszyNr-0,IloscSegm)];
        Najblizsze[2]=wskTablicySegm[nrP(NajblizszyNr+1,IloscSegm)];
        Najblizsze[3]=wskTablicySegm[nrP(NajblizszyNr+2,IloscSegm)];
       }
     else
       {
        Najblizsze[0]=wskTablicySegm[nrP(NajblizszyNr-1,IloscSegm)];
        Najblizsze[1]=wskTablicySegm[nrP(NajblizszyNr-0,IloscSegm)];
        Najblizsze[2]=wskTablicySegm[nrP(NajblizszyNr+1,IloscSegm)];
        Najblizsze[3]=wskTablicySegm[nrP(NajblizszyNr+2,IloscSegm)];
       };

     Najblizszy=wskTablicySegm[NajblizszyNr];
    };
};
//-----------------------------------------------------------------------------
bool Knot3D::SprawdzCzySaPrzekrycia(int A, int B, bool Szybciej)
                                    //znajduje przekrycia do liczby 'IlePrzekrycSprawdzac'
                                    //po dowolnej transformacji wezla w przedziale <A,B)
                                    //i wstawia do zbioru przekryc
{
 //ile punktow jest na najciasniejszym okregu
 //tylko jesli nie zmieniamy odleglosci elementow!!!
 //(w przeciwnym razie sprawdzamy sumaryczna odl od 
 //danego punktu i robimy to dla kazdego wiersza tabeli przekryc

 //DlugoscSegmSrednia=WyznaczDlugoscSegmSrednia();
 //int PrzekryciaPewne=static_cast<int>
                    //(M_PI_2/asinl(0.5*DlugoscSegmSrednia/PromienKrzywiznyMin));
if(NieSprawdzacPrzekryc) return true;


 int PrzekryciaPewneZmnieniane=0;

 if(2*PrzekryciaPewne>IloscSegm) return false;

 int Roznica=(B-A)%IloscSegm;
 A=nrP(A,IloscSegm);
 B=!Roznica?A+IloscSegm:A+Roznica;
 int KoniecMax=A<B?A+IloscSegm:B+IloscSegm;

 int Sprawdzacz=Roznica-PrzekryciaPewne-1;

 int Krement=Roznica>=0?1:-1; //albo INkrementacja albo DEkrementacja ;D
 Roznica=Krement>0?0:Krement*Roznica-1;

 if(Krement>=0)
   {
    PrzekryciaPewneZmnieniane=PrzekryciaPewne;
     for(int Wiersz=A; Wiersz!=B; Wiersz++)
        {//powtorzyc dla kazdego wiersza sprawdzanie odpowiedniego przedzialu
         int Poczatek=Wiersz+PrzekryciaPewne;
         int Koniec=Szybciej?KoniecMax-(PrzekryciaPewneZmnieniane?PrzekryciaPewneZmnieniane--:0)
                             :Wiersz+IloscSegm-PrzekryciaPewne;


         //W TEJ WERSJI POCZATEK SPRAWDZANEGO PRZEDZIALU MUSI BYC MNIEJSZY NIZ KONIEC
         if(Poczatek+1<Koniec)
           {//sprawdzanie przedzialu
            SprawdzCzySaPrzekryciaPrzedzialu(Poczatek, Koniec, Wiersz);
           }
         if(PrzekryciaPojedynczo.size()>=IlePrzekrycSprawdzac)
           {//jesli osiagnieto ilosc szukanych przekryc
            return true;
           }
        }
    }
   else
    {
     PrzekryciaPewneZmnieniane=0;
     for(int Wiersz=A; Wiersz!=B; Wiersz--)
       {//powtorzyc dla kazdego wiersza sprawdzanie odpowiedniego przedzialu
        int Poczatek=Wiersz+PrzekryciaPewne;
        int Koniec=Szybciej?KoniecMax-(Sprawdzacz--?0:PrzekryciaPewneZmnieniane++)
                            :Wiersz+IloscSegm-PrzekryciaPewne;

        //W TEJ WERSJI POCZATEK SPRAWDZANEGO PRZEDZIALU MUSI BYC MNIEJSZY NIZ KONIEC
        if(Poczatek+1<Koniec)
          {//sprawdzanie przedzialu
           SprawdzCzySaPrzekryciaPrzedzialu(Poczatek, Koniec, Wiersz);
          }

        if(PrzekryciaPojedynczo.size()>=IlePrzekrycSprawdzac)
          {//jesli osiagnieto ilosc szukanych przekryc
           return true;
          }
       }
    }

 return PrzekryciaPojedynczo.size()?true:false;  //jesli nie wykryto przekryc ->false
}
//------------------------------------------------------------------------------

bool Knot3D::SprawdzCzySaPrzekryciaPoGieciu(int A, int B)   //znajduje przekrycia do liczby 'IlePrzekrycSprawdzac'
                                                             //po gieciu wezla w przedziale (A,B) (czyli zakladajac
                                                             //ze wzajemne polozenie punktow tego przedzialu nie 
                                                             //zmienilo sie) i wstawia do zbioru przekryc
{
 //ile punktow jest na najciasniejszym okregu
 //tylko jesli nie zmieniamy odleglosci elementow!!!
 //(w przeciwnym razie sprawdzamy sumaryczna odl od 
 //danego punktu i robimy to dla kazdego wiersza tabeli przekryc

 //DlugoscSegmSrednia=WyznaczDlugoscSegmSrednia();
 //int PrzekryciaPewne=static_cast<int>
                    //(M_PI_2/asinl(0.5*DlugoscSegmSrednia/PromienKrzywiznyMin));


 int PrzekryciaPewneRosnace=0;
 int PrzekryciaPewneMalejace=PrzekryciaPewne-1;

 if(2*PrzekryciaPewne>IloscSegm) return false;

 int Roznica=(B-A)%IloscSegm;
 B=((!Roznica)?A+IloscSegm:A+Roznica);
 int PoczatekMin=(A>B?A:B);
 int KoniecMax=(A>B?A+IloscSegm:B+IloscSegm);

 int Krement=((Roznica>=0)?1:-1); //albo INkrementacja albo DEkrementacja ;D
 Roznica=(Roznica?Roznica:IloscSegm);
 Roznica=(Roznica>0)?Roznica:-Roznica;
 int Sprawdzacz=Roznica-PrzekryciaPewne-2;

 if(Krement>=0)
   {
    for(int Wiersz=A+1; Wiersz!=B; Wiersz++)
       {//powtorzyc dla kazdego wiersza sprawdzanie odpowiedniego przedzialu
        int Poczatek=PoczatekMin;//+(Sprawdzacz--?0:PrzekryciaPewneRosnace++);
        if(Wiersz+PrzekryciaPewne>PoczatekMin) Poczatek=Wiersz+PrzekryciaPewne;
        int Koniec=KoniecMax;//-(PrzekryciaPewneMalejace?(PrzekryciaPewneMalejace--):0);
        if(Wiersz+IloscSegm-PrzekryciaPewne<KoniecMax) Koniec=Wiersz+IloscSegm-PrzekryciaPewne;

        //W TEJ WERSJI POCZATEK SPRAWDZANEGO PRZEDZIALU MUSI BYC MNIEJSZY NIZ KONIEC
        if(Poczatek+1<Koniec)
          {//sprawdzanie przedzialu
           SprawdzCzySaPrzekryciaPrzedzialu(Poczatek, Koniec, Wiersz);
          }

        if(PrzekryciaPojedynczo.size()>=IlePrzekrycSprawdzac)
          {//jesli osiagnieto ilosc szukanych przekryc
           return true;
          }
       }
   }
 else
   {
    for(int Wiersz=A-1; Wiersz!=B; Wiersz--)
       {//powtorzyc dla kazdego wiersza sprawdzanie odpowiedniego przedzialu
        int Poczatek=PoczatekMin;//+(PrzekryciaPewneMalejace?PrzekryciaPewneMalejace--:0);
        if(Wiersz+PrzekryciaPewne>PoczatekMin) Poczatek=Wiersz+PrzekryciaPewne;
        int Koniec=KoniecMax;//-(Sprawdzacz--?0:PrzekryciaPewneRosnace++);
        if(Wiersz+IloscSegm-PrzekryciaPewne<KoniecMax) Koniec=Wiersz+IloscSegm-PrzekryciaPewne;

        //W TEJ WERSJI POCZATEK SPRAWDZANEGO PRZEDZIALU MUSI BYC MNIEJSZY NIZ KONIEC
        if(Poczatek+1<Koniec)
          {//sprawdzanie przedzialu
           SprawdzCzySaPrzekryciaPrzedzialu(Poczatek, Koniec, Wiersz);
          }

        if(PrzekryciaPojedynczo.size()>=IlePrzekrycSprawdzac)
          {//jesli osiagnieto ilosc szukanych przekryc
           return true;
          }
        }
   }

 return PrzekryciaPojedynczo.size()?true:false;  //jesli nie wykryto przekryc ->false
}
//------------------------------------------------------------------------------
bool Knot3D::SprawdzCzySaPrzekryciaPrzedzialu(int Poczatek,
                                               int Koniec,
                                               int Wiersz)
{
double MinOdlKwadr=4.0000000001*PromienPrzekroju*PromienPrzekroju;
if(Poczatek+1>=Koniec||PrzekryciaPojedynczo.size()>=IlePrzekrycSprawdzac)
  {
   return false;
  };

int Koniec1,Poczatek1;

//wypelnij przerwe pomiedzy indeksami
//znajdz Srodek przedzialu
int Srodek=(Koniec+Poczatek)/2;

Wektor3D Odleglosc=wskTablicySegm[nrP(Wiersz,IloscSegm)]-wskTablicySegm[nrP(Srodek,IloscSegm)];

double OdlegloscPunktow=Odleglosc.lengthSq();

if(OdlegloscPunktow>MinOdlKwadr) //jesli nie ma przekrycia A->B
  {
   OdlegloscPunktow=sqrtl(OdlegloscPunktow);
   //odlozyc maxymalna 'ilosc' punktow na lewo i prawo, ktore na pewno nie 
   //przekrywaja sie z punktami o numerze wiersza
   int Ilosc=static_cast<int>((OdlegloscPunktow-2*PromienPrzekroju)/DlugoscSegmSrednia);

   //jesli calkowity przedzial jest krotszy niz podwojony lancuch
   //w jedna i druga strone srodka
   Koniec1=Srodek-Ilosc;
   Poczatek1=Srodek+Ilosc;
   //Koniec1=nrP(Srodek-Ilosc,IloscSegm);
   //Poczatek1=nrP(Srodek+Ilosc,IloscSegm);  // ta linijka wprowadza problem adresowania

   SprawdzCzySaPrzekryciaPrzedzialu(Poczatek, Koniec1, Wiersz);
   SprawdzCzySaPrzekryciaPrzedzialu(Poczatek1, Koniec, Wiersz);
  }
else
  {
   PrzekryciaPojedynczo.insert(nrP(Wiersz,IloscSegm));
   PrzekryciaPojedynczo.insert(nrP(Srodek,IloscSegm));
   Przekrycia.insert(Przekrycie(nrP(Wiersz,IloscSegm),
                               nrP(Srodek,IloscSegm),true));
   PrzekryciaSa=true;
   return true;
  };
}
//------------------------------------------------------------------------------
 bool Knot3D::UstawPromienPrzekroju(double Ile)   //ustawia nowy promien przekroju 
                                              //dla calego wezla na 'Ile';
                                              // jesli sie uda zwraca 1,
                                              //nie - 0
{
 PromienPrzekroju=Ile;
 PromienKrzywiznyMin=PromienPrzekroju; 
 KatGieciaMax=WyznaczKatGieciaMax();
 PromienMax=WyznaczPromienMax();
 SprawdzKrzywSrodk();
 SprawdzCzySaPrzekrycia();
}
//------------------------------------------------------------------------------
double Knot3D::WyznaczEnergieOdpych()  //upewnic sie, ze algorytm dobry
                                        //upewnic sie, ze algorytm dobry
                                        //upewnic sie, ze algorytm dobry
                                        //upewnic sie, ze algorytm dobry
                                        //upewnic sie, ze algorytm dobry
{
 double Energia=0.0;

 int IleLiczyc=75,
     IleSkakac=IloscSegm/IleLiczyc,
     i=0;

 Wektor3D Roznica;

 Wektor3D* wskPomoc1=wskTablicySegm;
 Wektor3D* wskPomoc2;

 for(;i<IloscSegm-IleSkakac;i+=IleSkakac)
    {
     wskPomoc2=wskPomoc1+IleSkakac;

     for(int j=i+IleSkakac;j<IloscSegm;j+=IleSkakac)
        {
         Roznica=*wskPomoc1-*wskPomoc2;
         Energia+=1.0/Roznica.length();
         wskPomoc2+=IleSkakac;
        };

     wskPomoc1+=IleSkakac;
    };

 Roznica=*wskPomoc1-wskTablicySegm[nrP(IloscSegm,i+IleSkakac)];
 Energia+=1.0/Roznica.length();

 return Energia;
}
//------------------------------------------------------------------------------
double Knot3D::WyznaczEnergieKrzyw()
{
 double Energia=0.0,Kat,Odleglosc;
 Wektor3D Roznica;
 Wektor3D* wskPomoc=wskTablicySegm;
 for(int i=0;i<IloscSegm-2;i++)
    {
     Roznica=*(wskPomoc+2)-*wskPomoc;
     Odleglosc=Roznica.lengthSq();
     Kat=sqrtl(4.0001*DlugoscSegmSrednia*DlugoscSegmSrednia-Odleglosc)
             /DlugoscSegmSrednia;
     Energia+=Kat;
     wskPomoc++;
    };
    
 Roznica=wskTablicySegm[IloscSegm-2]-wskTablicySegm[0];
 Odleglosc=Roznica.lengthSq();
 Kat=sqrtl(4.0001*DlugoscSegmSrednia*DlugoscSegmSrednia-Odleglosc)
             /DlugoscSegmSrednia;
 Energia+=Kat;
     
 Roznica=wskTablicySegm[IloscSegm-1]-wskTablicySegm[1];
 Odleglosc=Roznica.lengthSq();
 Kat=sqrtl(4.0001*DlugoscSegmSrednia*DlugoscSegmSrednia-Odleglosc)
         /DlugoscSegmSrednia;
 Energia+=Kat;

 return Energia;
}
//------------------------------------------------------------------------------
double Knot3D::WyznaczEnergieOdSrod()
{
 double Energia=0;
 Wektor3D* wskSegmWezla=wskTablicySegm;
 for(int i=0;i<IloscSegm;i++)
    {
     Energia+=1.0/((wskSegmWezla++)->length());
    };
 return Energia;
}
//------------------------------------------------------------------------------
Wektor3D Knot3D::WyznaczMomentor(Knot3D* const wskW,
                                  const Wektor3D& Srodek)
                                 //wyznacza momentor wzgledem drugiego wezla
                                 //i danego wektora
{                                 
 return ::WyznaczMomentor(wskW->wskTablicySegm,wskTablicySegm,IloscSegm,Srodek);  
}
//------------------------------------------------------------------------------
void Knot3D::UsunPed(Knot3D* const wskW)
{
 Wektor3D W=WyznaczSrodekMasy();
 W-=wskW->WyznaczSrodekMasy();
 Przesun(W);
}
//------------------------------------------------------------------------------
void Knot3D::UsunMomentor(Knot3D* const wskW)
{
 Macierz3D MacierzSredniaOdwrotna;//=wskW->WyznaczMacierzBezwl();
 MacierzSredniaOdwrotna=WyznaczMacierzBezwl();
 //MacierzSredniaOdwrotna*=0.5;
 MacierzSredniaOdwrotna=MacierzSredniaOdwrotna.inverse();         
 Wektor3D Kat=WyznaczMomentor(wskW);
 Kat=MacierzSredniaOdwrotna*Kat;
 ObracajWektor(Kat);
}
//------------------------------------------------------------------------------
void Knot3D::UsunMomentorW(Knot3D* const wskW, const Wektor3D& W)
{
 Wektor3D Momentor=WyznaczMomentor(wskW);
}
//---------------------------------------------------------------------------------------
void Knot3D::WypiszInfo()
{
 cout<<"Promien Przekroju: \t\t\t"<<PromienPrzekroju<<endl;
 cout<<"Dlugosc Wezla: \t\t\t\t"<<DlugoscSegmSrednia*IloscSegm<<endl;
 cout<<"Dlugosc Segmentow Srednia: \t\t"<<DlugoscSegmSrednia<<endl;
 cout<<"Promien Krzywizny Minimalny: \t\t"<<PromienKrzywiznyMin<<endl;
 cout<<"Promien maksymalny: \t\t\t"<<PromienMax<<endl;
 cout<<"Kat giecia maksymalny w stopniach: \t"<<KatGieciaMax*180.0/M_PI<<endl;
 cout<<"Kat giecia maksymalny w radianch: \t"<<KatGieciaMax<<endl;
 cout<<"Srodek Masy: \t\t\t\t"<<"["<<SrodekMasy<<"]"<<endl;
 //cout<<"WyznaczMacierzBezwl();
 //cout<<"A=WyznaczMomentorObrotu(Wektor3D(0,0,M_PI/2.0));
 //cout<<"MomentorPI2=A.length();
 //cout<<"MomentorChwilMax=MomentorWzglMax*MomentorPI2;
 
 //----------------------cout<<Przekrycia;
 cout<<endl; 
 _Rb_tree_const_iterator<Przekrycie> ItPrzekryc=Przekrycia.begin();
 int Rozmiar=Przekrycia.size();
 Przekrycie U;
 if(!Rozmiar)
   {
    cout<<"Brak przekryc."<<endl;
   }
 else
   {
    cout<<"Przekrycia punktow (pary numerow): ";
    for(int i=0;i<Rozmiar-1;i++) 
       {
        U=*ItPrzekryc;
        ItPrzekryc++;
        cout<<U<<",";
       };
    U=*ItPrzekryc;
    cout<<U;
   };
   cout<<endl;

 //------------------cout<<Krzywizna;
 _Rb_tree_const_iterator<int> ItKrzywizny=SegmentyPrzekrKrzyw.begin();
 Rozmiar=SegmentyPrzekrKrzyw.size();
 if(!Rozmiar)
   {
    cout<<"Krzywizna OK."<<endl;
   }
 else
   {
    cout<<"Krzywizne przekraczaja punkty (numery): ";
    for(int i=0;i<Rozmiar-1;i++) 
       {
        cout<<*ItKrzywizny++<<",";
       };
    cout<<*ItKrzywizny;
    cout<<endl;
   };
 cout<<endl;
}
//----------------------------------------------------------------------------------
void Knot3D::WyczyscPrzekrycia() //czysci zbiory przekrywajacych sie punktow
{
 PrzekryciaSa=false;
 Przekrycia.erase(Przekrycia.begin(),Przekrycia.end());
 PrzekryciaPojedynczo.erase(PrzekryciaPojedynczo.begin(),PrzekryciaPojedynczo.end()); 
}
//----------------------------------------------------------------------------------
void Knot3D::WyczyscSegmentyPrzekrKrzyw() //czysci zbiory przekrywajacych sie punktow
{
 KrzywiznaOK=true;
 SegmentyPrzekrKrzyw.erase(SegmentyPrzekrKrzyw.begin(),SegmentyPrzekrKrzyw.end());
}
//---------------------------------------------------------------------------------
int Knot3D::WyznaczPrzekryciaPewne(double DlugoscSrednia)
{
 if(!DlugoscSrednia)
   {
    PrzekryciaPewne=static_cast<int>
                    (M_PI_2/asinl(0.5*DlugoscSegmSrednia/PromienKrzywiznyMin));
   }
 else
   {
    PrzekryciaPewne=static_cast<int>
                    (M_PI_2/asinl(0.5*DlugoscSrednia/PromienKrzywiznyMin));
   }
 return PrzekryciaPewne;
}
//------------------------------------------------------------------------------
double Knot3D::Modul()
{
double Modul=0;
Wektor3D* wskSkoczek=wskTablicySegm;
for(int i=0;i<IloscSegm;i++)   Modul+=(*wskSkoczek++).length();
return Modul;
}
//------------------------------------------------------------------------------
Knot3D Knot3D::operator+(Knot3D& Plusowany)
{
int MniejszaIloscSegm=(IloscSegm<=Plusowany.IloscSegm)?IloscSegm:Plusowany.IloscSegm;
Knot3D Suma(*this);
Wektor3D* wskSkoczek1=Suma.wskTablicySegm;
Wektor3D* wskSkoczek2=Plusowany.wskTablicySegm;
for(int i=0;i<MniejszaIloscSegm;i++)   (*wskSkoczek1++)+=(*wskSkoczek2++);
return Suma;
}
//------------------------------------------------------------------------------
Knot3D Knot3D::operator-(Knot3D& Minusowany)
{
 int MniejszaIloscSegm=(IloscSegm<=Minusowany.IloscSegm)?IloscSegm:Minusowany.IloscSegm;
 Knot3D Roznica(*this);
 Wektor3D* wskSkoczek1=Roznica.wskTablicySegm;
 Wektor3D* wskSkoczek2=Minusowany.wskTablicySegm;
 for(int i=0;i<MniejszaIloscSegm;i++)   (*wskSkoczek1++)-=(*wskSkoczek2++);
 return Roznica;
}
//------------------------------------------------------------------------------
Knot3D Knot3D::operator*(double Mnozna)
{
 Knot3D Pomnozony(*this);
 Pomnozony.Skaluj(Mnozna);
 return Pomnozony;
}
//------------------------------------------------------------------------------
Knot3D Knot3D::operator/(double Dzielna)
{
 Knot3D Podzielony(*this);
 Skaluj(1.0/Dzielna);
 return Podzielony;
}
//------------------------------------------------------------------------------
Knot3D& Knot3D::operator+=(Knot3D& Plusowany)
{
int MniejszaIloscSegm=(IloscSegm<=Plusowany.IloscSegm)?IloscSegm:Plusowany.IloscSegm;
Wektor3D* wskSkoczek1=wskTablicySegm;
Wektor3D* wskSkoczek2=Plusowany.wskTablicySegm;
for(int i=0;i<MniejszaIloscSegm;i++)   (*wskSkoczek1++)+=(*wskSkoczek2++);
return *this;
}
//------------------------------------------------------------------------------
Knot3D& Knot3D::operator-=(Knot3D& Minusowany)
{
 int MniejszaIloscSegm=(IloscSegm<=Minusowany.IloscSegm)?IloscSegm:Minusowany.IloscSegm;
 Wektor3D* wskSkoczek1=wskTablicySegm;
 Wektor3D* wskSkoczek2=Minusowany.wskTablicySegm;
 for(int i=0;i<MniejszaIloscSegm;i++)   (*wskSkoczek1++)-=(*wskSkoczek2++);
 return *this;
}
//------------------------------------------------------------------------------
Knot3D& Knot3D::operator*=(double Mnozna)
{
 Skaluj(Mnozna);
 return *this;
}
//------------------------------------------------------------------------------
Knot3D& Knot3D::operator/=(double Dzielna)
{
 Skaluj(1.0/Dzielna);
 return *this;
}
//------------------------------------------------------------------------------
 void Knot3D::SkalujMin()
{
  while (!SprawdzCzySaPrzekryciaPoGieciu()) {
     WyczyscPrzekrycia();
     PromienMax=WyznaczPromienMax();

     if (PromienMax<PromienPrzekroju*1.05) return; //dla bezpieczenstwa ucieczka

     Skaluj(1.00001 - 0.1*PromienPrzekroju/PromienMax);
    };

 WyczyscPrzekrycia();
 SprawdzCzySaPrzekrycia();

 double odlegloscMin=PromienMax;
 Wektor3D Odl;

 set<Przekrycie,Porownanie>::iterator i=Przekrycia.begin();
 
 for(int j=Przekrycia.size();j--;i++) {
     Odl=wskTablicySegm[(*i).A]-wskTablicySegm[(*i).B];
     if(Odl.length()<odlegloscMin) odlegloscMin=Odl.length();
    }
 Skaluj(2.00002*double(PromienPrzekroju)*powl(odlegloscMin,-1));
}
//------------------------------------------------------------------------------
void Knot3D::WyrownajSegmentyPoLamanej(int nrSegmPocz, int NaIleRozlozycBlad)
{
 nrSegmPocz=nrP(nrSegmPocz,IloscSegm);

 if(NaIleRozlozycBlad<=0) NaIleRozlozycBlad=IloscSegm;

 double doublePomoc;
 double BrakujacaDlugoscSegm;
 double OdlegloscOdPoczStaregoSegm;
 WyznaczDlugoscSegmSrednia();

 Wektor3D* wskWektor3DStareSegm =new Wektor3D[IloscSegm];
 Wektor3D* wskWektor3DPomoc1;
 Wektor3D* wskWektor3DPomoc2;

 Wektor3D wekPomoc;

 wskWektor3DPomoc1=wskTablicySegm;
 wskWektor3DPomoc2=wskWektor3DStareSegm;

 for(int i=IloscSegm; i--; *wskWektor3DPomoc2++=*wskWektor3DPomoc1++);

 //rosnace indeksy
 int nrStarySegm=nrSegmPocz;
 int nrNowySegm=nrP(nrSegmPocz+1,IloscSegm);
 OdlegloscOdPoczStaregoSegm=0;
 BrakujacaDlugoscSegm=DlugoscSegmSrednia;

 for(int i=NaIleRozlozycBlad/2;i;) {
     nrStarySegm=nrP(nrStarySegm,IloscSegm);
     wekPomoc=wskWektor3DStareSegm[nrP(nrStarySegm+1,IloscSegm)]-wskWektor3DStareSegm[nrStarySegm];
     doublePomoc=wekPomoc.length();

    if(OdlegloscOdPoczStaregoSegm + BrakujacaDlugoscSegm > doublePomoc) {
        BrakujacaDlugoscSegm-=doublePomoc-OdlegloscOdPoczStaregoSegm;
        OdlegloscOdPoczStaregoSegm=0;
        nrStarySegm++;
        }
    else {
        OdlegloscOdPoczStaregoSegm+=BrakujacaDlugoscSegm;
        BrakujacaDlugoscSegm=DlugoscSegmSrednia;
        wskTablicySegm[nrP(nrNowySegm++,IloscSegm)]=wskWektor3DStareSegm[nrStarySegm]+(wekPomoc/doublePomoc)*OdlegloscOdPoczStaregoSegm;
        i--;
        }
    }

 //malejace indeksy
 nrStarySegm=nrSegmPocz;
 nrNowySegm=nrP(nrSegmPocz-1,IloscSegm);
 OdlegloscOdPoczStaregoSegm=0;
 BrakujacaDlugoscSegm=DlugoscSegmSrednia;

 for(int i=(NaIleRozlozycBlad-1)/2;i;) {
    nrStarySegm=nrP(nrStarySegm,IloscSegm);
    wekPomoc=wskWektor3DStareSegm[nrP(nrStarySegm-1,IloscSegm)]-wskWektor3DStareSegm[nrStarySegm];
    doublePomoc=wekPomoc.length();

    if(OdlegloscOdPoczStaregoSegm+BrakujacaDlugoscSegm>doublePomoc) {
        BrakujacaDlugoscSegm-=doublePomoc-OdlegloscOdPoczStaregoSegm;
        OdlegloscOdPoczStaregoSegm=0;
        nrStarySegm--;
        }
    else {
        OdlegloscOdPoczStaregoSegm+=BrakujacaDlugoscSegm;
        BrakujacaDlugoscSegm=DlugoscSegmSrednia;
        wskTablicySegm[nrP(nrNowySegm--,IloscSegm)]=wskWektor3DStareSegm[nrStarySegm]+(wekPomoc/doublePomoc)*OdlegloscOdPoczStaregoSegm;
        i--;
        }
    }
  delete[] wskWektor3DStareSegm;
}
//-----------------------------------------------------------------------------
void Knot3D::RozsunPrzekrycia(double Epsilon) //zaklada, ze juz sa wyznaczone
{
 if(Przekrycia.empty()) return;
 double Mnoznik;
 Wektor3D Odl,Srodek;

 set<Przekrycie,Porownanie>::iterator i;

 int SrodekPrzedzialu;
 
 while(SprawdzCzySaPrzekrycia()) {
    i=Przekrycia.begin();
    for(int j=Przekrycia.size();j--;i++) {
        Odl=wskTablicySegm[(*i).B]-wskTablicySegm[(*i).A];
        Srodek=wskTablicySegm[(*i).B]+wskTablicySegm[(*i).A];   Srodek*=0.5;
        Mnoznik=(PromienPrzekroju+0.5*Epsilon)/Odl.length();
        wskTablicySegm[(*i).A]=Srodek-Odl*Mnoznik;
        wskTablicySegm[(*i).B]=Srodek+Odl*Mnoznik;
        }
    WyczyscPrzekrycia();
    }
}
//----------------------------------------------------------------------------
void Knot3D::CenterMirror()
{
 Wektor3D zero(0,0,0);
 Wektor3D *wsk=wskTablicySegm;
 for(int i=IloscSegm;i;i--) *wsk++=zero-*wsk;
}
//------------------------------------------------------------------------------
//TODO:
//      -cos nie tak z energia krzywizny
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
