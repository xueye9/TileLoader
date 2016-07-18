#include "TileMatrix.h"


TileMatrix::TileMatrix(double dbDPI, double dbMetersPerUnit)
{
	m_dbDPI_ = dbDPI;
	m_dbMetersPerUnit = dbMetersPerUnit;
}

void TileMatrix::improve()
{
	// 一个像素表示CRS单位长度
	m_dbPixelSpan_ = m_dbScaleDenominator * 0.02540 / m_dbDPI_ / m_dbMetersPerUnit;

	double dbTileSpanX = m_nTileWidth * m_dbPixelSpan_;
	double dbTileSpanY = m_nTileHeight * m_dbPixelSpan_;

	m_ptBottomRightCorner_.x = m_ptTopLeftCorner.x + (dbTileSpanX * m_nMatrixWidth);
	m_ptBottomRightCorner_.y = m_ptTopLeftCorner.y - (dbTileSpanY * m_nMatrixHeight);

	// 计算crs(0, 0) 点对应的图片像素坐标
	// crs的左上角点对应像素坐标(0, 0)
	double dbTemp = abs(m_ptTopLeftCorner.x / m_dbPixelSpan_) + 0.5;
	m_ptBitmapOrigo.x = floor( dbTemp );

	dbTemp = abs(m_ptTopLeftCorner.y / m_dbPixelSpan_) + 0.5;
	m_ptBitmapOrigo.y = floor( dbTemp );
}

// 四舍五入 负值按绝对值四舍五入后加符号
int approach(double dbVal)
{
	if(dbVal < 0)
	{
		return (int)floor( dbVal - 0.5 );
	}
	else
	{
		return (int)floor(dbVal + 0.5 );
	}
}

int TileMatrix::location(double xCRS, double yCRS, int& nCol, int& nRow) const
{
	// 检查坐标范围
	if( xCRS < m_ptTopLeftCorner.x || xCRS > m_ptBottomRightCorner_.x ||	
		yCRS < m_ptBottomRightCorner_.y || yCRS > m_ptTopLeftCorner.y)
		return e_outofRange;

	// 计算坐标距crs(0,0)的距离
	int dbXPixelSpan = approach( (xCRS / m_dbPixelSpan_) );
	int dbYPixelSpan = approach( (yCRS / m_dbPixelSpan_) );

	// 计算CRS(xCRS, yCRS) 对应的像素坐标
	// 四舍五入
	int nXPixel = int(m_ptBitmapOrigo.x) + dbXPixelSpan;

	// 因为像素坐标系和CRS坐标系的Y轴方向相反，像素坐标系Y轴向下为正方向，CRS坐标系Y轴向上为正方向
	int nYPixel = int(m_ptBitmapOrigo.y) - dbYPixelSpan;

	nCol = nXPixel / m_nTileWidth ;
	nRow = nYPixel / m_nTileHeight;

	return e_success;
}

int TileMatrix::CRS2PixelCoordinate(double xCRS, double yCRS, int& xPixel, int& yPixel) const
{
	// 检查坐标范围
	if( xCRS < m_ptTopLeftCorner.x || xCRS > m_ptBottomRightCorner_.x ||	
		yCRS < m_ptBottomRightCorner_.y || yCRS > m_ptTopLeftCorner.y)
		return e_outofRange;

	// 计算坐标距crs(0,0)的距离
	int dbXPixelSpan = approach( (xCRS / m_dbPixelSpan_) );
	int dbYPixelSpan = approach( (yCRS / m_dbPixelSpan_) );

	// 计算CRS(xCRS, yCRS) 对应的像素坐标
	// 四舍五入
	xPixel = int(m_ptBitmapOrigo.x) + dbXPixelSpan;

	// 因为像素坐标系和CRS坐标系的Y轴方向相反，像素坐标系Y轴向下为正方向，CRS坐标系Y轴向上为正方向
	yPixel = int(m_ptBitmapOrigo.y) - dbYPixelSpan;

	return e_success;
}

int TileMatrix::getTileBBox(int nCol, int nRow, bbox& box)
{
	// x轴瓦片地理长度
	double dbTileSpanX = m_nTileWidth * m_dbPixelSpan_;

	// y轴瓦片地理长度
	double dbTileSpanY = m_nTileHeight * m_dbPixelSpan_;

	box.minx = m_ptTopLeftCorner.x + nCol * dbTileSpanX;
	box.maxx = box.minx + dbTileSpanX;


	box.maxy = m_ptTopLeftCorner.y - nRow * dbTileSpanY;
	box.miny = m_ptTopLeftCorner.y + dbTileSpanY;
	
	return e_success;
}
