//
// Simple libcurl sample using async POST request and digest authentication.
// 2012-08-05, F.Muecke
//

#include "curlClient.h"
#include <boost/format.hpp>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
	string url("http://test.mueckeimnetz.de/protected_dir/evaldata.php");
	string username("httpclient");
	string password("baramundi");
	string message("data=test12345");

	if (argc == 5)
	{
		url = argv[1];
		username = argv[2];
		password = argv[3];
		message = argv[4];
	}

	CurlClient client;
	std::string result;

	string message2(message.rbegin(), message.rend());

	const int count = 100;
	for (int i = 0; i < count; ++i)
	{
		boost::format idFormat = boost::format("test-%d") % i;
		client.AddPostRequest(idFormat.str(), url, username, password, CurlClient::auth_basic, message);
	}

	cout << "processing " << count << " requests..." << endl;
	int nRunning(0);
	int nCalls(0);
	do
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

		++nCalls;
		nRunning = client.Update();
		cout << nRunning << " left" << endl;
	}
	while (nRunning > 0);

	//cout << "has response 'one': " << client.HasResponse("one") << endl;
	//string response;
	//cout << "one: " << client.GetResponse("one", response) << ": " << response << endl;
	//cout << "two: " << client.GetResponse("two", response) << ": " << response << endl;

	return 0;
}
