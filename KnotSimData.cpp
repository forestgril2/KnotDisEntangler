#ifndef KNOTSIMDATA_CPP
#define KNOTSIMDATA_CPP

#ifndef KNOTSIMDATA_H
#include "KnotSimData.h"
#endif

#include "funkcjezew.h"

//------------------------------------------------------------------------------------
bool KnotSimData::GenerujZTablicyGiec(GiecieWezla* wskGiec, long int iloscgiec,
    long int iloscudanych, long long int iloscwszystkichgiec,
    long long int iloscwszystkichudanychgiec, int liczbasegmentow)
{
  if (wskGiec == 0 || liczbasegmentow < 4 || iloscgiec < 1) return false;
  IloscGiec = iloscgiec;
  IloscUdanychGiec = iloscudanych;
  IloscWszystkichGiec = iloscwszystkichgiec;
  IloscWszystkichUdanychGiec = iloscwszystkichudanychgiec;

  //wygenerowac tablice danych odpowiednich typow z tablicy GiecieWezla
  int* wskTablicaA = new int[IloscUdanychGiec];
  int* wskTablicaOdlAB = new int[IloscUdanychGiec];
  double* wskKatyGiec = new double[IloscUdanychGiec];
  double* wskOdlrazyKat = new double[IloscUdanychGiec];

  GiecieWezla* skoczekGiec = wskGiec;
  int* skoczekint1 = wskTablicaA;
  int* skoczekint2 = wskTablicaOdlAB;
  double* skoczekdouble1 = wskKatyGiec;
  double* skoczekdouble2 = wskOdlrazyKat;

  for (int i = 0; i < IloscUdanychGiec; i++)
  {
    *skoczekint1++ = (skoczekGiec)->Segment1;
    *skoczekint2 = (skoczekGiec)->odlSegment12 - 1;
    *skoczekdouble1 = (skoczekGiec++)->Kat;
    *skoczekdouble2++ = (*skoczekint2++) * (*skoczekdouble1++);
  }
  //wygenerowac histogramy z tych tablic
  wskHistPunktyA = new Histogram<int>(wskTablicaA, 0, liczbasegmentow,
      iloscudanych, liczbasegmentow);
  wskHistOdlAB = new Histogram<int>(wskTablicaOdlAB, 2,
      (liczbasegmentow + 1) / 2 + 1, iloscudanych,
      (liczbasegmentow + 1) / 2 - 1);
  wskHistKatyGiec = new Histogram<double>(wskKatyGiec, iloscudanych, 200);
  double Odch = OdchylenieStand<double>(wskOdlrazyKat, iloscudanych, 0.0);

  long int* pointer;

  //tutaj zrobmy takie histogramy OdlAB, ze granice przedzialow ustalone sa dla zliczen <= 1/100 max
  {
    double PlusGranica = 6 * Odch;
    Histogram<double> Probny(wskOdlrazyKat, -6 * Odch, 6 * Odch, iloscudanych,
        200);
    pointer = Probny.wskTablicyPrzedzialow;
    Probny.IloscDanychPrzedzMax = ZnajdzMax(pointer, Probny.IloscPrzedz);
    int i = 0;
    for (; Probny.wskTablicyPrzedzialow[i] != Probny.IloscDanychPrzedzMax; i++)
      ; ///szukamy i startowe dotad OK
    do
      PlusGranica = Probny.PoczatekPrzedz + Probny.SzerokoscPrzedz * (i + 1);
    //cout<<"PlusGranica:   "<<PlusGranica<<endl<<endl<<flush;}
    while (Probny.IloscDanychPrzedzMax
        / (Probny.wskTablicyPrzedzialow[i] ? Probny.wskTablicyPrzedzialow[i] : 1)
        <= 100 && i++ < Probny.IloscPrzedz);

    wskHistOdlABrazyKatyGiec = new Histogram<double>(wskOdlrazyKat,
        -PlusGranica, PlusGranica, iloscudanych, 200);
  }

  pointer = wskHistPunktyA->wskTablicyPrzedzialow;
  wskHistPunktyA->IloscDanychPrzedzMax = ZnajdzMax(pointer,
      wskHistPunktyA->IloscPrzedz);

  pointer = wskHistOdlAB->wskTablicyPrzedzialow;
  wskHistOdlAB->IloscDanychPrzedzMax = ZnajdzMax(pointer,
      wskHistOdlAB->IloscPrzedz);

  pointer = wskHistKatyGiec->wskTablicyPrzedzialow;
  wskHistKatyGiec->IloscDanychPrzedzMax = ZnajdzMax(pointer,
      wskHistKatyGiec->IloscPrzedz);

  pointer = wskHistOdlABrazyKatyGiec->wskTablicyPrzedzialow;
  wskHistOdlABrazyKatyGiec->IloscDanychPrzedzMax = ZnajdzMax(pointer,
      wskHistOdlABrazyKatyGiec->IloscPrzedz);
}
//-----------------------------------------------------------------------------------
void KnotSimData::ZapiszBin(char* nazwapliku)
{
  ofstream strPliku;
  strPliku.open(nazwapliku, ios::binary | ios::out | ios::app);
  strPliku.write((char*) (&IloscGiec), sizeof(IloscGiec));
  strPliku.write((char*) (&IloscUdanychGiec), sizeof(IloscUdanychGiec));
  strPliku.write((char*) (&IloscWszystkichGiec), sizeof(IloscWszystkichGiec));
  strPliku.write((char*) (&IloscWszystkichUdanychGiec),
      sizeof(IloscWszystkichUdanychGiec));
  strPliku.close();

  wskHistPunktyA->ZapiszBin(nazwapliku);
  wskHistOdlAB->ZapiszBin(nazwapliku);
  wskHistKatyGiec->ZapiszBin(nazwapliku);
  wskHistOdlABrazyKatyGiec->ZapiszBin(nazwapliku);

  wskWezelSymulowany->ZapiszBin(nazwapliku);
}
//---------------------------------------------------------------------------------
bool KnotSimData::WczytajBin(char* nazwapliku, long int nr)
{
  ifstream strPliku;
  strPliku.open(nazwapliku, ios::binary | ios::in);
  if (strPliku.fail()) return false;
  strPliku.read((char*) &IloscGiec, sizeof(IloscGiec));
  strPliku.read((char*) &IloscUdanychGiec, sizeof(IloscUdanychGiec));
  strPliku.read((char*) &IloscWszystkichGiec, sizeof(IloscWszystkichGiec));
  strPliku.read((char*) &IloscWszystkichUdanychGiec,
      sizeof(IloscWszystkichUdanychGiec));

  cout << "--- HISTOGRAMY ---" << endl;
  cout << "wczytano IloscGiec = " << IloscGiec << endl;
  cout << "wczytano IloscUdanychGiec = " << IloscUdanychGiec << endl;
  cout << "wczytano IloscWszystkichGiec = " << IloscWszystkichGiec << endl;
  cout << "wczytano IloscWszystkichUdanychGiec = " << IloscWszystkichUdanychGiec
      << endl << endl << flush;

  cout << "--- Punkty A ---" << endl;
  wskHistPunktyA->WczytajBinStr(&strPliku);
  cout << "--- Odleglosci AB ---" << IloscGiec << endl;
  wskHistOdlAB->WczytajBinStr(&strPliku);
  cout << "--- Katy Giec ---" << IloscGiec << endl;
  wskHistKatyGiec->WczytajBinStr(&strPliku);
  cout << "--- Katy Razy Odleglosci ---" << IloscGiec << endl;
  wskHistOdlABrazyKatyGiec->WczytajBinStr(&strPliku);

  wskWezelSymulowany->WczytajBinOdtad(&strPliku);

  strPliku.close();

  return true;
}
//----------------------------------------------------------------------------------
ostream& operator<<(ostream & stru, KnotSimData & x)
{
  stru << "============= DANE SYMULACJI WEZLA ===================" << endl
      << endl;
  stru << "IloscGiec: \t\t\t" << x.IloscGiec << endl;
  stru << "IloscUdanychGiec: \t\t\t" << x.IloscUdanychGiec << endl << endl;
  stru << "IloscWszystkichGiec: \t\t\t" << x.IloscWszystkichGiec << endl;
  stru << "IloscWszystkichUdanychGiec: \t\t\t" << x.IloscWszystkichUdanychGiec
      << endl << endl;
  stru << "HISTOGRAM PUNKTOW A:" << endl << endl;
  stru << *(x.wskHistPunktyA) << endl;
  stru << "HISTOGRAM ODLEGLOSCI AB:" << endl << endl;
  stru << *(x.wskHistOdlAB) << endl;
  stru << "HISTOGRAM KATOW GIEC:" << endl << endl;
  stru << *(x.wskHistKatyGiec) << endl;
  stru << "HISTOGRAM ILOCZYNOW ODLEGLOSCI RAZY KATY:" << endl << endl;
  stru << *(x.wskHistOdlABrazyKatyGiec) << endl;
  stru << "WEZEL SYMULOWANY:" << endl;
  stru << *(x.wskWezelSymulowany) << endl << endl;
}
//-----------------------------------------------------------------------------------
void KnotSimData::ZapiszTxt(char* nazwapliku)
{
  ofstream strPliku;
  strPliku.setf(ios::fixed);
  strPliku.precision(16);
  strPliku.open(nazwapliku, ios_base::app);
  strPliku << *this;
  strPliku.close();
}
//-------------------------------------------------------------------------------
KnotSimData::KnotSimData() :
    IloscGiec(0), IloscUdanychGiec(0), IloscWszystkichGiec(0),
        IloscWszystkichUdanychGiec(0)

