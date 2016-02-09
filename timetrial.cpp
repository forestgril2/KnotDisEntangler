#include <cstdlib>
#include <cstdlib>
#include <iostream>
using namespace std;
#include "funkcjezew.h"



int main(int argc, char *argv[])
{
    char c;
    int R=-1;
    double wsk[]={1.0,200.0,-3.4,9.5,-100.0};
    ZnajdzMin<double>(wsk+4,-4);
    ZnajdzMax<double>(wsk+4,-4);


    system("PAUSE");
    return EXIT_SUCCESS; 


}
