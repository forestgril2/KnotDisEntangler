#ifndef KNOTSIMULATOR_CPP
#define KNOTSIMULATOR_CPP

#ifndef KNOTSIMULATOR_H
#include "KnotSimulator.h"
#endif

#ifndef SONO_H
#include "SONO.h"
#endif

#ifndef FUNKCJEZEW_H
#include "funkcjezew.h"
#endif

//---------------------------------------------------------------------------------------------
KnotSimulator::KnotSimulator(char* a,char* b,char* c,char* d,char* e)
{
     Knot3D W(3);
     Symulowany=W;
     // Pomoc=Knot3D(7);	

      wskNazwyDomyslna=a;
      wskNazwyWynikAuto=b;
      wskNazwyWynikMoja=c;
      wskNazwyWynikAutotxt=d;
      wskNazwyWynikMojatxt=e;

     wskstrPlikuBin=&strPlikuBin;
     wskstrPlikuMojaBin=&strPlikuMojaBin; 

     wskHistogramySymulacji=new KnotSimData;
     PierwszyRazHistogram=false;

     //jaka czesc maksymalnej wartosci w przedzialach zliczania uwzzlgedniac 
     ParametrPunktuA=0.85;
     ParametrOdleglosciAB=0.45;
     ParametrZmianyWezla=200;

     nrmojebin=0;  			//nr wezla w moim pliku binarnym
     nrmojetxt=0;  			//nr wezla w moim pliku binarnym
     nrtxt=0;				//nr wezla w pliku txt
     defaultIleRazySONO=100;		//ile wykonywac shrinkow miedzy wizualizacja eliminujac przekrycia
     defaultIleRazyBONO=10000;		//ile wykonywac giec miedzy wizualizacja eliminujac przekrycia
     CoIleUdanychGiecZapis=200000;	
     CoIleSONOZapis=50000;

          LicznikStanowNizszejEnergii=0;//ile razy w czasie BONO uzyskano nisza energie
          LicznikStanowWyzszejEnergii=0;//ile razy w czasie BONO uzyskano wyzsza energie
          LicznikNieudanychGiec=0;	//ile giec spowodowalo przekrycia
          LicznikUdanychGiec=0;		//ile bylo giec bez przekryc
          LicznikProbGiec=0;		//ile bylo wszystkich prob giec
          LicznikSONO=0;		//ile razy suma SONO
          LicznikBONO=0;		//ile razy w sumie gieto BONO
          prevLicznikStanowNizszejEnergii=0;//ile razy w czasie BONO uzyskano nisza energie
          prevLicznikStanowWyzszejEnergii=0;//ile razy w czasie BONO uzyskano wyzsza energie
          prevLicznikNieudanychGiec=0;	//ile giec spowodowalo przekrycia
          prevLicznikUdanychGiec=0;		//ile bylo giec bez przekryc
          prevLicznikProbGiec=0;		//ile bylo wszystkich prob giec
          prevLicznikSONO=0;		//ile razy suma SONO
          prevLicznikBONO=0;		//ile razy w sumie gieto BONO


        Epsilon=1E-10;			//dopuszczalna odchylka
        StosunekPomocniczy=1;		//stosunek EnergiiOdpychania do EnergiiKrzywizny???
        Energia=1;
        EnergiaNowa=1;
        EnergiaPocz=1;			//Poczatkowa energia wezla ???????????????????? kiedy?
        StosunekEnergiiDoPocz=1;	//iloraz energii do poczatkowej
        StosunekEnergiiDoPoczMin=1;	//minimalny odnotowany iloraz energii do poczatkowej
        StosunekEnTermDoEnPocz=0.0001;  //tutaj zawarta jest temperatura ukladu
        StosunekKataDoMax=1;		//dla bezpieczenstwa<1
        WagaEnergiiOdpych=2;		//energiom przypisane sa wagi
        StosunekGestosciStanow=0.5,	//ilosc stanow wynikajaca z energii
        ZmianaKatrazyOdl=0,
        prevZmianaKatrazyOdl=0,


        prevTime=time(0);		

     SONO=0;  				//czy wlaczony algorytm
     BONO=0;  				//czy wlaczony algorytm

}
//--------------------------------------------------------------------------------------------------------------
void KnotSimulator::UstawParametry()
{
    wskTablicyUdanychGiec=new GiecieWezla[CoIleUdanychGiecZapis];

    wskTablicyPomocniczejWektorow=new Wektor3D[Symulowany.IloscSegm];
    Symulowany.UstawPromienPrzekroju(0.5);
    Symulowany.WyznaczWszystko();
    Symulowany.Przesun(Wektor3D(0,0,0)-Symulowany.SrodekMasy);
    PromienMaksObiektu=Symulowany.PromienMax;
    Poczatkowy=Pomoc=Symulowany;
    WagaEnergiiOdpych=10*Symulowany.WyznaczEnergieKrzyw()/Symulowany.WyznaczEnergieOdpych();///---------------------

    EnergiaPocz=Energia=EnergiaNowa=Symulowany.WyznaczEnergieKrzyw();//+WagaEnergiiOdpych*Symulowany.WyznaczEnergieOdpych();
					///W.WyznaczEnergieOdSrod();

    DlugoscWezla=DlugoscWezlaPoczatkowa=Symulowany.IloscSegm*Symulowany.DlugoscSegmSrednia;

    Symulowany.IlePrzekrycSprawdzac=Symulowany.IloscSegm;
    Symulowany.IlePunktowKrzywSprawdzac=Symulowany.IloscSegm;

    PrzygotujSONO();

    //EnergiaZerowa=
}
//--------------------------------------------------------------------------------------------------------------
void KnotSimulator::PrzygotujSONO()
{
 ZZ.Wprowadz((WK3D*)Symulowany.wskTablicySegm, Symulowany.IloscSegm);
 ZZ.Szybkosc(2);
}
//---------------------------------------------------------------------------------------------------------------
void KnotSimulator::ZastosujAlgorytmy(long int IleRazyBONO,long int IleRazySONO)
{
 if(!SONO&&!BONO) return;
 else if (!IleRazyBONO&&!IleRazySONO) {
         IleRazyBONO=defaultIleRazyBONO;
         IleRazySONO=defaultIleRazySONO;
         }
 Knot3D& W=Symulowany;
 
 if(SONO) //algorytm zaciskania
   {
    if(!(LicznikSONO%CoIleSONOZapis))  //zapisywanie do pliku
      {
       W.ZapiszTxt(wskNazwyWynikAutotxt);
       W.ZapiszBin(wskNazwyWynikAuto);
      }
    for(int i=0;i<IleRazySONO;i++)
       {
        ZZ.Zaciskaj();
        LicznikSONO++;
       }
        //nieusuwanie dlugosci
    DlugoscWezlaPoczatkowa=W.WyznaczDlugoscSegmSrednia()*W.IloscSegm;
    DlugoscWezla=W.WyznaczDlugoscSegmSrednia()*W.IloscSegm;
  }

 if(BONO)  //ALGORYTM ROZPLATUJACY
   {
    //wczytany ostatnio wezel jest wzorcem dlugosci:
    DlugoscWezlaPoczatkowa=Pomoc.IloscSegm*Pomoc.DlugoscSegmSrednia; 

    double KatGiecia;
    int ZapamietaneIlePrzekrycSprawdzac;
    int ZapamietaneIlePunktowKrzywSprawdzac;

    {//bedziemy odrzucac kazde przekrycie i kazde przekroczenie krzywizny
     ZapamietaneIlePrzekrycSprawdzac=W.IlePrzekrycSprawdzac;
     ZapamietaneIlePunktowKrzywSprawdzac=W.IlePunktowKrzywSprawdzac;
     W.IlePrzekrycSprawdzac=1;
     W.IlePunktowKrzywSprawdzac=1;
    }

    for(int i=0;i<IleRazyBONO;i++)          //PETLA OBLICZEN
       {
        LicznikBONO++; //zwiekszamy licznik podstawowy

        {//LOSOWANIA
         if (PierwszyRazHistogram) {//wyznaczenie "naturalnego" histogramu
            W.UstawPunktGiecia1(W.Losownik.LosujInt(W.IloscSegm-1));
            W.UstawPunktGiecia2(W.Losownik.LosujInt(W.IloscSegm-1));

            W.KatGieciaMax=W.WyznaczKatGieciaMax();  //ta liczba powinna zalezec od gestosci kulek!

            KatGiecia=W.Losownik.LosujLongDouble(-StosunekKataDoMax*W.KatGieciaMax,W.KatGieciaMax*StosunekKataDoMax);
            }
	 else {//korzystamy z wyznaczonego histogramu uwzgledniajac parametry sterujace :
               //ParametrPunktuA,ParametrOdleglosciAB,ParametrZmianyWezla
             int baza;

             funkcja1:
            	int probka=W.Losownik.LosujInt(W.IloscSegm-1); W.UstawPunktGiecia1(probka); ///---------<----UWAGA!!!
              baza=wskHistogramySymulacji->wskHistPunktyA->IloscDanychPrzedzMax;
            	if (W.Losownik.LosujInt(baza*ParametrPunktuA)<wskHistogramySymulacji->wskHistPunktyA->wskTablicyPrzedzialow[probka]) 
                    {
                    W.UstawPunktGiecia1(probka);
                    }
                else goto funkcja1;


             funkcja2:
/*            	probka=static_cast<int>(W.Losownik.LosujLongDouble(W.IloscSegm/2,(W.IloscSegm-1)/2));
                if (probka<0) probka=-probka;
                if (probka>(W.IloscSegm-1)/2) probka=(W.IloscSegm-1)/2-probka;
*/
                probka=W.Losownik.LosujInt(W.IloscSegm/2-1);
                //W.UstawPunktGiecia2(W.PunktGiecia1+probka);
                baza=wskHistogramySymulacji->wskHistOdlAB->IloscDanychPrzedzMax;
            	if (W.Losownik.LosujInt(baza*ParametrOdleglosciAB)<wskHistogramySymulacji->wskHistOdlAB->wskTablicyPrzedzialow[probka])  {
                    W.UstawPunktGiecia2(W.PunktGiecia1+probka);
                    }
                else goto funkcja2;
	     //if (W.Losownik.LosujLongDouble()<0.8) 
		//W.UstawPunktGiecia2(W.PunktGiecia1+W.Losownik.LosujInt((W.IloscSegm-1)/2,2));

             funkcja3:
            	int probka2;
                baza=wskHistogramySymulacji->wskHistOdlABrazyKatyGiec->IloscDanychPrzedzMax;
		double Los=W.Losownik.LosujGaussa(ParametrZmianyWezla);
                //if (W.Losownik.LosujLongDouble()<0.1) 
			//Los=W.Losownik.LosujGaussa(3);
                //if (Los>=-4.0 &&Los <4.0) probka2=int((Los+4.0)/0.08);
                //else goto funkcja3;
            	//if (W.Losownik.LosujInt(baza)<
			//wskHistogramySymulacji->wskHistOdlABrazyKatyGiec->wskTablicyPrzedzialow[probka2]) 
		    KatGiecia=Los/probka;
		if (KatGiecia<-StosunekKataDoMax*W.KatGieciaMax) KatGiecia=-StosunekKataDoMax*W.KatGieciaMax;
		if (KatGiecia>StosunekKataDoMax*W.KatGieciaMax) KatGiecia=StosunekKataDoMax*W.KatGieciaMax;

            }
        }

        //gniemy mniejsza czesc dla oszczednosci czasu:
        if(odlP(W.PunktGiecia1,W.PunktGiecia2,W.IloscSegm)>W.IloscSegm/2) 
          {
           W.UstawPunktGiecia1(W.PunktGiecia2);  //zamiana punktow
           //odrzucamy polowe wyborow dla odleglosci rownej 2 (okolo polowy to odrzucone wybory dla odleglosci 1)
	   if(odlP(W.PunktGiecia1,W.PunktGiecia2,W.IloscSegm)==2&&W.Losownik.LosujInt(1)) {	
              W.UstawPunktGiecia2(W.Losownik.LosujInt(W.IloscSegm-1));
              if (odlP(W.PunktGiecia1,W.PunktGiecia2,W.IloscSegm)>W.IloscSegm/2) {
                  W.UstawPunktGiecia1(W.PunktGiecia2);
                  }
              }
           KatGiecia=-KatGiecia;
          }

        {//zapamietanie stanu wyginanej czesci przed gieciem:
           if(W.PunktGiecia1<W.PunktGiecia2)  //tablica dzielona albo nie
             {//niedzielona
              wskPomocniczyWektora1=&(wskTablicyPomocniczejWektorow[W.PunktGiecia1+1]);
              wskPomocniczyWektora2=&(W.wskTablicySegm[W.PunktGiecia1+1]);
              for(int i=W.PunktGiecia1+1;i<W.PunktGiecia2;i++)
                 {
                  *wskPomocniczyWektora1++=*wskPomocniczyWektora2++;
                 }
             }
           else
             {//dzielona
              wskPomocniczyWektora1=&(wskTablicyPomocniczejWektorow[W.PunktGiecia1+1]);
              wskPomocniczyWektora2=&(W.wskTablicySegm[W.PunktGiecia1+1]);
              for(int i=W.PunktGiecia1+1;i<W.IloscSegm;i++)
                 {
                  *wskPomocniczyWektora1++=*wskPomocniczyWektora2++;
                 }
              wskPomocniczyWektora1=wskTablicyPomocniczejWektorow;
              wskPomocniczyWektora2=W.wskTablicySegm;
              for(int i=0;i<W.PunktGiecia2;i++)
                 {
                  *wskPomocniczyWektora1++=*wskPomocniczyWektora2++;
                 }
             }
        }
        {//GIECIE WEZLA
         KatGiecia=W.Zegnij(KatGiecia);   
         //W.DodajGiecie(GiecieWezla(W.PunktGiecia1,W.PunktGiecia2,KatGiecia,true)); //zapis do listy
        }
        {//czyszczenie zbioru przekryc i giec
         W.WyczyscPrzekrycia();                //ZAPOMINAMY O WYZNACZONYCH POPRZEDNIO PRZEKRYCIACH...
	 W.WyczyscSegmentyPrzekrKrzyw();       //ZAPOMINAMY O WYZNACZONEJ POPRZEDNIO KRZYWIZNIE...
        }
	
        if(W.PunktGiecia1<W.PunktGiecia2) //okreslenie przedzialu dla potrzeb sprawdzania przekryc
          {//jesli giecie niedzielone
            A=W.PunktGiecia1;
            B=W.PunktGiecia2;
           }
        else
          {//jesli giecie dzielone
            A=W.PunktGiecia1;
            B=W.PunktGiecia2+W.IloscSegm;
           }

        if(W.SprawdzCzySaPrzekryciaPoGieciu(A,B) ///uwarunkowania podstawowe (krzywizna i twardosc)
           || !W.SprawdzKrzywProm(W.PunktGiecia1,
                                  W.PunktGiecia1+1==W.IloscSegm?
                                  W.IloscSegm-2:W.PunktGiecia1+1)
           || !W.SprawdzKrzywProm(W.PunktGiecia2,
                                  W.PunktGiecia2+1==W.IloscSegm?
                                  W.IloscSegm-2:W.PunktGiecia2+1))

           {  //przywracamy stan poprzedni ale najpierw zwiekszamy ilosc nieudanych giec:
              LicznikNieudanychGiec++;
              //W.Zegnij(-KatGiecia); //albo kopiowanie tablicy, jak ponizej
              if(W.PunktGiecia1<W.PunktGiecia2) //tablica dzielona lub nie
                {//niedzielona
                wskPomocniczyWektora1=&(wskTablicyPomocniczejWektorow[W.PunktGiecia1+1]);
                wskPomocniczyWektora2=&(W.wskTablicySegm[W.PunktGiecia1+1]);
                for(int i=W.PunktGiecia1+1;i<W.PunktGiecia2;i++)
                   {
                    *wskPomocniczyWektora2++=*wskPomocniczyWektora1++;
                   }
               }
              else
                {//dzielona
                wskPomocniczyWektora1=&(wskTablicyPomocniczejWektorow[W.PunktGiecia1+1]);
                wskPomocniczyWektora2=&(W.wskTablicySegm[W.PunktGiecia1+1]);
                for(int i=W.PunktGiecia1+1;i<W.IloscSegm;i++)
                   {
                    *wskPomocniczyWektora2++=*wskPomocniczyWektora1++;
                   }
                wskPomocniczyWektora1=wskTablicyPomocniczejWektorow;
                wskPomocniczyWektora2=W.wskTablicySegm;
                for(int i=0;i<W.PunktGiecia2;i++)
                   {
                    *wskPomocniczyWektora2++=*wskPomocniczyWektora1++;
                   }
                }
             }
         else                            ///uwarunkowania energetyczne
           {//najpierw zwiekszamy licznik udanych giec
            LicznikUdanychGiec++;

            //obliczenie zmiany wezla
            double zmiana=KatGiecia*(odlP(W.PunktGiecia1,W.PunktGiecia2,W.IloscSegm)-1)/Symulowany.IloscSegm;
            ZmianaKatrazyOdl+=zmiana<0?-zmiana:zmiana;

            if (PierwszyRazHistogram){//zapisujemy giecie do tablicy udanych giec miedzy zapisami
                wskTablicyUdanychGiec[LicznikUdanychGiec%CoIleUdanychGiecZapis]=
                                 GiecieWezla(W.PunktGiecia1,
                                             odlP(W.PunktGiecia1,W.PunktGiecia2,W.IloscSegm),
                                             KatGiecia,
                                             true);}

            if(!(LicznikUdanychGiec-prevLicznikUdanychGiec-CoIleUdanychGiecZapis)) {//zapisywanie do pliku
               //W.ZapiszTxt(wskNazwyWynikAutotxt); za dużo zajmuje miejsca na dysku :/
               W.ZapiszBin(wskNazwyWynikAuto);
               PlikerWynikow.IloscWezlow++;
 
               if(PierwszyRazHistogram) {//stosuje na razie tylko pierwszy histogram, bo pozniej degeneracja
                        //tworzy sie KnotSimData dla aktualnych parametrow
                        delete wskprevHistogramySymulacji;
	       		wskprevHistogramySymulacji=wskHistogramySymulacji; //odniesienie dla nastepnego zapisu
  
               		wskHistogramySymulacji=new KnotSimData(wskTablicyUdanychGiec,
						      LicznikBONO-prevLicznikBONO,
						      LicznikUdanychGiec-prevLicznikUdanychGiec,
						      W.IloscSegm);
		
			wskHistogramySymulacji->ZapiszBin("KnotSimData.bin");
                	wskHistogramySymulacji->ZapiszTxt("histo.txt");
                        }

		PierwszyRazHistogram=false;

                {//zapis stanu licznikow dla odniesienia w nastepnym zapisie
    		 prevLicznikStanowNizszejEnergii=LicznikStanowNizszejEnergii;	
    		 prevLicznikStanowWyzszejEnergii=LicznikStanowWyzszejEnergii;
    		 prevLicznikNieudanychGiec=LicznikNieudanychGiec;	
    		 prevLicznikUdanychGiec=LicznikUdanychGiec;		
    		 prevLicznikProbGiec=LicznikProbGiec;		
    		 prevLicznikSONO=LicznikSONO;			
    		 prevLicznikBONO=LicznikBONO;
                 ZmianaNaSek=prevZmianaNaSek=double(ZmianaKatrazyOdl)/double(time(0)-prevTime);
                 prevTime=time(0);
                 prevZmianaKatrazyOdl=ZmianaKatrazyOdl;
                 ZmianaKatrazyOdl=0;
    		}
              }
           else  ZmianaNaSek=double(ZmianaKatrazyOdl)/double(time(0)-prevTime);
	/* 	
              {//OBLICZENIE NOWEJ ENERGII WEZLA
             EnergiaNowa=W.WyznaczEnergieKrzyw();//+WagaEnergiiOdpych*W.WyznaczEnergieOdpych();
			///W.WyznaczEnergieOdSrod();
               }
            if(Energia<EnergiaNowa)
              {//jesli nowy stan ma wyzsza energie, najpierw zwiekszamy odpowiedni licznik:
               LicznikStanowWyzszejEnergii++;
               //obliczamy Stosunek Gestosci Stanow dla tych energii
               StosunekGestosciStanow=powl(M_E,(Energia-EnergiaNowa)/(StosunekEnTermDoEnPocz*EnergiaPocz));
               if(W.Losownik.LosujLongDouble()>=StosunekGestosciStanow) //losujemy liczbe
                 {//jesli nie zmiescilismy sie w tej liczbie (mniejszej niz 1) //przywracamy stan poprzedni:
                  //W.Zegnij(-KatGiecia);
                  if(W.PunktGiecia1<W.PunktGiecia2) //tablica dzielona lub nie
                    {//niedzielona
                     wskPomocniczyWektora1=&(wskTablicyPomocniczejWektorow[W.PunktGiecia1+1]);
                     wskPomocniczyWektora2=&(W.wskTablicySegm[W.PunktGiecia1+1]);
                     for(int i=W.PunktGiecia1+1;i<W.PunktGiecia2;i++)
                        {
                         *wskPomocniczyWektora2++=*wskPomocniczyWektora1++;
                        }
                    }
                  else
                    {//dzielona
                     wskPomocniczyWektora1=&(wskTablicyPomocniczejWektorow[W.PunktGiecia1+1]);
                     wskPomocniczyWektora2=&(W.wskTablicySegm[W.PunktGiecia1+1]);
                     for(int i=W.PunktGiecia1+1;i<W.IloscSegm;i++)
                        {
                         *wskPomocniczyWektora2++=*wskPomocniczyWektora1++;
                        }
                     wskPomocniczyWektora1=wskTablicyPomocniczejWektorow;
                     wskPomocniczyWektora2=W.wskTablicySegm;
                     for(int i=0;i<W.PunktGiecia2;i++)
                        {
                         *wskPomocniczyWektora2++=*wskPomocniczyWektora1++;
                        }
                    }
                   }
                 else
                   {//zostawiamy, wiec Nowa WYZSZA Energia wchodzi w wezel.... ;)
                    Energia=EnergiaNowa;
                   }
                }
            else
              {//zostawiamy, wiec Nowa  NIZSZA Energia wchodzi w wezel.... ;) 
               //(zwiekszamy tez licznik stanow nizszej energii
               LicznikStanowWyzszejEnergii++;
               Energia=EnergiaNowa;
              } */

           }
       }
		

    {//przywracamy ustawienia sprawdzania krzywizny i przekryc
     W.IlePrzekrycSprawdzac=ZapamietaneIlePrzekrycSprawdzac;
     W.IlePunktowKrzywSprawdzac=ZapamietaneIlePunktowKrzywSprawdzac;
    }
   }
 
 Symulowany.WyznaczWszystko();
//*
 {//ENERGIA WEZLA
  Energia=Symulowany.WyznaczEnergieKrzyw();//+WagaEnergiiOdpych*Symulowany.WyznaczEnergieOdpych();
			///Symulowany.WyznaczEnergieOdSrod();
  StosunekEnergiiDoPocz=Energia/EnergiaPocz;
  StosunekEnergiiDoPoczMin=StosunekEnergiiDoPocz<StosunekEnergiiDoPoczMin?
                           StosunekEnergiiDoPocz:
                           StosunekEnergiiDoPoczMin;
 }//*/

 {//usuwanie dlugosci
  DlugoscWezlaPoczatkowa=Pomoc.WyznaczDlugoscSegmSrednia()*Pomoc.IloscSegm;
  DlugoscWezla=Symulowany.WyznaczDlugoscSegmSrednia()*Symulowany.IloscSegm;
  StosunekPomocniczy=DlugoscWezlaPoczatkowa/DlugoscWezla;
  if(StosunekPomocniczy-1.0>Epsilon
     ||StosunekPomocniczy-1.0<-Epsilon) //jesli wzgledna roznica dlugosci jest wieksza niz 1E-10
    {//probujemy przeskalowac wezel
    Symulowany.Skaluj(StosunekPomocniczy);
     if(Symulowany.SprawdzCzySaPrzekrycia()  //jesli skalujac niechcacy przekrylim
        || !Symulowany.SprawdzKrzywProm()    //albo niechcacy przekrzywilim 
        || !Symulowany.SprawdzKrzywProm())
       {//skalowanie z powrotem
        Symulowany.Skaluj(1.0/StosunekPomocniczy);
       }
    }
 }

 {//ZASADY ZACHOWANIA:
  Symulowany.Przesun(Wektor3D(0,0,0)-W.WyznaczSrodekMasy());
  Symulowany.UsunMomentor(&Pomoc);
  Symulowany.UsunMomentor(&Pomoc);
 }

}