{
  wskHistPunktyA = new Histogram<int>();
  wskHistOdlAB = new Histogram<int>();
  wskHistKatyGiec = new Histogram<double>();
  wskHistOdlABrazyKatyGiec = new Histogram<double>();
}
//---------------------------------------------------------------------------------
KnotSimData::KnotSimData(GiecieWezla* wskGiec, long int iloscgiec,
    long int iloscudanych, long long int iloscwszystkichgiec,
    long long int iloscwszystkichudanychgiec, int liczbasegmentow)
{
  GenerujZTablicyGiec(wskGiec, iloscgiec, iloscudanych, iloscwszystkichgiec,
      iloscwszystkichudanychgiec, liczbasegmentow);
}
//-------------------------------------------------------------------------------
bool PrzeskalujHistogramy(int NowaIloscSegm)
{
  //wskHistPunktyA->UstawIloscPrzedz(NowaIloscSegm);
  //wskHistOdlAB->UstawIloscPrzedz((NowaIloscSegm+1)/2-1);
  //double stos=double(NowaIloscSegm)/double(wskHistPunktyA->IloscPrzedz);
  //wskHistOdlABrazyKatyGiec->UstawPoczKoniecPrzedz(stos*wskHistOdlABrazyKatyGiec->PoczatekPrzedz,stos*wskHistOdlABrazyKatyGiec->KoniecPrzedz);
}
//---------------------------------------------------------------------------------
#endif //KNOTSIMDATA_CPP

