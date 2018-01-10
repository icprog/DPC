/**
  * @file   mosaicdata.h
  * @brief  构造雷达组网的栅格图层用到的数据实体.
  *
  *  这里是详细说明。
  * @author Fanghua Guo
  * @par   Copyright (c):
  *          Fanghua Guo.
  *         All Rights Reserved
  * @date   2016:09:29
  * @note   这里是注意事项。
  * @version <版本号>
  */
#ifndef mosaicdata_h_
#define mosaicdata_h_

#include "commontypedef.h"
#include "qgscoordinatereferencesystem.h"
#include "Common/PPIStruct.h"
#include <QImage>
#include "qgsrectangle.h"
#include <QSharedPointer>


class MosaicData
{
public:
    MosaicData();
    ~MosaicData();

    PRODUCTRGBCOLOR ProductColor;		// 保存原始图例数据
    PRODUCTRGBCOLOR CurrentProductColor;// 当前使用的图例数据
    PRODUCTRGBCOLOR BeforeBlinkColor;	// 闪烁之前使用的图例数据
    PRODUCTCODE ProductCode;

    QImage *ProductImage;				// 正常产品图
    QImage *BlinkImage;					// 闪烁掏空之后的产品图

    // 组网产品数据格式
    MOSAICDATAHEADER MosaicHeader;
    std::vector<RADARSITEINFO>  SiteInfo;
    std::vector<short> PosData;
    std::vector<unsigned char> Data;

    // 雷达中心经纬度
    double CentralLon;
    double CentralLat;

    //雷达产品图的宽度和高度
    int ProductWidth;
    int ProductHeight;

    // 产品图例信息图片的宽度和高度
    int ImgWidth;
    int ImgHeight;

    // 字体像素
    int FontSize;
    // 点与点之间的像素间距
    int BufSize;

    QString StrUri;	// 产品文件全路径
    int LevelIndex;	// 产品高度层索引
    QString Proj4String;
    QSharedPointer<QgsCoordinateReferenceSystem> Crs;

    int Redraw; // 0:表示不重绘, 1:表示正常重绘, 2:表示闪烁重绘

    int Blink; // -1:表示不是绘制闪烁图,0:绘制闪烁图，1:显示原图，2:显示闪烁图

    QgsRectangle OldViewExtent;
};

#endif