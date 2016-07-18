#include "Loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "glog/logging.h"

#include "config.h"

#define R_FAIL -1


void Tile::clone(Tile** pNew)
{
	*pNew = new Tile();

	memcpy((*pNew)->pData, this->pData, this->nMaxSize);
	(*pNew)->nSize = this->nSize;
	(*pNew)->nMaxSize = this->nMaxSize;
}

size_t function(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t written = 0;
	if (ptr)
		written = fwrite(ptr, size, nmemb, (FILE*)stream);

	LOG(INFO) << stream;

	return written;
}

size_t callback_WriteTile(void *data, size_t size, size_t nmemb, void *tile)
{
	Tile* p = (Tile*)tile;

	int nNewCharCount = size * nmemb;

	size_t nPos = p->nSize;
	p->nSize += nNewCharCount;
	if (p->nSize > p->nMaxSize)
	{
		p->pData = (char*)realloc(p->pData, p->nSize);
		p->nMaxSize = p->nSize;
	}

	memcpy(p->pData + nPos, data, nNewCharCount);

	return nNewCharCount;
}

Loader::Loader()
{

}

Loader::~Loader()
{
}

int Loader::loadTile(std::string sUrl, int level, int row, int col, Tile* pTile, long& lCurlcode)
{
	CURL* handle = curl_easy_init();
	if (nullptr == handle)
		return R_FAIL;

	char szTemp[32];
	memset(szTemp, 0, 32);
	_itoa_s(level, szTemp, 10);
	size_t nPos = sUrl.find("{level}");
	sUrl.replace(nPos, 7, szTemp);

	memset(szTemp, 0, 32);
	_itoa_s(row, szTemp, 10);
	nPos = sUrl.find("{row}");
	sUrl.replace(nPos, 5, szTemp);

	memset(szTemp, 0, 32);
	_itoa_s(col, szTemp, 10);
	nPos = sUrl.find("{col}");
	sUrl.replace(nPos, 5, szTemp);

	CURLcode code = curl_easy_setopt(handle, CURLOPT_URL, sUrl.c_str());
	if (CURLE_OK != code)
	{
		LOG(WARNING) << "curl 设置url失败" << "," << sUrl;
		curl_easy_cleanup(handle);
		return R_FAIL;
	}

	// 控制在控制台打印详细信息的选项
	//code = curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
	//if (CURLE_OK != code)
	//	return R_FAIL;

	if (0 == Config::Instance()->getProtocol().compare("https"))
	{
		code = curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L); // 跳过证书检查  
		if (CURLE_OK != code)
		{
			LOG(WARNING) << "curl 设置跳过https证书失败" << "," << sUrl;
			curl_easy_cleanup(handle);
			return R_FAIL;
		}
	}

    std::string sEncoding =	Config::Instance()->getEncoding();
	if (!sEncoding.empty())
	{
		code = curl_easy_setopt(handle, CURLOPT_ACCEPT_ENCODING, sEncoding.c_str());
		if (CURLE_OK != code)
		{
			LOG(WARNING) << "curl 设置跳过文件编码失败" << "," << sUrl;;
			curl_easy_cleanup(handle);
			return R_FAIL;
		}
	}

	// 进度条
	code = curl_easy_setopt(handle, CURLOPT_NOPROGRESS);
	if (CURLE_OK != code)
	{
		LOG(WARNING) << "curl 取消进度状态失败" << "," << sUrl;;
		curl_easy_cleanup(handle);
		return R_FAIL; 
	}

	// 超时时间，100ms的超时时长
	code = curl_easy_setopt(handle, CURLOPT_TIMEOUT, 100);		
	if (CURLE_OK != code)
	{
		LOG(WARNING) << "curl 设置超时时间失败" << "," << sUrl;;
		curl_easy_cleanup(handle);
		return R_FAIL;
	}

	// 模拟为火狐浏览器，不然谷歌不能直接访问
	code = curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla / 5.0 (Windows NT 10.0; WOW64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 48.0.2564.10 Safari / 537.36");
	if (CURLE_OK != code)
	{
		LOG(WARNING) << "curl 设置代理失败" << "," << sUrl;;
		curl_easy_cleanup(handle);
		return R_FAIL;
	}

	// 设置写回调函数
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, callback_WriteTile);
	if (CURLE_OK != code)
	{
		LOG(WARNING) << "curl 设置写数据回调函数失败" << "," << sUrl;;
		curl_easy_cleanup(handle);
		return R_FAIL;
	}

	code = curl_easy_setopt(handle, CURLOPT_WRITEDATA, pTile);
	if (CURLE_OK != code)
	{
		LOG(WARNING) << "curl 设置写数据对象失败" << "," << sUrl;;
		curl_easy_cleanup(handle);
		return R_FAIL;
	}

	code = curl_easy_perform(handle);
	if (CURLE_OK != code)
	{
		LOG(WARNING) << "load tile failed(level row col code)," << level << "," << row << "," << col << "," << code << "," << sUrl;
		curl_easy_cleanup(handle);

		return R_FAIL;
	}

	code = curl_easy_getinfo(handle, CURLINFO_HTTP_CODE, &lCurlcode);

	if (CURLE_OK != code || lCurlcode >= 400) // += 400~600 都是错的
	{
		LOG(WARNING) << "load tile failed(level row col code),"<< level << "," << row << "," << col << "," << code << "," << sUrl;
		curl_easy_cleanup(handle);
		return R_FAIL;
	}

	curl_easy_cleanup(handle);

	return 0;
}

std::string Loader::getUrl(const std::string & sUrlTemplate, int level, int row, int col)
{
	std::string sUrl(sUrlTemplate);

	char szTemp[32];
	memset(szTemp, 0, 32);
	_itoa_s(level, szTemp, 10);
	size_t nPos = sUrl.find("{level}");
	sUrl.replace(nPos, 7, szTemp);

	memset(szTemp, 0, 32);
	_itoa_s(row, szTemp, 10);
	nPos = sUrl.find("{row}");
	sUrl.replace(nPos, 5, szTemp);

	memset(szTemp, 0, 32);
	_itoa_s(col, szTemp, 10);
	nPos = sUrl.find("{col}");
	sUrl.replace(nPos, 5, szTemp);

	return sUrl;
}
