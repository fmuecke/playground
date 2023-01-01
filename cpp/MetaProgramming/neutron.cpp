/* The following example code contains parts taken from the book
 * "C++ Templates - The Complete Guide"  by David Vandervoorde
 * and Nicolai M. Josuttis, Addison-Wesley, 2003
 *
 * (C) Copyright Florian E. Muecke.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */
#include <iostream>
#include <Windows.h>

//----------------
// IfThenElse<..>
//----------------
template<bool, typename Ta, typename Tb>
class IfThenElse;

template<typename Ta, typename Tb>
struct IfThenElse<true, Ta, Tb>
{
	typedef Ta ResultT;
};

template<typename Ta, typename Tb>
struct IfThenElse<false, Ta, Tb>
{
	typedef Tb ResultT;
};

// Value<..> helper
template<int N>
struct Value
{
	enum { result = N };
};

//----------
// Sqrt<..>
//----------
template<int N, int I=1>
struct Sqrt
{
	typedef typename IfThenElse< (I*I<N), Sqrt<N,I+1>, Value<I> >::ResultT SubT;
	enum { result = SubT::result };
};	

//---------
// Pow<..>
//---------
/*
int myPow(int x, int p) 
{  
	if (p == 0) return 1;  
	if (p == 1) return x;  
	return x * myPow(x, p-1);
}
*/
template<int X, int P>
struct Pow
{
	enum { result = X*Pow<X,P-1>::result };
};
template<int X>
struct Pow<X,0>
{
	enum { result = 1 };
};
template<int X>
struct Pow<X,1>
{
	enum { result = X };
};

//---------------
// Factorial<..>
//---------------
template<int N, int I=1>
struct Fac
{
	enum { result = I*Fac<N,I+1>::result };
};	

template<int N>
struct Fac<N, N>
{
	enum { result = N };
};



//------------
// Binary<..>
//------------
template< unsigned long long N >
struct Binary
{  
	enum { value = (N % 10) + 2 * Binary< N / 10 > :: value } ;
};
template<>
struct Binary<0>
{  
	enum { value = 0 } ;
};

//------
// Main
//------
int main()
{
	std::cout << "square root of 1024 is " << Sqrt<1024>::result << std::endl;
	std::cout << "factorial of 12 is " << Fac<12>::result << std::endl;
	std::cout << "pow(2,8) is " << Pow<2,30>::result << std::endl;
	std::cout << "binary(5) is " << Binary<5>::value << std::endl;
	system("pause");
	return 0;
}