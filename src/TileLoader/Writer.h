#ifndef THP_BUNDLE_WRITER_H__
#define THP_BUNDLE_WRITER_H__

/// 标准c
#include <stdio.h>

/// 标准c++
#include <string>


#include "ParamDef.h"

class Bundle;
/*!
 * \class BundleReader
 *
 * \brief ArcGis 瓦片服务压缩文件读取器
 * \detail bundle和bundlx必须在同一目录下，且索引文件名必须和bundle文件名相同
 * \author Baldwin
 * \date 六月 2015
 */
class Writer
{
public:

	enum
	{
		R_FAIL = -1,
		R_SUCCESS = 0,
	};
	Writer();
	~Writer();

	// 创建文件，有文件时清空文件内容
	int createBundle(const BundleID& tNo, const std::string& sDir);

	void close();

	int writePngToBundle(char* szPng, int nPngSize, const TileID& tTileID, std::string* pErrorMsg);

	std::string getFileName(const BundleID& tNo, const std::string& sDir) const;
private:
	/// 返回写入瓦片在 文件中的偏移位置		
	int _writeToBundle(char* szPng, int nPngSize, FILE* pFile, std::string* pErrorMsg);

	FILE* m_pBundle;
	FILE* m_pBundlx;

	BundleID m_tBundleID;
};


#endif // THP_BUNDLE_WRITER_H__
