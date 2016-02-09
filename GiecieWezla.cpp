#ifndef GIECIEWEZLA_CPP
#define GIECIEWEZLA_CPP

#ifndef GIECIEWEZLA_H
#include "GiecieWezla.h"
#endif

//------------------------------------------------------------------------------
ostream& operator<<(ostream& stru, GiecieWezla& Giecie)
{
 stru<<"["<<Giecie.Segment1<<"-"<<Giecie.odlSegment12<<","<<Giecie.Kat<<"]";
 return stru ;
};
//------------------------------------------------------------------------------
istream& operator>>(istream& stru, GiecieWezla& Giecie)
{
 char znak;
 stru>> znak >>Giecie.Segment1>>znak>>Giecie.odlSegment12>>znak>>Giecie.Kat>>znak;
 return stru ;
};

//------------------------------------------------------------------------------

#endif
