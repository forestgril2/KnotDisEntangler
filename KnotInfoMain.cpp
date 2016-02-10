#include <cstdlib>
#include <iostream>
#include <fstream>
#include <deque>
#include <set>
#include <vector>
#include <math.h>

using namespace std;

#include "SONO.cpp"
#include "GiecieWezla.cpp"
#include "funkcjezew.cpp"
#include "Losowania.cpp"
#include "Knot3D.cpp"
#include "KnotPliker.cpp"
#include "KnotSimulator.cpp"

const short int IloscLancuchowTextMax = 5;
bool TrybTekstowyOdczytu = false;

char wskNazwyDomyslna[64] = {"newgordian.txt"};
char wskNazwyWynikAuto[64] = {"wynik.knb"};
char wskNazwyWynikMoja[64] = {"ciekawe.knb"};
char wskNazwyWynikAutotxt[64] = {"wynik.knt"};
char wskNazwyWynikMojatxt[64] = {"ciekawe.knt"};

char* LancuchyNazw[] = {wskNazwyDomyslna, wskNazwyWynikAuto, wskNazwyWynikMoja,
    wskNazwyWynikAutotxt, wskNazwyWynikMojatxt};

Knot3D W, Poczatkowy;

int main(int argc, char *argv[])
{
  cout << endl;

  char Pomocnik[64] = "";
  int nrParametruFunkcjiMain = 1;
  int nrPrzekazanegoLancucha = 0;
  long int NumerWczytywany = 0;
  double ParametrDouble = 0;

  for (; nrParametruFunkcjiMain < argc;)
  {
    if ( //(argv[nrParametruFunkcjiMain])[0]=='/'||   //czy jest znak parametru '-' lub '/' ?
    (argv[nrParametruFunkcjiMain])[0] == '-')
    {
      strcpy(Pomocnik, &(argv[nrParametruFunkcjiMain][1]));
      //---------------------------------------
      //rozne przypadki parametrow sterujacych:
      //---------------------------------------
      switch (Pomocnik[0])
      {
        case 'i':		//'i' oznacza input -  dalej nazwa pliku wczytyw.
          nrParametruFunkcjiMain++;

          if (nrParametruFunkcjiMain >= argc || //jesli nastepny argument pusty
              //(argv[nrParametruFunkcjiMain])[0]=='\\'||    //lub jest oznaczony jak parametr
              (argv[nrParametruFunkcjiMain])[0] == '-')
          {
            cout << "UWAGA: nie podales nazwy pliku do wczytywania!" << endl;
            cout << "\t(Zostanie uzyta domyslna nazwa: \t" << wskNazwyDomyslna
                << " \t)" << endl;

            nrParametruFunkcjiMain--; //uznajemy, zesmy nie wczytali jeszcze
          }
          else
          {
            strcpy(argv[nrParametruFunkcjiMain + 1], wskNazwyDomyslna);

            cout << "Domyslna nazwa pliku wczytywania: \t" << wskNazwyDomyslna
                << endl;
            cout << endl;
            nrPrzekazanegoLancucha++;
          }
          ;

          nrParametruFunkcjiMain++;
          break;

        case 'o':		//'o' oznacza output -  dalej nazwa pliku zapis
          nrParametruFunkcjiMain++;

          if (nrParametruFunkcjiMain >= argc || //jesli nastepny argument pusty
              //(argv[nrParametruFunkcjiMain])[0]=='/'||       //lub jest oznaczony jak parametr
              (argv[nrParametruFunkcjiMain])[0] == '-')
          {
            cout
                << "UWAGA: nie podano nazwy pliku do automatycznego zapisu wynikow!"
                << endl;
            cout << "\t(Zostanie uzyta domyslna nazwa: \t" << wskNazwyWynikAuto
                << " \t)" << endl;
            cout << endl;

            nrParametruFunkcjiMain--; //uznajemy, zesmy nie wczytali jeszcze
          }
          else
          {
            strcpy(argv[nrParametruFunkcjiMain + 1], wskNazwyWynikAuto);

            cout << "Domyslna nazwa pliku automatycznego zapisu: \t"
                << wskNazwyWynikAuto << endl;
            cout << endl;
            nrPrzekazanegoLancucha++;
          }
          ;
          nrParametruFunkcjiMain++;
          break;

        case 't':  //'t' - tekstowy tryb odczytu
          nrParametruFunkcjiMain++;
          TrybTekstowyOdczytu = true;
          cout << "UWAGA: Wybrano tryb tekstowy odczytu." << endl;
          break;

        case 'b':  //'b' - binarny tryb odczytu
          nrParametruFunkcjiMain++;
          TrybTekstowyOdczytu = false;
          cout << "UWAGA: Wybrano tryb binarny (domyslny) odczytu." << endl;
          break;

        case 'n':  //'n' - numer wezla do wczytania z pliku
          nrParametruFunkcjiMain++;

          if (nrParametruFunkcjiMain >= argc || //jesli nastepny argument pusty
              //(argv[nrParametruFunkcjiMain])[0]=='/'||    //lub jest oznaczeny jak parametr
              (argv[nrParametruFunkcjiMain])[0] == '-')
          {
            cout << "UWAGA: nie podano numeru wczytywanego wezla!" << endl;
            cout << "\t(Wczytany zostanie pierwszy wezel pliku)" << endl;

            //nrParametruFunkcjiMain--;  //cofamy wczytanie tego parametru
          }
          else
          {
            strcpy(Pomocnik, argv[nrParametruFunkcjiMain]);
            NumerWczytywany = atoi(Pomocnik);
            if (NumerWczytywany == 0)
            {
              cout << "Numer wczytywany podano 0 albo lancuch textowy: \t"
                  << Pomocnik << endl;
              cout << "\t(Wczytany zostanie pierwszy wezel pliku)" << endl;
            };
            nrParametruFunkcjiMain++;
          }
          ;
          break;

        default: //to na samym koncu, jak zaden parametr nie pasuje
          cout
              << "UWAGA: podano pusty lub nieznany parametr (nie zostanie uzyty.)"
              << endl;
          nrParametruFunkcjiMain++;
          break;
      }
    }
    else  //jesli nie ma oznaczenia parametru to przekazujemy lancuch/nazwe
    {
      if (nrPrzekazanegoLancucha < IloscLancuchowTextMax)
      {
        strcpy(LancuchyNazw[nrPrzekazanegoLancucha],
            argv[nrParametruFunkcjiMain]);
      }
      else
      {
        cout
            << "UWAGA: Przekazano nadmierna ilosc lancuchow tekstowych! (Nie zostana uzyte.)"
            << endl;
        cout << endl;
      };

      nrPrzekazanegoLancucha++;
      nrParametruFunkcjiMain++;
    };
  };

  cout << endl;
  cout << "Nazwa pliku wczytywania: \t\t\t" << wskNazwyDomyslna << endl;
  cout << "Nazwa pliku zapisu automatycznego (binarnie): \t"
      << wskNazwyWynikAuto << endl;
  cout << "Nazwa pliku zapisu wyboru (binarnie): \t \t" << wskNazwyWynikMoja
      << endl;
  cout << "Nazwa pliku zapisu automatycznego (tekstowo): \t"
      << wskNazwyWynikAutotxt << endl;
  cout << "Nazwa pliku zapisu wyboru (tekstowo): \t \t" << wskNazwyWynikMojatxt
      << endl;
  cout << endl;

  if (TrybTekstowyOdczytu)
  {
    W.WczytajTxt(wskNazwyDomyslna, NumerWczytywany);
  }
  else
  {
    W.WczytajBin(wskNazwyDomyslna, NumerWczytywany);
  };

  //zapiszmy poczatkowy stan wezla
  Poczatkowy = W;

  cout << "---------------------------------------" << endl;
  cout << "Informacje o wezle numer \t" << NumerWczytywany << "\tz pliku \t"
      << wskNazwyDomyslna << endl;
  cout << endl;
  W.WyznaczWszystko();
  W.WypiszInfo();
  double pierwsza = W.DlugoscSegmSrednia;

  cout << "----------KONIEC INFORMACJI------------" << endl << endl;

  return EXIT_SUCCESS;
}

long int IleWezlowWPlikuBin(char* nazwa);
long int IleWezlowWPlikuTxt(char* nazwa);
