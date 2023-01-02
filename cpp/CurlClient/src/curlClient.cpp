//
// Simple libcurl sample using async POST request and digest authentication.
// 2012-08-05, F.Muecke
//

#include "curlClient.h"

#include <curl/curl.h>
#include <curl/easy.h>
#include <iostream>

#pragma comment(lib, "libcurl.lib")

CurlClient::CurlClient()
	: m_multiHandle(curl_multi_init())
	, m_nRunning(0)
	, m_data()
	, m_idToCurlLookup()
	, m_curlToPosLookup()
{
	/*CURLcode ret = */curl_global_init(CURL_GLOBAL_WIN32);

	m_data.reserve(1024); // avoid reallocation!
}

CurlClient::~CurlClient()
{
	for (auto i = m_data.begin(); i != m_data.end(); ++i)
	{
		curl_multi_remove_handle(m_multiHandle, (*i)->pCurl);
		curl_easy_cleanup((*i)->pCurl);
	}

	m_data.clear();

	curl_multi_cleanup(m_multiHandle);
}

size_t CurlClient::writeResponseCallback(void* pContent, size_t size, size_t nmemb, void* pUserp)
{
	size_t realSize = size * nmemb;
	PResponseData pResponseData = *static_cast<PResponseData*>(pUserp);

	boost::lock_guard<boost::mutex> lock(pResponseData->mutex);
	pResponseData->data.append(static_cast<const char*>(pContent), realSize);

	return realSize;
}

int CurlClient::Update()
{
	curl_multi_perform(m_multiHandle, &m_nRunning);
	
	int nQueuedMessages(0);
	const CURLMsg* pMsg = curl_multi_info_read(m_multiHandle, &nQueuedMessages);
	while (pMsg)
	{
		if (CURLMSG_DONE == pMsg->msg)
		{
			size_t pos = m_curlToPosLookup[pMsg->easy_handle];
			m_data[pos]->completed = true;
		}

		pMsg = curl_multi_info_read(m_multiHandle, &nQueuedMessages);
	}

	return m_nRunning;
}

bool CurlClient::AddPostRequest(
	std::string const& id,
	std::string const& url, 
	std::string const& username, 
	std::string const& password,
	EAuthType authType,
	std::string const& message)
{
	CURL* pCurl = NULL;
	size_t pos(0);
	
	// re-use curl handle if present for given id
	auto itCurl = m_idToCurlLookup.find(id);
	if (itCurl != m_idToCurlLookup.end())
	{
		pos = m_curlToPosLookup[itCurl->second];
		boost::lock_guard<boost::mutex> guard(m_data[pos]->mutex);
		if (!m_data[pos]->completed)
		{
			return false;
		}
	
		m_data[pos]->data.clear();
		m_data[pos]->completed = false;
		pCurl = m_data[pos]->pCurl;
	}
	else
	{
		pCurl = curl_easy_init();
		if (pCurl)
		{
			PResponseData pRd(new ResponseData(pCurl));
			m_data.push_back(pRd);
			pos = m_data.size() - 1;

			// add lookup reference
			m_idToCurlLookup.insert(std::make_pair(id, pCurl)); 
			m_curlToPosLookup.insert(std::make_pair(pCurl, pos)); 

			// sent callback data
			curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, writeResponseCallback);
			curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, static_cast<void*>(&m_data[pos]));

			// add handle for processing
			curl_multi_add_handle(m_multiHandle, pCurl);
		}
	}

	if (pCurl)
	{
		// set user and password
		const std::string usernameWithPassword(username + ":" + password);
		curl_easy_setopt(pCurl, CURLOPT_USERPWD, usernameWithPassword.c_str());

		// set authentication type
		curl_easy_setopt(pCurl, CURLOPT_HTTPAUTH, 
			(auth_digest == authType)? CURLAUTH_DIGEST : CURLAUTH_BASIC);

		// set url
		curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str());

		// set post params
		curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, message.c_str());

		// additional cURL options
		curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 1L);

		return true;
	}

	return false;
}

bool CurlClient::HasResponse(std::string const& id)
{
	auto itPos = m_idToCurlLookup.find(id);
	if (itPos != m_idToCurlLookup.end())
	{
		size_t pos = m_curlToPosLookup[itPos->second];
	
		if (pos < m_data.size())
		{
			boost::lock_guard<boost::mutex> guard(m_data[pos]->mutex);
			return m_data[pos]->completed;
		}
	}

	return false;
}

int const CurlClient::GetResponse(std::string const& id, std::string& response)
{
	int retVal(0);
	response.clear();

	auto itPos = m_idToCurlLookup.find(id);
	if (itPos != m_idToCurlLookup.end())
	{
		size_t pos = m_curlToPosLookup[itPos->second];
	
		if (pos < m_data.size())
		{
			boost::lock_guard<boost::mutex> guard(m_data[pos]->mutex);
			response = m_data[pos]->data;
			curl_easy_getinfo(m_data[pos]->pCurl, CURLINFO_RESPONSE_CODE, &retVal);
		}
	}

	return retVal;
}


//int CurlClient::Post(std::string const& url, std::string const& message, std::string& /*result*/)
//{
//	curl_global_init(CURL_GLOBAL_WIN32);
//
//	CURL* pCurl = curl_easy_init();
//
//	if (pCurl)
//	{
//		const std::string usernameWithPassword(m_username + ":" + m_password);
//		curl_easy_setopt(pCurl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC); // CURLAUTH_DIGEST);
//		curl_easy_setopt(pCurl, CURLOPT_USERPWD, usernameWithPassword.c_str());
//		curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str());
//		curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, message.c_str());
//
//		// more cURL options
//		curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 1L);
//
//		// send all data to this function
//		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
// 
//		// we pass our 'chunk' struct to the callback function
//		curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, (void *)&m_chunk);
//
//		// Perform the request, res will get the return code
//		CURLcode res = curl_easy_perform(pCurl);
//
//		if (res != CURLE_OK)
//		{
//			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
//		}
//
//		int responseCode(0);
//		res = curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &responseCode);
//
//		// always cleanup
//		curl_easy_cleanup(pCurl);
//	}
//
//	return 0;
//}