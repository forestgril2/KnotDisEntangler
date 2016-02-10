#ifndef ZBIORSYMFONIA_H
#define ZBIORSYMFONIA_H

//------------------------------------------------------------
// Program z paragrafu 17.2
//	                str 411
//------------------------------------------------------------
// Sprawdzony na Windows XP,  kompilator: Microsoft Visual C++ 6.0
//
// UWAGA: Kompilacja za pomoca tego, odbiegajacego nieco od standardu
// kompilatora wymaga ujecia w komentarz jednej linii

// Sprawdzony na Linuksie,   
//    kompilator: GNU gcc version 4.0.2 (SuSE Linux)
//#########################################################

#include <iostream>
using namespace std;

#include <cstring>

///////////////////////////////////////////////////////////////

template<class typOBJ> class Zbior;

// Aby byla mozliwa ponizsza deklaracja, szablonu funkcji, najpierw (powyzej)
// musielismy zadeklarowac co to znaczy "Zbior".

template<class typOBJ>
ostream & operator<<(ostream & stru, Zbior<typOBJ> & spis);

template<class typOBJ>	  			// deklaracja zwiastujaca
class iteratorZbioru;

/////////////////////////////////////////////////////////////////////////
////////////// szablon klasy pojemnikowej ///////////////////////////////
/////////////////////////////////////////////////////////////////////////
template<class typOBJ>				// ((1))
class Zbior
{
public:
  // pomocnicza klasa wezlow----------------------------------
  struct wezel
  {
    typOBJ * wskobj;
    wezel * nastepca;
    wezel * poprzednik;
    // ------------ konstruktor
    wezel(typOBJ chowany) :
        nastepca(NULL), poprzednik(NULL)
    {
      wskobj = new typOBJ;
      *wskobj = chowany;
    }

    ~wezel()
    {
      delete wskobj;
    }
  };
  //----------------------------------
private:
  // Zbior realizujemy jako podwojnie laczona liste
  wezel *pierwszyW;
  wezel *ostatniW;
  int dlugosc;				// ile elementow zawiera biezaco Zbior
public:
  Zbior()		// ------- konstruktor
  {
    pierwszyW = ostatniW = NULL;
    dlugosc = 0;
  }
  ~Zbior();
  //----------------
  // to przyda sie dla skrocenia dalszych zapisow
  typedef iteratorZbioru<typOBJ> itZb;

  //----------------
  // znajdz dany element w Zbiorze
  itZb znajdz(typOBJ obj); 					// ((2))

  int wstaw(typOBJ obj);

  //----------------
  void usun(itZb & iter);
  void usun(itZb & pocz, itZb & kon);
  void usun(typOBJ ob);
  //----------------
  int rozmiar() const
  {
    return dlugosc;
  }

private:
  itZb gdzie_dac(typOBJ obj);
  //--------------------------------
  void daje_na_poczatek(wezel* nowyW, itZb & iter);
  void daje_w_srodku(wezel* nowyW, itZb & iter);
  void daje_na_koniec(wezel* nowyW);

  void usuwam_pierwszy(itZb & iter);
  void usuwam_ostatni(itZb & iter);
  void usuwam_ze_srodka(itZb & iter);
  //----------------
  // zaprzyjazniony operator wypisywania
  friend ostream & operator<<<typOBJ> // <---- tak powinno byc, ale dla VC++ 6.0 trzeba to ujac w komentarz
  (ostream & stru, Zbior<typOBJ> & x);

  // deklaracja przyjazni z klasa iteratora
  friend class iteratorZbioru<typOBJ> ;
};

