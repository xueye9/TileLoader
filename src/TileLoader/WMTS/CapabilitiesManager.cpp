#include "StdAfx.h"
#include "CapabilitiesManager.h"


DEFINE_SINGLETON(CapabilitiesManager)

bool CapabilitiesManager::addCapabilities(QSharedPointer<Capabilities> spCapabilities)
{
	QMap<QString, QSharedPointer<Capabilities> >::const_iterator it = m_mpQStr2Capabilites.find( spCapabilities->getIdentifier() );
	if( it != m_mpQStr2Capabilites.end() )
		return false;

	m_mpQStr2Capabilites.insert(spCapabilities->getIdentifier(), spCapabilities);
	return true;
}

QSharedPointer<Capabilities> CapabilitiesManager::getCapabilities(const QString& qsIdentifier) const
{
	QMap<QString, QSharedPointer<Capabilities> >::const_iterator it = m_mpQStr2Capabilites.find( qsIdentifier );
	if( it != m_mpQStr2Capabilites.end() )
		return it.data();

	return QSharedPointer<Capabilities>();
}

int CapabilitiesManager::removeCapabilities(const QString& qsIdentifier)
{
	QMap<QString, QSharedPointer<Capabilities> >::iterator it = m_mpQStr2Capabilites.find( qsIdentifier );
	if( it != m_mpQStr2Capabilites.end() )
	{
		m_mpQStr2Capabilites.erase( it );
		return 0;
	}

	return 1;
}
