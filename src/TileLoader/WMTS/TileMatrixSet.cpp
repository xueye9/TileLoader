#include "TileMatrixSet.h"

void TileMatrixSet::setIdentifier(const std::string& qsIdentifier)
{
	m_qsIdentifier_ = qsIdentifier;
}

std::string TileMatrixSet::getIdentifier() const
{
	return m_qsIdentifier_;
}

void TileMatrixSet::setSupportedCRS(const std::string& qsCRS)
{
	m_qsSupportedCRS_ = qsCRS;
}	

std::string TileMatrixSet::getSupportedCRS() const
{
	return m_qsSupportedCRS_;
}

int TileMatrixSet::addTileMatrix( std::shared_ptr<TileMatrix> spTileMatrix)
{
	if( !spTileMatrix )
		return e_fail;

	std::map<std::string, std::shared_ptr<TileMatrix> >::const_iterator it = m_mpQstr2TileMatrix_.find(spTileMatrix->m_qsIdentifier);
	if( it != m_mpQstr2TileMatrix_.end() )
		return e_fail;

	m_mpQstr2TileMatrix_.insert( std::make_pair(spTileMatrix->m_qsIdentifier, spTileMatrix) );

	m_vTileMatries.push_back(spTileMatrix);

	return e_success;
}

std::shared_ptr<TileMatrix> TileMatrixSet::getTileMatrix(const std::string& qsId) const
{
	std::map<std::string, std::shared_ptr<TileMatrix> >::const_iterator it = m_mpQstr2TileMatrix_.find(qsId);
	if( it != m_mpQstr2TileMatrix_.end() )
		return it->second;
	
	return std::shared_ptr<TileMatrix>();
}

int TileMatrixSet::location(double xCRS, double yCRS, const std::string& qsTileMatrixIdentifier, int& nCol, int& nRow)
{
	std::map<std::string, std::shared_ptr<TileMatrix> >::const_iterator it = m_mpQstr2TileMatrix_.find(qsTileMatrixIdentifier); 
	if( it == m_mpQstr2TileMatrix_.end() )
		return e_fail;

	std::shared_ptr<TileMatrix> spTileMatrix = it->second;
	if( !spTileMatrix )
		return e_fail;

	return spTileMatrix->location(xCRS, yCRS, nCol, nRow);
}

int TileMatrixSet::getCount()
{
	return m_mpQstr2TileMatrix_.size();
}

const std::vector< std::shared_ptr<TileMatrix> >& TileMatrixSet::getTileMatries() const
{
	return m_vTileMatries;
}
