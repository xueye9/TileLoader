/*!
 * \file Capabilities.h
 * \author Baldwin
 * \brief 一个WMTS的Capabilities
 *
 * TODO: long description
 * \date 九月 2015
 * \copyright	Copyright (c) 2015, 北京国科恒通电气自动化科技有限公司\n
	All rights reserved.
 */
#ifndef CAPABILITIES_H__
#define CAPABILITIES_H__

#include <map>
#include <string>
#include <memory>
//#include <QMap>
//#include <QString>
//#include <QSharedPointer>

#include "TileMatrixSet.h"

class Capabilities
{
public:
	Capabilities();

	void setIdentifier(const std::string& qsIdentifier);
	std::string getIdentifier();

	// Provider 略

	// layer 略

	// tilematrixset

	/// @pTileMatrixSet 堆上创建的对象，如返回为true调用者不用管理其生命周期，返回false需要调用者管理器生命周期
	bool addTileMatrixSet(std::shared_ptr<TileMatrixSet> pTileMatrixSet);

	/// 获取切片矩阵集
	std::shared_ptr<TileMatrixSet> getTileMatrixSet(const std::string& qsIdentifier) const;

	/// 删除切片举证集
	int removeTileMatrixSet(const std::string& qsIdentifier);

private:
	std::string m_qsUrl;
	// 切片矩阵集的集合
	std::map<std::string, std::shared_ptr<TileMatrixSet> > m_mpQs2TMS_;
};

#endif // CAPABILITIES_H__