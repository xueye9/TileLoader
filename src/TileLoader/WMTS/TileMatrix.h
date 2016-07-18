/*!
 * \file TileMatrix.h
 * \author Baldwin
 * \brief 切片矩阵
 *
 * TODO: long description
 * \date 九月 2015
 * \copyright	Copyright (c) 2015, 北京国科恒通电气自动化科技有限公司\n
	All rights reserved.
 */

#ifndef TILEMATRIX_H__
#define TILEMATRIX_H__
#include <vector>
#include <string>
//#include <CGePoint2d.h>

// emulates mapbox::box2d
class bbox {
public:
	double minx;
	double miny;
	double maxx;
	double maxy;
	bbox(double _minx, double _miny, double _maxx, double _maxy) :
		minx(_minx),
		miny(_miny),
		maxx(_maxx),
		maxy(_maxy) { }

	double width() const {
		return maxx - minx;
	}

	double height() const {
		return maxy - miny;
	}
};

struct  point2d
{
	double x;
	double y;
};

//地球周长 
//#define   EarthGirth  (40075016.6856)

class TileMatrix
{
	enum
	{
		e_success = 0,
		e_fail,
		e_outofRange,
	};
public:
	TileMatrix(double dbDPI, double dbMetersPerUnit);

	// ID
	std::string m_qsIdentifier;

	// 地图比例尺分母
	double m_dbScaleDenominator;

	// 对应TileMatrixSet指定的CRS
	point2d m_ptTopLeftCorner;

	// 瓦片宽度，单位：像素
	int m_nTileWidth;

	// 瓦片高度，单位：像素
	int m_nTileHeight;

	// 切片矩阵宽度，单位：瓦片个数
	int m_nMatrixWidth;

	// 切片矩阵高度度，单位：瓦片个数
	int m_nMatrixHeight;

	// 完善内部其他的信息
	void improve();

	/// 定位坐标所在的瓦片行列号
	/// 坐标为 getSupportedCRS 下的坐标
	int location(double xCRS, double yCRS, int& nCol, int& nRow) const;

	// CRS 坐标获对应的像素坐标
	int CRS2PixelCoordinate(double xCRS, double yCRS, int& xPixel, int& yPixel) const;

	int getTileBBox(int nCol, int nRow, bbox& box);

private:

	TileMatrix();

	// DPI: 每英寸的像素数，1英寸 = 0.0254米
	// OGC标准DPI为 90.71（即采用0.028mm作为一个像素的物理宽度）
	// 天地图使用国家标准规定的96 DPI（见《电子地图规范》）
	double m_dbDPI_;

	// CRS 参考系下一个单位长度对应的米数
	double m_dbMetersPerUnit;

	// 对应TileMatrixSet指定的CRS
	point2d m_ptBottomRightCorner_;

	// 每个像素代表的CRS单位长度
	double m_dbPixelSpan_;

	// CRS (0.0,0.0)对应的像素坐标
	/// 如果把整个地球映射到一张地图上,中心点的像素座标
	/// 是中心点=经纬度(0,0)
	point2d m_ptBitmapOrigo;

	//地球周长
	//#define   EarthGirth  (40075016.6856)
};

#endif // TILEMATRIX_H__