///////////////////////////////////////////////////////////////
template<class typOBJ>
iteratorZbioru<typOBJ> Zbior<typOBJ>::znajdz(typOBJ wzor)		       		// ((3))
{
  // definicja dwoch pomocniczych iteratorow
  itZb dobry(*this), szperacz(*this);

  // Sprawdzamy dlugosc listy.
  // Jesli zero (Zbior pusty), to zadanego obiektu w pojemniku na pewno nie ma
  if (!dlugosc)
  {
    szperacz.wybranyW = NULL;
    return szperacz;
  }

  // skoro dlugosc jest niezerowa, to zaczynamy szukac

  // sprawdzamy czy moze obiekt jest na samym poczatku listy
  if (wzor == *szperacz)
  {
    szperacz.wybranyW = pierwszyW;
    return szperacz;
  }

  // jesli nie na poczatku, to zacznijmy sondowac Zbior-liste
  // wedlug metody jak przy zgadywaniu liczby

  // pierwotnie skaczemy skokiem o dlugosci rownej dlugosci Zbioru
  // na ostatni element
  for (int skok = dlugosc - 1;; skok = skok / 2)
  {
    if (skok == 0) skok = 1;

    szperacz.skocz_o(skok);  // wlasciwe przesuniecie iteratora sondujacego

    // czy skoczylismy za daleko? (tzn. osiagnelismy koniec Zbioru
    if (szperacz == 0)
    {
      // tak sie zdarzy wtedy, gdy co prawda danego
      // obiektu w Zbiorze nie ma, ale gdyby byl,
      // to musialby stac na samym kocu
      return szperacz;
    }

    // to jest moment zadania pytania, czyli
    // porownanie poszukiwanego obiektu z tym, na ktorego
    // pokazuje szperacz.

    if (*szperacz < wzor)
    { 			// poszukiwany jest wiekszy, wiec iterator
      // _dobry_ mozemy przesunac na pozycje szperacza
      dobry = szperacz;

      // czy to juz koniec pojemnika ?
      if (szperacz.wybranyW == ostatniW)
      {
        // tzn. obiektu nie bylo w pojemniku
        szperacz.wybranyW = NULL;
        return szperacz;
      }
    }
    else
    {
      // jesli zas poszukiwany obiekt nie jest wiekszy od tego,
      // na ktory pokazuje iterator szperacz....

      if (wzor == *szperacz) // --- a moze jest mu rowny ?
      {
        return szperacz; // tak? - to jest ZNALEZIONE!!!!
      }

      // nie, tzn. ze sondujacym iteratorem skoczylismy za daleko.
      if (skok == 1)
      { 		// jesli skoczylismy za daleko skokiem o dlugosci =1
        // tzn. ze poprzedni obiekt byl tez niedobry
        // czyli miedzy tymi dwoma obiektami powinien byc
        // umieszczony szukany - a go nie ma.
        // Informujemy wiec o nieznalezieniu
        szperacz.wybranyW = NULL;
        return szperacz;
      }
      else
      {
        // jesli skoczylismy, skokiem dluzszym niz 1, a pokazywany
        // szperaczem obiekt jest wiekszy od szukanego,
        // to znaczy, ze skoczylismy za daleko.
        // Wrocmy tam, skad skoczylismy (po to, by za chwile
        // zrobic krotszy skok)

        // cofamy szperacza do ostatniej dobrej pozycji
        szperacz = dobry;
      }
    }
  }
}
/*************************************************************/
template<class typOBJ>
int 		     				// <- udalo sie czy nie ?
Zbior<typOBJ>::wstaw(typOBJ obj)       				// ((4))
{

  itZb iter(*this);

  iter = znajdz(obj);

  if (iter.wybranyW != NULL) 	       				// ((5))
  {
    // tzn. taki element w Zbiorze juz jest, wiec ignorujemy to wstawienie
    return 0;
  }

  iter = gdzie_dac(obj);

  // ======================================================
  // dalej jest juz tak, jak w liscie. Wstawiamy nowy wezel
  // przed wezlem pokazywanym wlasnie ustawionym iteratorem iter
  // ======================================================

  wezel * nowyW = new wezel(obj);		 	//definicja wezla
  // od razu umieszczamy w nim informacje o przedmiocie

  // nowyW->wskobj = &obj ;    TO JEST USUNIETA INSTRUKCJA

  dlugosc++;				// aktualizujemy dlugosc Zbioru

  // Teraz zajmujemy sie wpieciem tego wezla do listy
  if (iter.wybranyW != NULL)
  {
    if (iter.wybranyW == pierwszyW)
    {
      daje_na_poczatek(nowyW, iter);
    }
    else
    {
      daje_w_srodku(nowyW, iter);
    }
  }
  else
    daje_na_koniec(nowyW);
  return 1;
}

