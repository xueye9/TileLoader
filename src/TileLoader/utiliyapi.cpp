#include "utiliyapi.h"
#include <io.h>
#include <direct.h>
#include <math.h>

#ifndef MIN
#define MIN(_x,_y) (_x) > (_y) ? (_y):(_x)
#endif // !MIN


bool checkDestination(std::string const& sDestinationDir)
{
	int nPrePos(0);
	while (_access(sDestinationDir.c_str(), 0) != -1)
	{
		// 创建
		int nPos, nPos0, nPos1;

		nPos0 = sDestinationDir.find('\\', nPrePos);
		if (nPos0 < 0)
		{
			nPos1 = sDestinationDir.find('/', nPrePos);
			if (nPos1 < 0)
			{
				break;
			}
			else
			{
				nPos = nPos1;
			}
		}
		else
		{
			nPos1 = sDestinationDir.find('/', nPrePos);
			if (nPos1 < 0)
			{
				nPos = nPos0;
			}
			else
			{
				nPos = MIN(nPos0, nPos1);
			}
		}

		std::string sParent = sDestinationDir.substr(0, nPos);

		// 没有文件夹则创建
		if (-1 == _access(sParent.c_str(), 0))
		{
			if (_mkdir(sParent.c_str()) < 0)
			{
				return false;
			}
		}

		nPrePos = nPos + 1;
	}

	if (_mkdir(sDestinationDir.c_str()) < 0)
	{

		return false;
	}
	else
	{
		return true;
	}
}