/*!
 * \file GWMTSCapabilitiesParser.h
 *
 * \author Think
 * \date 六月 2015
 *
 * 
 */
// file
// WMTS Capabilities 解析器
#ifndef GWMTSCAPABILITIESPARSER_H__
#define GWMTSCAPABILITIESPARSER_H__
//#include <QString>
#include <string>
#include <map>
#include <vector>
//#include "../WMTSParamDef.h"
#include "Capabilities.h"
//#include <QSharedPointer>
#include <memory>

#include "rapidxml/rapidxml.hpp"  
#include "rapidxml/rapidxml_utils.hpp"  
#include "rapidxml/rapidxml_print.hpp"

class GWMTSCapabilitiesParser
{
public:
	
	// 解析 WMTS Capabilities 
	// strCapabilities Capabilities XML内容
	// pWMTS 解析的内容填充目标
	// nDPI
	// dbMetersPerUnit WGS84默认 40075016.6856 / 360.0 = 赤道/360.0
	std::shared_ptr<Capabilities> Parse(const std::string& strCapabilities, /*WMTSConnector* pConnector,*/ double dbDPI = 90.71);


private:
	void _parseContents(const rapidxml::xml_node<> * pContents, std::shared_ptr<Capabilities> spCapabilities);
	
	std::shared_ptr<TileMatrixSet> _parseTileMatrixSet(const rapidxml::xml_node<> * pTileMatrixSet);
	
	std::shared_ptr<TileMatrix> _parseTileMatrix(const rapidxml::xml_node<> * pTileMatrixNode, double dbMetersPerUnit);

	double getMetersPerCRSUnit(const std::string& qsCRS);

	double m_dbDPI;
};

#endif // GWMTSCAPABILITIESPARSER_H__