//------------------------------------------------------------------------------------------------------------------
void KnotSimulator::WczytajPlik(char* nazwapliku)
{
 PlikerWynikow.WczytajPlik(nazwapliku);
 PlikerWynikow.WczytajKnot(&Symulowany,PlikerWynikow.IloscWezlow-1);
}
//-------------------------------------------------------------------------------------------------------
void KnotSimulator::WczytajKnot(MiejsceNaLiscie miejsce,long int numer)
{
 switch(miejsce)
 {
  case pierwszy:
     PlikerWynikow.WczytajKnot(&Symulowany,0);
       break;

 case wybrany:
     PlikerWynikow.WczytajKnot(&Symulowany,numer);
       break;

 case ostatni:
     PlikerWynikow.WczytajKnot(&Symulowany,PlikerWynikow.IloscWezlow-1);
       break;

 default:
     PlikerWynikow.WczytajKnot(&Symulowany,0);
       break;

 }
}
//----------------------------------------------------------------------------
void KnotSimulator::ZamknijPlik()
{
    wskstrPlikuBin->close();
    //wskstrPlikuMojaBin->close();
}
//-------------------------------------------------------------------------
Knot3D& KnotSimulator::WskazKnotSymulowany()
{
 return Symulowany;
}
//-------------------------------------------------------------------------------
void KnotSimulator::InfoKonsolowe()
{
  DlugoscWezlaPoczatkowa=Pomoc.DlugoscSegmSrednia*Pomoc.IloscSegm; 
  DlugoscWezla=Symulowany.DlugoscSegmSrednia*Pomoc.IloscSegm;

  cout<<"Dlugosc pocz: \t"<<DlugoscWezlaPoczatkowa<<"\t|";
        cout<<"Energ. pocz.: \t"<<EnergiaPocz<<"\t|";
              cout<<"LicznikBONO: \t"<<LicznikBONO<<"\t|";
                    cout<<"PromienMax: \t"<<Symulowany.PromienMax<<"\t|";
  cout<<endl;
  cout<<"Dlugosc: \t"<<DlugoscWezla<<"\t|";
        cout<<"Czas od zapisu: \t"<<time(0)-prevTime<<"\t|";
              cout<<"Licznik giec: \t"<<LicznikUdanychGiec<<"\t\t|";
                    cout<<"Zmiana wezla/sek: \t"<<double(ZmianaKatrazyOdl)/double(time(0)-prevTime)<<"\t|";
  cout<<endl;
  cout<<"Stos. dl.: \t"<<DlugoscWezla/DlugoscWezlaPoczatkowa<<"\t|";
        cout<<"Stos. en.: \t"<<StosunekEnergiiDoPocz<<"\t|";
              cout<<"Stos. giec: \t"<<double(LicznikUdanychGiec-prevLicznikUdanychGiec)/double(LicznikBONO-prevLicznikBONO+1)<<"\t|";
                    cout<<"StosunekEnTermDoEnPocz: \t"<<StosunekEnTermDoEnPocz<<"\t|";
  cout<<endl;
  cout<<"Co ile BONO: \t \t"<<defaultIleRazyBONO<<"\t|";
        cout<<"Ilosc Wezlow w Pliku: \t"<<PlikerWynikow.IloscWezlow<<"\t|";
              cout<<"Nazwa pliku: \t"<<wskNazwyWynikAuto<<"\t|";
                    cout<<"Ostatni wczytany nr: \t"<<PlikerWynikow.AktualnyKnot<<"\t|";
  cout<<endl;
  cout<<endl;
 }
//-----------------------------------------------------------------------
 void KnotSimulator::UstawCoIleUdanychGiecZapis(long int coile)
{
 long int stare=CoIleUdanychGiecZapis;
 CoIleUdanychGiecZapis=coile;
 if (stare==coile) return;
 if (stare>CoIleUdanychGiecZapis) {//dwa przypadki
    int modulostare=LicznikUdanychGiec%stare;
    int modulonowe=LicznikUdanychGiec%CoIleUdanychGiecZapis;
    if (modulonowe==modulostare) {//nie przekroczono jeszcze giec zapisu dla nowego mniejszego 
       //tworzy sie nowa mniejsza tablice i przepisuje do niej
       }
    else {//przekroczono ilosc giec zapisu dla nowego mniejszego 
       //tworzy sie KnotSimData dla tego, co jest,zapisuje, zapisuje wezel, 
       }
    }
 else {//zwiekszyc rozmiar tablicy giec i przepisac to, co jest
 
    }
}
//----------------------------------------------------------------------
 void KnotSimulator::UstawCoIleSONOZapis(long int coile)
{
 CoIleSONOZapis=coile;
}
//------------------------------------------------------------------------
#endif




