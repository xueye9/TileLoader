#ifndef CONFIG_H__
#define CONFIG_H__
#include <vector>
#include <string>

class Config
{
public:

	enum ResourceType
	{
		/// 谷歌街道
		googlemap_street=0,

		/// 谷歌卫星
		googlemap_satellite,
	};

	enum OutPutType
	{
		unknown = -1,
		arcgis_compact, // bundle
		explode, // 松散文件

	};

	static Config* Instance();

	Config();
	~Config();

	const std::string& getResourceUrl() const;

	int setSlaveCount();
	int getSlaveCount() const;

	bool setBBox(double left, double bottom, double right, double top);
	bool getBBox(double& left, double& bottom, double& right, double& top) const;

	std::string getOutDir() const;
	OutPutType getOutDataType() const;

	std::vector<int> getLevels() const;

	ResourceType getResourceType();

	bool load(const std::string& sfile);

	bool _loadOrigin(void* p);
	bool _loadOut(void* p);

	std::string getWMTSCapabilitiesFilePath();

	double getDPI();

	std::string getSuffix() const
	{
		return m_sOutFileSuffix;
	}

	std::string getEncoding()
	{
		return m_sEncoding;
	}

	const std::string& getTileMatrixSet() const
	{
		return m_sTileMatrixSet;
	}

	int getSRID() const
	{
		return m_nSrid;
	}

	std::string getProtocol() const
	{
		return m_sProtocol;
	}

	int getLowerLimit() const
	{
		return m_nLowerLimit;
	}

	int getUpperLimit() const
	{
		return m_nUpperLimit;
	}
private:
	// 初始化输出目录
	void _iniOutDir();

private:
	static Config* _ins;

	int m_nSlavesCount;

	double m_dbLeft, m_dbBottom, m_dbRight, m_dbTop;

	std::vector<int> m_vLevels;

	int m_nSrid;

	std::string m_sUrl;

	std::string m_sOutDir;

	// 资源类型
	std::string m_sType;

	// wmts元数据文件
	std::string m_sWMTSCapabalitiesFile;

	double m_dbDPI;

	// 输出文件后缀名
	std::string m_sOutFileSuffix;

	std::string m_sEncoding;

	// 访问互联网协议类型
	std::string m_sProtocol;

	std::string m_sTileMatrixSet;

	// 输出文件类型 arcigs:compact, explode
	std::string m_sOutputType;

	int m_nLowerLimit;
	int m_nUpperLimit;
};




#endif // config_h__
