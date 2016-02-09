#ifndef LOSOWANIA_CPP
#define LOSOWANIA_CPP

#ifndef LOSOWANIA_H
#include "Losowania.h"
#endif

//------------------------------------------------------------------------------
//----------------DEFINICJE SKLADOWYCH NIE-INLINE-------------------------------
//------------------------------------------------------------------------------
//----------------------------------------------------------------------- 

//konstruktor:
Losowania::Losowania()
{
seed=1002360;
a=16807;
m=2147483647;
q=127773;
r=2836;
conv=1.0/m;
PI=3.1415926535897932384626433832795;

b=seed;
IloscLosowan=0; 
IleRazyObroconoCalkowicieLicznikLosowan=0;
CzyPozostGauss=0;
PozostalyGauss=0;
}
#endif
