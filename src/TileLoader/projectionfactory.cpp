#include "projectionfactory.h"

// glog
#include "glog/logging.h"

// rapidjson
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"

#include <fstream>

ProjectionFactory* ProjectionFactory::_ins = nullptr;


ProjectionFactory::ProjectionFactory()
{
}

ProjectionFactory::~ProjectionFactory()
{
}

int ProjectionFactory::loadFromJson(const std::string& sFile)
{
	std::ifstream stream(sFile.c_str(), std::ios_base::in);
	if (!stream.is_open())
	{
		LOG(ERROR) << "缺少投影文件：spatial_ref_sys.json";
		return -1;
	}

	std::string sJson(std::istreambuf_iterator<char>(stream.rdbuf()), (std::istreambuf_iterator<char>()));
	stream.close();

	using namespace rapidjson;

	Document doc;
	doc.Parse(sJson.c_str());

	Value& sSpatialRefSyses = doc["spatial_ref_syses"];
	if (!sSpatialRefSyses.IsArray())
	{
		return -1; 
	}

	int nCount = 0;
	for (SizeType i = 0; i < sSpatialRefSyses.Size(); i++)
	{
		const Value& vSpatialRefSys = sSpatialRefSyses[i];

		const Value& vSrid = vSpatialRefSys["srid"];
		int nSrid = vSrid.GetInt();

		const Value& vAuthName = vSpatialRefSys["auth_name"];
		const std::string& sAuthName = vAuthName.GetString();

		const Value& vAuthSrid = vSpatialRefSys["auth_srid"];
		int nAuthSrid =	vAuthSrid.GetInt();

		const Value& vSpatialRefSysName = vSpatialRefSys["ref_sys_name"];
		const std::string& sSpatialRefSysName = vSpatialRefSysName.GetString();

		const Value& vProj4Text = vSpatialRefSys["proj4text"];
		const std::string& sProj4text = vProj4Text.GetString();

		std::shared_ptr<Projection> spProjection(new Projection(nSrid, sAuthName, nAuthSrid, sProj4text, sSpatialRefSysName));

		m_mapProjects.insert(std::make_pair(nSrid, spProjection));

		++nCount;
	}

	return nCount;
}

std::shared_ptr<Projection> ProjectionFactory::getProjection(int nSrid)
{
	std::map<int, std::shared_ptr<Projection> >::const_iterator it = m_mapProjects.find(nSrid);
	if (it == m_mapProjects.end())
	{
		return std::shared_ptr<Projection>();
	}
	else
	{
		return it->second;
	}
}

ProjectionFactory* ProjectionFactory::Instance()
{
	if (_ins == nullptr)
	{
		_ins = new ProjectionFactory;
	}

	return _ins;
}

void ProjectionFactory::deInstance()
{
	delete _ins;
}
