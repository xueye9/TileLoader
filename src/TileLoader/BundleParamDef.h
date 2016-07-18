#ifndef BUNDLEPARAMDEF_H__
#define BUNDLEPARAMDEF_H__

#define BUNDLE_MAX_PATH		512

// bundlx 前后各有16个字节无用 怀疑存放了一些描述信息 = 80KB
#define BUNDLX_SIZE		81952 

// bundlx 前后各有16个字节无用 怀疑存放了一些描述信息
#define BUNDLE_TOTAL		81952
#define BUNDLX_DOM			16		
#define BUNDLX_DOMX2		32
#define BUNDLX_CONTENT_SIZE 81920
#define BUNDLX_NODE_SIZE	5

// 128 * 128 个
#define BUNDLE_EDGE				128
#define BUNDLE_MAX_TILE_NUM		16384
// bundle文件中记录Tile占用内存字节数的字节长度
#define BUNDLE_TILE_SIZE	4

// bundle 文件头大小
#define BUNDLE_HEADER_SIZE	60

// bundle 描述含有瓦片数量位置（字节偏移量)
#define BUNDLE_POS_TILECOUNT 0x10

// bundle 描述文件大小（字节数)数据的字节偏移
#define BUNDLE_POS_FILESIZE  0x18

// bundle 描述包含瓦片数量在用(字节数)
#define INT_SIZE 4

// 记录瓦片起始行在文件中的偏移量
#define BUNDLE_POS_BEGINROW  0x2c

// 记录瓦片起始列在文件中的偏移量
#define BUNDLE_POS_BEGINCOL  0x34

// 记录瓦片结束行在文件中的偏移量
#define BUNDLE_POS_ENDROW	0x30

// 记录瓦片结束列在文件中的偏移量
#define BUNDLE_POS_ENDCOL  0x38






#endif // BundleParamDef_h__
