//klasa obslugujaca pliki .knb
#ifndef KNOTPLIKER_H
#define KNOTPLIKER_H

#include <fstream>
#include <vector>
#include <iostream>

using namespace std;


//deklaracja zapowiadajaca
class Knot3D;

class DaneWezlaPliku
{
 public:
 long int PozycjaPliku;
 int      IloscSegm;
 int      IloscGiec;
 
 DaneWezlaPliku(long int Pozycja, int Segm, int Giecia) :
	PozycjaPliku(Pozycja),
	IloscSegm(Segm),
	IloscGiec(Giecia)
        {};
 
};

class KnotPliker
{
 public:
 char*    NazwaPliku;	//ten plik otwiera KnotPliker
 ifstream* wskstrPW; 	//strumienia pliku wezlow
 ifstream strPW; 	//wskaznik strumienia pliku wezlow

 long int IloscWezlow;
 long int AktualnyKnot;
 long int KoniecPliku;

 vector<DaneWezlaPliku> DaneWezlow;

 int WczytajPlik(char* nazwapliku);
 void WczytajKnot(Knot3D*,long int Numer);
 void ZapiszCoIle(char* wskNazwy,int CoIle,long int Start=0,long int Koniec=-1);
 //Knot3D& pliker::operator[] 
};

#endif
