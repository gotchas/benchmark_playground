#include <iostream>
#include <iomanip>
#include <climits>

//Correct wraparound in a random number generator in 175.vpr

#define  IA  1103515245u
#define  IC 12345u
#define  IM  2147483648u


static  unsigned  int  c_rand = 0;


/*  Creates  a  random  integer  [0... imax] (inclusive ) */
template <class Integer>
Integer  my_irand (Integer imax) {
  Integer  ival;

  /*  c_rand  = (c_rand  * IA + IC) % IM; */
  c_rand =  c_rand * IA + IC;
  //  Use  overflow  to  wrap
  ival = c_rand & (IM - 1);
  /*  Modulus  */
  ival = (Integer) (( float) ival * (float) (imax + 0.999) / (float) IM);

  return  ival;
}


int main() 
{
   for(auto r=1000LL; r<LONG_LONG_MAX/2; r*=2) 
   {
     std::cout << r << ":\t";
     for(auto i=0; i!=10; ++i)
     {
       std::cout << std::setprecision(3) << my_irand(r)/(double)r << "\t";
     }
     std::cout << "\n";
   }
   std::cout << LONG_LONG_MAX << "\n";
   return 0;
}


