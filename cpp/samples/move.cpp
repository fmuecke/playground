#include <iostream>
#include <string>
using namespace std;

struct S
{
	S(string const& str) : _s{str} {}
	S(S&& other) : _s{move(other._s)}
	{ 	
		cout << "move called" << endl;
	}
	
	string _s;
};

S f() 
{
	auto s = S{"hallo"};
	return s;
}

int main()
{
	auto s = f();
	cout << s._s << endl;
}