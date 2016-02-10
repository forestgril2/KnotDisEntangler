#ifndef KNOTSIMDATA_H
#define KNOTSIMDATA_H

#ifndef HISTOGRAM_H
#include "Histogram.h"
#endif

#ifndef KNOT3D_H
#include "Knot3D.h"
#endif

class KnotSimData
{
public:
  ///-----------funkcje-------------------------
  KnotSimData();

  KnotSimData(GiecieWezla* wskGiec, long int iloscgiec, long int iloscudanych,
      long long int iloscwszystkichgiec,
      long long int iloscwszystkichudanychgiec, int liczbasegmentow);
  //konstruktor kopiujacy
  KnotSimData(const KnotSimData&);

  bool GenerujZTablicyGiec(GiecieWezla* wskGiec, long int iloscgiec,
      long int iloscudanych, long long int iloscwszystkichgiec,
      long long int iloscwszystkichudanychgiec, int liczbasegmentow);

  bool PrzeskalujHistogramy(int NowaIloscSegm);

  bool WczytajBin(char* nazwapliku, long int nr = 0);
  void ZapiszBin(char* nazwapliku);
  void ZapiszTxt(char* nazwapliku);

  void WypiszInfo();

  //operatory
  double Modul();
  KnotSimData& operator+=(KnotSimData& Plusowany);
  KnotSimData& operator-=(KnotSimData& Minusowany);
  KnotSimData& operator*=(double);
  KnotSimData& operator/=(double);
  KnotSimData operator+(KnotSimData& Plusowany);
  KnotSimData operator-(KnotSimData& Minusowany);
  KnotSimData operator*(double);
  KnotSimData operator/(double);

  //operatory strumieniowe
  friend ostream& operator<<(ostream & stru, KnotSimData & x);
  //wypisywanie wezla
  friend istream& operator>>(istream& stru, KnotSimData & x);

  ///------------dane/zmienne---------------------
  long int IloscGiec;
  long int IloscUdanychGiec;
  long long int IloscWszystkichGiec;
  long long int IloscWszystkichUdanychGiec;

  Knot3D* wskWezelSymulowany;

  Histogram<int>* wskHistPunktyA;

  Histogram<int>* wskHistOdlAB; //zawsze <= niz polowa ilosci segmentow

  Histogram<double>* wskHistKatyGiec;

  Histogram<double>* wskHistOdlABrazyKatyGiec;
};

#endif

//	-histogramy...
//      -optymalizacja parametrow symulacji w biegu 
//		-kat giecie
//		-odchylenie standardowe
//		-proba funkcji roznicy wezla
//              -losowanie punktu 2 w dobrej odleglosci (odch stand tez)

