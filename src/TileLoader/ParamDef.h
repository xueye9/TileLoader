#ifndef THP_WMTSPARAMDEF_H__
#define THP_WMTSPARAMDEF_H__

/// 最长路径长度
#define THP_MAX_PATH	512

/// 最大切片等级
#define THP_MAX_LEVEL	22

/// 默认图层LRU使用内存，单位MB
#define THP_WMTS_DEFAULT_MEM_OCCUPY		512//1024

/// 图层缓存大小，单位kb
#define THP_WMTS_DEFAULT_LRU_CACHE      1048576 // 1024*1024 1GB

/// 文件索引后缀名
#define THP_WMTS_BUNDLE_EXIST_IDXFILE_POSFIX	".bdi"

//localhost:9092/WMTS?service=WMTS&request=GetTile&version=1.0.0&
//layer=img&style=default&format=tiles&TileMatrixSet=c&TileMatrix=3&TileRow=2&TileCol=2
#define WMTS_SERVICE		"SERVICE"
#define WMTS_REQUEST		"REQUEST"
#define WMTS_VERSION		"VERSION"
#define WMTS_LAYER			"LAYER"
#define WMTS_LAYER_STYLE	"STYLE"
#define WMTS_TILE_FORMAT	"FORMAT"
#define WMTS_TILEMATRIXSET	"TILEMATRIXSET"
#define WMTS_TILEMATRIX		"TILEMATRIX"
#define WMTS_TILEROW		"TILEROW"
#define WMTS_TILECOL		"TILECOL"

#define WMTS_SERVICE_VALUE				"WMTS"

#define WMTS_REQUEST_VL_GETTILE			"GetTile"
#define WMTS_REQUEST_VL_CAPABILITIES	"Capabilities"

#define WMTS_VERSION_VL					"1.0.0"

// LRU最多的冷资源个数
#define LAYLRU_MAX_COLDBUNDLE_NUM		10 

#ifndef NULL
#define NULL 0
#endif// #ifndef NULL

/// 描述某等级的bundle存在性 
struct TileID
{
	/// 等级
	int level;

	/// 行号
	int row;

	/// 列号
	int col;
};

/// 束文件(后缀名为：bundle) 编号
struct BundleID
{
	/// 等级
	int nLevel;

	// bundle 所在等级的行列号可以通过unBunldeID反算，为了减少计算量添加两个变量，实际bundle编号
	// 不用行列编号参与
	/// bundle 所在等级的行号 
	int nBeginRow;

	/// bundle 所在等级的列号 
	int nBeginCol;
};


#endif // THP_WMTSPARAMDEF_H__