/*************************************************************/
template<class typOBJ>
iteratorZbioru<typOBJ> Zbior<typOBJ>::gdzie_dac(typOBJ wzor)				// ((6))
{
  // definicja pomocniczych iteratorow
  itZb szperacz(*this), dobry(*this);

  // sprawdzamy objetosc Zbioru.
  if (!dlugosc)
  {		    		// Jesli zero (Zbior pusty),
    szperacz.wybranyW = NULL;  	// to wstaw na poczatku
    return szperacz;
  }

  // dlugosc niezerowa, wiec zaczynamy szukac
  // gdzie jest punkt, w ktorym dany obiekt powinien sie znalezc.

  if (wzor < *szperacz)
  {
    // ten element nadaje sie na sam poczatek listy ?
    szperacz.wybranyW = pierwszyW;
    return szperacz;
  }

  // nie, to kroczymy przez liste -------------------------

  // zaczyna sie wiec procedura znana nam z funkcji  sprawdzajacej "znajdz"

  for (int skok = dlugosc - 1;; skok = skok / 2)
  {
    if (skok == 0) skok = 1;
    szperacz.skocz_o(skok);

    // czy szperaczem skoczylismy za koniec listy?
    if (szperacz == 0) return szperacz;

    // Porownanie obiektu wzor z obiektem pokazywanym
    // przez iterator szperacz, po to, by sprawdzic,
    // czy dalej mamy szukac powyzej czy ponizej
    if (*szperacz < wzor)
    {
      // NIE, nie za daleko, wiec dobry moze
      // przesunac sie na pozycje szperacza
      dobry = szperacz;

      // a moze to juz koniec pojemnika ?
      if (szperacz.wybranyW == ostatniW)
      {
        // to znaczy, ze chcemy wstawic obiekt

        // wiekszy od wszystkich innych

        // wobec tego odpowiednio ustawiamy szperacza
        szperacz.za_koniec();
        return szperacz;
      }
    }
    else
    {
      // jesli zas wzorcowy obiekt nie jest wiekszy od tego
      // pokazywanego szperaczem to...
      if (skok == 1)
      {
        // jesli do tego wiekszego obiektu skoczylismy
        // ze skokiem 1, tzn. ze poprzedni obiekt byl mniejszy.
        // Jestesmy wiec miedzy za malym, a za duzym obiektem.
        // Czyli wpiecie do listy ma sie odbyc przed tym
        // obiektem, na ktory pokazuje szperacz.
        return szperacz;
        // SUKCES, ZNALEZLISMY WLASCIWE MIEJSCE
      }
      else
      {
        // jesli skok > 1, a odszukany szperaczem element jest
        // wiekszy od wzorcowego, to po prostu byl to za dlugi skok
        // cofamy wiec szperacza do ostatniej dobrej pozycji
        szperacz = dobry;
      }
    }
  }
}
/*************************************************************/
/*************************************************************/
template<class typOBJ>
void Zbior<typOBJ>::daje_na_poczatek(wezel* nowyW, itZb & iter)
{
  pierwszyW = nowyW;
  nowyW->nastepca = iter.wybranyW;

  iter.wybranyW->poprzednik = nowyW;
  nowyW->poprzednik = NULL;

  iter.wybranyW = nowyW;
}
/*************************************************************/
template<class typOBJ>
void Zbior<typOBJ>::daje_w_srodku(wezel* nowyW, itZb & iter)
{
  // znalezienie poprzednika do biezacego
  // i wpisanie mu nowego nastepcy
  (iter.wybranyW->poprzednik)->nastepca = nowyW;

  // wybranyW - to nastepca nowego
  nowyW->nastepca = iter.wybranyW;
  nowyW->poprzednik = iter.wybranyW->poprzednik;
  iter.wybranyW->poprzednik = nowyW;
}
/*************************************************************/
template<class typOBJ>
void Zbior<typOBJ>::daje_na_koniec(wezel* nowyW)
{
  if (!pierwszyW)
  {
    // jesli nic nie ma na liscie
    pierwszyW = nowyW;
  }
  else
  {
    // na liscie juz cos jest
    ostatniW->nastepca = nowyW;
    nowyW->poprzednik = ostatniW;
  }
  ostatniW = nowyW; 			// (a biezacy niech nadal ma NULL)
}

