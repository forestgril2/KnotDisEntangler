#include <string.h>
#include <GL/glui.h>
#include <GL/glut.h>
#include "Wezel3D.h"

// glowne parametry symulacji:

//domyslne nazwy plikow:
char wskNazwyDomyslna[64] = {"newgordian.txt"};
char wskNazwyWynikAuto[64] = {"wynik.knb"};
char wskNazwyWynikMoja[64] = {"ciekawe.knb"};
char wskNazwyWynikAutotxt[64] = {"wynik.knt"};
char wskNazwyWynikMojatxt[64] = {"ciekawe.knt"};

//strumien wyjsciowych danych:
//std::iostream Ekran;

static int slices = 16;
static int stacks = 10;

static int A,
    B,          	//przedzialy sprawdzane po gieciach
    nrmojebin = 0, nrmojetxt = 0, nrbin = 0, nrtxt = 0, IleRazySONO = 2500,
    IleRazyBONO = 5000, CoIleUdanychGiecZapis = 3000;

static long int LicznikStanowNizszejEnergii = 0,
    LicznikStanowWyzszejEnergii = 0, LicznikNieudanychGiec = 0,
    LicznikUdanychGiec = 0, LicznikProbGiec = 0, LicznikSONO = 0, LicznikBONO =
        0;

Wektor3D* wskTablicyPomocniczejWektorow;
Wektor3D* wskPomocniczyWektora1;
Wektor3D* wskPomocniczyWektora2;

static double x = 0, y = 0, Epsilon = 1E-10, StosunekPomocniczy = 1,
    Energia = 0, EnergiaNowa = 0, EnergiaPocz = 0, StosunekEnergiiDoPocz = 1,
    StosunekEnergiiDoPoczMin = 1, StosunekEnTermDoEnPocz = 0.0001,
    WagaEnergiiOdpych = 0, StosunekGestosciStanow = 0.5, DlugoscWezlaPoczatkowa,
    DlugoscWezla, PromienMaksObiektu;

static bool SONO = 0,  //czy wlaczony algorytm
    BONO = 0;  //czy wlaczony algorytm

struct Kolor
{
  //punkty
  bool ZbytKrzywy;
  bool Przekryty;
  bool Wybrany;
  bool Giecia1;
  bool Giecia2;
};

static Wezel3D W(300), Pomoc;

//---------!!!!!!!!!!!!!!!!!!! potrzebne do SONO
CORECT ZZ;

