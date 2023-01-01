#include <iostream>
#include <Windows.h>

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


// actual function

template<int N>
struct Value
{
	enum { result = N };
};

template<int N, int I=1>
struct Sqrt
{
	typedef typename IfThenElse< (I*I<N), Sqrt<N,N+1>, Value<I> >::ResultT 
		SubT;
	enum { result = SubT::result };
	//enum { result = (I*I<N)? Sqrt<N,N+1> : Value<I> }; // all sides are evaluated!!!!
};	

int main()
{
	std::cout << Sqrt<16>::result << std::endl;
	system("pause");
	return 0;
}