/*************************************************************/
//usuniecie wezla wybranego iteratorem
template<class typOBJ>
void Zbior<typOBJ>::usun(itZb & iter)			  	// ((7))
{
  if (iter.wybranyW == NULL) return;  	// NULLa nie usuwa sie
  dlugosc--;  	     	       		// dlugosc listy

  if (iter.wybranyW == pierwszyW)
    usuwam_pierwszy(iter);
  else
  {
    if (iter.wybranyW == ostatniW)
    {
      usuwam_ostatni(iter);
    }
    else
    {
      usuwam_ze_srodka(iter);
    }
  }
}
/*************************************************************/
template<class typOBJ>
void Zbior<typOBJ>::usuwam_pierwszy(itZb & iter)
{
  // czyli mamy usunac pierwszy
  pierwszyW = pierwszyW->nastepca;
  delete iter.wybranyW;
  iter.wybranyW = pierwszyW;
  if (pierwszyW != NULL)
    pierwszyW->poprzednik = NULL;
  else
    ostatniW = NULL;
}
/*************************************************************/
template<class typOBJ>
void Zbior<typOBJ>::usuwam_ze_srodka(itZb & iter)
{
  // dobrze najpierw znalezc poprzedni wezel
  wezel * poprzW = iter.wybranyW->poprzednik;
  poprzW->nastepca = iter.wybranyW->nastepca;
  (iter.wybranyW->nastepca)->poprzednik = poprzW;
  delete iter.wybranyW;
  iter.wybranyW = poprzW; 		// ustawienie na poprzednim
}
/*************************************************************/
template<class typOBJ>
void Zbior<typOBJ>::usuwam_ostatni(itZb & iter)
{
  // dobrze najpierw znalezc poprzedni wezel
  wezel * poprzW = iter.wybranyW->poprzednik;

  poprzW->nastepca = NULL;
  ostatniW = poprzW;
  delete iter.wybranyW;
  iter.wybranyW = NULL;
}
/*************************************************************/
template<class typOBJ>
void Zbior<typOBJ>::usun(typOBJ ob) 				// ((8))
{
  itZb iter(*this);
  iter = znajdz(ob);
  if (iter) usun(iter);
}
/*************************************************************/
template<class typOBJ>
void Zbior<typOBJ>::usun(itZb & pocz, itZb & kon)		// ((9))

{
  itZb iter(*this);
  iter = pocz;
  while (!(iter == kon))
  {
    usun(*iter++);
  }
}
/*************************************************************/
template<class typOBJ>
Zbior<typOBJ>::~Zbior()
{
  itZb iter(*this);
  for (int i = 0; i < dlugosc; i++)
  {
    delete (iter++).wybranyW;
  }
}
/*************************************************************/
template<class typOBJ>
ostream & operator<<(ostream & stru, Zbior<typOBJ> & spis)
{
  iteratorZbioru<typOBJ> skoczek(spis);
  for (int i = 0; i < spis.rozmiar(); i++)
  {
    stru << *(skoczek++) << "  ";
  }
  stru << endl;
  return stru;
}

/////////////////////////////////////////////////////////////////
////////////////// szablon klasy iteratora Zbioru //////////////
////////////////////////////////////////////////////////////////
template<class typOBJ>
class iteratorZbioru
{
  // deklaracja przyjazni z klasa pojemnikowa
  friend class Zbior<typOBJ> ;

  typedef iteratorZbioru itZb; 	// dla wygody

  // skladniki-dane ---------------
  // wskaznik do wezla, na ktory teraz pokazuje iterator
  typename Zbior<typOBJ>::wezel * wybranyW;
  // referencja egzemplarza pojemnika,  na ktorym pracuje iterator
  Zbior<typOBJ> & pojemnik;

public:
  // --------------------------funkcje skladowe---------------

  // ----------konstruktor
  iteratorZbioru(Zbior<typOBJ> & pojem) :
      pojemnik(pojem)
  {
    wybranyW = pojemnik.pierwszyW;       	       // pierwotnie na poczatku
  }

  // ------------------------------------- operacje przesuniecia -------
  itZb& za_koniec()
  {
    wybranyW = NULL;
    return *this;
  }
  //------------------
  itZb & operator++()
  {
    if (wybranyW) wybranyW = wybranyW->nastepca;
    return *this;
  }
  //------------
  itZb operator++(int)
  {
    itZb stary = *this;
    ++*this;
    return stary;
  }
  //------------------
  typOBJ & operator*()
  {
    return *(wybranyW->wskobj);
  }
  //------------------
  //itZb& na_wezel(int nr);
  void skocz_o(long ile);

  // operator przypisania
  itZb &
  operator=(const itZb & prawy)
  {
    wybranyW = prawy.wybranyW;
    return *this;
  }

  // operator logiczny porownania
  int operator==(const itZb & prawy)		        // it1 = prawy
  {
    if (wybranyW == prawy.wybranyW)
      return 1;
    else
      return 0;
  }

