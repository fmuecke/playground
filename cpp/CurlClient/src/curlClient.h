#ifndef _CURL_CLIENT_H__
#define _CURL_CLIENT_H__

//
// Simple libcurl sample using async POST request and digest authentication.
// 2012-08-05, F.Muecke
//

#include <boost/thread.hpp>
#include <string>
#include <vector>
#include <map>

typedef void CURL;
typedef void CURLM;

class CurlClient
{
public:
	enum EAuthType
	{
		auth_basic, auth_digest
	};

	CurlClient();
	virtual ~CurlClient();

	int Update();

	bool AddPostRequest(
		std::string const& id, 
		std::string const& url, 
		std::string const& username, 
		std::string const& password,
		EAuthType authType,
		std::string const& message);

	bool HasResponse(std::string const& id);
	int const GetResponse(std::string const& id, std::string& response);

private:
	static size_t writeResponseCallback(void* pContent, size_t size, size_t nMemb, void* pUserp);

	// needed for write callback
	struct ResponseData
	{
		ResponseData(CURL* curlPtr) 
			: pCurl(curlPtr), data(), mutex(), completed(false)
		{}

		CURL* pCurl;
		std::string data;
		boost::mutex mutex;
		bool completed;
	};
	typedef std::shared_ptr<ResponseData> PResponseData;

	CURLM* m_multiHandle;	
	int m_nRunning;

	std::vector<PResponseData> m_data;
	std::map<std::string, CURL*> m_idToCurlLookup;
	std::map<CURL*, size_t> m_curlToPosLookup;
};


#endif  // _CURL_CLIENT_H__