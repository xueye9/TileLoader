#include "config.h"
#include "rapidxml/rapidxml.hpp"  
#include "rapidxml/rapidxml_utils.hpp"  
#include "rapidxml/rapidxml_print.hpp" 
#include <iostream>

#include "glog/logging.h"

using namespace rapidxml;


Config* Config::_ins = nullptr;

Config* Config::Instance()
{
	if (nullptr == _ins)
		_ins = new Config();

	return _ins;
}

Config::Config()
{
	m_sEncoding = "";
}

Config::~Config()
{
}

const std::string& Config::getResourceUrl() const
{
	return m_sUrl;
}

int Config::setSlaveCount()
{
	return 0;
}

int Config::getSlaveCount() const
{
	return m_nSlavesCount;
}

bool Config::setBBox(double left, double bottom, double right, double top)
{
	return false;
}

bool Config::getBBox(double& left, double& bottom, double& right, double& top) const
{
	left = m_dbLeft;
	top = m_dbTop;

	right = m_dbRight;
	bottom = m_dbBottom;

	return true;
}

std::string  Config::getOutDir() const
{
	return m_sOutDir;
}

Config::OutPutType Config::getOutDataType() const
{
	if (0 == m_sOutputType.compare("arcgis:compact"))
	{
		return arcgis_compact;
	}
	else if(0 == m_sOutputType.compare("explode"))
	{
		return explode;
	}
	else
	{
		return unknown;
	}
}

std::vector<int> Config::getLevels() const
{
	return m_vLevels;
}

Config::ResourceType Config::getResourceType()
{
	return googlemap_street;
}

bool Config::load(const std::string& sfile)
{
	file<> fdoc( sfile.c_str() );

	xml_document<>   doc;
	doc.parse<0>(fdoc.data());

	std::cout << doc.name() << std::endl;

	//! 获取根节点  
	xml_node<>* root = doc.first_node();
	if (NULL == root)
		return false;

	rapidxml::xml_node<> * node = root->first_node("slavenum");
	if (NULL == node)
		return false;
	m_nSlavesCount = atoi(node->value());

	node = root->first_node("lowerlimit");
	if (NULL == node)
		return false;
	m_nLowerLimit = atoi(node->value());

	node = root->first_node("upperlimit");
	if (NULL == node)
		return false;
	m_nUpperLimit = atoi(node->value());

	node = root->first_node("left");
	if (NULL == node)
		return false;
	m_dbLeft = atof(node->value());

	node = root->first_node("bottom");
	if (NULL == node)
		return false;
	m_dbBottom = atof(node->value());

	node = root->first_node("right");
	if (NULL == node)
		return false;
	m_dbRight = atof(node->value());

	node = root->first_node("top");
	if (NULL == node)
		return false;
	m_dbTop = atof(node->value());

	node = root->first_node("origin");
	if (NULL == node)
		return false;

	if (!_loadOrigin(node))
		return false;

	node = root->first_node("out");
	if (NULL == node)
		return false;

	if (!_loadOut(node))
		return false;

	return true;
}

bool Config::_loadOrigin(void* p)
{
	rapidxml::xml_node<> *pNode = (rapidxml::xml_node<> *)p;
	rapidxml::xml_node<>* node = pNode->first_node("url");
	if (NULL == node)
		return false;

	m_sUrl = node->value();

	node = pNode->first_node("srid");
	if ( node )
	{
		m_nSrid = atof(node->value()); 
	}
	else
	{
		LOG(ERROR) << "检查配置文件，srid";
		return false;
	}
	
	node = pNode->first_node("wmts:Capabilities");
	if (node)
	{
		m_sWMTSCapabalitiesFile = node->value();
	}
	else
	{
		LOG(ERROR) << "检查配置文件：wmts:Capabilities";
		return false;
	}

	node = pNode->first_node("wmts:TileMatrixSet");
	if (node)
	{
		m_sTileMatrixSet = node->value();
	}
	else
	{
		LOG(ERROR) << "检查配置文件：wmts:TileMatrixSet";
		return false;
	}

	node = pNode->first_node("dpi");
	if (node)
	{ 
		m_dbDPI = atof(node->value());
	}
	else
	{
		LOG(ERROR) << "检查配置文件：dpi";
		return false;
	}

	node = pNode->first_node("level");
	if (node)
	{
		for (node = pNode->first_node("level"); node != nullptr; node = node->next_sibling())
		{
			m_vLevels.push_back(atoi(node->value()));
		}
	}
	else
	{
		LOG(ERROR) << "检查配置文件：level";
		return false;
	}
	
	node = pNode->first_node("Content-Encoding");
	if (node)
	{
		m_sEncoding = node->value(); 
	}
	else
	{
		LOG(ERROR) << "检查配置文件：Content-Encoding";
		return false;
	}

	node = pNode->first_node("protocol");
	if (node)
	{
		m_sProtocol = node->value();
	}
	else
	{
		LOG(ERROR) << "检查配置文件：protocol";
		return false;
	}

	return true;
}

bool Config::_loadOut(void* p)
{
	rapidxml::xml_node<> *pNode = (rapidxml::xml_node<> *)p;
	rapidxml::xml_node<>* node = pNode->first_node("type");
	if (node)
	{
		m_sOutputType = node->value();
	}
	else
	{
		LOG(ERROR) << "检查配置文件：output:type";
		return false;
	}

	node = pNode->first_node("dir");
	if (node)
	{
		m_sOutDir = node->value(); 
	}
	else
	{ 
		LOG(ERROR) << "检查配置文件：output:dir";
		return false;
	}
	

	node = pNode->first_node("suffix");
	if (node)
	{
		m_sOutFileSuffix = node->value();
	}
	else
	{ 
		LOG(ERROR) << "检查配置文件：output:suffix";
		return false;
	}

	return true;
}

std::string Config::getWMTSCapabilitiesFilePath()
{
	return m_sWMTSCapabalitiesFile;
}

double Config::getDPI()
{
	return m_dbDPI;
}

