#include <iostream>

#include "toml.h"

using namespace std;

int main()
{
	auto f = toml::parseFile("utf8.toml");
	auto t = f.value.as<toml::Table>();
	auto tx = toml::Value(t);
	
	ostringstream os;
	tx.writeFormatted(&os, toml::FormatFlag::FORMAT_INDENT);

	auto str = os.str();
	for (auto const& x : t)
	{
		cout << x.first << endl;
		auto d = x.second.find("double");

		auto const& table = x.second.as<toml::Table>();
		for (auto const& y : table)
		{
			cout << y.first << "->" << y.second << endl;
			//y.second.as<int>();
		}
	}

	cout << f.valid() << ", " << f.errorReason << endl;
	auto testdata = f.value.find("TestData");
	//auto d = testdata->findChild("double");


	auto f2 = toml::parseFile("utf8_with_bom.toml");
	cout << f2.valid() << ", " << f2.errorReason << endl;

	cout << (f.value == f2.value);

}