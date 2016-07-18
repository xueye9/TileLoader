#ifndef CAPABILITIESMANAGER_H__
#define CAPABILITIESMANAGER_H__
#include <QMap>
#include <QString>
#include <QSharedPointer>
#include "TileMatrixSet.h"
#include <CSingleton.h>
#include "Capabilities.h"

class CapabilitiesManager;

class CapabilitiesManager
{
public:
	DECLARE_SINGLETON(CapabilitiesManager);

	/// @pTileMatrixSet 堆上创建的对象，如返回为true调用者不用管理其生命周期，返回false需要调用者管理器生命周期
	bool addCapabilities(QSharedPointer<Capabilities> spCapabilities);

	/// 获取
	QSharedPointer<Capabilities> getCapabilities(const QString& qsIdentifier) const;

	/// 删除
	int removeCapabilities(const QString& qsIdentifier);

private:
	// 切片举证集的集合
	QMap<QString, QSharedPointer<Capabilities> > m_mpQStr2Capabilites;
};


#endif // CAPABILITIESMANAGER_H__
