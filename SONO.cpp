#ifndef SONO_CPP
#define SONO_CPP

#ifndef SONO_H
#include "SONO.h"
#endif

#ifndef NULL
#define NULL 0
#endif

// ==========================================================================
double Dlugosc(WK3D *Wezel, int IloscSegm)
{
  WK3D A=Wezel[IloscSegm-1]-Wezel[0];
  double Lc= (modul(A));
  for(int i=1; i<IloscSegm; i++)
  {
  A=Wezel[i]-Wezel[i-1];
  Lc+= modul(A);
  };
  return Lc;
}
// ==========================================================================
// --------------------------------------------------------------------------
CORECT::CORECT():
         Wezel(NULL),
         Si(0),
         TablicaNumerowElementowA(NULL),
         TablicaNumerowElementowB(NULL),
         TablicaElementow(NULL),
         SredniaDlugosc(1),
         Sb(1),
         DlugoscTrwania(4),
         ParametrZaciskania(0.3),
         Ot(1E-10)
{ }
// --------------------------------------------------------------------------
CORECT::~CORECT()
{
  if(Wezel!=0) Wprowadz(NULL,0);
}
// --------------------------------------------------------------------------
void CORECT::Odswiez(double nad)
{
  int *NTablicaNumerowElementowA=NULL,
      *NTablicaNumerowElementowB=NULL;
  int Nowa=RezSt( nad,&NTablicaNumerowElementowA,&NTablicaNumerowElementowB);
  delete []TablicaNumerowElementowA;   TablicaNumerowElementowA=NTablicaNumerowElementowA;
  delete []TablicaNumerowElementowB;   TablicaNumerowElementowB=NTablicaNumerowElementowB;
  Si=Nowa;
}
//---------------------------------------------------------------------------
void CORECT::Wprowadz( WK3D *W, int IloscSegmNowa, double ParametrZaciskaniaNowy)
{
  if(Wezel!=NULL)
  {
    delete[] TablicaElementow;  TablicaElementow =NULL;
    delete[] TablicaNumerowElementowA;  TablicaNumerowElementowA =NULL;
    delete[] TablicaNumerowElementowB;  TablicaNumerowElementowB =NULL;
  }
  Wezel=W;         IloscSegm=IloscSegmNowa;
  if(Wezel!=NULL)
  {
    ParametrZaciskania=ParametrZaciskaniaNowy;
    DlugoscWezla=::Dlugosc(Wezel,IloscSegm);
    SredniaDlugosc=DlugoscWezla/IloscSegm;
    TablicaElementow=new WK3D[IloscSegm];
    TablicaNumerowElementowA =NULL;
    TablicaNumerowElementowB =NULL;   
    Si=0;
    Odswiez(Ot);
    Pp=Ot;
  }
}
//---------------------------------------------------------------------------
void CORECT::Sprawdz( void)
{
  static int Licznik=0;
  double PromienMax=0.0;
  for(int i=0; i<IloscSegm; i++){
     if(modul(TablicaElementow[i])>PromienMax){
        PromienMax=modul(TablicaElementow[i]);
        };
     };

  Pp-=2.0*PromienMax;
  if(Pp<0.0 || Licznik==1000)
  {
    Pp=500*PromienMax;
    if( Pp>0.15)
     Pp=0.15;
    Odswiez( Pp);
    Licznik=0;
  }
  Licznik++;
}
//---------------------------------------------------------------------------
void CORECT::FStykPR( void)
{
  double Om=1.0+Ot,lh, Ws;
  WK3D LH,werLH, Sw;
  for(int k=0,i,j; k<Si; k++)
  {
    i=TablicaNumerowElementowA[k]; j=TablicaNumerowElementowB[k];
    LH=Wezel[j]-Wezel[i];
    lh=modul(LH);
    if(lh<Om )
    {
      Ws=(1.0-lh);
      werLH=wersor(LH);
      Sw=Ws*werLH;
      TablicaElementow[i]-=Sw;
      TablicaElementow[j]+=Sw;
    }
  }
}
//---------------------------------------------------------------------------
void CORECT::FSegmentPR()
{
  double Mwo=1.0/SredniaDlugosc,Ss=(SredniaDlugosc-Sb)*Mwo,lh,Ws;
  WK3D LH,Sw;
  for(int i=0,j;i<IloscSegm;i++)
  {
    j=(i+1)%IloscSegm;
    LH=Wezel[j]-Wezel[i];
    lh=modul(LH);
    Ws=(lh-SredniaDlugosc)*Mwo+Ss;
    Sw=(Ws/lh)*LH;
    TablicaElementow[i]+=Sw;
    TablicaElementow[j]-=Sw;
  }
}
//---------------------------------------------------------------------------
void CORECT::Zaciskaj( void)
{
  WK3D ZERO=WK3D(0.0,0.0,0.0);
  DlugoscWezla=::Dlugosc(Wezel,IloscSegm);
  SredniaDlugosc=DlugoscWezla/IloscSegm;
  Sb=(1.0-ParametrZaciskania)*SredniaDlugosc;
  double oDlugoscTrwania=1.0/(DlugoscTrwania*IloscSegm);

  for(int i=0; i<IloscSegm; i++)   TablicaElementow[i]=ZERO;
  FStykPR();
  FSegmentPR();
  for(int i=0; i<IloscSegm; i++)    TablicaElementow[i]*=oDlugoscTrwania;
  for(int i=0; i<IloscSegm; i++)    Wezel[i]+=TablicaElementow[i];
  Sprawdz();
}
//---------------------------------------------------------------------------
int CORECT::RezSt(double Nd, int **ta, int **tb)
{
   WK3D A; 
    
  double SredniaDlugosc_1=IloscSegm/DlugoscWezla,
         J1S= 1+SredniaDlugosc,
         J2S= J1S+SredniaDlugosc,
         odl=1.0+Nd,
         lh;
  
 int mg= static_cast<int>(SredniaDlugosc<=1.0?0.5*M_PI/asin(SredniaDlugosc)-1e-10:1),
     StIn=0;
  
  for(int jkon,i=0;i<IloscSegm;i++){
     jkon=(IloscSegm-(i<mg?mg-i:0));

     for(int j=i+mg+1; j<jkon; j++){
     A=Wezel[j]-Wezel[i];
     lh=modul(A);
     if(lh<=odl) StIn++;
     if(lh>J2S)  j+= static_cast<int>((lh-J1S)*SredniaDlugosc_1);
    }
  }
  if( StIn==0)  return 0;
  *ta=new int[ StIn];
  *tb=new int[ StIn];
  
  StIn=0;
  
  int *taw=*ta,*tbw=*tb;
  for(int jkon,i=0;i<IloscSegm;i++){
     jkon=(IloscSegm-(i<mg?mg-i:0));
     for(int j=i+mg+1; j<jkon; j++){
         A=Wezel[j]-Wezel[i];
         lh=modul(A);
         if(lh<=odl) {taw[StIn]=i; tbw[StIn]=j; StIn++;}
         if(lh>J2S)  j+= static_cast<int>((lh-J1S)*SredniaDlugosc_1);
     }
  }
  return StIn;
}
// --------------------------------------------------------------------------
void Torus( WK3D *AP,int IP)
{
  double R=1.2,S=0.8;
  double x,y,z,dt;
  dt=2.0*M_PI/IP;
  for(int i=0; i<IP; i++)
  {
    x=R*sin(2*i*dt)+S*cos(3*i*dt)*sin(2*i*dt);
    y=R*cos(2*i*dt)+S*cos(3*i*dt)*cos(2*i*dt);
    z=                 S*sin(3*i*dt);
    AP[i]=WK3D(x,y,z);
  }
}
// --------------------------------------------------------------------------
/*int main(int argc, char* argv[])
{
  int IloscSegm=1000;
  WK3D Wezel[1000];
  Torus( Wezel,IloscSegm);
  CORECT ZZ;
  ZZ.Wprowadz( Wezel, IloscSegm);
  ZZ.Szybkosc(2.0);
  int L=0,S=0;

  while( !kbhit())  //getch()!=27)//
  {
    ZZ.Zaciskaj();
    L++;
    if(L%100==0) {L=0;  gotoxy(2,2);  cout<<ZZ.Dlugosc()<<"  "<<S++<<"  "; }

  }
  return 0;
}
//---------------------------------------------------------------------------
*/

#endif
