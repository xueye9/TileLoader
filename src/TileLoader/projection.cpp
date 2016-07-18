#include "projection.h"
#include "projectionfactory.h"

// proj4
#include "proj/proj_api.h"

// glog
#include "glog/logging.h"


Projection::Projection(int nSrid, const std::string& sAuthName, int nAuthSrid, const std::string& sProj4Text, const std::string& sRefSysName)
{
	m_nSrid = nSrid;
	m_sAuthName = sAuthName;
	m_nAuthSrid = nAuthSrid;
	m_sProj4text = sProj4Text;
	m_sRefSysName = sRefSysName;
}

Projection::Projection()
{

}

Projection::~Projection()
{

}

bool Projection::trans(int nSrid, double * dbPts, int nCount)
{
    std::shared_ptr<Projection> spDestProjection = ProjectionFactory::Instance()->getProjection(nSrid);
	
	projPJ pjOrigin = nullptr;
	if (!(pjOrigin = pj_init_plus(this->getPro4text().c_str())))
	{
		LOG(ERROR) << "初始化坐标系：" << this->m_nSrid << " 失败";
		return false;
	}

	// 经纬度转弧度
	if (pj_is_latlong(pjOrigin))
	{
		for (int i = 0; i < nCount; ++i)
		{
			dbPts[2 * i] = dbPts[2 * i] * DEG_TO_RAD;
			dbPts[2 * i+1] = dbPts[2 * i + 1] * DEG_TO_RAD;
		}
	}

	projPJ pjDest = nullptr;
	if (!(pjDest = pj_init_plus(spDestProjection->getPro4text().c_str())))
	{
		LOG(ERROR) << "初始化坐标系：" << spDestProjection->getSRID() << " 失败";
		return false;
	}

	bool bSuccess = true;
	long lCount = (long)nCount;
	int nOffSet = 1;
	int nRet = pj_transform(pjOrigin, pjDest, lCount, 2, dbPts, dbPts + 1, NULL);
	if (nRet != 0)
	{
		LOG(ERROR) << "投影错误：" << nRet;
		bSuccess = false;
	}

	// 经纬度转弧度
	if (pj_is_latlong(pjDest))
	{
		for (int i = 0; i < nCount; ++i)
		{
			dbPts[2 * i] = dbPts[2 * i] * RAD_TO_DEG;
			dbPts[2 * i + 1] = dbPts[2 * i + 1] * RAD_TO_DEG;
		}
	}

	pj_free(pjDest);
	pj_free(pjOrigin);
	return bSuccess;
}
