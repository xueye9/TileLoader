#ifndef PROJECTIONFACTORY_H__
#define PROJECTIONFACTORY_H__

#include "projection.h"

// stl
#include <map>
#include <memory>

class ProjectionFactory
{
public:
	ProjectionFactory();
	~ProjectionFactory();

	/// @brief     loadFromJson 从json文件加载投影
	/// @detail    ProjectionFactory::loadFromJson
	/// @param[in] const std::string & sFile json文件
	/// @return    int 加载的投影个数
	int loadFromJson(const std::string& sFile);

	std::shared_ptr<Projection> getProjection(int nSrid);

	static ProjectionFactory* Instance();
	static void deInstance();

private:
	std::map<int, std::shared_ptr<Projection> > m_mapProjects;

	static ProjectionFactory* _ins;
};

#endif // PROJECTIONFACTORY_H__
