#include <GL/glui.h>
#include <GL/glut.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <deque>
#include <set>
#include <vector>
#include <math.h>

using namespace std;

#include "GiecieWezla.h"
#include "funkcjezew.h"
#include "Losowania.h"
#include "Knot3D.h"
#include "KnotPliker.h"
#include "Histogram.h"
#include "KnotSimData.h"
#include "KnotSimulator.h"

//domyslne nazwy plikow:
char wskNazwyDomyslna[256] = {"knot.knt"};
char wskNazwyWynikAuto[256] = {"wynik.knb"};
char wskNazwyWynikMoja[256] = {"ciekawe.knb"};
char wskNazwyWynikAutotxt[256] = {"wynik.knt"};
char wskNazwyWynikMojatxt[256] = {"ciekawe.knt"};

KnotSimulator SymulatorGlowny(wskNazwyDomyslna, wskNazwyWynikAuto,
    wskNazwyWynikMoja, wskNazwyWynikAutotxt, wskNazwyWynikMojatxt);

KnotSimulator& S = SymulatorGlowny;
Knot3D& W = SymulatorGlowny.Symulowany;

static int slices = 10;
static int stacks = 6;

double x = 0, y = 0;

double StosunekPromieniaWizjiDoPromieniaMax = 1.2;

bool WszystkieKolory = true;

struct Kolor
{
  //punkty
  bool ZbytKrzywy;
  bool Przekryty;
  bool Wybrany;
  bool Giecia1;
  bool Giecia2;
};

