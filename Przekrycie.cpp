#ifndef PRZEKRYCIE_CPP
#define PRZEKRYCIE_CPP

#include <iostream>

#ifndef PRZEKRYCIE_H
#include "Przekrycie.h"
#endif

std::ostream& operator<<(std::ostream& L, Przekrycie& R)
{
  L << "[" << R.A << "," << R.B << "]";
  return L;
}

#endif
