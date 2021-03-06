#ifndef KNOTSIMULATOR_CPP
#define KNOTSIMULATOR_CPP

#ifndef KNOTSIMULATOR_H
#include "KnotSimulator.h"
#endif

#ifndef LOSOWANIA_H
#include "Losowania.h"
#endif

#ifndef FUNKCJEZEW_H
#include "funkcjezew.h"
#endif

//---------------------------------------------------------------------------------------------
KnotSimulator::KnotSimulator(char* a, char* b, char* c, char* d, char* e)
{
  Knot3D W(3);
  Symulowany = W;
  // Pomoc=Knot3D(7);

  wskNazwyDomyslna = a;
  wskNazwyWynikAuto = b;
  wskNazwyWynikMoja = c;
  wskNazwyWynikAutotxt = d;
  wskNazwyWynikMojatxt = e;

  wskstrPlikuBin = &strPlikuBin;
  wskstrPlikuMojaBin = &strPlikuMojaBin;

  wskHistogramySymulacji = new KnotSimData;
  PierwszyRazHistogram = true;

  //jaka czesc maksymalnej wartosci w przedzialach zliczania uwzzlgedniac
  ParametrPunktuA = 0.85; //0.85;
  ParametrOdleglosciAB = 0.65; //0.45;
  OdchStandParametruZmianyWezla = 5000;

  nrmojebin = 0;  			//nr wezla w moim pliku binarnym
  nrmojetxt = 0;  			//nr wezla w moim pliku binarnym
  nrtxt = 0;				//nr wezla w pliku txt
  defaultIleRazySONO = 5000;//ile wykonywac shrinkow miedzy wizualizacja eliminujac przekrycia
  defaultIleRazyBONO = 5000;//ile wykonywac giec miedzy wizualizacja eliminujac przekrycia
  CoIleUdanychGiecZapis = 50000;
  CoIleSONOZapis = 50000;
  CoIleUdanychNaturalnychGiecHistogram = 50000;
  StosunekGiecNaturalnych = 0.0;

  LicznikStanowWiekszegoPrawdopodobienstwa = 0;	//ile razy w czasie BONO uzyskano nisza energie
  LicznikStanowMniejszegoPrawdopodobienstwa = 0;//ile razy w czasie BONO uzyskano wyzsza energie
  LicznikNieudanychGiec = 0;	//ile giec spowodowalo przekrycia
  LicznikUdanychGiec = 0;		//ile bylo giec bez przekryc
  LicznikProbGiec = 0;		//ile bylo wszystkich prob giec
  LicznikNieudanychShrink = 0;
  LicznikUdanychShrink = 0;
  LicznikSONO = 0;		//ile razy suma SONO
  LicznikBONO = 0;                //ile razy w sumie gieto BONO
  LicznikUdanychNaturalnychGiec = 0;
  prevLicznikUdanychNaturalnychGiec = 0;
  prevLicznikStanowWiekszegoPrawdopodobienstwa = 0; //ile razy w czasie BONO uzyskano nisza energie
  prevLicznikStanowMniejszegoPrawdopodobienstwa = 0; //ile razy w czasie BONO uzyskano wyzsza energie
  prevLicznikNieudanychGiec = 0;	//ile giec spowodowalo przekrycia
  prevLicznikUdanychGiec = 0;		//ile bylo giec bez przekryc
  prevLicznikUdanychGiecZapisu = 0;
  prevLicznikProbGiec = 0;		//ile bylo wszystkich prob giec
  prevLicznikSONO = 0;		//ile razy suma SONO
  prevLicznikBONO = 0;		//ile razy w sumie gieto BONO
  prevLicznikBONOHistogramow = 0; //licznikBONO podczas ostatniego zapisu histogramow

  Metropolis = off;
  Temperatura = 1.0;
  Epsilon = 1E-10;			//dopuszczalna odchylka
  StosunekPomocniczy = 1;	//stosunek ZmiennejMetropolisaOdpychania do ZmiennejMetropolisaKrzywizny???
  ZmiennaMetropolisa = 1;
  ZmiennaMetropolisaNowa = 1;
  ZmiennaMetropolisaPocz = 1;	//Poczatkowa energia wezla ???????????????????? kiedy?
  StosunekZmiennejMetropolisaDoPocz = 1;//iloraz ZmiennejMetropolisa do poczatkowej
  StosunekZmiennejMetropolisaDoPoczMin = 1;	//minimalny odnotowany iloraz ZmiennejMetropolisa do poczatkowej
  StosunekEnTermDoEnPocz = 0.0001;  //tutaj zawarta jest temperatura ukladu
  StosunekKataDoMax = 1.0;		//dla bezpieczenstwa<1
  WagaZmiennejMetropolisaOdpych = 2;		//energiom przypisane sa wagi
  SumaNizszychPrawdpodobienstwPrzejsc = 0,//ilosc stanow wynikajaca z ZmiennejMetropolisa
  prevSumaNizszychPrawdpodobienstwPrzejsc = 0, ZmianaKatrazyOdl = 0, prevZmianaKatrazyOdl =
      0,

  prevTime = time(0);

  SONO = 0;  				//czy wlaczony algorytm
  BONO = 0;  				//czy wlaczony algorytm

}
//--------------------------------------------------------------------------------------------------------------
void KnotSimulator::UstawParametry()
{
  delete[] wskTablicyUdanychGiec;
  wskTablicyUdanychGiec = new GiecieWezla[CoIleUdanychGiecZapis];
  LicznikUdanychNaturalnychGiec = 0;
  prevLicznikUdanychNaturalnychGiec = 0;

  delete[] wskTablicyPomocniczejWektorow;
  wskTablicyPomocniczejWektorow = new Wektor3D[Symulowany.IloscSegm];
  Symulowany.UstawPromienPrzekroju(0.5);
  Symulowany.WyznaczWszystko();
  Symulowany.Przesun(Wektor3D(0, 0, 0) - Symulowany.SrodekMasy);
  PromienMaksObiektu = Symulowany.PromienMax;
  Poczatkowy = Pomoc = Symulowany;

  ZmiennaMetropolisaNowa = 0.0;
  ZmiennaMetropolisa = ZmiennaMetropolisaNowa;
  ZmiennaMetropolisaPocz = ZmiennaMetropolisa;

  DlugoscWezla = DlugoscWezlaPoczatkowa = Symulowany.IloscSegm
      * Symulowany.DlugoscSegmSrednia;

  Symulowany.IlePrzekrycSprawdzac = Symulowany.IloscSegm;
  Symulowany.IlePunktowKrzywSprawdzac = Symulowany.IloscSegm;

  prevTime = time(0);
  prevClock = clock();

  //if(!PierwszyRazHistogram) wskHistogramySymulacji->PrzeskalujHistogramy();
}
//---------------------------------------------------------------------------------------------------------------
void KnotSimulator::ZastosujAlgorytmy(long int IleRazyBONO,
    long int IleRazySONO)
{
  if (!SONO && !BONO)
    return;
  else if (!IleRazyBONO && !IleRazySONO)
  {
    IleRazyBONO = defaultIleRazyBONO;
    IleRazySONO = defaultIleRazySONO;
  }

  if (BONO)  //ALGORYTM ROZPLATUJACY
  {
    //wczytany ostatnio wezel jest wzorcem dlugosci:
    DlugoscWezlaPoczatkowa = Pomoc.IloscSegm * Pomoc.DlugoscSegmSrednia;

    int ZapamietaneIlePrzekrycSprawdzac;
    int ZapamietaneIlePunktowKrzywSprawdzac;

    {  //bedziemy odrzucac kazde przekrycie i kazde przekroczenie krzywizny
      ZapamietaneIlePrzekrycSprawdzac = Symulowany.IlePrzekrycSprawdzac;
      ZapamietaneIlePunktowKrzywSprawdzac = Symulowany.IlePunktowKrzywSprawdzac;
      Symulowany.IlePrzekrycSprawdzac = 1;
      Symulowany.IlePunktowKrzywSprawdzac = 1;
    }

    { //PETLA OBLICZEN
      if (Metropolis == off)
        for (int i = 0; i < IleRazyBONO; i++)
          ZastosujBONO();
      else if (Metropolis == DistRep)
        for (int i = 0; i < IleRazyBONO; i++)
          ZastosujBONO(DistantRepulsiveKnotEnergy,
              StosunekGestosciStanowBoltzmanna);
      else if (Metropolis == DistAttr)
        for (int i = 0; i < IleRazyBONO; i++)
          ZastosujBONO(DistantAttractiveKnotEnergy,
              StosunekGestosciStanowBoltzmanna);
      else if (Metropolis == CloseRep)
        for (int i = 0; i < IleRazyBONO; i++)
          ZastosujBONO(CloseRepulsiveKnotEnergy,
              StosunekGestosciStanowBoltzmanna);
      else
        for (int i = 0; i < IleRazyBONO; i++)
          ZastosujBONO(CloseAttractiveKnotEnergy,
              StosunekGestosciStanowBoltzmanna);
    }

    { //przywracamy ustawienia sprawdzania krzywizny i przekryc
      Symulowany.IlePrzekrycSprawdzac = ZapamietaneIlePrzekrycSprawdzac;
      Symulowany.IlePunktowKrzywSprawdzac = ZapamietaneIlePunktowKrzywSprawdzac;
    }
  }

  if (SONO) // wlacza algorytm zaciskania
  {
    if (!BONO && !(LicznikSONO % CoIleSONOZapis))  //zapisywanie do pliku
    {
      Symulowany.ZapiszBin(wskNazwyWynikAuto);
    }

    for (int i = 0; i < IleRazySONO; i++)
      ZastosujSONO();
    Symulowany.WyrownajSegmentyPoLamanej(
        Losownik.LosujInt(Symulowany.IloscSegm));
    Symulowany.RozsunPrzekrycia();

    LicznikSONO++;
    Pomoc = Symulowany;
    //zapamietanie nowej dlugosci
    DlugoscWezlaPoczatkowa = Symulowany.WyznaczDlugoscSegmSrednia()
        * Symulowany.IloscSegm;
    DlugoscWezla = DlugoscWezlaPoczatkowa;
  }

  Symulowany.WyznaczWszystko();
//*
  {  //Zmienna w algorytmie metropolisa-hastingsa
    if (Metropolis == off)
      ZmiennaMetropolisa = 0;
    else if (Metropolis == DistRep)
      ZmiennaMetropolisa = DistantRepulsiveKnotEnergy(&Symulowany);
    else if (Metropolis == DistAttr)
      ZmiennaMetropolisa = DistantAttractiveKnotEnergy(&Symulowany);
    else if (Metropolis == CloseRep)
      ZmiennaMetropolisa = CloseRepulsiveKnotEnergy(&Symulowany);
    else
      ZmiennaMetropolisa = CloseAttractiveKnotEnergy(&Symulowany);

    StosunekZmiennejMetropolisaDoPocz = ZmiennaMetropolisa
        / ZmiennaMetropolisaPocz;
    StosunekZmiennejMetropolisaDoPoczMin =
        StosunekZmiennejMetropolisaDoPocz
            < StosunekZmiennejMetropolisaDoPoczMin ?
            StosunekZmiennejMetropolisaDoPocz :
            StosunekZmiennejMetropolisaDoPoczMin;
  }  //*/

  {  //usuwanie zmian dlugosci

    DlugoscWezlaPoczatkowa = Pomoc.WyznaczDlugoscSegmSrednia()
        * Pomoc.IloscSegm;
    DlugoscWezla = Symulowany.WyznaczDlugoscSegmSrednia()
        * Symulowany.IloscSegm;
    StosunekPomocniczy = DlugoscWezlaPoczatkowa / DlugoscWezla;
    if (StosunekPomocniczy - 1.0 > Epsilon
        || StosunekPomocniczy - 1.0 < -Epsilon) //jesli wzgledna roznica dlugosci jest wieksza niz 1E-10
    { //probujemy przeskalowac wezel
      Symulowany.Skaluj(StosunekPomocniczy);
      if (Symulowany.SprawdzCzySaPrzekrycia() //jesli skalujac niechcacy przekrylim
      || !Symulowany.SprawdzKrzywProm()    //albo niechcacy przekrzywilim
          || !Symulowany.SprawdzKrzywProm())
      {    //skalowanie z powrotem
        Symulowany.Skaluj(1.0 / StosunekPomocniczy);
      }
    }
  }

  {    //ZASADY ZACHOWANIA:
    Symulowany.Przesun(Wektor3D(0, 0, 0) - Symulowany.WyznaczSrodekMasy());
    Symulowany.UsunMomentor(&Pomoc);
    Symulowany.UsunMomentor(&Pomoc);
  }

}

