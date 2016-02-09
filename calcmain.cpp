#include <cstdlib>
#include <iostream>
#include "Wezel3D.h"

using namespace std;

int main(int argc, char *argv[])
{
    int Ile=300;
    
    Wezel3D W(Ile),P;
    //W.WczytajTxt("newgordian.txt");
    W.Skaluj(3.5);
    /*
    P=W;
    cout<<"W:  "<<endl<<W<<endl;
    cout<<"P:  "<<endl<<P<<endl;
    Wektor3D Punkt;
    int Wybrany=0;
    double tab[]={1,5,3,1,2,3,4,2,6}; 
    */
    W.WyznaczWszystko();
    Macierz3D M=W.MacierzBezwl.inverse();
    cout<<"macierz:  "<<endl<<W.MacierzBezwl<<endl;
    cout<<"wyznacznik:  "<<endl<<W.MacierzBezwl.det()<<endl;
    //cout<<(M.at(0,2)*M.at(2,1)-M.at(0,1)*M.at(2,2))/M.det()<<endl;
    //cout<<(M.at(1,2)*M.at(2,0)-M.at(1,0)*M.at(2,2))/M.det()<<endl;
    cout<<"odwrotna:  "<<endl<<M<<endl;
    cout<<endl;
    
    cout<<"PromienPrzekrojuWezla="<<W.PromienPrzekroju<<endl;
    //cout<<"PromienKrzywiznyMin="<<W.PromienKrzywiznyMin<<endl; 
    cout<<endl;
    cout<<"PromienMax="<<W.PromienMax<<endl; 
    cout<<endl;
    cout<<"KatGieciaMax="<<W.KatGieciaMax<<"   oraz w stopniach: "
        <<W.KatGieciaMax*180/M_PI<<endl;
    cout<<endl;
    cout<<"SrodekMasy="<<W.SrodekMasy<<endl;
    
    cout<<endl;
    
    Wektor3D kacik(0,W.KatGieciaMax,0);
    cout<<"Kacik:   "<<kacik<<endl<<endl;
    cout<<"Momentor k="<<W.WyznaczMomentorObrotu(kacik)<<endl;
    cout<<endl;
    cout<<"MomentorChwilMax="<<W.MomentorChwilMax<<endl;
    cout<<endl;
    /*
    cout<<"KrzywiznaOK="<<W.KrzywiznaOK<<endl; 
    cout<<"PzekryciaSa="<<W.SprawdzCzySaPrzekrycia()<<endl; 
    int z=0,i=300;
    Punkt=Wektor3D(100,100,100);
    cout<<Punkt<<endl;
    //W.WybierzSegm(naj);
    cout<<"najblizej ="<<W.ZnajdzNajblSegm(&z,&Ile,&Punkt)<<endl;
    cout<<"najblizej ="<<W.wskTablicySegm[W.ZnajdzNajblSegm(&z,&Ile,&Punkt)]<<endl;
    cout<<"odleglosc ="<<(Punkt-W.wskTablicySegm[W.ZnajdzNajblSegm(&z,&Ile,&Punkt)]).length()<<endl;
    cout<<endl;
    Punkt=Wektor3D(-100,-100,-100);
    cout<<Punkt<<endl;
    cout<<endl;
    cout<<"najblizej ="<<W.ZnajdzNajblSegm(&z,&Ile,&Punkt)<<endl;
    cout<<"najblizej ="<<W.wskTablicySegm[W.ZnajdzNajblSegm(&z,&Ile,&Punkt)]<<endl;
    cout<<"odleglosc ="<<(Punkt-W.wskTablicySegm[W.ZnajdzNajblSegm(&z,&Ile,&Punkt)]).length()<<endl;
    cout<<endl;
    double a; 
    cout<<"const cast<int> 2.66 ="<<static_cast<int>(2.9999999999999991)<<endl;
    cout<<endl;
    cout<<"odlP(0,4,8)="<<odlP(1,4,8)<<endl;
    cout<<"odlP(4,0,8)="<<odlP(4,1,8)<<endl;
    */
    Wezel3D W2;
    W2=W;
    /*
    W.UstawPunktGiecia1(20);
    W.UstawPunktGiecia1(120);
    W.Zegnij(W.KatGieciaMax/3); 
    cout<<endl;
    cout<<"Obracamy wokol o kacik."<<endl<<endl;
    //W.ObracajWektor(kacik);
    cout<<endl;
    cout<<"SrodekMasy="<<W.WyznaczSrodekMasy()<<endl;
    cout<<endl;
    W.UsunPed(&W2);
    Wektor3D Srodek=W.WyznaczSrodekMasy(), Momentor=W.WyznaczMomentor(&W2);
    double pocz=Momentor.length();
    cout<<"WyznaczMomentor wzgledem punktu domyslnego: "
        <<endl<<Momentor<<endl<<pocz;
    cout<<endl; 
    cout<<endl; 
    Momentor=WyznaczMomentor(W2.wskTablicySegm,
                              W.wskTablicySegm,
                              W.IloscSegm,
                              Srodek);        
    cout<<"WyznaczMomentor wzgledem [0,0,0]: "
        <<endl<<Momentor<<endl<<Momentor.length()<<endl;                            
    cout<<endl;
    cout<<endl;
    cout<<"Usuwamy momentor."<<endl;
    
    system("PAUSE");
    W.IlePrzekrycSprawdzac=1;
    for(int i=0;i<100000;i++)
    {
    W.Zegnij(0.1);
    W.SprawdzCzySaPrzekrycia();
    };
    
    system("PAUSE");
    
    Momentor=W.WyznaczMomentor(&W2);
    cout<<"WyznaczMomentor wzgledem punktu domyslnego: "
        <<endl<<Momentor<<endl<<pocz/Momentor.length()<<endl<<Momentor.length();;   
    cout<<endl;
    cout<<"ost g:    "<<W.OstatnieGiecia.max_size()<<endl;
    cout<<endl;
    
    system("PAUSE");
    
    Wektor3D P1,P2;
    
    for(int j=0; j<1; j++)
       {
        for(int i=1; i<W.IloscSegm-1; i++) 
           {
            P1=W.wskTablicySegm[i-1]-W.wskTablicySegm[i];
            P2=W.wskTablicySegm[i+1]-W.wskTablicySegm[i];
                        M=Macierz3D::parallelOZ(W.wskTablicySegm[i]-
                                                W.wskTablicySegm[nrP(i+100,W.IloscSegm)]);
            P1=M*P1;
            P2=M*P2;
            cout<<WyznaczMaxKatGieciaPlus(P1,P2,1.4)<<endl;
           };
       };

	
    */
    
    /*
    double a=3.14159,x;
    int b=676767,y;
    char c='x',z;
    ofstream plik;
    ifstream plik1;
    
    char Nazwa[]="dupa";
    
    
    //plik.setf(ios::fixed);
    //plik.precision(16);
    plik.open(Nazwa,ios_base::app|ios_base::binary);
    plik.write((char*)(W.wskTablicySegm),sizeof(double));
    plik.write((char*)(&b),sizeof(b));
    plik.write((char*)(&c),sizeof(c));
    plik.close();
    
    plik1.open(Nazwa,ios_base::in|ios_base::binary);
    plik1.read((char*)(&x),sizeof(double));
    plik1.read((char*)(&y),sizeof(int));
    plik1.read((char*)(&z),sizeof(char));
    plik1.close();
    cout<<x<<endl<<y<<endl<<z<<endl<<endl;
    */
    
    /*
    int ilosc=1;
    int nr=0;
    for(;ilosc;nr++)
       {
        cout<<nr<<endl<<endl;
        W.WczytajTxt("in.txt",nr);
        ilosc=W.IloscSegm;
        W.ZapiszBin("tenpercent0.bin");
       };
    */
    char c;
    int R=-1;
    cin>>c;
    Wektor3D* wsk;
    for(long int i=0;i<1E9;i++) R>0?wsk--:wsk++;
    cout<<"JUUUUUUUUUUUUUZ";
    cin>>c;

    system("PAUSE");
    return EXIT_SUCCESS;


}