/* GLUT callback Handlers */
//-------------------------------------------------
static void resize(int width, int height)
{
  PromienMaksObiektu = W.WyznaczPromienMax();
  if (height == 0) height = 1;
  const float ar = (float) width / (float) height;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  double alfa = 720 * atan(PromienMaksObiektu / 100.0) / (2 * M_PI);
  gluPerspective(alfa, ar, 10, 100 + 3 * PromienMaksObiektu);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

static void display(void)
{
  //const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
  //const double a = t*90.0;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3d(0, 1, 0);

  if (SONO) //algorytm zaciskania
  {
    if (!(LicznikUdanychGiec % CoIleUdanychGiecZapis))  //zapisywanie do pliku
    {
      W.ZapiszTxt(wskNazwyWynikAutotxt);
      W.ZapiszBin(wskNazwyWynikAuto);
    }
    for (int i = 0; i < IleRazySONO; i++)
    {
      ZZ.Zaciskaj();
      LicznikSONO++;
    }
    //nieusuwanie dlugosci
    DlugoscWezlaPoczatkowa = W.WyznaczDlugoscElemSrednia() * W.IloscElem;
    DlugoscWezla = W.WyznaczDlugoscElemSrednia() * W.IloscElem;
  }

  if (BONO)  //ALGORYTM ROZPLATUJACY
  {
    double KatGiecia;
    int ZapamietaneIlePrzekrycSprawdzac;
    int ZapamietaneIlePunktowKrzywSprawdzac;

    {  //bedziemy odrzucac kazde przekrycie i kazde przekroczenie krzywizny
      ZapamietaneIlePrzekrycSprawdzac = W.IlePrzekrycSprawdzac;
      ZapamietaneIlePunktowKrzywSprawdzac = W.IlePunktowKrzywSprawdzac;
      W.IlePrzekrycSprawdzac = 1;
      W.IlePunktowKrzywSprawdzac = 1;
    }

    for (int i = 0; i < IleRazyBONO; i++)          //PETLA OBLICZEN
    {
      LicznikBONO++; //zwiekszamy licznik podstawowy
      { //LOSOWANIA
        W.UstawPunktGiecia1(W.Losownik.LosujInt(W.IloscElem - 1));
        W.UstawPunktGiecia2(W.Losownik.LosujInt(W.IloscElem - 1));

        W.KatGieciaMax = 0.9 * W.WyznaczKatGieciaMax(); //ta liczba powinna zalezec od gestosci kulek!

        KatGiecia = W.Losownik.LosujLongDouble(-W.KatGieciaMax, W.KatGieciaMax);
      }

      //gniemy mniejsza czesc dla oszczednosci czasu:
      if (odlP(W.PunktGiecia1, W.PunktGiecia2, W.IloscElem) > W.IloscElem / 2)
      {
        W.UstawPunktGiecia1(W.PunktGiecia2);  //zamiana punktow
        KatGiecia = -KatGiecia;
      }
      {  //zapamietanie stanu wyginanej czesci przed gieciem:
        if (W.PunktGiecia1 < W.PunktGiecia2)  //tablica dzielona albo nie
        {  //niedzielona
          wskPomocniczyWektora1 = &(wskTablicyPomocniczejWektorow[W.PunktGiecia1
              + 1]);
          wskPomocniczyWektora2 = &(W.wskTablicyElem[W.PunktGiecia1 + 1]);
          for (int i = W.PunktGiecia1 + 1; i < W.PunktGiecia2; i++)
          {
            *wskPomocniczyWektora1++ = *wskPomocniczyWektora2++;
          }
        }
        else
        {  //dzielona
          wskPomocniczyWektora1 = &(wskTablicyPomocniczejWektorow[W.PunktGiecia1
              + 1]);
          wskPomocniczyWektora2 = &(W.wskTablicyElem[W.PunktGiecia1 + 1]);
          for (int i = W.PunktGiecia1 + 1; i < W.IloscElem; i++)
          {
            *wskPomocniczyWektora1++ = *wskPomocniczyWektora2++;
          }
          wskPomocniczyWektora1 = wskTablicyPomocniczejWektorow;
          wskPomocniczyWektora2 = W.wskTablicyElem;
          for (int i = 0; i < W.PunktGiecia2; i++)
          {
            *wskPomocniczyWektora1++ = *wskPomocniczyWektora2++;
          }
        }
      }
      {  //GIECIE WEZLA
        KatGiecia = W.Zegnij(KatGiecia);
        //W.DodajGiecie(GiecieWezla(W.PunktGiecia1,W.PunktGiecia2,KatGiecia,true)); //zapis do listy
      }
      {  //czyszczenie zbioru przekryc i giec
        W.WyczyscPrzekrycia(); //ZAPOMINAMY O WYZNACZONYCH POPRZEDNIO PRZEKRYCIACH...
        W.WyczyscElementyPrzekrKrzyw(); //ZAPOMINAMY O WYZNACZONEJ POPRZEDNIO KRZYWIZNIE...
      }

      if (W.PunktGiecia1 < W.PunktGiecia2) //okreslenie przedzialu dla potrzeb sprawdzania przekryc
      { //jesli giecie niedzielone
        A = W.PunktGiecia1;
        B = W.PunktGiecia2;
      }
      else
      { //jesli giecie dzielone
        A = W.PunktGiecia1;
        B = W.PunktGiecia2 + W.IloscElem;
      }

      if (W.SprawdzCzySaPrzekryciaPoGieciu(A, B) ///uwarunkowania podstawowe (krzywizna i twardosc)
          || !W.SprawdzKrzywProm(W.PunktGiecia1,
              W.PunktGiecia1 + 1 == W.IloscElem ?
                  W.IloscElem - 2 : W.PunktGiecia1 + 1)
          || !W.SprawdzKrzywProm(W.PunktGiecia2,
              W.PunktGiecia2 + 1 == W.IloscElem ?
                  W.IloscElem - 2 : W.PunktGiecia2 + 1))

      { //przywracamy stan poprzedni ale najpierw zwiekszamy ilosc nieudanych giec:
        LicznikNieudanychGiec++;
        //W.Zegnij(-KatGiecia); //albo kopiowanie tablicy, jak ponizej
        if (W.PunktGiecia1 < W.PunktGiecia2) //tablica dzielona lub nie
        { //niedzielona
          wskPomocniczyWektora1 = &(wskTablicyPomocniczejWektorow[W.PunktGiecia1
              + 1]);
          wskPomocniczyWektora2 = &(W.wskTablicyElem[W.PunktGiecia1 + 1]);
          for (int i = W.PunktGiecia1 + 1; i < W.PunktGiecia2; i++)
          {
            *wskPomocniczyWektora2++ = *wskPomocniczyWektora1++;
          }
        }
        else
        { //dzielona
          wskPomocniczyWektora1 = &(wskTablicyPomocniczejWektorow[W.PunktGiecia1
              + 1]);
          wskPomocniczyWektora2 = &(W.wskTablicyElem[W.PunktGiecia1 + 1]);
          for (int i = W.PunktGiecia1 + 1; i < W.IloscElem; i++)
          {
            *wskPomocniczyWektora2++ = *wskPomocniczyWektora1++;
          }
          wskPomocniczyWektora1 = wskTablicyPomocniczejWektorow;
          wskPomocniczyWektora2 = W.wskTablicyElem;
          for (int i = 0; i < W.PunktGiecia2; i++)
          {
            *wskPomocniczyWektora2++ = *wskPomocniczyWektora1++;
          }
        }
      }
      else                            ///uwarunkowania energetyczne
      {                            //a wczesniej zwiekszamy licznik udanych giec
        LicznikUdanychGiec++;
        Energia = EnergiaNowa;
        /*
         if(!(LicznikUdanychGiec%CoIleUdanychGiecZapis)) //zapisywanie do pliku
         {
         W.ZapiszTxt(wskNazwyWynikAutotxt);
         W.ZapiszBin(wskNazwyWynikAuto);
         }

         {//OBLICZENIE NOWEJ ENERGII WEZLA
         EnergiaNowa=///W.WyznaczEnergieKrzyw()+WagaEnergiiOdpych*W.WyznaczEnergieOdpych();
         +W.WyznaczEnergieOdSrod();
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
         wskPomocniczyWektora2=&(W.wskTablicyElem[W.PunktGiecia1+1]);
         for(int i=W.PunktGiecia1+1;i<W.PunktGiecia2;i++)
         {
         *wskPomocniczyWektora2++=*wskPomocniczyWektora1++;
         }
         }
         else
         {//dzielona
         wskPomocniczyWektora1=&(wskTablicyPomocniczejWektorow[W.PunktGiecia1+1]);
         wskPomocniczyWektora2=&(W.wskTablicyElem[W.PunktGiecia1+1]);
         for(int i=W.PunktGiecia1+1;i<W.IloscElem;i++)
         {
         *wskPomocniczyWektora2++=*wskPomocniczyWektora1++;
         }
         wskPomocniczyWektora1=wskTablicyPomocniczejWektorow;
         wskPomocniczyWektora2=W.wskTablicyElem;
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
         }
         */

      }
    }

    {                  //przywracamy ustawienia sprawdzania krzywizny i przekryc
      W.IlePrzekrycSprawdzac = ZapamietaneIlePrzekrycSprawdzac;
      W.IlePunktowKrzywSprawdzac = ZapamietaneIlePunktowKrzywSprawdzac;
    }
  }

  {                            //ENERGIa WEZLA ///
    Energia = +W.WyznaczEnergieOdSrod(); ///WagaEnergiiOdpych*W.WyznaczEnergieOdpych()+.WyznaczEnergieKrzyw();
    StosunekEnergiiDoPocz = Energia / EnergiaPocz;
    StosunekEnergiiDoPoczMin =
        StosunekEnergiiDoPocz < StosunekEnergiiDoPoczMin ?
            StosunekEnergiiDoPocz : StosunekEnergiiDoPoczMin;
  }

  {                            //usuwanie dlugosci
    DlugoscWezla = W.WyznaczDlugoscElemSrednia() * W.IloscElem;
    StosunekPomocniczy = DlugoscWezlaPoczatkowa / DlugoscWezla;
    if (StosunekPomocniczy - 1.0 > Epsilon
        || StosunekPomocniczy - 1.0 < -Epsilon) //jesli wzgledna roznica dlugosci jest wieksza niz 1E-10
    { //probujemy przeskalowac wezel
      W.Skaluj(StosunekPomocniczy);
      if (W.SprawdzCzySaPrzekrycia()  //jesli skalujac niechcacy przekrylim
      || !W.SprawdzKrzywProm()    //albo niechcacy przekrzywilim
          || !W.SprawdzKrzywProm())
      {    //skalowanie z powrotem
        W.Skaluj(1.0 / StosunekPomocniczy);
      }
    }
  }

  {    //ZASADY ZACHOWANIA:
    W.Przesun(Wektor3D() - W.WyznaczSrodekMasy());
    W.UsunMomentor(&Pomoc);
  }

  {    //KOMUNIKATY KONSOLOWE
    cout << "Dlugosc pocz: \t" << DlugoscWezlaPoczatkowa << "\t|";
    cout << "Energ. pocz.: \t" << EnergiaPocz << "\t|";
    cout << "LicznikBONO: \t" << LicznikBONO << "\t\t|";
    cout << "PUSTE: \t" << W.PromienPrzekroju << "\t|";
    cout << endl;
    cout << "Dlugosc: \t" << DlugoscWezla << "\t|";
    cout << "Energia: \t" << Energia << "\t|";
    cout << "Licznik giec: \t" << LicznikUdanychGiec << "\t\t|";
    cout << "Stosunek GS: \t" << StosunekGestosciStanow << "\t|";
    cout << endl;
    cout << "Stos. dl.: \t" << DlugoscWezla / DlugoscWezlaPoczatkowa << "\t|";
    cout << "Stos. en.: \t" << StosunekEnergiiDoPocz << "\t|";
    cout << "Stos. giec: \t"
        << double(LicznikUdanychGiec) / double(LicznikBONO + 1) << "\t|";
    cout << "kB*T/EnPocz: \t" << StosunekEnTermDoEnPocz << "\t|";
    cout << endl;
    cout << "Promien: \t" << W.PromienPrzekroju << "\t|";
    cout << "Stos. min.: \t" << StosunekEnergiiDoPoczMin << "\t|";
    cout << "Stos. giec: \t"
        << double(LicznikUdanychGiec) / double(LicznikBONO + 1) << "\t|";
    cout << "PUSTE: \t" << StosunekEnTermDoEnPocz << "\t|";
    cout << endl;
    cout << endl;
  }

  for (int i = 0; i < W.IloscElem; i++)              //RYSOWANIE
  {              //rysowanie kulki
                 //if(i%2){   //rysowanie co drugiego
    glPushMatrix();
    float a, b, c, rr = 0.0f, gg = 0.5f, bb = 0.0f;

    a = W.wskTablicyElem[i].x;
    b = W.wskTablicyElem[i].y;
    c = W.wskTablicyElem[i].z;

    PromienMaksObiektu = W.WyznaczPromienMax();
    glTranslated(a, b, c - (100 + 1.2 * PromienMaksObiektu));

    if (i == W.PunktGiecia1)
    {
      bb += 1.0;
      gg -= 0.5;
    }
    if (i == W.PunktGiecia2)
    {
      bb += 0.4f;
      gg -= 0.5;
    }
    if (W.PrzekryciaPojedynczo.find(i) != W.PrzekryciaPojedynczo.end())
    {
      rr += 0.37;
      gg = 0;
    }
    if (W.ElementyPrzekrKrzyw.find(i) != W.ElementyPrzekrKrzyw.end())
    {
      rr += 0.63;
      gg = 0;
    }

    if (W.ElementyWybrane.znajdz(i))
    {    //max kolor ziel
      gg = 1;
    }

    glColor3f(rr, gg, bb);
    glutSolidSphere(W.PromienPrzekroju, slices, stacks);

    glPopMatrix();    //};
  }

  glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 27: //escape
      exit(0);
      break;

    case 'G':
      StosunekEnTermDoEnPocz *= 1.1;
      break;

    case 'F':
      StosunekEnTermDoEnPocz *= 0.90909090909090909;
      break;

    case 'g':
      StosunekEnTermDoEnPocz *= 1.003;
      break;

    case 'f':
      StosunekEnTermDoEnPocz *= 0.9970089730807577268;
      break;

    case 'q':
      W.ObracajZ(-0.017453292519943295 / 2.0);
      Pomoc = W;
      break;

    case 'e':
      W.ObracajZ(0.017453292519943295 / 2.0);
      Pomoc = W;
      break;

    case 'w':
      W.ObracajX(0.017453292519943295 / 2.0);
      Pomoc = W;
      break;

    case 's':
      W.ObracajX(-0.017453292519943295 / 2.0);
      Pomoc = W;
      break;

    case 'a':
      W.ObracajY(0.017453292519943295 / 2.0);
      Pomoc = W;
      break;

    case 'd':
      W.ObracajY(-0.017453292519943295 / 2.0);
      Pomoc = W;
      break;

    case 'Q':
      W.ObracajZ(-3 * 0.052359877559829885);
      Pomoc = W;
      break;

    case 'E':
      W.ObracajZ(3 * 0.052359877559829885);
      Pomoc = W;
      break;

    case 'W':
      W.ObracajX(3 * 0.052359877559829885);
      Pomoc = W;
      break;

    case 'S':
      W.ObracajX(-3 * 0.052359877559829885);
      Pomoc = W;
      break;

    case 'A':
      W.ObracajY(3 * 0.052359877559829885);
      Pomoc = W;
      break;

    case 'D':
      W.ObracajY(-3 * 0.052359877559829885);
      Pomoc = W;
      break;

    case '+':
      W.Skaluj(1.1);
      W.WyznaczWszystko();
      DlugoscWezla = DlugoscWezlaPoczatkowa = W.IloscElem
          * W.DlugoscElemSrednia;
      Pomoc = W;
      break;

    case '_':
      W.Skaluj(0.90909090909090909);
      W.WyznaczWszystko();
      Pomoc = W;
      DlugoscWezla = DlugoscWezlaPoczatkowa = W.IloscElem
          * W.DlugoscElemSrednia;
      break;

    case ')':
      W.Skaluj(1.003);
      W.WyznaczWszystko();
      Pomoc = W;
      DlugoscWezla = DlugoscWezlaPoczatkowa = W.IloscElem
          * W.DlugoscElemSrednia;
      break;

    case '(':
      W.Skaluj(0.9970089730807577268);
      W.WyznaczWszystko();
      Pomoc = W;
      DlugoscWezla = DlugoscWezlaPoczatkowa = W.IloscElem
          * W.DlugoscElemSrednia;
      break;

    case '[':
      W.UstawPunktGiecia1(W.PunktGiecia1 - 1);
      break;

    case ']':
      W.UstawPunktGiecia1(W.PunktGiecia1 + 1);
      break;

    case '{':
      W.UstawPunktGiecia2(W.PunktGiecia2 - 1);
      break;

    case '}':
      W.UstawPunktGiecia2(W.PunktGiecia2 + 1);
      break;

    case ',':
      W.Zegnij(-2 * 0.0349065850398865915);
      W.WyznaczWszystko();
      W.Przesun(Wektor3D() - W.WyznaczSrodekMasy());
      W.UsunMomentor(&Pomoc);
      Pomoc = W;
      break;

    case '.':
      W.Zegnij(2 * 0.0349065850398865915);
      W.WyznaczWszystko();
      W.Przesun(Wektor3D() - W.WyznaczSrodekMasy());
      W.UsunMomentor(&Pomoc);
      Pomoc = W;
      break;

    case '<':
      Pomoc = W;
      W.Zegnij(-0.00872664625997164788);
      W.WyznaczWszystko();
      W.Przesun(Wektor3D() - W.WyznaczSrodekMasy());
      W.UsunMomentor(&Pomoc);
      Pomoc = W;
      break;

    case '>':
      Pomoc = W;
      W.Zegnij(0.00872664625997164788);
      W.WyznaczWszystko();
      W.Przesun(Wektor3D() - W.WyznaczSrodekMasy());
      W.UsunMomentor(&Pomoc);
      Pomoc = W;
      break;

    case 'u':
      BONO = BONO ? 0 : 1;
      break;

    case 'i':
      SONO = SONO ? 0 : 1;
      break;

    case 'z':
      W.ZapiszBin(wskNazwyWynikMoja);
      break;

    case 'Z':
      W.ZapiszTxt(wskNazwyWynikMojatxt);
      break;

    case 'o':
      nrmojebin--;
      nrmojebin = nrmojebin < 0 ? 0 : nrmojebin;
      W.WczytajBin(wskNazwyWynikMoja, nrmojebin);
      Pomoc = W;
      W.WyznaczWszystko();
      delete[] wskTablicyPomocniczejWektorow;
      wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscElem];
      break;

    case 'O':
      nrmojetxt--;
      nrmojetxt = nrmojetxt < 0 ? 0 : nrmojetxt;
      W.WczytajTxt(wskNazwyWynikMojatxt, nrmojetxt);
      Pomoc = W;
      W.WyznaczWszystko();
      delete[] wskTablicyPomocniczejWektorow;
      wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscElem];
      break;

    case 'p':
      W.WczytajBin(wskNazwyWynikMoja, ++nrmojebin);
      Pomoc = W;
      W.WyznaczWszystko();
      delete[] wskTablicyPomocniczejWektorow;
      wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscElem];
      break;

    case 'P':
      W.WczytajTxt(wskNazwyWynikMojatxt, ++nrmojetxt);
      Pomoc = W;
      W.WyznaczWszystko();
      delete[] wskTablicyPomocniczejWektorow;
      wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscElem];
      break;

    case 'x':
      nrbin--;
      nrbin = nrbin < 0 ? 0 : nrbin;
      W.WczytajBin(wskNazwyWynikAuto, nrbin);
      W.WyznaczWszystko();
      delete[] wskTablicyPomocniczejWektorow;
      wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscElem];
      break;

    case 'c':
      W.WczytajBin(wskNazwyWynikAuto, ++nrbin);
      Pomoc = W;
      W.WyznaczWszystko();
      delete[] wskTablicyPomocniczejWektorow;
      wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscElem];
      break;

    case 'X':
      nrtxt--;
      nrtxt = nrtxt < 0 ? 0 : nrtxt;
      W.WczytajTxt(wskNazwyWynikAutotxt, nrtxt);
      Pomoc = W;
      W.WyznaczWszystko();
      delete[] wskTablicyPomocniczejWektorow;
      wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscElem];
      break;

    case 'C':
      W.WczytajTxt(wskNazwyWynikAutotxt, ++nrtxt);
      Pomoc = W;
      W.WyznaczWszystko();
      delete[] wskTablicyPomocniczejWektorow;
      wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscElem];
      break;

    case 'v':
      nrbin -= 10;
      nrbin = nrbin < 0 ? 0 : nrbin;
      W.WczytajBin(wskNazwyWynikAuto, nrbin);
      W.WyznaczWszystko();
      delete[] wskTablicyPomocniczejWektorow;
      wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscElem];
      break;

    case 'b':
      nrbin += 10;
      W.WczytajBin(wskNazwyWynikAuto, nrbin);
      Pomoc = W;
      W.WyznaczWszystko();
      delete[] wskTablicyPomocniczejWektorow;
      wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscElem];
      break;

    case 'V':
      nrtxt -= 10;
      nrtxt = nrtxt < 0 ? 0 : nrtxt;
      W.WczytajTxt(wskNazwyWynikAutotxt, nrtxt);
      Pomoc = W;
      W.WyznaczWszystko();
      delete[] wskTablicyPomocniczejWektorow;
      wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscElem];
      break;

    case 'B':
      nrtxt += 10;
      W.WczytajTxt(wskNazwyWynikAutotxt, nrtxt);
      Pomoc = W;
      W.WyznaczWszystko();
      delete[] wskTablicyPomocniczejWektorow;
      wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscElem];
      break;

    case 'n':
      nrbin -= 100;
      nrbin = nrbin < 0 ? 0 : nrbin;
      W.WczytajBin(wskNazwyWynikAuto, nrbin);
      W.WyznaczWszystko();
      delete[] wskTablicyPomocniczejWektorow;
      wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscElem];
      break;

    case 'm':
      nrbin += 100;
      W.WczytajBin(wskNazwyWynikAuto, nrbin);
      Pomoc = W;
      W.WyznaczWszystko();
      delete[] wskTablicyPomocniczejWektorow;
      wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscElem];
      break;

    case 'N':
      nrtxt -= 100;
      nrtxt = nrtxt < 0 ? 0 : nrtxt;
      W.WczytajTxt(wskNazwyWynikAutotxt, nrtxt);
      Pomoc = W;
      W.WyznaczWszystko();
      delete[] wskTablicyPomocniczejWektorow;
      wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscElem];
      break;

    case 'M':
      nrtxt += 100;
      W.WczytajTxt(wskNazwyWynikAutotxt, nrtxt);
      Pomoc = W;
      W.WyznaczWszystko();
      delete[] wskTablicyPomocniczejWektorow;
      wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscElem];
      break;

    case 't':
      stacks++;
      break;

    case 'T':
      stacks--;
      break;

    case 'y':
      slices++;
      break;

    case 'Y':
      slices--;
      break;

    case 'r':
      W.PromienPrzekroju == 1 ? W.PromienPrzekroju = 0.5 : W.PromienPrzekroju =
                                    1;
      W.WyznaczWszystko();
      Pomoc = W;
      break;
  }
  glutPostRedisplay();
}