//------------------------------------------------------------------------------------------------------------------
void KnotSimulator::WczytajPlik(char* nazwapliku)
{
  PlikerWynikow.WczytajPlik(nazwapliku);
  PlikerWynikow.WczytajKnot(&Symulowany, PlikerWynikow.IloscWezlow - 1);
}
//-------------------------------------------------------------------------------------------------------
void KnotSimulator::WczytajKnot(MiejsceNaLiscie miejsce, long int numer)
{
  switch (miejsce)
  {
    case pierwszy:
      PlikerWynikow.WczytajKnot(&Symulowany, 0);
      break;

    case wybrany:
      PlikerWynikow.WczytajKnot(&Symulowany, numer);
      break;

    case ostatni:
      PlikerWynikow.WczytajKnot(&Symulowany, PlikerWynikow.IloscWezlow - 1);
      break;

    default:
      PlikerWynikow.WczytajKnot(&Symulowany, 0);
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
  DlugoscWezlaPoczatkowa = Pomoc.DlugoscSegmSrednia * Pomoc.IloscSegm;
  DlugoscWezla = Symulowany.DlugoscSegmSrednia * Pomoc.IloscSegm;

  char MetroMode[16];

  switch (Metropolis)
  {

    case DistRep:
      strcpy(MetroMode, "DistRep");
      break;
    case DistAttr:
      strcpy(MetroMode, "DistAttr");
      break;
    case CloseRep:
      strcpy(MetroMode, "CloseRep");
      break;
    case CloseAttr:
      strcpy(MetroMode, "CloseAttr");
      break;

    default:
    case off:
      strcpy(MetroMode, "OFF");
      break;
  }

  CzasMiniety = ((double) (clock() - prevClock)) / CLOCKS_PER_SEC;

  cout << "Dlugosc: \t" << DlugoscWezla << "\t|";
  cout << "Zmienna Metropolisa\t" << ZmiennaMetropolisa << "\t|";
  cout << "LicznikBONO: \t\t" << LicznikBONO << "\t|";
  cout << "Srednie Prawd. Nizsz.: \t"
      << double(SumaNizszychPrawdpodobienstwPrzejsc)
          / double(
              LicznikStanowMniejszegoPrawdopodobienstwa
                  - prevLicznikStanowMniejszegoPrawdopodobienstwa) << "\t|";

  cout << endl;
  cout << "Mniejsze P.: \t"
      << double(
          LicznikStanowMniejszegoPrawdopodobienstwa
              - prevLicznikStanowMniejszegoPrawdopodobienstwa)
          / double(LicznikUdanychGiec - prevLicznikUdanychGiec) << "\t|";

  cout << "Czas od zapisu: \t\t" << time(0) - prevTime << "\t|";
  cout << "Licznik giec: \t" << LicznikUdanychGiec << "\t\t|";
  cout << "Zmiana wezla/sek: \t"
      << (ZmianaKatrazyOdl - prevZmianaKatrazyOdl) / CzasMiniety << "\t|";
  cout << endl;
  cout << "Tryb Metropolis: \t" << MetroMode << "\t|";
  cout << "Temperatura: \t\t" << Temperatura << "\t|";
  cout << "Stos. giec: \t"
      << double(LicznikUdanychGiec - prevLicznikUdanychGiec)
          / double(LicznikBONO - prevLicznikBONO + 1) << "\t|";
  cout << "StosunekKataDoMax: \t" << StosunekKataDoMax << "\t|";
  cout << endl;
  cout << "Co ile BONO: \t \t" << defaultIleRazyBONO << "\t|";
  cout << "Ilosc Wezlow w Pliku: \t\t" << PlikerWynikow.IloscWezlow << "\t|";
  cout << "Nazwa pliku: \t" << wskNazwyWynikAuto << "\t|";
  cout << "Ostatni wczytany nr: \t" << PlikerWynikow.AktualnyKnot << "\t|";
  cout << endl;
  cout << "Udane naturalne: \t" << LicznikUdanychNaturalnychGiec << "\t|";
  cout << "Ile robic naturalnych.: \t" << StosunekGiecNaturalnych << "\t|";
  cout << "IloscSegm: \t" << Symulowany.IloscSegm << "\t|";
  cout << "PierwszyRazHistogram?: \t" << PierwszyRazHistogram << "\t|";
  cout << endl;
  cout << endl;

  prevClock = clock();
  prevLicznikStanowMniejszegoPrawdopodobienstwa =
      LicznikStanowMniejszegoPrawdopodobienstwa;
  prevLicznikUdanychGiec = LicznikUdanychGiec;
  prevLicznikBONO = LicznikBONO;
  prevZmianaKatrazyOdl = ZmianaKatrazyOdl;
  SumaNizszychPrawdpodobienstwPrzejsc = 0;
}
//-----------------------------------------------------------------------
void KnotSimulator::UstawCoIleUdanychGiecZapis(long int coile)
{
  long int stare = CoIleUdanychGiecZapis;
  CoIleUdanychGiecZapis = coile;
  if (stare == coile) return;
  if (stare > CoIleUdanychGiecZapis)
  {    //dwa przypadki
    int modulostare = LicznikUdanychGiec % stare;
    int modulonowe = LicznikUdanychGiec % CoIleUdanychGiecZapis;
    if (modulonowe == modulostare)
    {    //nie przekroczono jeszcze giec zapisu dla nowego mniejszego
      //tworzy sie nowa mniejsza tablice i przepisuje do niej
    }
    else
    {       //przekroczono ilosc giec zapisu dla nowego mniejszego
      //tworzy sie KnotSimData dla tego, co jest,zapisuje, zapisuje wezel,
    }
  }
  else
  {       //zwiekszyc rozmiar tablicy giec i przepisac to, co jest

  }
}
//----------------------------------------------------------------------
void KnotSimulator::UstawCoIleSONOZapis(long int coile)
{
  CoIleSONOZapis = coile;
}
//----------------------------------------------------------------------------------
void KnotSimulator::ZastosujSONO()
{
  int PunktPrzedzialu1, PunktPrzedzialu2, SzerokoscPrzedzialu;

  {       //LOSOWANIA
    PunktPrzedzialu1 = Losownik.LosujInt(Symulowany.IloscSegm - 1);
    SzerokoscPrzedzialu = int(
        2.5 * Symulowany.PromienPrzekroju / Symulowany.DlugoscSegmSrednia);
    SzerokoscPrzedzialu = Losownik.LosujInt(SzerokoscPrzedzialu, 2);
    PunktPrzedzialu2 = nrP(PunktPrzedzialu1 + SzerokoscPrzedzialu,
        Symulowany.IloscSegm);
  }

  {       //zapamietanie stanu modyfikowanej czesci:
    if (PunktPrzedzialu1 < PunktPrzedzialu2)
    {       //tablica dzielona albo nie
      //niedzielona
      wskPomocniczyWektora1 = &(wskTablicyPomocniczejWektorow[PunktPrzedzialu1
          + 1]);
      wskPomocniczyWektora2 =
          &(Symulowany.wskTablicySegm[PunktPrzedzialu1 + 1]);
      for (int i = PunktPrzedzialu1 + 1; i < PunktPrzedzialu2; i++)
      {
        *wskPomocniczyWektora1++ = *wskPomocniczyWektora2++;
      }
    }
    else
    {  //dzielona
      wskPomocniczyWektora1 = &(wskTablicyPomocniczejWektorow[PunktPrzedzialu1
          + 1]);
      wskPomocniczyWektora2 =
          &(Symulowany.wskTablicySegm[PunktPrzedzialu1 + 1]);
      for (int i = PunktPrzedzialu1 + 1; i < Symulowany.IloscSegm; i++)
      {
        *wskPomocniczyWektora1++ = *wskPomocniczyWektora2++;
      }
      wskPomocniczyWektora1 = wskTablicyPomocniczejWektorow;
      wskPomocniczyWektora2 = Symulowany.wskTablicySegm;
      for (int i = 0; i < PunktPrzedzialu2; i++)
      {
        *wskPomocniczyWektora1++ = *wskPomocniczyWektora2++;
      }
    }
  }

  {  //MODYFIKACJA POLOZEN - SHRINK
    Wektor3D Roznica = Symulowany.wskTablicySegm[PunktPrzedzialu2]
        - Symulowany.wskTablicySegm[PunktPrzedzialu1];

    if (PunktPrzedzialu1 < PunktPrzedzialu2)
    {  //tablica dzielona albo nie
      //niedzielona
      wskPomocniczyWektora2 =
          &(Symulowany.wskTablicySegm[PunktPrzedzialu1 + 1]);
      for (int i = PunktPrzedzialu1 + 1; i < PunktPrzedzialu2; i++)
      {
        *wskPomocniczyWektora2++ = Symulowany.wskTablicySegm[PunktPrzedzialu1]
            + Roznica * (i - PunktPrzedzialu1) / SzerokoscPrzedzialu;
      }
    }
    else
    {  //dzielona
      wskPomocniczyWektora2 =
          &(Symulowany.wskTablicySegm[PunktPrzedzialu1 + 1]);
      for (int i = PunktPrzedzialu1 + 1; i < Symulowany.IloscSegm; i++)
      {
        *wskPomocniczyWektora2++ = Symulowany.wskTablicySegm[PunktPrzedzialu1]
            + Roznica * (i - PunktPrzedzialu1) / SzerokoscPrzedzialu;
      }
      wskPomocniczyWektora2 = Symulowany.wskTablicySegm;
      for (int i = 0; i < PunktPrzedzialu2; i++)
      {
        *wskPomocniczyWektora2++ = Symulowany.wskTablicySegm[PunktPrzedzialu1]
            + Roznica * (Symulowany.IloscSegm - PunktPrzedzialu1 + i)
                / SzerokoscPrzedzialu;
      }
    }
  }

  {  //czyszczenie zbioru przekryc i giec
    Symulowany.WyczyscPrzekrycia(); //ZAPOMINAMY O WYZNACZONYCH POPRZEDNIO PRZEKRYCIACH...
    Symulowany.WyczyscSegmentyPrzekrKrzyw(); //ZAPOMINAMY O WYZNACZONEJ POPRZEDNIO KRZYWIZNIE...
  }

  if (PunktPrzedzialu1 < PunktPrzedzialu2)
  {         //okreslenie przedzialu dla potrzeb sprawdzania przekryc
    //jesli giecie niedzielone
    A = PunktPrzedzialu1;
    B = PunktPrzedzialu2;
  }
  else
  {    //jesli giecie dzielone
    A = PunktPrzedzialu1;
    B = PunktPrzedzialu2 + Symulowany.IloscSegm;
  }

  if (Symulowany.SprawdzCzySaPrzekryciaPoGieciu(A, B) ///uwarunkowania podstawowe (krzywizna i twardosc)
      || !Symulowany.SprawdzKrzywProm(PunktPrzedzialu1,
          PunktPrzedzialu1 + 1 == Symulowany.IloscSegm ?
              Symulowany.IloscSegm - 2 : PunktPrzedzialu1 + 1)
      || !Symulowany.SprawdzKrzywProm(PunktPrzedzialu2,
          PunktPrzedzialu2 + 1 == Symulowany.IloscSegm ?
              Symulowany.IloscSegm - 2 : PunktPrzedzialu2 + 1))

  { //przywracamy stan poprzedni ale najpierw zwiekszamy ilosc nieudanych giec:
    // Symulowany.RozsunPrzekrycia();
    LicznikNieudanychShrink++;
    //W.Zegnij(-KatGiecia); //albo kopiowanie tablicy, jak ponizej
    if (PunktPrzedzialu1 < PunktPrzedzialu2) //tablica dzielona lub nie
    { //niedzielona
      wskPomocniczyWektora1 = &(wskTablicyPomocniczejWektorow[PunktPrzedzialu1
          + 1]);
      wskPomocniczyWektora2 =
          &(Symulowany.wskTablicySegm[PunktPrzedzialu1 + 1]);
      for (int i = PunktPrzedzialu1 + 1; i < PunktPrzedzialu2; i++)
      {
        *wskPomocniczyWektora2++ = *wskPomocniczyWektora1++;
      }
    }
    else
    { //dzielona
      wskPomocniczyWektora1 = &(wskTablicyPomocniczejWektorow[PunktPrzedzialu1
          + 1]);
      wskPomocniczyWektora2 =
          &(Symulowany.wskTablicySegm[PunktPrzedzialu1 + 1]);

      for (int i = PunktPrzedzialu1 + 1; i < Symulowany.IloscSegm; i++)
      {
        *wskPomocniczyWektora2++ = *wskPomocniczyWektora1++;
      }
      wskPomocniczyWektora1 = wskTablicyPomocniczejWektorow;
      wskPomocniczyWektora2 = Symulowany.wskTablicySegm;
      for (int i = 0; i < PunktPrzedzialu2; i++)
      {
        *wskPomocniczyWektora2++ = *wskPomocniczyWektora1++;
      }
    }
  }
  else
  { /// histogramy, liczniki i zapis
    //najpierw zwiekszamy licznik udanych giec
    LicznikUdanychShrink++;
  }
}
//------------------------------------------------------------------------
void KnotSimulator::ZastosujBONO(double (*wskFunZmiennaMetropolisa)(Knot3D*),
    double (*wskFunRozkladZmiennejMetropolisa)(double, double, double))
//funkcja ZastosujBONO dostaje wskaznik do funkcji typu double dostajacej wskaznik do wezla
//oraz do fukcji rozkladu prawdopodobienstwa dla tej zmiennej
{
  double KatGiecia;
  bool NaturalnyHistogram =
      PierwszyRazHistogram
          || ((Losownik.LosujLongDouble() < StosunekGiecNaturalnych) ?
              true : false);

  {                            //LOSOWANIA
    if (NaturalnyHistogram)
    {              //wyznaczenie "naturalnego" histogramu co 20 giec naturalnych
      Symulowany.UstawPunktGiecia1(Losownik.LosujInt(Symulowany.IloscSegm - 1));
      Symulowany.UstawPunktGiecia2(
          Symulowany.PunktGiecia1
              + Losownik.LosujInt(Symulowany.IloscSegm - 1));

      Symulowany.KatGieciaMax = Symulowany.WyznaczKatGieciaMax(); //ta liczba powinna zalezec od gestosci kulek!

      KatGiecia = Losownik.LosujLongDouble(
          -StosunekKataDoMax * Symulowany.KatGieciaMax,
          Symulowany.KatGieciaMax * StosunekKataDoMax);
    }
    else
    { //korzystamy z wyznaczonego histogramu uwzgledniajac parametry sterujace :
      //ParametrPunktuA,ParametrOdleglosciAB,OdchStandParametruZmianyWezla
      int baza;
      int probka;

      funkcja1: do
      {
        probka = Losownik.LosujInt(Symulowany.IloscSegm - 1);
        Symulowany.UstawPunktGiecia1(probka);

        baza = wskHistogramySymulacji->wskHistPunktyA->IloscDanychPrzedzMax;
      }
      while (Losownik.LosujInt(baza * ParametrPunktuA)
          > wskHistogramySymulacji->wskHistPunktyA->wskTablicyPrzedzialow[probka]);

      funkcja2: do
      {
        probka = Losownik.LosujInt(Symulowany.IloscSegm / 2 - 1);
        Symulowany.UstawPunktGiecia2(Symulowany.PunktGiecia1 + probka + 1);
        baza = wskHistogramySymulacji->wskHistOdlAB->IloscDanychPrzedzMax;
      }
      while (Losownik.LosujInt(baza * ParametrOdleglosciAB)
          > wskHistogramySymulacji->wskHistOdlAB->wskTablicyPrzedzialow[probka]);

      funkcja3: int probka2;
      baza =
          wskHistogramySymulacji->wskHistOdlABrazyKatyGiec->IloscDanychPrzedzMax;
      double Los = Losownik.LosujGaussa(OdchStandParametruZmianyWezla);
      KatGiecia = Los / probka;
      if (KatGiecia < -StosunekKataDoMax * Symulowany.KatGieciaMax)
        KatGiecia = -StosunekKataDoMax * Symulowany.KatGieciaMax;
      if (KatGiecia > StosunekKataDoMax * Symulowany.KatGieciaMax)
        KatGiecia = StosunekKataDoMax * Symulowany.KatGieciaMax;
    }
  }

  //gniemy mniejsza czesc dla oszczednosci czasu:
  if (odlP(Symulowany.PunktGiecia1, Symulowany.PunktGiecia2,
      Symulowany.IloscSegm) > Symulowany.IloscSegm / 2)
  {
    Symulowany.UstawPunktGiecia1(Symulowany.PunktGiecia2);  //zamiana punktow
    //odrzucamy polowe wyborow dla odleglosci rownej 2 (okolo polowy to odrzucone wybory dla odleglosci 1)
    if (odlP(Symulowany.PunktGiecia1, Symulowany.PunktGiecia2,
        Symulowany.IloscSegm) == 2 && Losownik.LosujInt(1))
    {
      Symulowany.UstawPunktGiecia2(Losownik.LosujInt(Symulowany.IloscSegm - 1));

      if (odlP(Symulowany.PunktGiecia1, Symulowany.PunktGiecia2,
          Symulowany.IloscSegm) > Symulowany.IloscSegm / 2)
      {
        Symulowany.UstawPunktGiecia1(Symulowany.PunktGiecia2);
      }
    }
    KatGiecia = -KatGiecia;
  }

  {   //zapamietanie stanu wyginanej czesci przed gieciem:
    if (Symulowany.PunktGiecia1 < Symulowany.PunktGiecia2)
    {   //tablica dzielona albo nie
      //niedzielona
      wskPomocniczyWektora1 =
          &(wskTablicyPomocniczejWektorow[Symulowany.PunktGiecia1 + 1]);
      wskPomocniczyWektora2 =
          &(Symulowany.wskTablicySegm[Symulowany.PunktGiecia1 + 1]);
      for (int i = Symulowany.PunktGiecia1 + 1; i < Symulowany.PunktGiecia2;
          i++)
      {
        *wskPomocniczyWektora1++ = *wskPomocniczyWektora2++;
      }
    }
    else
    {  //dzielona
      wskPomocniczyWektora1 =
          &(wskTablicyPomocniczejWektorow[Symulowany.PunktGiecia1 + 1]);
      wskPomocniczyWektora2 =
          &(Symulowany.wskTablicySegm[Symulowany.PunktGiecia1 + 1]);
      for (int i = Symulowany.PunktGiecia1 + 1; i < Symulowany.IloscSegm; i++)
      {
        *wskPomocniczyWektora1++ = *wskPomocniczyWektora2++;
      }
      wskPomocniczyWektora1 = wskTablicyPomocniczejWektorow;
      wskPomocniczyWektora2 = Symulowany.wskTablicySegm;
      for (int i = 0; i < Symulowany.PunktGiecia2; i++)
      {
        *wskPomocniczyWektora1++ = *wskPomocniczyWektora2++;
      }
    }
  }

  {  //GIECIE WEZLA
    KatGiecia = Symulowany.Zegnij(KatGiecia);
    //W.DodajGiecie(GiecieWezla(W.PunktGiecia1,W.PunktGiecia2,KatGiecia,true)); //zapis do listy
  }

  {  //czyszczenie zbioru przekryc i giec
    Symulowany.WyczyscPrzekrycia(); //ZAPOMINAMY O WYZNACZONYCH POPRZEDNIO PRZEKRYCIACH...
    Symulowany.WyczyscSegmentyPrzekrKrzyw(); //ZAPOMINAMY O WYZNACZONEJ POPRZEDNIO KRZYWIZNIE...
  }

  if (Symulowany.PunktGiecia1 < Symulowany.PunktGiecia2)
  {       //okreslenie przedzialu dla potrzeb sprawdzania przekryc
    //jesli giecie niedzielone
    A = Symulowany.PunktGiecia1;
    B = Symulowany.PunktGiecia2;
  }
  else
  {    //jesli giecie dzielone
    A = Symulowany.PunktGiecia1;
    B = Symulowany.PunktGiecia2 + Symulowany.IloscSegm;
  }

  if (Symulowany.SprawdzCzySaPrzekryciaPoGieciu(A, B) ///uwarunkowania podstawowe (krzywizna i twardosc)
      || !Symulowany.SprawdzKrzywProm(Symulowany.PunktGiecia1,
          Symulowany.PunktGiecia1 + 1 == Symulowany.IloscSegm ?
              Symulowany.IloscSegm - 2 : Symulowany.PunktGiecia1 + 1)
      || !Symulowany.SprawdzKrzywProm(Symulowany.PunktGiecia2,
          Symulowany.PunktGiecia2 + 1 == Symulowany.IloscSegm ?
              Symulowany.IloscSegm - 2 : Symulowany.PunktGiecia2 + 1))

  {  //przywracamy stan poprzedni ale najpierw zwiekszamy ilosc nieudanych giec:

    LicznikNieudanychGiec++;
    //W.Zegnij(-KatGiecia); //albo kopiowanie tablicy, jak ponizej
    if (Symulowany.PunktGiecia1 < Symulowany.PunktGiecia2) //tablica dzielona lub nie
    { //niedzielona
      wskPomocniczyWektora1 =
          &(wskTablicyPomocniczejWektorow[Symulowany.PunktGiecia1 + 1]);
      wskPomocniczyWektora2 =
          &(Symulowany.wskTablicySegm[Symulowany.PunktGiecia1 + 1]);
      for (int i = Symulowany.PunktGiecia1 + 1; i < Symulowany.PunktGiecia2;
          i++)
      {
        *wskPomocniczyWektora2++ = *wskPomocniczyWektora1++;
      }
    }
    else
    { //dzielona
      wskPomocniczyWektora1 =
          &(wskTablicyPomocniczejWektorow[Symulowany.PunktGiecia1 + 1]);
      wskPomocniczyWektora2 =
          &(Symulowany.wskTablicySegm[Symulowany.PunktGiecia1 + 1]);

      for (int i = Symulowany.PunktGiecia1 + 1; i < Symulowany.IloscSegm; i++)
      {
        *wskPomocniczyWektora2++ = *wskPomocniczyWektora1++;
      }
      wskPomocniczyWektora1 = wskTablicyPomocniczejWektorow;
      wskPomocniczyWektora2 = Symulowany.wskTablicySegm;
      for (int i = 0; i < Symulowany.PunktGiecia2; i++)
      {
        *wskPomocniczyWektora2++ = *wskPomocniczyWektora1++;
      }
    }
  }
  else
  { /// giecie udane - histogramy, liczniki, zapis, uwarunkowania energetyczne
    //najpierw zwiekszamy licznik udanych giec
    LicznikUdanychGiec++;
    wskHistogramySymulacji->IloscWszystkichUdanychGiec++;
    if (NaturalnyHistogram) LicznikUdanychNaturalnychGiec++;

    //obliczenie zmiany wezla
    double zmiana = KatGiecia
        * (odlP(Symulowany.PunktGiecia1, Symulowany.PunktGiecia2,
            Symulowany.IloscSegm) - 1) / Symulowany.IloscSegm;
    ZmianaKatrazyOdl += zmiana < 0 ? -zmiana : zmiana;

    if (NaturalnyHistogram)
    {   //zapisujemy giecie do tablicy udanych giec miedzy zapisami
      wskTablicyUdanychGiec[LicznikUdanychGiec % CoIleUdanychGiecZapis] =
          GiecieWezla(Symulowany.PunktGiecia1,
              odlP(Symulowany.PunktGiecia1, Symulowany.PunktGiecia2,
                  Symulowany.IloscSegm), KatGiecia, true);
    }

    if (!(LicznikUdanychNaturalnychGiec - prevLicznikUdanychNaturalnychGiec
        - CoIleUdanychNaturalnychGiecHistogram))
    {   //tworzenie nowego  KnotSimData, usuwanie starych giec
      StworzHistogramy();
      PierwszyRazHistogram = false;
      delete[] wskTablicyUdanychGiec;
      wskTablicyUdanychGiec =
          new GiecieWezla[CoIleUdanychNaturalnychGiecHistogram];
      prevLicznikUdanychNaturalnychGiec = LicznikUdanychNaturalnychGiec;
      prevLicznikBONOHistogramow = LicznikBONO;
    }

    if (!(LicznikUdanychGiec - prevLicznikUdanychGiecZapisu
        - CoIleUdanychGiecZapis))
    {   //zapisywanie do pliku

      //W.ZapiszTxt(wskNazwyWynikAutotxt); za dużo zajmuje miejsca na dysku :/
      Symulowany.ZapiszBin(wskNazwyWynikAuto);
      PlikerWynikow.IloscWezlow++;

      {   //zapis stanu licznikow dla odniesienia w nastepnym zapisie
        prevLicznikStanowWiekszegoPrawdopodobienstwa =
            LicznikStanowWiekszegoPrawdopodobienstwa;
        prevLicznikStanowMniejszegoPrawdopodobienstwa =
            LicznikStanowMniejszegoPrawdopodobienstwa;
        prevLicznikNieudanychGiec = LicznikNieudanychGiec;
        prevLicznikUdanychGiecZapisu = LicznikUdanychGiec;
        prevLicznikUdanychGiec = LicznikUdanychGiec;
        prevLicznikProbGiec = LicznikProbGiec;
        prevLicznikSONO = LicznikSONO;
        prevLicznikBONO = LicznikBONO;
        SumaNizszychPrawdpodobienstwPrzejsc = 0;
        ZmianaNaSek = prevZmianaNaSek = double(ZmianaKatrazyOdl)
            / double(time(0) - prevTime);
        prevTime = time(0);
        prevZmianaKatrazyOdl = ZmianaKatrazyOdl;
      }
    }
    else
      ZmianaNaSek = double(ZmianaKatrazyOdl) / double(time(0) - prevTime); //zeby bylo ciagle widac zmiany zmian wezla

    if (wskFunZmiennaMetropolisa)
    { //uwarunkowania inne - energia, krzywizna, srednie odleglosci itp

      { //OBLICZENIE NOWEJ ZmiennejMetropolisa WEZLA
        ZmiennaMetropolisaNowa = wskFunZmiennaMetropolisa(&Symulowany); ///<<<<<<<<<--------------UWAGA, tu wskaznik do funkcji zewnetrznej
      }

      double StosunekGestosciStanowChwilowy;

      { //OBLICZENIE StosunekGestosciStanowChwilowy
        StosunekGestosciStanowChwilowy = wskFunRozkladZmiennejMetropolisa(
            ZmiennaMetropolisaNowa, ZmiennaMetropolisa, Temperatura);
      }

      if (StosunekGestosciStanowChwilowy < 1)
      { //jesli do tego stanu przejscia prawdopodobienstwo <1
        //najpierw zwiekszamy odpowiedni licznik:
        LicznikStanowMniejszegoPrawdopodobienstwa++;
        SumaNizszychPrawdpodobienstwPrzejsc += StosunekGestosciStanowChwilowy;

        if (Losownik.LosujLongDouble() >= StosunekGestosciStanowChwilowy)
        {            //losujemy liczbe
          //jesli nie zmiescilismy sie w tej liczbie (mniejszej niz 1) //przywracamy stan poprzedni:
          //Symulowany.Zegnij(-KatGiecia);

          if (Symulowany.PunktGiecia1 < Symulowany.PunktGiecia2)
          {                //tablica dzielona lub nie
            //niedzielona
            wskPomocniczyWektora1 =
                &(wskTablicyPomocniczejWektorow[Symulowany.PunktGiecia1 + 1]);
            wskPomocniczyWektora2 =
                &(Symulowany.wskTablicySegm[Symulowany.PunktGiecia1 + 1]);
            for (int i = Symulowany.PunktGiecia1 + 1;
                i < Symulowany.PunktGiecia2; i++)
            {
              *wskPomocniczyWektora2++ = *wskPomocniczyWektora1++;
            }
          }
          else
          {                    //dzielona
            wskPomocniczyWektora1 =
                &(wskTablicyPomocniczejWektorow[Symulowany.PunktGiecia1 + 1]);
            wskPomocniczyWektora2 =
                &(Symulowany.wskTablicySegm[Symulowany.PunktGiecia1 + 1]);

            for (int i = Symulowany.PunktGiecia1 + 1; i < Symulowany.IloscSegm;
                i++)
            {
              *wskPomocniczyWektora2++ = *wskPomocniczyWektora1++;
            }

            wskPomocniczyWektora1 = wskTablicyPomocniczejWektorow;
            wskPomocniczyWektora2 = Symulowany.wskTablicySegm;

            for (int i = 0; i < Symulowany.PunktGiecia2; i++)
            {
              *wskPomocniczyWektora2++ = *wskPomocniczyWektora1++;
            }
          }
        }
        else
        {       //zostawiamy, wiec NowaZmiennaMetropolisa wchodzi w wezel.... ;)
          ZmiennaMetropolisa = ZmiennaMetropolisaNowa;
        }
      }
      else
      {       //zostawiamy, wiec Nowa ZmiennaMetropolisa wchodzi w wezel.... ;)
        //(zwiekszamy tez licznik stanow Wiekszego Prawdopodobienstwa
        LicznikStanowWiekszegoPrawdopodobienstwa++;
        ZmiennaMetropolisa = ZmiennaMetropolisaNowa;
      }
    }
  }

  wskHistogramySymulacji->IloscWszystkichGiec++;  //zwiekszamy licznik calkowity
  LicznikBONO++; //zwiekszamy licznik podstawowy
}
//------------------------------------------------------------------------------
void KnotSimulator::StworzHistogramy()
{ //stosuje na razie tylko pierwszy histogram, bo pozniej degeneracja
//tworzy sie KnotSimData dla aktualnych parametrow
  delete wskprevHistogramySymulacji;
  wskprevHistogramySymulacji = wskHistogramySymulacji; //odniesienie dla nastepnego zapisu
  if (wskprevHistogramySymulacji->IloscWszystkichGiec < 0)
    wskprevHistogramySymulacji->IloscWszystkichGiec = 0;
  if (wskprevHistogramySymulacji->IloscWszystkichUdanychGiec < 0)
    wskprevHistogramySymulacji->IloscWszystkichUdanychGiec = 0;

  wskHistogramySymulacji = new KnotSimData(wskTablicyUdanychGiec,
      LicznikBONO - prevLicznikBONOHistogramow,
      LicznikUdanychNaturalnychGiec - prevLicznikUdanychNaturalnychGiec,
      wskprevHistogramySymulacji->IloscWszystkichGiec,
      wskprevHistogramySymulacji->IloscWszystkichUdanychGiec,
      Symulowany.IloscSegm);

  wskHistogramySymulacji->wskWezelSymulowany = &Symulowany;

  wskHistogramySymulacji->ZapiszTxt("histo.txt");
  wskHistogramySymulacji->ZapiszBin("KnotSimData.bin");

  PierwszyRazHistogram = false;
}

//------------------------------------------------------------------------
#endif

//TODO:
// skalowanie histogramow
// histogram pliker
//