//macierze OpenGL obrotow
static Macierz4D MacierzPomocnicza;
static GLdouble ObrotyGL[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
/* GLUT callback Handlers */
//-------------------------------------------------
static void resize(int width, int height)
{
  S.PromienMaksObiektu = W.WyznaczPromienMax();
  if (height == 0) height = 1;
  const float ar = (float) width / (float) height;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  double alfa = 720 * atan(S.PromienMaksObiektu / 100.0) / (2 * M_PI);
  //gluPerspective(alfa,ar, 10, 100+3*S.PromienMaksObiektu);
  double stos = StosunekPromieniaWizjiDoPromieniaMax;
  glOrtho(-stos * ar * S.PromienMaksObiektu, stos * ar * S.PromienMaksObiektu,
      -stos * S.PromienMaksObiektu, stos * S.PromienMaksObiektu,
      -1.1 * S.PromienMaksObiektu, 1.1 * S.PromienMaksObiektu);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

static void display(void)
{
  //const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
  //const double a = t*90.0;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3d(0, 1, 0);

  S.ZastosujAlgorytmy();
  S.InfoKonsolowe();

  glMatrixMode(GL_MODELVIEW);
  S.PromienMaksObiektu = W.WyznaczPromienMax();
  glLoadIdentity();
  //glTranslated(0,0,2.3*S.PromienMaksObiektu);
  glMultMatrixd(ObrotyGL);

  for (int i = 0; i < W.IloscSegm; i++)              //RYSOWANIE
  {              //rysowanie kulki
                 //zachowanie widoku modelu
    glPushMatrix();
    float a, b, c, rr = 0.0f, gg = 0.5f, bb = 0.0f;

    a = W.wskTablicySegm[i].x;
    b = W.wskTablicySegm[i].y;
    c = W.wskTablicySegm[i].z;

    glTranslated(a, b, c);

    if (WszystkieKolory)
    {
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
      if (W.SegmentyPrzekrKrzyw.find(i) != W.SegmentyPrzekrKrzyw.end())
      {
        rr += 0.63;
        gg = 0;
      }

      if (W.SegmentyWybrane.znajdz(i))
      {    //max kolor ziel
        gg = 1;
      }
    }

    glColor3f(rr, gg, bb);
    glutSolidSphere(W.PromienPrzekroju, slices, stacks);

    //przywrocenie widoku modelu
    glPopMatrix();
  }
  glFlush();
  glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
  switch (key)
  {

    case '%':
      slices = 10;
      stacks = 6;
      break;

    case '7':
      W.UstawPromienPrzekroju(W.PromienPrzekroju * (1.0 / 1.05));
      W.WyznaczWszystko();
      break;

    case '8':
      W.UstawPromienPrzekroju(W.PromienPrzekroju * (1.05));
      W.WyznaczWszystko();
      break;

    case '5':
      W.Przesun(Wektor3D(-0.03 * S.PromienMaksObiektu, 0, 0));
      break;

    case '6':
      W.Przesun(Wektor3D(0.03 * S.PromienMaksObiektu, 0, 0));
      break;

    case '!':
      W.WczytajTxtSzkielet(wskNazwyDomyslna, 0);
      S.UstawParametry();
      break;

    case '1':
      WszystkieKolory = !WszystkieKolory;
      break;

    case '2':
      W.UstawIloscSegm2();
      S.UstawParametry();
      break;

    case '@':
      W.CenterMirror();
      S.UstawParametry();
      break;

    case '3':
      W.UstawIloscSegm(int(W.IloscSegm * (1.0 / 1.0333)));
      S.UstawParametry();
      break;

    case '4':
      W.UstawIloscSegm(int(W.IloscSegm * (1.0333)));
      S.UstawParametry();
      break;

    case '#':
      W.UstawIloscSegm(int(W.IloscSegm * (1.0 / 1.111)));
      S.UstawParametry();
      break;

    case '$':
      W.UstawIloscSegm(int(W.IloscSegm * (1.111)));
      S.UstawParametry();
      break;

    case 27: //escape
      exit(0);
      break;

    case 'h':
      MacierzPomocnicza.identity();
      for (int i = 0; i < 16; i++)
        ObrotyGL[i] = MacierzPomocnicza.at(i / 4, i % 4);
      break;

    case 'G':
      S.Temperatura *= 1.1;
      break;

    case 'F':
      S.Temperatura *= 0.90909090909090909;
      break;

    case 'g':
      S.Temperatura *= 1.003;
      break;

    case 'f':
      S.Temperatura *= 0.9970089730807577268;
      break;

    case 'K':
      S.StosunekKataDoMax *= 1.1;
      break;

    case 'J':
      S.StosunekKataDoMax *= 0.90909090909090909;
      break;

    case 'k':
      S.StosunekKataDoMax *= 1.003;
      break;

    case 'j':
      S.StosunekKataDoMax *= 0.9970089730807577268;
      break;

    case 'q':
      MacierzPomocnicza = Macierz4D(ObrotyGL);
      MacierzPomocnicza = MacierzPomocnicza.createRotationAroundAxis(0, 0,
          -0.017453292519943295 / 2.0) * MacierzPomocnicza;
      for (int i = 0; i < 16; i++)
        ObrotyGL[i] = MacierzPomocnicza.at(i / 4, i % 4);
      //W.ObracajZ(-0.017453292519943295/2.0);  Pomoc=W;
      break;

    case 'e':
      MacierzPomocnicza = Macierz4D(ObrotyGL);
      MacierzPomocnicza = MacierzPomocnicza.createRotationAroundAxis(0, 0,
          0.017453292519943295 / 2.0) * MacierzPomocnicza;
      for (int i = 0; i < 16; i++)
        ObrotyGL[i] = MacierzPomocnicza.at(i / 4, i % 4);
      //W.ObracajZ(0.017453292519943295/2.0);  Pomoc=W;
      break;

    case 'w':
      MacierzPomocnicza = Macierz4D(ObrotyGL);
      MacierzPomocnicza = MacierzPomocnicza.createRotationAroundAxis(
          0.017453292519943295 / 2.0, 0, 0) * MacierzPomocnicza;
      for (int i = 0; i < 16; i++)
        ObrotyGL[i] = MacierzPomocnicza.at(i / 4, i % 4);
      //W.ObracajX(0.017453292519943295/2.0);  Pomoc=W;
      break;

    case 's':
      MacierzPomocnicza = Macierz4D(ObrotyGL);
      MacierzPomocnicza = MacierzPomocnicza.createRotationAroundAxis(
          -0.017453292519943295 / 2.0, 0, 0) * MacierzPomocnicza;
      for (int i = 0; i < 16; i++)
        ObrotyGL[i] = MacierzPomocnicza.at(i / 4, i % 4);
      //W.ObracajX(-0.017453292519943295/2.0); Pomoc=W;
      break;

    case 'a':
      MacierzPomocnicza = Macierz4D(ObrotyGL);
      MacierzPomocnicza = MacierzPomocnicza.createRotationAroundAxis(0,
          0.017453292519943295 / 2.0, 0) * MacierzPomocnicza;
      for (int i = 0; i < 16; i++)
        ObrotyGL[i] = MacierzPomocnicza.at(i / 4, i % 4);
      //W.ObracajY(0.017453292519943295/2.0);   Pomoc=W;
      break;

    case 'd':
      MacierzPomocnicza = Macierz4D(ObrotyGL);
      MacierzPomocnicza = MacierzPomocnicza.createRotationAroundAxis(0,
          -0.017453292519943295 / 2.0, 0) * MacierzPomocnicza;
      for (int i = 0; i < 16; i++)
        ObrotyGL[i] = MacierzPomocnicza.at(i / 4, i % 4);
      //W.ObracajY(-0.017453292519943295/2.0);  Pomoc=W;
      break;

    case 'Q':
      MacierzPomocnicza = Macierz4D(ObrotyGL);
      MacierzPomocnicza = MacierzPomocnicza.createRotationAroundAxis(0, 0,
          -3 * 0.052359877559829885) * MacierzPomocnicza;
      for (int i = 0; i < 16; i++)
        ObrotyGL[i] = MacierzPomocnicza.at(i / 4, i % 4);
      //W.ObracajZ(-3*0.052359877559829885);  Pomoc=W;
      break;

    case 'E':
      MacierzPomocnicza = Macierz4D(ObrotyGL);
      MacierzPomocnicza = MacierzPomocnicza.createRotationAroundAxis(0, 0,
          3 * 0.052359877559829885) * MacierzPomocnicza;
      for (int i = 0; i < 16; i++)
        ObrotyGL[i] = MacierzPomocnicza.at(i / 4, i % 4);
      //W.ObracajZ(3*0.052359877559829885);   Pomoc=W;
      break;

    case 'W':
      MacierzPomocnicza = Macierz4D(ObrotyGL);
      MacierzPomocnicza = MacierzPomocnicza.createRotationAroundAxis(
          3 * 0.052359877559829885, 0, 0) * MacierzPomocnicza;
      for (int i = 0; i < 16; i++)
        ObrotyGL[i] = MacierzPomocnicza.at(i / 4, i % 4);
      //W.ObracajX(3*0.052359877559829885);   Pomoc=W;
      break;

    case 'S':
      MacierzPomocnicza = Macierz4D(ObrotyGL);
      MacierzPomocnicza = MacierzPomocnicza.createRotationAroundAxis(
          -3 * 0.052359877559829885, 0, 0) * MacierzPomocnicza;
      for (int i = 0; i < 16; i++)
        ObrotyGL[i] = MacierzPomocnicza.at(i / 4, i % 4);
      //W.ObracajX(-3*0.052359877559829885);  Pomoc=W;
      break;

    case 'A':
      MacierzPomocnicza = Macierz4D(ObrotyGL);
      MacierzPomocnicza = MacierzPomocnicza.createRotationAroundAxis(0,
          3 * 0.052359877559829885, 0) * MacierzPomocnicza;
      for (int i = 0; i < 16; i++)
        ObrotyGL[i] = MacierzPomocnicza.at(i / 4, i % 4);
      //W.ObracajY(3*0.052359877559829885);   Pomoc=W;
      break;

    case 'D':
      MacierzPomocnicza = Macierz4D(ObrotyGL);
      MacierzPomocnicza = MacierzPomocnicza.createRotationAroundAxis(0,
          -3 * 0.052359877559829885, 0) * MacierzPomocnicza;
      for (int i = 0; i < 16; i++)
        ObrotyGL[i] = MacierzPomocnicza.at(i / 4, i % 4);
      //W.ObracajY(-3*0.052359877559829885);    Pomoc=W;
      break;

    case '|':
      W.SkalujMin();
      W.WyznaczWszystko();
      S.DlugoscWezla = S.DlugoscWezlaPoczatkowa = W.IloscSegm
          * W.DlugoscSegmSrednia;
      S.Pomoc = W;
      break;

    case '"':
      if (S.Metropolis == off)
        S.Metropolis = CloseRep;
      else if (S.Metropolis == CloseRep)
        S.Metropolis = DistRep;
      else if (S.Metropolis == DistRep)
        S.Metropolis = CloseAttr;
      else if (S.Metropolis == CloseAttr)
        S.Metropolis = DistAttr;
      else
        S.Metropolis = off;
      break;

    case ':':
      S.PierwszyRazHistogram = !S.PierwszyRazHistogram;
      break;

    case '+':
      W.Skaluj(1.1);
      W.WyznaczWszystko();
      S.DlugoscWezla = S.DlugoscWezlaPoczatkowa = W.IloscSegm
          * W.DlugoscSegmSrednia;
      S.Pomoc = W;
      break;

    case '_':
      W.Skaluj(double(1.0) / double(1.1));
      W.WyznaczWszystko();
      S.Pomoc = W;
      S.DlugoscWezla = S.DlugoscWezlaPoczatkowa = W.IloscSegm
          * W.DlugoscSegmSrednia;
      break;

    case '=':
      W.Skaluj(1.01);
      W.WyznaczWszystko();
      S.Pomoc = W;
      S.DlugoscWezla = S.DlugoscWezlaPoczatkowa = W.IloscSegm
          * W.DlugoscSegmSrednia;
      break;

    case '-':
      W.Skaluj(double(1.0) / double(1.01));
      W.WyznaczWszystko();
      S.Pomoc = W;
      S.DlugoscWezla = S.DlugoscWezlaPoczatkowa = W.IloscSegm
          * W.DlugoscSegmSrednia;
      break;

    case ')':
      W.Skaluj(1.001);
      W.WyznaczWszystko();
      S.Pomoc = W;
      S.DlugoscWezla = S.DlugoscWezlaPoczatkowa = W.IloscSegm
          * W.DlugoscSegmSrednia;
      break;

    case '(':
      W.Skaluj(double(1.0) / double(1.001));
      W.WyznaczWszystko();
      S.Pomoc = W;
      S.DlugoscWezla = S.DlugoscWezlaPoczatkowa = W.IloscSegm
          * W.DlugoscSegmSrednia;
      break;

    case '0':
      W.Skaluj(1.0001);
      W.WyznaczWszystko();
      S.Pomoc = W;
      S.DlugoscWezla = S.DlugoscWezlaPoczatkowa = W.IloscSegm
          * W.DlugoscSegmSrednia;
      break;

    case '9':
      W.Skaluj(double(1.0) / double(1.0001));
      W.WyznaczWszystko();
      S.Pomoc = W;
      S.DlugoscWezla = S.DlugoscWezlaPoczatkowa = W.IloscSegm
          * W.DlugoscSegmSrednia;
      break;

    case '[':
      S.defaultIleRazyBONO = int(S.defaultIleRazyBONO * 0.90909090909090909);
      if (S.defaultIleRazyBONO < 10) S.defaultIleRazyBONO = 10;
      break;

    case ']':
      S.defaultIleRazyBONO = int(S.defaultIleRazyBONO * 1.1);
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
      W.UsunMomentor(&S.Pomoc);
      S.Pomoc = W;
      break;

    case '.':
      W.Zegnij(2 * 0.0349065850398865915);
      W.WyznaczWszystko();
      W.Przesun(Wektor3D() - W.WyznaczSrodekMasy());
      W.UsunMomentor(&S.Pomoc);
      S.Pomoc = W;
      break;

    case '<':
      S.Pomoc = W;
      W.Zegnij(-0.00872664625997164788);
      W.WyznaczWszystko();
      W.Przesun(Wektor3D() - W.WyznaczSrodekMasy());
      W.UsunMomentor(&S.Pomoc);
      S.Pomoc = W;
      break;

    case '>':
      S.Pomoc = W;
      W.Zegnij(0.00872664625997164788);
      W.WyznaczWszystko();
      W.Przesun(Wektor3D() - W.WyznaczSrodekMasy());
      W.UsunMomentor(&S.Pomoc);
      S.Pomoc = W;
      break;

    case 'u':
      S.BONO = S.BONO ? 0 : 1;
      break;

    case 'i':
      S.SONO = S.SONO ? 0 : 1;
      break;

    case 'z':
      W.ZapiszBin(wskNazwyWynikMoja);
      break;

    case 'Z':
      W.ZapiszTxt(wskNazwyWynikMojatxt);
      break;

    case 'o':
      S.nrmojebin--;
      S.nrmojebin = S.nrmojebin < 0 ? 0 : S.nrmojebin;
      W.WczytajBin(S.wskstrPlikuMojaBin, S.nrmojebin);
      S.Pomoc = W;
      W.WyznaczWszystko();
      delete[] S.wskTablicyPomocniczejWektorow;
      S.wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscSegm];
      break;

    case 'O':
      S.nrmojetxt--;
      S.nrmojetxt = S.nrmojetxt < 0 ? 0 : S.nrmojetxt;
      W.WczytajTxt(wskNazwyWynikMojatxt, S.nrmojetxt);
      S.Pomoc = W;
      W.WyznaczWszystko();
      delete[] S.wskTablicyPomocniczejWektorow;
      S.wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscSegm];
      break;

    case 'p':
      W.WczytajBin(S.wskstrPlikuMojaBin, ++S.nrmojebin);
      S.Pomoc = W;
      W.WyznaczWszystko();
      delete[] S.wskTablicyPomocniczejWektorow;
      S.wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscSegm];
      break;

    case 'P':
      W.WczytajTxt(wskNazwyWynikMojatxt, ++S.nrmojetxt);
      S.Pomoc = W;
      W.WyznaczWszystko();
      delete[] S.wskTablicyPomocniczejWektorow;
      S.wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscSegm];
      break;

    case 'x':
      S.WczytajKnot(wybrany, S.PlikerWynikow.AktualnyKnot - 1);
      S.Pomoc = W;
      W.WyznaczWszystko();
      delete[] S.wskTablicyPomocniczejWektorow;
      S.wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscSegm];
      S.InfoKonsolowe();
      cout << flush;
      break;

    case 'c':
      S.WczytajKnot(wybrany, S.PlikerWynikow.AktualnyKnot + 1);
      S.Pomoc = W;
      W.WyznaczWszystko();
      delete[] S.wskTablicyPomocniczejWektorow;
      S.wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscSegm];
      S.InfoKonsolowe();
      cout << flush;
      break;

    case 'X':
      S.nrtxt--;
      S.nrtxt = S.nrtxt < 0 ? 0 : S.nrtxt;
      W.WczytajTxt(wskNazwyWynikAutotxt, S.nrtxt);
      S.Pomoc = W;
      W.WyznaczWszystko();
      delete[] S.wskTablicyPomocniczejWektorow;
      S.wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscSegm];
      break;

    case 'C':
      W.WczytajTxt(wskNazwyWynikAutotxt, ++S.nrtxt);
      S.Pomoc = W;
      W.WyznaczWszystko();
      delete[] S.wskTablicyPomocniczejWektorow;
      S.wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscSegm];
      break;

    case 'v':
      S.PlikerWynikow.WczytajKnot(&W, S.PlikerWynikow.AktualnyKnot - 10);
      S.Pomoc = W;
      W.WyznaczWszystko();
      delete[] S.wskTablicyPomocniczejWektorow;
      S.wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscSegm];
      break;

    case 'b':
      S.PlikerWynikow.WczytajKnot(&W, S.PlikerWynikow.AktualnyKnot + 10);
      S.Pomoc = W;
      W.WyznaczWszystko();
      delete[] S.wskTablicyPomocniczejWektorow;
      S.wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscSegm];
      break;

    case 'V':
      S.nrtxt -= 10;
      S.nrtxt = S.nrtxt < 0 ? 0 : S.nrtxt;
      W.WczytajTxt(wskNazwyWynikAutotxt, S.nrtxt);
      S.Pomoc = W;
      W.WyznaczWszystko();
      delete[] S.wskTablicyPomocniczejWektorow;
      S.wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscSegm];
      break;

    case 'B':
      S.nrtxt += 10;
      W.WczytajTxt(wskNazwyWynikAutotxt, S.nrtxt);
      S.Pomoc = W;
      W.WyznaczWszystko();
      delete[] S.wskTablicyPomocniczejWektorow;
      S.wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscSegm];
      break;

    case 'n':
      S.PlikerWynikow.WczytajKnot(&W, S.PlikerWynikow.AktualnyKnot - 100);
      S.Pomoc = W;
      W.WyznaczWszystko();
      delete[] S.wskTablicyPomocniczejWektorow;
      S.wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscSegm];
      break;

    case 'm':
      S.PlikerWynikow.WczytajKnot(&W, S.PlikerWynikow.AktualnyKnot + 100);
      S.Pomoc = W;
      W.WyznaczWszystko();
      delete[] S.wskTablicyPomocniczejWektorow;
      S.wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscSegm];
      break;

    case 'N':
      W.WczytajTxt(wskNazwyDomyslna, 0);
      S.Pomoc = W;
      W.WyznaczWszystko();
      S.DlugoscWezla = S.DlugoscWezlaPoczatkowa = W.IloscSegm
          * W.DlugoscSegmSrednia;
      delete[] S.wskTablicyPomocniczejWektorow;
      S.wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscSegm];
      break;

    case 'M':
      S.nrtxt += 100;
      W.WczytajTxt(wskNazwyWynikAutotxt, S.nrtxt);
      S.Pomoc = W;
      W.WyznaczWszystko();
      delete[] S.wskTablicyPomocniczejWektorow;
      S.wskTablicyPomocniczejWektorow = new Wektor3D[W.IloscSegm];
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
      S.Pomoc = W;
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

    x = (a / w - 0.5) * 2 * StosunekPromieniaWizjiDoPromieniaMax * W.PromienMax
        * xy_aspect;
    y = -(b / h - 0.5) * 2 * StosunekPromieniaWizjiDoPromieniaMax
        * W.PromienMax;

    Wektor3D P;
    Macierz3D M;
    MacierzPomocnicza = Macierz4D(ObrotyGL);

    M.at(0, 0) = MacierzPomocnicza.at(0, 0);
    M.at(0, 1) = MacierzPomocnicza.at(0, 1);
    M.at(0, 2) = MacierzPomocnicza.at(0, 2);
    M.at(1, 0) = MacierzPomocnicza.at(1, 0);
    M.at(1, 1) = MacierzPomocnicza.at(1, 1);
    M.at(1, 2) = MacierzPomocnicza.at(1, 2);
    M.at(2, 0) = MacierzPomocnicza.at(2, 0);
    M.at(2, 1) = MacierzPomocnicza.at(2, 1);
    M.at(2, 2) = MacierzPomocnicza.at(2, 2);

    M = M.inverse();

    double najbl = 19;
    int n = 0, wybr = 0, koniec = 0;

    for (int i = 0; !koniec; i++)
    {
      P = M * Wektor3D(x, y, W.PromienMax - W.PromienPrzekroju * i);

      n = W.ZnajdzNajblSegm(NULL, NULL, &P);

      if ((W.wskTablicySegm[n] - P).length() < W.PromienPrzekroju
          || i * W.PromienPrzekroju > 4 * W.PromienMax)
      {
        koniec = 1;
        wybr = n;
      }
    }

    if (button == GLUT_LEFT_BUTTON) W.UstawPunktGiecia(wybr);
    if (button == GLUT_RIGHT_BUTTON) W.WybierzSegm(wybr);

  }
}

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
  char Pomocnik[256] = "";
  int nrParametruFunkcjiMain = 1;
  int nrPrzekazanegoLancucha = 0;
  double ParametrDouble = 0;

  if (argc != 2)
  {
    cout << "Niepoprawne parametry!" << endl;
    cout << "Powinno byc: <plik wczytywany i do auto zapisu> " << endl;
    cout << "Stosuje domyslnie plik:\t" << wskNazwyWynikAuto << endl << endl;
  }
  else
  {
    strcpy(wskNazwyWynikAuto, argv[1]);
  }

  //strPlikuMojaTxt.open(wskNazwyWynikMojaTxt,ios_base::in);

  { //ustawienia konsolowego strumienia wyjsciowego
    cout.setf(ios::fixed);
    cout.precision(8);
  }

  W = S.WskazKnotSymulowany();

  SymulatorGlowny.wskHistogramySymulacji->wskWezelSymulowany = &W;

  if (SymulatorGlowny.wskHistogramySymulacji->WczytajBin("KnotSimData.bin"))
  {
    SymulatorGlowny.PierwszyRazHistogram = false;
    cout << "Wczytano histogramy i wezel z pliku   KnotSimData.bin" << endl
        << endl << flush;
  }

  W = S.WskazKnotSymulowany();
  SymulatorGlowny.WczytajPlik(wskNazwyWynikAuto);
  SymulatorGlowny.WczytajKnot(ostatni);
  SymulatorGlowny.UstawParametry();
  SymulatorGlowny.InfoKonsolowe();

  glutInit(&argc, argv);
  glutInitWindowSize(350, 350);
  glutInitWindowPosition(0, 25);
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