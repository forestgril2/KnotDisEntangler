#ifndef KNOTPLIKER_CPP
#define KNOTPLIKER_CPP

#include <fstream>
#include <iostream>

#ifndef WEKTOR3D_H
#include "Wektor3D.h"
#endif

#ifndef GIECIEWEZLA_H
#include "GiecieWezla.h"
#endif

#ifndef KNOT3D_H
#include "Knot3D.h"
#endif

#ifndef KNOTPLIKER_H
#include "KnotPliker.h"
#endif

//----------------------------------------------------------------------------------------
int KnotPliker::WczytajPlik(char* nazwa)
{
  //cout.setf(ios::fixed);
  //cout.precision(4);
  int ProcentWczytany = 0, Nowy;
  //plik binarny do odczytu
  NazwaPliku = new char[64];
  strcpy(NazwaPliku, nazwa);
  strPW.open(nazwa, ios::binary);
  wskstrPW = &strPW;
  DaneWezlow.erase(DaneWezlow.begin(), DaneWezlow.end());
  wskstrPW->seekg(0, ios_base::end);
  KoniecPliku = static_cast<long int>(wskstrPW->tellg());
  IloscWezlow = 0;
  wskstrPW->seekg(0, ios_base::beg);
  long int poczatekpliku = static_cast<long int>(wskstrPW->tellg());
  long int roznica = KoniecPliku - poczatekpliku;

  //dane wezla:
  long int Pozycja;
  int Segm;
  int Giecia;
  cout << "Wczytywanie pozycji wezlow w pliku" << " " << NazwaPliku << "...."
      << ProcentWczytany << "%\r";
  cout << flush;
  for (; wskstrPW->tellg() < KoniecPliku; IloscWezlow++)
  {
    Pozycja = static_cast<long int>(wskstrPW->tellg());
    Nowy = int(100 * float(Pozycja) / float(roznica));

    if (Nowy > ProcentWczytany)
    {
      cout << "Wczytywanie pozycji wezlow w pliku" << " " << NazwaPliku
          << "...." << ProcentWczytany << "%\r";
      ProcentWczytany = Nowy;
      cout << flush;
    }

    wskstrPW->read((char*) &Segm, sizeof(Segm));
    wskstrPW->seekg(sizeof(Wektor3D) * Segm, ios_base::cur);
    wskstrPW->read((char*) &Giecia, sizeof(Giecia));
    wskstrPW->seekg(sizeof(GiecieWezla) * Giecia, ios_base::cur);

    DaneWezlow.insert(DaneWezlow.end(), DaneWezlaPliku(Pozycja, Segm, Giecia));
  };

  cout << "Wczytywanie pozycji wezlow w pliku " << NazwaPliku
      << "....100%  Zrobione!" << endl << endl;
}
//-------------------------------------------------------------------------------------------
void KnotPliker::WczytajKnot(Knot3D* wskWezla, long int Numer)
{
  if (Numer < 0)
  {
    Numer = AktualnyKnot = 0;
    WczytajKnot(wskWezla, AktualnyKnot);
  }

  if (Numer >= IloscWezlow)
  {
    Numer = AktualnyKnot = IloscWezlow - 1;
    return WczytajKnot(wskWezla, AktualnyKnot);
  }
  else
  {
    AktualnyKnot = Numer;
    int StaraIloscSegm = wskWezla->IloscSegm;
    wskstrPW->seekg(DaneWezlow[Numer].PozycjaPliku);

    //przeniesione prawie zywcem z Knot3D::WczytajBin(ifstream*,int)
    wskstrPW->read((char*) &wskWezla->IloscSegm, sizeof(wskWezla->IloscSegm)); //wczyt ilosc elem
    if (wskWezla->IloscSegm <= 0)
    {
      wskWezla->IloscSegm = StaraIloscSegm;
      return;
    }
    
    delete[] wskWezla->wskTablicySegm;
    wskWezla->wskTablicySegm = new Wektor3D[wskWezla->IloscSegm];
    Wektor3D* wskPomoc = wskWezla->wskTablicySegm;
    
    for (int i = 0; i < wskWezla->IloscSegm; i++)
    {
      wskstrPW->read((char*) wskPomoc, sizeof(Wektor3D));
      wskPomoc++;
    }

    wskstrPW->read((char*) &wskWezla->IloscGiec, sizeof(wskWezla->IloscGiec));
    wskWezla->OstatnieGiecia.resize(wskWezla->IloscGiec);
    for (int i = 0; i < wskWezla->IloscGiec; i++)
    {
      wskstrPW->read((char*) (&wskWezla->OstatnieGiecia[i]),
          sizeof(GiecieWezla));
    }
    wskWezla->WyznaczWszystko();
    return;
  }
}
//---------------------------------------------------------------------------------------------
void KnotPliker::ZapiszCoIle(char* wskNazwy, int CoIle, long int Start,
    long int Koniec)
{
  int ProcentZapisu = 0, Nowy;
  Knot3D W;
  if (Koniec <= 0 || Koniec >= IloscWezlow) Koniec = IloscWezlow - 1;
  if (Start <= 0) Start = 0;
  if (Start >= IloscWezlow) Start = IloscWezlow - 1;

  long int i;
  for (i = Start; i <= Koniec; i += CoIle)
  {
    WczytajKnot(&W, i);
    W.ZapiszBin(wskNazwy);

    Nowy = int(100 * float(i - Start) / float(Koniec));

    if (Nowy > ProcentZapisu)
    {
      cout << "Zapisywanie wezlow w pliku...." << Nowy << "%\r";
      ProcentZapisu = Nowy;
      cout << flush;
    }
  }

  if (i - CoIle < Koniec)
  {
    WczytajKnot(&W, Koniec);
    W.ZapiszBin(wskNazwy);
  }

  cout << "Zapisywanie wezlow w pliku.... 100%   Zrobione!" << endl << endl;
}
//----------------------------------------------------------------------------------------------

#endif
