#include "Capabilities.h"

Capabilities::Capabilities()
{

}

void Capabilities::setIdentifier(const std::string& qsIdentifier)
{
	m_qsUrl = qsIdentifier;
}

std::string Capabilities::getIdentifier()
{
	return m_qsUrl;
}

bool Capabilities::addTileMatrixSet( std::shared_ptr<TileMatrixSet> spTileMatrixSet)
{
	if( !spTileMatrixSet )
		return false;

	std::map <std::string, std::shared_ptr<TileMatrixSet> >::const_iterator it = m_mpQs2TMS_.find( spTileMatrixSet->getIdentifier() );
	if( it != m_mpQs2TMS_.end() )
		return false;

	m_mpQs2TMS_.insert( std::make_pair(spTileMatrixSet->getIdentifier(), spTileMatrixSet) );
	return true;
}

std::shared_ptr<TileMatrixSet> Capabilities::getTileMatrixSet(const std::string& qsIdentifier) const
{
	std::map<std::string, std::shared_ptr<TileMatrixSet> >::const_iterator it = m_mpQs2TMS_.find( qsIdentifier );
	if( it != m_mpQs2TMS_.end() )
		return it->second;

	return std::shared_ptr<TileMatrixSet>();
}

int Capabilities::removeTileMatrixSet(const std::string& qsIdentifier)
{
	std::map<std::string, std::shared_ptr<TileMatrixSet> >::const_iterator it = m_mpQs2TMS_.find( qsIdentifier );
	if( it != m_mpQs2TMS_.end() )
	{
		m_mpQs2TMS_.erase(qsIdentifier);
		return 0;// successs
	}

	return 0;
}

