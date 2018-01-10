/**
  * @file   mosaicdataprovider.h
  * @brief  雷达组网栅格图层的构造类.
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
#ifndef mosaicdataprovider_h_
#define mosaicdataprovider_h_
#include "qgsrasterdataprovider.h"
#include "qgsrendercontext.h"
#include "mosaicdata.h"
#include <QSharedPointer>
#include <QString>


typedef QSharedPointer<MosaicData> CMosaicDataPtr;

class MosaicDataProvider : public QgsRasterDataProvider
{
public:
    MosaicDataProvider();
    MosaicDataProvider(const QString &uri);
    MosaicDataProvider(const MosaicDataProvider &other);

    ~MosaicDataProvider();

    virtual QgsCoordinateReferenceSystem crs();

    virtual QgsRectangle extent();

    virtual bool isValid();

    virtual QString name() const;

    virtual QString description() const;

    virtual QImage* draw( const QgsRectangle & viewExtent, int pixelWidth, int pixelHeight );

    //************************************
    // Method:    metadata
    // FullName:  RadarDataProvider::metadata
    // Access:    virtual public
    // Returns:   QT_NAMESPACE::QString
    // Qualifier: 用来获取当前图例范围 格式 图例矩形区域 坐上点位置+右下点位置
    //************************************
    virtual QString metadata();

    virtual QString lastErrorTitle();

    virtual QString lastError();

    virtual QgsRasterInterface * clone() const;

    virtual QGis::DataType dataType( int bandNo ) const;

    virtual QGis::DataType srcDataType( int bandNo ) const;

    virtual int bandCount() const;

    virtual QgsRasterBlock * block(int theBandNo, const QgsRectangle &theExtent, int theWidth, int theHeight, QgsRenderContext *renderCxt = NULL);

    //************************************
    // Method:    write
    // FullName:  RadarDataProvider::write
    // Access:    virtual public
    // Returns:   bool
    // Qualifier: 重写write方法，用来传递当前产品区域的窗体的大小
    //			  (使用第三个参数和第四个参数传递图层信息区域的大小)
    //			  此方法第二用途：用来进行图像处理，通过第二个参数band，接受外界的各类图像处理指令
    // Parameter: void * data
    // Parameter: int band 当此方法用户图像处理时，指令说明如下
    //			  band = 10000 指示图像恢复默认值
    //			  *******滤色功能*************,此时xOffset,yOffset 表示鼠标对于整个LegendGraphic的相对位置
    //			  band = 20001 指示图像向上滤色
    //			  band = 20002 指示图像向下滤色
    //			  band = 20003 滤色重置
    //			  ********图像合并功能********,此时xOffset,yOffset 表示鼠标对于整个LegendGraphic的相对位置
    //			  band = 30001 指示图像向上合并
    //			  band = 30002 指示图像向下合并
    //			  band = 30003 合并重置
    //            *******闪烁功能预留**********
    // Parameter: int width	当前窗体的宽度
    // Parameter: int height 当前窗体的高度
    // Parameter: int xOffset
    // Parameter: int yOffset
    //************************************
    virtual bool write (void *data, int band, int width, int height, int xOffset, int yOffset);

    // 图例
    virtual QImage getLegendGraphic( double scale = 0.0, bool forceRefresh = false , const QgsRectangle * visibleExtent = 0 );


    //************************************
    // Method:    fromWindowToColorBar
    // FullName:  RadarDataProvider::fromWindowToColorBar
    // Access:    private
    // Returns:   int
    // Qualifier:  根据坐标颜色图例的上位置定位色标矩形
    // Parameter: int Xoffset
    // Parameter: int Yoffset
    //************************************
    int fromWindowToColorBar(int Xoffset, int Yoffset);

    float getValueByPosition(int x,int y,int w,int h);

    bool getValueSum(void *data, int count);

    //************************************
    // Method:    FlatToEarthCoordinate
    // FullName:  RadarDataProvider::FlatToEarthCoordinate
    // Access:    private
    // Returns:   void
    // Qualifier: 根据雷达站东经，北纬，仰角，方位角，实际距离 计算指定点经纬度
    // Parameter: double e0
    // Parameter: double n0
    // Parameter: double el
    // Parameter: double az
    // Parameter: double r
    // Parameter: float & Lon
    // Parameter: float & Lat
    //************************************
    void FlatToEarthCoordinate(double e0, double n0, double el, double az, double r,float &Lon,float &Lat);

    //************************************
    // Method:    verTopolar
    // FullName:  RadarDataProvider::verTopolar
    // Access:    private
    // Returns:   void
    // Qualifier:直角坐标转极坐标
    // Parameter: double x
    // Parameter: double y
    // Parameter: double * r
    // Parameter: double * az
    //************************************
    void verTopolar( double x, double y, double *r, double *az );

    //************************************
    // Method:    drawInteractColorBar
    // FullName:  RadarDataProvider::drawInteractColorBar
    // Access:    private
    // Returns:   void
    // Qualifier:  绘制交互产品图例
    // Parameter: QImage & colorImage
    //************************************
    void drawInteractColorBar(QImage &colorImage);

private:
    bool parseMosaicFile();

    PRODUCTCODE getProductCodeInfoByEnName(QString name);

    //************************************
    // Method:    drawProduct
    // FullName:  MosaicDataProvider::drawProduct
    // Access:    private
    // Returns:   void
    // Qualifier: 绘制组网产品
    // Parameter: QImage & image
    // Parameter: const QgsRectangle & viewExtent
    // Parameter: int theWidth
    // Parameter: int theHeight
    //************************************
    void drawProduct(QImage &image, const QgsRectangle &viewExtent, int theWidth, int theHeight);

    /**
     * @brief: 绘制站号
     * @param: QImage & image
     * @param: const QgsRectangle & viewExtent
     * @param: int theWidth
     * @param: int theHeight
     * @return: void: 描述返回值
    */
    void drawStationNumber(QImage &image, const QgsRectangle &viewExtent, int theWidth, int theHeight);

    //************************************
    // Method:    drawColorBar
    // FullName:  RadarDataProvider::drawColorBar
    // Access:    private
    // Returns:   void
    // Qualifier: 绘制雷达图例条
    // Parameter: QImage & colorImage
    //			  flag=1,白背景黑字，flag=2，黑背景白字
    //************************************
    void drawColorBar(QImage &colorImage, int flag);

    bool loadColor(const QString &colorName);

    QRgb getColorByValue(float value);

    //转世界时为北京时
    void convertGCTToBJT(int year, int month, int day, int hour, int minute, int second, QString &date, QString &time);

    //生成时间
    void generateDateTime(int year, int month, int day, int hour, int minute, int second, QString &date, QString &time);

    void getLonLatFromPoint(float lon0, float lat0, float posx, float posy, float &lon, float &lat);

    //************************************
    // Method:    insertSrsRecordToQgisDb
    // FullName:  RadarDataProvider::insertSrsRecordToQgisDb
    // Access:    private
    // Returns:   bool--在qgis.db表中查询到记录，或者插入记录成功时返回true，否则返回false
    // Qualifier:
    // Parameter: const QString & proj4Str
    //************************************
    bool insertSrsRecordToQgisDb(const QString &proj4Str);

    //************************************
    // Method:    transformExtent
    // FullName:  RadarDataProvider::transformExtent
    // Access:    private
    // Returns:   QgsRectangle
    // Qualifier: 根据原坐标系和目的坐标系，转换矩形范围
    // Parameter: QgsRenderContext * renderCxt
    // Parameter: const QgsRectangle & rect
    //************************************
    QgsRectangle transformExtent(QgsRenderContext *renderCxt, const QgsRectangle &rect);

    //判断数据是否合法,true表示合法，false表示不合法
    bool isValidData(int a, int low, int up);

private:
    QList<PRODUCTRGBCOLOR> m_productColorLst;// 颜色值
    QList<PRODUCTCODE> m_productCodeLst; // 产品名称色标
    CMosaicDataPtr m_mosaicData;

    // 图例矩形条范围
    QRect m_legendRect;
    //当前产品的高度层
    int m_height;

};


#endif //mosaicdataprovider_h_