  // operator konwersji
  operator int()		       // potrzebne do: if(iterator)
  {
    if (wybranyW != NULL)
      return 1;
    else
      return 0;
  }

};
/*************************************************************/
template<class typOBJ>
void iteratorZbioru<typOBJ>::skocz_o(long nr)           		// ((10))
{
  for (int i = 0; i < nr; i++)
    (*this)++;
}

/******************************** *****************************/

/*****************************************
 Zmiany w linijkach:

 ------------------
 #include <iostream>
 using namespace std;

 -------------------
 //	friend class itZb ;    <--- zapis z uzyciem typedef (VC++ 6.0 nie akceptuje)

 friend class iteratorZbioru<typOBJ>;  // <--- zwykly zapis, zamiast powyzszego

 -------------------
 Zagodnie z tendencja pojemnikow bibliotecznych zamienialem
 ten pojemnik z "posredniego" na "bezposredni" pamietasz to
 zapewne z przykladowych programow na temat pojemnikow typu lista.
 Taka zmiana nie ma wplywu na dzialanie tego programu, ale mimo
 to postanowilem ja dokonac.

 Wiaze sie to z nastepujacymi dwoma zmianami

 // ------------ konstruktor
 wezel(typOBJ & chowany) : nastepca(NULL) ,poprzednik(NULL) // NOVUM
 {
 wskobj = new typOBJ;         // NOVUM
 *wskobj = chowany ;           // NOVUM
 }
 ~wezel() { delete wskobj; }       // NOVUM

 #### oraz #########

 // ======================================================
 // dalej jest juz tak, jak w liscie. Wstawiamy nowy wezel
 // przed wezlem pokazywanym wlasnie ustawionym iteratorem iter
 // ======================================================

 wezel * nowyW = new wezel(obj) ;//definicja wezla    // ZMIANA
 // od razu umieszczamy w nim informacje o przedmiocie

 // nowyW->wskobj = &obj ;    TO JEST USUNIETA INSTRUKCJA

 dalej po staremu.

 -------------------
 Visual c++6.0 nie przyjmuje pierwszej z ponizszych instrukcji (mimo ze jest poprawna!)

 //	friend class itZb ;                        <--- zamiast tego
 friend class iteratorZbioru<typOBJ>;      //  <---- teraz to
 ------------------

 // zaprzyjazniony operator wypisywania
 friend
 ostream & operator<<  <> (ostream & stru , Zbior<typOBJ> & x);   // <--- dodane <>

 bo:

 main.cpp:81: warning: friend declaration `std::ostream&
 operator<<(std::ostream&, Zbior<typOBJ>&)' declares a non-template function
 main.cpp:81: warning: (if this is not what you intended, make sure the function
 template has already been declared and add <> after the function name here)
 -Wno-non-template-friend disables this warning

 ------------------

 template <class typOBJ>
 void Zbior<typOBJ>::usun(typOBJ ob) 				// ((8))
 {
 itZb iter(*this) ;
 iter = znajdz(ob) ;
 if(iter) {   // <---- dodalem to if - na wypadek nieznalezienia
 // obiektu w Zbiorze
 usun(iter);
 }
 }
 --------------------------
 template <class typOBJ>
 void Zbior<typOBJ>::usun(itZb & pocz, itZb & kon)		// ((9))

 {
 itZb iter(*this) ;
 iter = pocz ;
 while(!(iter == kon))
 {
 usun(*iter++);      // <----------- zauwaz gwiazdke
 }
 }

 --------------------------------------------------------

 itZb & operator++()
 {
 if(wybranyW) wybranyW = wybranyW->nastepca ;                  <--- sprawdzenie   czy nie koniec
 return *this;
 }


 --------------------------
 int main()
 --------------------------

 Mozna by jeszcze dodac sprawdzenie czy te liczby sa rzeczywiscie w Zbiorze

 stad = liczby.znajdz(1.0); 					// ((16))
 dotad = liczby.znajdz(3.0);

 if(stad && dotad)
 {
 liczby.usun(stad, dotad) ;       				// ((17))
 cout << "po usuniecu liczb z zakresu [1-3) \n" << liczby ;
 }
 else
 cout << "Niemozliwe usuniecie przedzialu  [1,3) , \n"
 " bo nie udalo sie ustawienie jednego z iteratorow wyznaczajacych przedzial"
 << endl;


 **************************
 typename Zbior<typOBJ>::wezel * wybranyW ;


 *******************************************/

#endif
