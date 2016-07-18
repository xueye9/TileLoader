#include "GWMTSCapabilitiesParser.h"
#include "rapidxml/rapidxml.hpp"  
#include "rapidxml/rapidxml_utils.hpp"  
#include "rapidxml/rapidxml_print.hpp"

using namespace rapidxml;

std::shared_ptr<Capabilities> GWMTSCapabilitiesParser::Parse(const std::string& strCapabilities, double dbDPI)
{
	std::shared_ptr<Capabilities> spCapabilities( new Capabilities() );
	m_dbDPI = dbDPI;

	// 调用解析器代码
	file<> fdoc(strCapabilities.c_str());

	xml_document<>   doc;
	doc.parse<0>(fdoc.data());

	//Capabilities 
	xml_node<>* root = doc.first_node();
	if (!root)
	{
		return std::shared_ptr<Capabilities>();
	}

	xml_node<>* node = root->first_node("Contents");
	if (!node)
	{
		return std::shared_ptr<Capabilities>();
	}

	//xml_node<>* layer =	node->first_node("Layer");

	_parseContents(node, spCapabilities);

	return spCapabilities;
}

void GWMTSCapabilitiesParser::_parseContents(const rapidxml::xml_node<> * pContents, std::shared_ptr<Capabilities> spCapabilities)
{
	//xml_node<>* pLayer = pContents->first_node("Layer");

	//
	xml_node<>* pTileMatrixSet = pContents->first_node("TileMatrixSet");

	for (; pTileMatrixSet != nullptr; pTileMatrixSet = pTileMatrixSet->next_sibling() )
	{
		std::shared_ptr<TileMatrixSet> spTileMatrixSet = _parseTileMatrixSet(pTileMatrixSet);
		if (spTileMatrixSet)
			spCapabilities->addTileMatrixSet(spTileMatrixSet);
	}
	
}// 解析Contents节点

std::shared_ptr<TileMatrixSet> GWMTSCapabilitiesParser::_parseTileMatrixSet(const rapidxml::xml_node<> * pTileMatrixSet)
{
	std::shared_ptr<TileMatrixSet> spTileMatrixSet(new TileMatrixSet);

	xml_node<> * node = pTileMatrixSet->first_node("ows:Identifier");
	std::string sIdentifier = node->value();
	spTileMatrixSet->setIdentifier(sIdentifier);

	node = pTileMatrixSet->first_node("ows:SupportedCRS");
	std::string sSupportCRS = node->value();
	spTileMatrixSet->setSupportedCRS(sSupportCRS);
	double dbMetersPerUnit = getMetersPerCRSUnit(sSupportCRS);

	node = pTileMatrixSet->first_node("TileMatrix");

	for (; node != nullptr; node = node->next_sibling())
	{
		std::shared_ptr<TileMatrix> spTileMatrix = _parseTileMatrix(node, dbMetersPerUnit);
		if(spTileMatrix)
			spTileMatrixSet->addTileMatrix(spTileMatrix);
	}
	
	return spTileMatrixSet;
}

std::shared_ptr<TileMatrix> GWMTSCapabilitiesParser::_parseTileMatrix(const rapidxml::xml_node<> * pTileMatrixNode, double dbMetersPerUnit)
{
	std::shared_ptr<TileMatrix> spTileMatrix(new TileMatrix(m_dbDPI, dbMetersPerUnit));

	xml_node<>* pNode = pTileMatrixNode->first_node("ows:Identifier");
	if(nullptr == pNode)
		return std::shared_ptr<TileMatrix>();
	spTileMatrix->m_qsIdentifier = pNode->value();

	pNode = pTileMatrixNode->first_node("ScaleDenominator");
	if(nullptr == pNode)
		return std::shared_ptr<TileMatrix>();
	spTileMatrix->m_dbScaleDenominator = atof(pNode->value());

	pNode = pTileMatrixNode->first_node("TopLeftCorner");
	if(nullptr == pNode)
		return std::shared_ptr<TileMatrix>();
	std::string sTopLeftCorner = pNode->value();

	int nPos = sTopLeftCorner.find(' ');
	if (-1 == nPos)
		return std::shared_ptr<TileMatrix>();

	std::string sX = sTopLeftCorner.substr(0, nPos);
	std::string sY = sTopLeftCorner.substr(nPos + 1, sTopLeftCorner.size() - nPos - 1);

	double dbX = atof( sX.c_str() );
	double dbY = atof( sY.c_str() );
	if (dbY < 0)
	{
		double dbTemp = dbX;
		dbX = dbY;
		dbY = dbTemp;
	}

	spTileMatrix->m_ptTopLeftCorner.x = dbX;
	spTileMatrix->m_ptTopLeftCorner.y = dbY;

	pNode = pTileMatrixNode->first_node("TileWidth");
	if(nullptr == pNode)
		return std::shared_ptr<TileMatrix>();
	spTileMatrix->m_nTileWidth = atoi(pNode->value());

	pNode = pTileMatrixNode->first_node("TileHeight");
	if(nullptr == pNode)
		return std::shared_ptr<TileMatrix>();
	spTileMatrix->m_nTileHeight = atoi(pNode->value());

	pNode = pTileMatrixNode->first_node("MatrixWidth");
	if(nullptr == pNode)
		return std::shared_ptr<TileMatrix>();
	spTileMatrix->m_nMatrixWidth = atoi(pNode->value());

	pNode = pTileMatrixNode->first_node("MatrixHeight");
	if(nullptr == pNode)
		return std::shared_ptr<TileMatrix>();
	spTileMatrix->m_nMatrixHeight = atoi(pNode->value());

	spTileMatrix->improve();

	return spTileMatrix;
}

int getEPSG(const std::string& qsCRS)
{
	// 参考 http://www.epsg.org/ 的参考坐标系定义
	// 应该解析ogc的urn字符串，该处直接搜索关键字
	int nIndex = qsCRS.find("EPSG");
	if(nIndex < 0)
		return -1;

	nIndex = qsCRS.rfind(':');
	if( nIndex < 0)
		return -1;

	std::string qsSrid = qsCRS.substr(nIndex+1);
	int nSrid = atoi(qsSrid.c_str());
	return nSrid;
}

double GWMTSCapabilitiesParser::getMetersPerCRSUnit(const std::string& qsCRS)
{
	int nSrid = getEPSG(qsCRS);
	if( nSrid < 0)
		return 1.0;

	switch(nSrid)
	{
	case 4490://CGCS200 经纬度
		return 40075016.6855785 / 360.0;

	case 4326:// wgs84 经纬度
		return 40075016.6855785 / 360.0;

	// webmercator 的EPSG官方编号和非官方编号
	case 3785:
	case 900931:
		return 1.0;

	default:
		return	1.0;
	}
}


