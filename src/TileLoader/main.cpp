#include <iostream>
#include <sstream>
#include "Loader.h"
#include "glog/logging.h"
#include "glog/stl_logging.h"
//#include "StreetFormat.h"
#include "pthread/pthread.h"
#include "ThreadPool.h"
//#include "StreetMapWork.h"
//#include "SatelliteMapWork.h"

#include <direct.h>
#include <memory>

#include <windows.h>

#include "config.h"


#include "rapidxml/rapidxml.hpp"  
#include "rapidxml/rapidxml_utils.hpp"  
#include "rapidxml/rapidxml_print.hpp"

#include "Progress.h"

#include <stdlib.h>
#include <stdio.h>

#include "utiliyapi.h"

#include "distributor.h"

#include "projectionfactory.h"

#include "WMTS/GWMTSCapabilitiesParser.h"

#include <stdlib.h>

#pragma comment(lib, "libcurl.lib")
#pragma comment(lib, "libglog.lib")
#pragma comment(lib, "pthreadVC2.lib")
#pragma comment(lib, "proj_i.lib")

void initLog(const std::string & sDir);

int main(int argc, char** argv)
{
	google::InitGoogleLogging(argv[0]);

	std::string sPath(_pgmptr);

	int nPos0 = sPath.rfind('/');
	int nPos1 = sPath.rfind('\\');
	int nPos;
	if (nPos0 != std::string::npos && nPos1 != std::string::npos)
	{
		nPos = nPos0 > nPos1 ? nPos1 : nPos0;
	}
	else
	{
		nPos = nPos0 > nPos1 ? nPos0 : nPos1;
	}

	std::string sDir = sPath.substr(0, nPos);

	initLog(sDir);
	
	LOG(INFO) << "开始下载瓦片数据...";

	std::stringstream ss;
	ss << sDir << "/config.xml";
	std::string sConfigPath = ss.str();
	if (!Config::Instance()->load(ss.str()))
	{
		LOG(ERROR) << "请检查配置文件:" << sConfigPath;
		return -1;
	}

	ss.str("");
	ss << sDir << "/spatial_ref_sys.json";
	std::string sProjectionFile = ss.str();
	if (ProjectionFactory::Instance()->loadFromJson(sProjectionFile) < 0)
	{
		LOG(ERROR) << "投影数据库文件:" << sProjectionFile;
		return -1;
	}
	
	int nThreadNum = Config::Instance()->getSlaveCount();
	ThreadPool pool(nThreadNum);

	// 设置线程池任务上限
	pool.setLowerLimit( Config::Instance()->getLowerLimit() );
	pool.setUpperLimit( Config::Instance()->getUpperLimit() );

	pool.activate();

	double db84Box[4];
	Config::Instance()->getBBox(db84Box[0], db84Box[3], db84Box[2], db84Box[1]);

	std::shared_ptr<Projection> spProjection = ProjectionFactory::Instance()->getProjection(4326);
	if (!spProjection)
	{
		LOG(ERROR) << "找不到WGS84投影:" << 4326;
		return -1;
	}
	
	// 变换坐标到切片的投影坐标系
	int nSrid = Config::Instance()->getSRID();

	double dbBox[4];
	memcpy(dbBox, db84Box, 4 * sizeof(double));
	if (!spProjection->trans(nSrid, dbBox, 2))
	{
		return -1;
	}

	int nLevel= 0;

	std::vector<int> vLevels = Config::Instance()->getLevels();
	std::vector<int>::iterator it = vLevels.begin();

	const std::string& sTileMatrixSet = Config::Instance()->getTileMatrixSet();

	std::string sWMTSCapaFile = Config::Instance()->getWMTSCapabilitiesFilePath();

	GWMTSCapabilitiesParser Parser;
	std::shared_ptr<Capabilities> spCapa = Parser.Parse(sWMTSCapaFile, Config::Instance()->getDPI());

	Distributor* pDisp = nullptr;
	if (Config::arcgis_compact == Config::Instance()->getOutDataType())
	{
		pDisp = new BundleDistributor(&vLevels, &pool, spCapa, sTileMatrixSet, dbBox[0], dbBox[1], dbBox[2], dbBox[3]);
	}
	else if (Config::explode == Config::Instance()->getOutDataType())
	{
		pDisp = new ExplodeDistributor(&vLevels, &pool, spCapa, sTileMatrixSet, dbBox[0], dbBox[1], dbBox[2], dbBox[3]);
	}

	// 计算需要下载的瓦片总数
	int nTileCount = 0;
	for (it = vLevels.begin(); it != vLevels.end(); ++it)
	{
		nLevel = *it;
		int nBeginRow = 0;
		int nMaxRow = nLevel > 0 ? (1 << nLevel) : 0;
		int nBeginCol = 0;
		int nMaxCol = nLevel > 0 ? (1 << nLevel) : 0;

		int nRealMaxRow = 0;
		int nRealMaxCol = 0;

		pDisp->WMTSGetRowCol(nLevel, dbBox[0], dbBox[1], dbBox[2], dbBox[3],
			nBeginRow, nBeginCol, nMaxRow, nMaxCol);

		nTileCount += (nMaxRow - nBeginRow) * (nMaxCol - nBeginCol);
	}

	ThreadPool::getProgress()->setCount(nTileCount);

	pDisp->start();

	getchar();

	google::ShutdownGoogleLogging();

	return 0;
}

void initLog(const std::string & sDir)
{
	std::stringstream ss;

	ss << sDir << "/log";
	std::string sTemp = ss.str();
	checkDestination(sTemp);

	// 初始化日志
	ss.str("");
	ss << sDir << "/log/info_";
	sTemp = ss.str();
	google::SetLogDestination(google::GLOG_INFO, sTemp.c_str());

	ss.str("");
	ss << sDir << "/log/error_";
	sTemp = ss.str();
	google::SetLogDestination(google::GLOG_ERROR, sTemp.c_str());

	ss.str("");
	ss << sDir << "/log/warning_";
	sTemp = ss.str();
	google::SetLogDestination(google::GLOG_WARNING, sTemp.c_str());

	ss.str("");
	ss << sDir << "/log/fatal_";
	sTemp = ss.str();
	google::SetLogDestination(google::GLOG_FATAL, sTemp.c_str());

	google::SetStderrLogging(google::GLOG_ERROR);
	FLAGS_colorlogtostderr = true;
}