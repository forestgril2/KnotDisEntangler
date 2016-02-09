#ifndef PRZEKRYCIE_H
#define PRZEKRYCIE_H


class Przekrycie  //definiuje przekrycie dla pary lub jego brak
{
     public:
     //konstruktory
     Przekrycie(int a=0, int b=0, int J=false) : Jest(J),A(a),B(b)
     {}; 
     
     Przekrycie(const Przekrycie& P) 
     {
      Jest=P.Jest;
      A=P.A;
      B=P.B;
     };
             
     //DANE
     int A,B;
     bool Jest;
     
     //FUNKCJE OPERATOROWE 
    operator bool()
          {
          return Jest;
          };
           
     bool operator==(const Przekrycie& P)
           {
           return (A==P.A)&&(B==P.B);                         
           };
           
     bool operator<(const Przekrycie& P)
           {
           if (A>P.A) return false;
           return A<P.A?true:bool(B<P.B);                         
           };
           
     bool operator<=(const Przekrycie& P)
           {
           if (A>P.A) return false;
           return A<P.A?true:bool(B<=P.B);             
           };
     
     bool operator>(const Przekrycie& P)
           {
           if (A<P.A) return false;
           return A>P.A?true:bool(B>P.B);                                
           };
     
     bool operator>=(const Przekrycie& P)
           {
           if (A<P.A) return false;
           return A>P.A?true:bool(B>=P.B);                          
           };
};
//------------------------------------------------------------------------------------------------------------
struct Porownanie
{
  bool operator()(Przekrycie A, Przekrycie B) const
  {
    return A<B;
  }
};
//----------------------------------------------------------------------------------------------------------

#endif
