#ifndef PROJECTION_H__
#define PROJECTION_H__

// stl
#include <string>

class  Projection
{
public:
	 Projection(int nSrid, const std::string& sAuthName, int nAuthSrid, const std::string& sProj4Text, const std::string& sRefSysName);
	~Projection();

	/// @brief     trans 坐标投影
	/// @detail    Projection::trans 只支持相同基准面之间坐标系的投影变换，经纬度必须是小数形式的度
	/// @param[in] int nSrid  目标投影Srid
	/// @param[in] double dbPts 投影的点集，经纬度坐标是小数形式的度,二维点数组 x,y,x,y...
	/// @param[in] int nCount 点总数
	/// @return    bool 投影成功
	bool trans(int nSrid, double * dbPts, int nCount);

	const std::string& getPro4text() const
	{
		return m_sProj4text;
	}

	int getSRID() const
	{
		return m_nSrid;
	}
private:
	Projection();

	int m_nSrid;
	std::string m_sAuthName;
	int m_nAuthSrid;
	std::string m_sRefSysName;
	std::string m_sProj4text;
};

#endif // PROJECTION_H__