static void idle(void)
{
  glutPostRedisplay();
}

//MOJE:
static void Wybierz(int button, int state, int a, int b)
{
  W.WyznaczWszystko();
  if (state == GLUT_DOWN)
  {
    double h = glutGet(GLUT_WINDOW_HEIGHT);
    double w = glutGet(GLUT_WINDOW_WIDTH);

    float xy_aspect = w / h;

    x = (a / w - 0.5) * 2.3 * W.PromienMax * xy_aspect;
    y = -(b / h - 0.5) * 2.3 * W.PromienMax;

    double najbl = 19;
    int n = 0, wybr = 0, koniec = 0;
    for (int i = 0; !koniec; i++)
    {
      Wektor3D P(x, y, W.PromienMax - W.PromienPrzekroju * i);
      n = W.ZnajdzNajblElem(NULL, NULL, &P);
      if ((W.wskTablicyElem[n] - P).length() < W.PromienPrzekroju || i > 100)
      {
        koniec = 1;
        wybr = n;
      };
    };

    if (button == GLUT_LEFT_BUTTON) W.UstawPunktGiecia(wybr);
    if (button == GLUT_RIGHT_BUTTON) W.WybierzElem(wybr);

  };
}
;

const GLfloat light_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
const GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat light_position[] = {2.0f, 5.0f, 5.0f, 0.0f};

const GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
const GLfloat mat_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
const GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat high_shininess[] = {100.0f};

/* Program entry point */

int main(int argc, char *argv[])
{
  { //ustawienia strumienia wyjsciowego
    cout.setf(ios::fixed);
    cout.precision(8);
  }

  { //ustawienia wezla
    W.WczytajTxt(wskNazwyDomyslna);
    wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscElem];
    W.Skaluj(1.05);
    W.WyznaczWszystko();
    W.Przesun(Wektor3D(0, 0, 0) - W.SrodekMasy);
    PromienMaksObiektu = W.PromienMax;
    Pomoc = W;
    WagaEnergiiOdpych = 1; ///W.WyznaczEnergieKrzyw()/W.WyznaczEnergieOdpych();

    EnergiaPocz = Energia = EnergiaNowa = ///W.WyznaczEnergieKrzyw()+WagaEnergiiOdpych*W.WyznaczEnergieOdpych();
        +W.WyznaczEnergieOdSrod();

    DlugoscWezla = DlugoscWezlaPoczatkowa = W.IloscElem * W.DlugoscElemSrednia;

    W.IlePrzekrycSprawdzac = W.IloscElem;
    W.IlePunktowKrzywSprawdzac = W.IloscElem;

    //DLA SONO:
    ZZ.Wprowadz((WK3D*) W.wskTablicyElem, W.IloscElem);
    ZZ.Szybkosc(2.0);

    //EnergiaZerowa=
  }

  glutInit(&argc, argv);
  glutInitWindowSize(250, 250);
  glutInitWindowPosition(0, 50);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

  glutCreateWindow("Gordian UnKnot Entangler!!!");

  glutReshapeFunc(resize);
  glutDisplayFunc(display);
  glutKeyboardFunc(key);
  glutIdleFunc(idle);

  //MOJE:
  glutMouseFunc(Wybierz);

  glClearColor(1, 1, 1, 1);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

  glutMainLoop();
  return EXIT_SUCCESS;
}
