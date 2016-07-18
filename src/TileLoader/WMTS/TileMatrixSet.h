/*!
 * \file TileMatrixSet.h
 * \author Baldwin
 * \brief WMTS服务切片矩阵集
 *
 * TODO: long description
 * \date 九月 2015
 * \copyright	Copyright (c) 2015, 北京国科恒通电气自动化科技有限公司\n
	All rights reserved.
 */
#ifndef TILEMATRIXSET_H__
#define TILEMATRIXSET_H__

//#include <QSharedPointer>
//#include <QVector>
//#include <QMap>
//#include <QString>
#include "TileMatrix.h"

#include <string>
#include <vector>
#include <map>
#include <memory>


class TileMatrixSet
{
	enum
	{
		e_success = 0,
		e_fail
	};
public:
	void setIdentifier(const std::string& qsIdentifier);
	std::string getIdentifier() const;

	void setSupportedCRS(const std::string& qsCRS);
	std::string getSupportedCRS() const;

	/// 新增一个切片举证，0-success，1-失败
	/// 参数是堆上创建的对象，添加成功后调用者不需要管理其生命周期
	int addTileMatrix(std::shared_ptr<TileMatrix> spTileMatrix);

	std::shared_ptr<TileMatrix> getTileMatrix(const std::string& qsId) const;

	/// 定位坐标所在的瓦片行列号
	/// 坐标为 getSupportedCRS 下的坐标
	/// @return 0-success 1-fail 2-坐标不再范围内
	int location(double xCRS, double yCRS, const std::string& qsTileMatrixIdentifier, int& nCol, int& nRow);

	// 获取TileMatrix 总数
	int getCount();

	// 获取
	const std::vector< std::shared_ptr<TileMatrix> >& getTileMatries() const;

private:
	// 标示 相当于ID
	std::string m_qsIdentifier_;

	// CRS SRID 
	std::string m_qsSupportedCRS_;

	std::map<std::string, std::shared_ptr<TileMatrix> > m_mpQstr2TileMatrix_;

	std::vector< std::shared_ptr<TileMatrix> > m_vTileMatries;
};

#endif // TILEMATRIXSET_H__