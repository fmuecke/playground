#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;
bool IsOdd(int i) { return (i%2) == 1; }
bool IsEven(int i) { return !IsOdd(i); }

template<typename Container, typename UnaryPredicate>
void range_copy_if(Container&& cIn, Container&& cOut, UnaryPredicate p)
{
	copy_if(begin(cIn), end(cIn), back_inserter(cOut), p);
}

int main()
{
	vector<int> v{1,2,3,4,5,6};
	auto pos_of_three = find(begin(v), end(v), 3);
	
	vector<int> even;
	vector<int> odd;
	copy_if(begin(v), end(v), back_inserter(odd), IsOdd);	
	range_copy_if(v, even, IsEven);
	
	cout << "even: ";
	for (auto i : even) cout << i << " ";
	
	cout << "\nodd: ";
	for (auto i : odd) cout << i << " ";
}