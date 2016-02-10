#ifndef HISTOGRAM_CPP
#define HISTOGRAM_CPP

#ifndef HISTOGRAM_H
#include "Histogram.h"
#endif

#include "funkcjezew.h"

//-------------------------------------------------------------------------------
template<class TYP>
Histogram<TYP>::Histogram()
{
}
//-------------------------------------------------------------------------------
template<class TYP>
Histogram<TYP>::~Histogram()
{
 if(IloscPrzedz>1) delete[] wskTablicyPrzedzialow;
}
//-------------------------------------------------------------------------------
template<class TYP>
Histogram<TYP>::Histogram(TYP* wskNowejTablicyDanych,//konstruktor nie dba o poprawnosc danych!!!
		     long int NowaIloscDanych,  //moze na przyklad adresowac za tablice
		     int IloscPrzedzialow,
		     KierunekLiczenia NowyKierunek) :
				
			IloscDanych(NowaIloscDanych),
			KierunekLiczeniaDanych(NowyKierunek),
                        StalePrzedzialy(false)
{
  TYP* wskSkoczek;

 if(KierunekLiczeniaDanych=OdPoczTablicy){wskPoczatekTablicyDanych=wskNowejTablicyDanych;}
 else{wskKoniecTablicyDanych=wskNowejTablicyDanych;};

 if(IloscDanych<=0||wskNowejTablicyDanych==NULL) {//jesli za malo danych
    IloscDanych=0;
    DanaMax=0;
    DanaMin=0;
    IloscPrzedz=1;
    PoczatekPrzedz=0;
    KoniecPrzedz=1;
   }
 else {//jesli nie brak danych do kreacji
    if(IloscPrzedzialow>0) IloscPrzedz=IloscPrzedzialow;
    else IloscPrzedz=DOMYSLNA_ILOSC_PRZEDZIALOW_HISTOGRAMU;
 
    wskTablicyPrzedzialow=new long int[IloscPrzedz];
    long int* skoczeklong=wskTablicyPrzedzialow;
    for (int i=0;i<IloscPrzedz;i++) *(skoczeklong++)=0;

    if(KierunekLiczeniaDanych==OdPoczTablicy){//normalne zliczanie danych
       
       DanaMin=ZnajdzMin(wskPoczatekTablicyDanych,IloscDanych);
       DanaMax=ZnajdzMax(wskPoczatekTablicyDanych,IloscDanych);
       SzerokoscPrzedz=(DanaMax-DanaMin)/IloscPrzedz;
       PoczatekPrzedz=DanaMin;
       KoniecPrzedz=DanaMin+IloscPrzedz*SzerokoscPrzedz*1.0000001;

       wskSkoczek=wskPoczatekTablicyDanych;

       for (long int i=0; i<IloscDanych; i++) wskTablicyPrzedzialow[static_cast<int>((*(wskSkoczek++)-DanaMin)/SzerokoscPrzedz)]++;
      }
    else{//zliczanie danych od konca tablicy
         DanaMin=ZnajdzMin(wskKoniecTablicyDanych,-IloscDanych+1);
	 DanaMax=ZnajdzMax(wskKoniecTablicyDanych,-IloscDanych+1);
         SzerokoscPrzedz=(DanaMax-DanaMin)/IloscPrzedz;
 	 PoczatekPrzedz=DanaMin;
         KoniecPrzedz=DanaMin+IloscPrzedz*SzerokoscPrzedz*1.0000001;

         for(long int i=IloscDanych; i>0; i++) wskTablicyPrzedzialow[static_cast<int>((*(wskSkoczek++)-DanaMin)/SzerokoscPrzedz)]++;
	}
   }
  IloscDanychPrzedzMax=ZnajdzMax(wskTablicyPrzedzialow, IloscPrzedz);
}
//---------------------------------------------------------------------------------------------------------------
template<class TYP>
Histogram<TYP>::Histogram(TYP* wskNowejTablicyDanych,	//wersja o stalych przedzialach
		  TYP poczatek,
		  TYP koniec,
		  long int NowaIloscDanych,
		  int IloscPrzedzialow,
		  KierunekLiczenia NowyKierunek) : 
			
			IloscDanych(NowaIloscDanych),
			KierunekLiczeniaDanych(NowyKierunek),
			PoczatekPrzedz(poczatek),
 			KoniecPrzedz(koniec),
			StalePrzedzialy(true)
{
 TYP* wskSkoczek;

 if(KierunekLiczeniaDanych=OdPoczTablicy){wskPoczatekTablicyDanych=wskNowejTablicyDanych;}
 else{wskKoniecTablicyDanych=wskNowejTablicyDanych;};

 if(IloscDanych<=0||wskNowejTablicyDanych==NULL) {//jesli za malo danych
    IloscDanych=0;
    DanaMax=0;
    DanaMin=0;
    IloscPrzedz=1;
    PoczatekPrzedz=0;
    KoniecPrzedz=1;
   }
 else {//jesli nie brak danych do kreacji
    if(IloscPrzedzialow>0) IloscPrzedz=IloscPrzedzialow;
    else IloscPrzedz=DOMYSLNA_ILOSC_PRZEDZIALOW_HISTOGRAMU;
 
    wskTablicyPrzedzialow=new long int[IloscPrzedz];
    long int* skoczeklong=wskTablicyPrzedzialow;
    for (int i=0;i<IloscPrzedz;i++) *(skoczeklong++)=0;

    if(KierunekLiczeniaDanych==OdPoczTablicy){//normalne zliczanie danych

       DanaMin=ZnajdzMin<TYP>(wskPoczatekTablicyDanych,IloscDanych);
       DanaMax=ZnajdzMax<TYP>(wskPoczatekTablicyDanych,IloscDanych);
       SzerokoscPrzedz=(KoniecPrzedz-PoczatekPrzedz)/IloscPrzedz;


       wskSkoczek=wskPoczatekTablicyDanych;
       for(long int i=0; i<IloscDanych; i++) {
          if (((*wskSkoczek)>=PoczatekPrzedz)&&((*wskSkoczek)<KoniecPrzedz))
             if (*wskSkoczek>=PoczatekPrzedz&&*wskSkoczek<KoniecPrzedz)
             wskTablicyPrzedzialow[int((*(wskSkoczek)-PoczatekPrzedz)/SzerokoscPrzedz)]++;
             wskSkoczek++;
          }

      }
    else{//zliczanie danych od konca tablicy
         DanaMin=ZnajdzMin(wskKoniecTablicyDanych,-IloscDanych+1);
	 DanaMax=ZnajdzMax(wskKoniecTablicyDanych,-IloscDanych+1);
         SzerokoscPrzedz=(KoniecPrzedz-PoczatekPrzedz)/IloscPrzedz;

         for(long int i=IloscDanych; i>0; i++) if (*wskSkoczek>=PoczatekPrzedz&&*wskSkoczek<KoniecPrzedz) wskTablicyPrzedzialow[static_cast<int>((*(wskSkoczek--)-PoczatekPrzedz)/SzerokoscPrzedz)]++;
	};
   };
 IloscDanychPrzedzMax=ZnajdzMax(wskTablicyPrzedzialow,IloscPrzedz);
}
//---------------------------------------------------------------------------------
template<class TYP>
void Histogram<TYP>::UstawIloscPrzedz(int NowaIloscPrzedz)
{
//wskNowejTablicyPrzedzialow=new long int[NowaIloscPrzedz];
//double NowaSzerokoscPrzedzialu=(KoniecPrzedz-PoczatekPrzedz)/NowaIloscPrzedz;
}
//---------------------------------------------------------------------------------
template<class TYP>
void Histogram<TYP>::ZapiszTxt(char* Nazwa)   //zapisuje do pliku sekwencyjnego 
{
 ofstream strPliku;
 strPliku.setf(ios::fixed);
 strPliku.precision(16);
 strPliku.open(Nazwa,ios_base::app);
 strPliku<<*this;
 strPliku.close(); 
}
//-----------------------------------------------------------------------------
template<class TYP>
void Histogram<TYP>::ZapiszBin(char* Nazwa)   //zapisuje do pliku sekwencyjnego 
{
 ofstream strPliku;
 strPliku.open(Nazwa,ios::binary|ios::out|ios::app);
 strPliku.write((char*)(&IloscDanych),sizeof(long int));
 strPliku.write((char*)(&IloscPrzedz),sizeof(int));
 strPliku.write((char*)(&SzerokoscPrzedz),sizeof(TYP));
 strPliku.write((char*)(&PoczatekPrzedz),sizeof(TYP));
 strPliku.write((char*)(&KoniecPrzedz),sizeof(TYP));
 strPliku.write((char*)(&StalePrzedzialy),sizeof(bool));
 strPliku.write((char*)(&DanaMin),sizeof(TYP));	
 strPliku.write((char*)(&DanaMax),sizeof(TYP));
 strPliku.write((char*)(&IloscDanychPrzedzMax),sizeof(IloscDanychPrzedzMax));
 for(int i=0;i<IloscPrzedz;i++) strPliku.write((char*)(&wskTablicyPrzedzialow[i]),sizeof(long int));
 strPliku.close();
}
//--------------------------------------------------------------------------------------------
template<class TYP>
void Histogram<TYP>::WczytajBinStr(ifstream* wskstrPliku)
{
  wskstrPliku->read((char*)(&IloscDanych),sizeof(IloscDanych));
 cout<<"wczytano IloscDanych = "<<IloscDanych<<endl;

 wskstrPliku->read((char*)(&IloscPrzedz),sizeof(IloscPrzedz));
 cout<<"wczytano IloscPrzedz = "<<IloscPrzedz<<endl;

 wskstrPliku->read((char*)(&SzerokoscPrzedz),sizeof(SzerokoscPrzedz));
 cout<<"wczytano SzerPrzedz = "<<SzerokoscPrzedz<<endl;

 wskstrPliku->read((char*)(&PoczatekPrzedz),sizeof(PoczatekPrzedz));
 cout<<"wczytano PoczPrzedz = "<<PoczatekPrzedz<<endl;

 wskstrPliku->read((char*)(&KoniecPrzedz),sizeof(KoniecPrzedz));
 cout<<"wczytano KonPrzedz = "<<KoniecPrzedz<<endl;

 wskstrPliku->read((char*)(&StalePrzedzialy),sizeof(StalePrzedzialy));
 cout<<"wczytano StalePrzedzialy = "<<StalePrzedzialy<<endl<<flush;

 wskstrPliku->read((char*)(&DanaMin),sizeof(DanaMin));	
 cout<<"wczytano DanaMin = "<<DanaMin<<endl;

 wskstrPliku->read((char*)(&DanaMax),sizeof(DanaMax));	
 cout<<"wczytano DanaMax = "<<DanaMax<<endl;


 wskstrPliku->read((char*)(&IloscDanychPrzedzMax),sizeof(IloscDanychPrzedzMax));
 cout<<"wczytano IloscDanychPrzedzMax = "<<IloscDanychPrzedzMax<<endl<<endl<<flush;

 delete[] wskTablicyPrzedzialow;

 wskTablicyPrzedzialow=new long int[IloscPrzedz];

 for (int i=0;i<IloscPrzedz;i++) wskstrPliku->read((char*)(&wskTablicyPrzedzialow[i]),sizeof(IloscPrzedz));

}

template class Histogram<int>;
template class Histogram<double>;
//---------------------------------------------------------------------------------------------------------
#endif

