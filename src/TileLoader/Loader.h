#ifndef LOADER_H__
#define LOADER_H__
#include <string>
#include <sstream>

class UrlFormat;

struct Tile
{
	Tile() :nSize(0),
		nMaxSize(2048)
	{
		pData = (char*)malloc(nMaxSize);
	}

	~Tile()
	{
		free(pData);
		pData = nullptr;
	}

	char* pData;
	size_t   nSize;

	// ×Ö½ÚÊý
	size_t nMaxSize;

	void clear()
	{
		nSize = 0;
	}

	void clone(Tile** pNew);
};

class Loader
{
public:
	Loader();
	~Loader();

	int loadTile(std::string sUrl, int level, int row, int col, Tile* pTile, long& lCurlcode);

	std::string getUrl(const std::string & sUrlTemplate, int level, int row, int col);
};

#endif // LOADER_H__
