#include "mosaicdataprovider.h"
#include "ctkLog.h"
#include <QApplication>
#include <QFile>
#include <QPainter>
#include <QSqlQuery>
#include <QSqlError>
#include <QMutex>
#include <math.h>
#include "meteogisexception.h"
#include "ctkPublicFun.h"


#ifdef WITH_MEM_CHECK
#include "vld.h"
#endif

#define INVALID_COLOR -255

MosaicDataProvider::MosaicDataProvider()
{
    m_mosaicData = CMosaicDataPtr(new MosaicData());
}

MosaicDataProvider::MosaicDataProvider(const QString &uri)
    : QgsRasterDataProvider(uri)
{
    QSLOG_INFO(QString::fromLocal8Bit("构造雷达的DataProvider！"));
    if (!readProductColor(m_productColorLst))
    {
        throw MeteoGisException(QString::fromLocal8Bit("读色标文件失败！"));
    }

    if (!readProductCode("MOSAIC", m_productCodeLst))
    {
        throw MeteoGisException(QString::fromLocal8Bit("读名称色标文件失败！"));
    }

    m_mosaicData = CMosaicDataPtr(new MosaicData());
    QStringList uirLst = uri.split(";");
    if (uirLst.count() != 2)
    {
        throw MeteoGisException(QString::fromLocal8Bit("传入参数格式必须为[文件全路径;高度层值（单位米）]！"));
    }
    m_mosaicData->StrUri = uirLst[0];
    m_height = uirLst[1].toInt();

    if(!parseMosaicFile())
    {
        throw MeteoGisException(QString::fromLocal8Bit("解析组网产品文件失败！"));
    }


}

MosaicDataProvider::MosaicDataProvider( const MosaicDataProvider &other )
{
    this->m_mosaicData = other.m_mosaicData;
    this->mExtent = other.mExtent;
}

MosaicDataProvider::~MosaicDataProvider()
{

}

QgsCoordinateReferenceSystem MosaicDataProvider::crs()
{
    //return *m_mosaicData->Crs;
    return QgsCoordinateReferenceSystem();
}

QgsRectangle MosaicDataProvider::extent()
{
    return mExtent;
}

bool MosaicDataProvider::isValid()
{
    return true;
}

QString MosaicDataProvider::name() const
{
    return QString("mosaic");
}

QString MosaicDataProvider::description() const
{
    //return QString("mosaic data provider");

    QDateTime gct_dt(QDate(m_mosaicData->MosaicHeader.iStYear, m_mosaicData->MosaicHeader.iStMonth, m_mosaicData->MosaicHeader.iStDay),
                     QTime(m_mosaicData->MosaicHeader.iStHour, m_mosaicData->MosaicHeader.iStMinute, m_mosaicData->MosaicHeader.iStSec));



    QString str = gct_dt.toString(QString::fromLocal8Bit("yyyy年MM月dd日hh时mm分 "));
    str += m_mosaicData->ProductCode.ProductZhName;
    if (m_height > 0)
    {
        str += QString::fromLocal8Bit(" 高度%1米").arg(m_height);
    }
    return str;
}

QImage* MosaicDataProvider::draw( const QgsRectangle & viewExtent, int pixelWidth, int pixelHeight )
{
    return 0;
}

QString MosaicDataProvider::metadata()
{
    return QString("%1_%2_%3_%4").arg(m_legendRect.x()).arg(m_legendRect.y()).arg(m_legendRect.width()).arg(m_legendRect.height());
}

QString MosaicDataProvider::lastErrorTitle()
{
    return QString();
}

QString MosaicDataProvider::lastError()
{
    return QString();
}

QgsRasterInterface * MosaicDataProvider::clone() const
{
    return new MosaicDataProvider(*this);
}

QGis::DataType MosaicDataProvider::dataType( int bandNo ) const
{
    return QGis::DataType::ARGB32;
}

QGis::DataType MosaicDataProvider::srcDataType( int bandNo ) const
{
    return QGis::DataType::ARGB32;
}

int MosaicDataProvider::bandCount() const
{
    return 1;
}

QgsRasterBlock * MosaicDataProvider::block( int theBandNo, const QgsRectangle &theExtent, int theWidth, int theHeight, QgsRenderContext *renderCxt )
{
    QgsRectangle viewRect = transformExtent(renderCxt, theExtent);
    mExtent = transformExtent(renderCxt, mExtent);

    QSLOG_INFO(QString::fromLocal8Bit("雷达产品当前视窗像素宽度:%1").arg(theWidth));
    QSLOG_INFO(QString::fromLocal8Bit("雷达产品当前视窗像素高度:%1").arg(theHeight));
    QSLOG_INFO(QString::fromLocal8Bit("雷达产品当前视窗范围:（%1,%2,%3,%4）").arg(viewRect.xMinimum()).arg(viewRect.yMinimum()).arg(viewRect.xMaximum()).arg(viewRect.yMaximum()));
    QSLOG_INFO(QString::fromLocal8Bit("雷达产品当前产品范围:（%1,%2,%3,%4）").arg(mExtent.xMinimum()).arg(mExtent.yMinimum()).arg(mExtent.xMaximum()).arg(mExtent.yMaximum()));

    // 主要是设置QgsRasterBlock中的QImage对象
    QgsRasterBlock *t_pBlock = new QgsRasterBlock(QGis::DataType::ARGB32, theWidth, theHeight);

    if ((m_mosaicData->ProductWidth == theWidth)
            && (m_mosaicData->ProductHeight == theHeight)
            && ( m_mosaicData->Redraw < 0))
    {
        // 仅仅是拖拽画布时，需要重绘
        if (m_mosaicData->ProductImage != NULL)
        {
            delete m_mosaicData->ProductImage;
            m_mosaicData->ProductImage = NULL;
        }
        m_mosaicData->ProductImage = new QImage(m_mosaicData->ProductWidth, m_mosaicData->ProductHeight, QImage::Format::Format_ARGB32);
        m_mosaicData->ProductImage->fill(Qt::transparent);
        drawProduct(*m_mosaicData->ProductImage, viewRect, m_mosaicData->ProductWidth, m_mosaicData->ProductHeight);
    }
    else
    {
        // 闪烁操作时，视窗大小发生改变时，需要重新绘制没有挖空的图像
        if ((m_mosaicData->Redraw == 2))
        {
            if ((m_mosaicData->OldViewExtent != theExtent)
                    || (m_mosaicData->ProductWidth != theWidth)
                    || (m_mosaicData->ProductHeight != theHeight))
            {

                QSLOG_INFO(QString::fromLocal8Bit("雷达产品视窗大小发生改变-----"));
                m_mosaicData->ProductWidth = theWidth;
                m_mosaicData->ProductHeight = theHeight;
                m_mosaicData->OldViewExtent = theExtent;
                m_mosaicData->Blink = 0;
                // 绘制雷达产品
                if (m_mosaicData->ProductImage != NULL)
                {
                    delete m_mosaicData->ProductImage;
                    m_mosaicData->ProductImage = NULL;
                }
                if (m_mosaicData->BlinkImage != NULL)
                {
                    delete m_mosaicData->BlinkImage;
                    m_mosaicData->BlinkImage = NULL;
                }
                PRODUCTRGBCOLOR tmpColor = m_mosaicData->CurrentProductColor;
                m_mosaicData->CurrentProductColor = m_mosaicData->BeforeBlinkColor;

                // 用上次滤色操作之后的色标，绘制闪烁没挖空的图像
                m_mosaicData->BlinkImage = new QImage(m_mosaicData->ProductWidth, m_mosaicData->ProductHeight, QImage::Format::Format_ARGB32);
                m_mosaicData->BlinkImage->fill(Qt::transparent);
                drawProduct(*m_mosaicData->BlinkImage, viewRect,m_mosaicData->ProductWidth, m_mosaicData->ProductHeight);
                m_mosaicData->CurrentProductColor = tmpColor;
            }
        }

        // 窗口尺寸发生变化时 或 通过write写入图像处理时重绘产品图片
        if ((m_mosaicData->ProductWidth != theWidth)
                || (m_mosaicData->ProductHeight != theHeight)
                || ( m_mosaicData->Redraw > 0))
        {
            m_mosaicData->ProductWidth = theWidth;
            m_mosaicData->ProductHeight = theHeight;

            // 绘制新色标的产品图（对于闪烁操作是闪烁挖空的图像）
            if (m_mosaicData->Blink <= 0)
            {
                QSLOG_INFO(QString::fromLocal8Bit("开始重绘雷达产品..."));

                // 闪烁但视窗没有发生改变时，把原来的产品图保存到BlinkImage中
                if (m_mosaicData->ProductImage != NULL)
                {
                    if (m_mosaicData->Redraw == 2)
                    {
                        *m_mosaicData->BlinkImage = *m_mosaicData->ProductImage;
                    }

                    delete m_mosaicData->ProductImage;
                    m_mosaicData->ProductImage = NULL;
                }
                m_mosaicData->ProductImage = new QImage(m_mosaicData->ProductWidth, m_mosaicData->ProductHeight, QImage::Format::Format_ARGB32);
                m_mosaicData->ProductImage->fill(Qt::transparent);
                drawProduct(*m_mosaicData->ProductImage, viewRect, m_mosaicData->ProductWidth, m_mosaicData->ProductHeight);

                QSLOG_INFO(QString::fromLocal8Bit("重绘雷达产品结束！"));
            }

            // 闪烁
            if (m_mosaicData->Redraw == 2)
            {
                if (m_mosaicData->Blink > 0)
                {
                    m_mosaicData->Blink = ( m_mosaicData->Blink == 1 ?  2:1);
                    if (m_mosaicData->Blink == 2)
                    {
                        t_pBlock->setImage(m_mosaicData->BlinkImage);
                        //QSLOG_INFO(QString("-------------BlinkImage width:%1,height%2").arg(m_mosaicData->BlinkImage->width()).arg(m_mosaicData->BlinkImage->height()));
                    }
                    else
                    {
                        t_pBlock->setImage(m_mosaicData->ProductImage);
                        //QSLOG_INFO(QString("-------------ProductImage width:%1,height%2").arg(m_mosaicData->ProductImage->width()).arg(m_mosaicData->ProductImage->height()));
                    }

                    return t_pBlock;
                }
                else
                {
                    m_mosaicData->Blink = 1;

                }
            }
        }
    }

    t_pBlock->setImage(m_mosaicData->ProductImage);
    return t_pBlock;
}

void MosaicDataProvider::drawProduct(QImage &image, const QgsRectangle &viewExtent, int theWidth, int theHeight)
{

    QSLOG_INFO(QString::fromLocal8Bit("正在绘制的产品文件为[%1]").arg(m_mosaicData->StrUri));

    int xLen = m_mosaicData->MosaicHeader.iLonGrids;
    int yLen = m_mosaicData->MosaicHeader.iLatGrids;

    QSLOG_INFO(QString::fromLocal8Bit("格点数据水平方向格点数：%1").arg(xLen));
    QSLOG_INFO(QString::fromLocal8Bit("格点数据竖直方向格点数：%1").arg(yLen));

    // 绘图经纬度格点数
    double lonGrids = viewExtent.width() * xLen / mExtent.width();
    double latGrids = viewExtent.height() * yLen / mExtent.height();
    QSLOG_INFO(QString::fromLocal8Bit("格点数据经度条数：%1").arg(lonGrids));
    QSLOG_INFO(QString::fromLocal8Bit("格点数据纬度条数：%1").arg(latGrids));

    // 绘图经纬度格点所占像素
    double xScale = theWidth * 1.0 / lonGrids;
    double yScale = theHeight * 1.0 / latGrids;

    // 绘图起止经纬度索引
    double lonStartIndex = qAbs(viewExtent.xMinimum() - mExtent.xMinimum())* xLen / mExtent.width();
    double latStartIndex = qAbs(viewExtent.yMaximum() - mExtent.yMaximum())* yLen / mExtent.height();
    double lonEndIndex = xLen - qAbs((mExtent.xMaximum() - viewExtent.xMaximum())* xLen / mExtent.width());
    double latEndIndex = yLen - qAbs((mExtent.yMinimum() - viewExtent.yMinimum())* yLen / mExtent.height());

    for (int j=0; j<theHeight; j++)
    {
        QRgb *pLine = (QRgb *) image.scanLine(j);

        int y = qRound(j / yScale + latStartIndex);
        for (int i=0; i<theWidth; i++)
        {
            int x = qRound(i / xScale + lonStartIndex);
            if(x<(int)lonStartIndex || y<(int)latStartIndex || x>(int)lonEndIndex || y>(int)latEndIndex)
            {
                continue;
            }

            // 获取m_mosaicData->LevelIndex所在高度层的数据索引
            int index = -1;
            if (m_mosaicData->LevelIndex > -1 && m_mosaicData->LevelIndex < m_mosaicData->MosaicHeader.iLevels)
            {
                index = m_mosaicData->LevelIndex * xLen * yLen + (yLen - y - 1) * xLen + x;
                if (index >= m_mosaicData->MosaicHeader.iLevels * xLen * yLen)
                {
                    continue;
                }
            }
            else if(m_mosaicData->MosaicHeader.iLevels == 0)
            {
                index = (yLen - y - 1) * xLen + x;
                if (index >= xLen*yLen)
                {
                    continue;
                }
            }

            if (index > -1)
            {
                unsigned char v = m_mosaicData->Data[index];

                if (v != CODE_INVALID && v != CODE_NO_OBSERV)
                {
                    float val = (v + m_mosaicData->MosaicHeader.addV) * float(m_mosaicData->MosaicHeader.mulV) * 0.1;
                    // float val = (v - 64) * 0.5;
                    pLine[i] = getColorByValue(val);
                }
            }
        }
    }

    // 绘制站号
    //drawStationNumber(image, viewExtent, theWidth, theHeight);
}

void MosaicDataProvider::drawStationNumber(QImage &image, const QgsRectangle &viewExtent, int theWidth, int theHeight)
{
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QFont font("微软雅黑", m_mosaicData->FontSize);
    painter.setFont(font);

    QColor fontColor = Qt::red;
    QPen pen(fontColor);
    QBrush bush(Qt::red);
    pen.setWidthF(2);
    painter.setPen(pen);
    painter.setBrush(bush);

    // 经纬度像素比
    double xRes = viewExtent.width() / theWidth;
    double yRes = viewExtent.height() / theHeight;

    for (int i=0; i<m_mosaicData->SiteInfo.size(); i++)
    {
        double lon = m_mosaicData->SiteInfo[i].SiteLon/1000.0;
        double lat = m_mosaicData->SiteInfo[i].SiteLat/1000.0;
        double x = (lon - viewExtent.xMinimum())/xRes;
        double y = (viewExtent.yMaximum() - lat)/yRes;
        painter.drawEllipse(QPointF(x, y), 1, 1);
        painter.drawText(QPointF(x - 5, y - 5), m_mosaicData->SiteInfo[i].SiteName);
    }
}

bool MosaicDataProvider::isValidData(int a, int low, int up)
{
    if(a<low || a>up)
    {
        return false;
    }

    return true;
}

bool MosaicDataProvider::loadColor(const QString &colorName)
{
    foreach(const PRODUCTRGBCOLOR &pclr, m_productColorLst)
    {
        if (colorName == pclr.name)
        {
            m_mosaicData->ProductColor = pclr;
            m_mosaicData->CurrentProductColor = pclr;
            m_mosaicData->BeforeBlinkColor = pclr;
            return true;
        }
    }

    QSLOG_ERROR(QString::fromLocal8Bit("未找到色标[%1]").arg(colorName));
    return false;
}

QRgb MosaicDataProvider::getColorByValue(float value)
{
    QVector<RGBCOLOR> &colorLst = m_mosaicData->CurrentProductColor.rgbColorLst;

    if (value < colorLst.first().value)
    {
        return qRgba(0, 0, 0, 0);
    }

    if (value >= colorLst.last().value)
    {
        return qRgba(colorLst.last().r,colorLst.last().g,colorLst.last().b, 255);
    }

    for (int i = 0; i < (int)colorLst.size() -1; i++)
    {
        if ((value >= colorLst[i].value) && (value <colorLst[i+1].value))
        {
            // 在进行图像滤色、闪烁的时候，做透明处理
            if((colorLst[i].r == INVALID_COLOR)
                    && (colorLst[i].g == INVALID_COLOR)
                    && (colorLst[i].b == INVALID_COLOR))
            {
                //QSLOG_INFO(QString("==== %1,%2,%3,%4").arg(colorLst[i].r).arg(colorLst[i].g).arg(colorLst[i].b).arg(colorLst[i].realValue));
                return qRgba(0, 0, 0, 0);
            }

            return qRgba(colorLst[i].r, colorLst[i].g, colorLst[i].b, 255);
        }
    }
    return qRgba(0, 0, 0, 0);
}

bool MosaicDataProvider::parseMosaicFile()
{
    m_mosaicData->LevelIndex = -1;
    if (readMosaicFile(m_mosaicData->StrUri, m_mosaicData->MosaicHeader, m_mosaicData->SiteInfo, m_mosaicData->Data, m_mosaicData->PosData) < 0)
    {
        return false;
    }

    for(int i=0; i<(int)m_mosaicData->PosData.size(); i++)
    {
        if(m_mosaicData->PosData[i] == m_height)
        {
            m_mosaicData->LevelIndex = i;
        }
    }

    if (m_mosaicData->MosaicHeader.iLevels > 0 && m_mosaicData->LevelIndex < 0)
    {
        QSLOG_INFO(QString::fromLocal8Bit("传入的产品高度层不正确！"));
        return false;
    }

    m_mosaicData->ProductCode = getProductCodeInfoByEnName(m_mosaicData->MosaicHeader.strDataType);
    if (!loadColor(m_mosaicData->ProductCode.ColorName))
    {
        return false;
    }

    mExtent = QgsRectangle(m_mosaicData->MosaicHeader.iFstLon/1000., m_mosaicData->MosaicHeader.iFstLat/1000.,
                           m_mosaicData->MosaicHeader.iLstLon/1000., m_mosaicData->MosaicHeader.iLstLat/1000. );

    return true;
}

QImage MosaicDataProvider::getLegendGraphic( double scale, bool forceRefresh, const QgsRectangle * visibleExtent )
{
    if (!forceRefresh)
    {
        // 绘制图例条
        QImage colorImage(m_mosaicData->ImgWidth, m_mosaicData->ImgHeight, QImage::Format::Format_ARGB32);
        colorImage.fill(Qt::transparent);
        drawColorBar(colorImage, scale);
        return colorImage;
    }

    // 绘制交互产品图例条
    int colorImageWidth = 210;
    int colorImageHeight = 315;
    QImage colorImage(colorImageWidth, colorImageHeight, QImage::Format::Format_ARGB32);
    colorImage.fill(Qt::white);
    drawInteractColorBar(colorImage);

    return colorImage;
}


void MosaicDataProvider::drawColorBar(QImage &colorImage, int flag)
{
    QPainter painter(&colorImage);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    // 矩形个数
    int nRectNum = m_mosaicData->ProductColor.rgbColorLst.size();
    if (nRectNum == 0)
    {
        return;
    }

    QFont font("微软雅黑", m_mosaicData->FontSize);
    font.setBold(true);
    painter.setFont(font);

    // 设置字体
    if (flag ==1)
    {
        painter.setPen(Qt::black);
    }
    else
    {
        painter.setPen(Qt::white);
    }

    QBrush brush(Qt::transparent);

    // 矩形距图片上边缘距离
    int topOffset = m_mosaicData->FontSize;
    // 字体距图片上边缘距离
    int fontTopOffset = topOffset;

    // 矩形距离图片左边缘距离
    int leftOffset = 5;
    int space = 5;

    // 矩形宽度
    int nRectWidth = 2.8*m_mosaicData->FontSize;
    // 矩形高度
    int nRectHeight = (colorImage.height() - 3*space)/(nRectNum+1);

    QRect *colorRect = new QRect[nRectNum];
    int i = 0;
    foreach(const RGBCOLOR &rgbColor, m_mosaicData->ProductColor.rgbColorLst)
    {
        // QSLOG_INFO(QString("%1 %2 %3").arg(rgbColor.r).arg(rgbColor.g).arg(rgbColor.b));
        brush.setColor(QColor(rgbColor.r, rgbColor.g, rgbColor.b));
        colorRect[i].setLeft(leftOffset);
        colorRect[i].setWidth(nRectWidth);
        colorRect[i].setTop(topOffset + i*nRectHeight);
        colorRect[i].setHeight(nRectHeight);

        //ND时，色标矩形设为透明色
        if(rgbColor.realValue.compare("ND") != 0)
        {
            painter.fillRect(colorRect[i], brush);
        }
        painter.drawRect(colorRect[i]);
        painter.drawText(colorRect[i].right()+space, colorRect[i].top()+space, rgbColor.realValue);
        i++;
    }

    if (colorRect != NULL)
    {
        delete[] colorRect;
        colorRect = NULL;
    }

    m_legendRect = QRect(leftOffset, topOffset,nRectWidth, nRectHeight*nRectNum );

    int unitTopOffset = topOffset + nRectHeight*nRectNum + m_mosaicData->FontSize + 5;
    QString productUnit = getUnitByColorName(m_mosaicData->ProductCode.ColorName);
    painter.drawText(2*leftOffset, unitTopOffset, productUnit);
}

void MosaicDataProvider::convertGCTToBJT(int year, int month, int day, int hour, int minute, int second, QString &date, QString &time)
{
    QDateTime gct_dt(QDate(year, month, day), QTime(hour, minute, second));
    QDateTime bjt_dt = gct_dt.addSecs(8*60*60);
    date = bjt_dt.date().toString("yyyy-MM-dd");
    time = bjt_dt.time().toString("hh:mm:ss");
}

bool MosaicDataProvider::write( void *data, int band, int width, int height, int xOffset, int yOffset )
{
    //QSLOG_DEBUG(QString::fromLocal8Bit("执行MosaicDataProvider::write方法！"));
    if (band == 0)
    {
        m_mosaicData->ImgWidth = width;
        m_mosaicData->ImgHeight = height;
        return true;
    }

    //获得当前鼠标位置的dBZ值
    if (band == 10000)
    {
        double lon = xOffset*1.0 / width;
        double lat = yOffset*1.0 / width;
        //计算DBZ值
        int xLen = m_mosaicData->MosaicHeader.iLonGrids;
        int yLen = m_mosaicData->MosaicHeader.iLatGrids;
        if (mExtent.contains(QgsPoint(lon, lat)))
        {
            int x = (lon - mExtent.xMinimum())* xLen / mExtent.width();
            int y = (mExtent.yMaximum() - lat)* yLen / mExtent.height();

            y = yLen - y -1;

            int index = -1;
            if (m_mosaicData->LevelIndex > -1 && m_mosaicData->LevelIndex < m_mosaicData->MosaicHeader.iLevels)
            {
                index = m_mosaicData->LevelIndex * xLen * yLen + y * xLen + x;
                if (index >= m_mosaicData->MosaicHeader.iLevels * xLen * yLen)
                {
                    return false;
                }
            }
            else if(m_mosaicData->MosaicHeader.iLevels == 0)
            {
                index = y * xLen + x;
                if (index >= xLen*yLen)
                {
                    return false;
                }
            }

            if (index > -1)
            {
                unsigned char v = m_mosaicData->Data[index];

                if (v != CODE_INVALID && v != CODE_NO_OBSERV)
                {
                    float val = (v + m_mosaicData->MosaicHeader.addV) * float(m_mosaicData->MosaicHeader.mulV) * 0.1;
                    QString productUnit = getUnitByColorName(m_mosaicData->ProductCode.ColorName);
                    QString str = QString::fromLocal8Bit("经度：%1°\n纬度：%2°\n数值：%4").arg(QString::number(lon, 'f', 3)).
                                  arg(QString::number(lat, 'f', 3)).arg(val);
                    str += productUnit;
                    strcpy((char *)data, str.toStdString().c_str());

                    return true;
                }
            }
        }

        QString str = QString::fromLocal8Bit("经度：%1°\n纬度：%2°\n数值：NA").arg(QString::number(lon, 'f', 3)).
                      arg(QString::number(lat, 'f', 3));

        strcpy((char *)data, str.toStdString().c_str());
        return true;
    }


    // *******滤色功能*************,此时xOffset,yOffset 表示鼠标对于整个LegendGraphic的相对位置
    PRODUCTRGBCOLOR &tempProductColor = m_mosaicData->CurrentProductColor;
    int nRectNum = tempProductColor.rgbColorLst.size();
    // 指示图像向上滤色
    if (band == 20001)
    {
        int position = fromWindowToColorBar(xOffset, yOffset);
        for (int i = 0; i <= position ; i++)
        {
            tempProductColor.rgbColorLst[i].r = INVALID_COLOR;
            tempProductColor.rgbColorLst[i].g = INVALID_COLOR;
            tempProductColor.rgbColorLst[i].b = INVALID_COLOR;
        }

        m_mosaicData->BeforeBlinkColor = tempProductColor;
        m_mosaicData->Redraw = 1;
        m_mosaicData->Blink = -1;
    }
    //指示图像向下滤色
    else if(band == 20002)
    {
        int position = fromWindowToColorBar(xOffset, yOffset);
        for (int i = position; i < nRectNum ; i++)
        {
            tempProductColor.rgbColorLst[i].r = INVALID_COLOR;
            tempProductColor.rgbColorLst[i].g = INVALID_COLOR;
            tempProductColor.rgbColorLst[i].b = INVALID_COLOR;
        }

        m_mosaicData->BeforeBlinkColor = tempProductColor;
        m_mosaicData->Redraw = 1;
        m_mosaicData->Blink = -1;
    }
    //********图像合并功能********,此时xOffset,yOffset 表示鼠标对于整个LegendGraphic的相对位置
    // 指示图像向上合并
    else if (band == 30001)
    {
        int position = fromWindowToColorBar(xOffset, yOffset);
        for (int i = 0; i <= position ; i++)
        {
            tempProductColor.rgbColorLst[i].r = m_mosaicData->ProductColor.rgbColorLst[position].r;
            tempProductColor.rgbColorLst[i].g = m_mosaicData->ProductColor.rgbColorLst[position].g;
            tempProductColor.rgbColorLst[i].b = m_mosaicData->ProductColor.rgbColorLst[position].b;
        }

        m_mosaicData->BeforeBlinkColor = tempProductColor;
        m_mosaicData->Redraw = 1;
        m_mosaicData->Blink = -1;
    }
    // 指示图像向下合并
    else if(band == 30002)
    {
        int position = fromWindowToColorBar(xOffset, yOffset);
        for (int i = position; i < nRectNum ; i++)
        {
            tempProductColor.rgbColorLst[i].r = m_mosaicData->ProductColor.rgbColorLst[position].r;
            tempProductColor.rgbColorLst[i].g = m_mosaicData->ProductColor.rgbColorLst[position].g;
            tempProductColor.rgbColorLst[i].b = m_mosaicData->ProductColor.rgbColorLst[position].b;
        }

        m_mosaicData->BeforeBlinkColor = tempProductColor;
        m_mosaicData->Redraw = 1;
        m_mosaicData->Blink = -1;
    }
    // 恢复图像
    else if (band == 30003)
    {
        tempProductColor = m_mosaicData->ProductColor;
        m_mosaicData->BeforeBlinkColor = tempProductColor;
        m_mosaicData->Redraw = 1;
        m_mosaicData->Blink = -1;
    }
    // 闪烁
    else if (band == 30004)
    {
        if(width == -11111)
        {
            tempProductColor = m_mosaicData->BeforeBlinkColor;
        }
        else
        {
            m_mosaicData->BeforeBlinkColor = tempProductColor;
            int position = fromWindowToColorBar(xOffset, yOffset);

            tempProductColor.rgbColorLst[position].r = INVALID_COLOR;
            tempProductColor.rgbColorLst[position].g = INVALID_COLOR;
            tempProductColor.rgbColorLst[position].b = INVALID_COLOR;
        }
        m_mosaicData->Redraw = 2;
        m_mosaicData->Blink = -1;
    }

    if (band == 40000)
    {
        return getValueSum(data,width);
    }
    return true;
}

bool MosaicDataProvider::insertSrsRecordToQgisDb( const QString &proj4Str )
{
    //// 访问数据库qgis.db中的表TBL_SRS的互斥锁
    //static QMutex s_mutexdb;

    //s_mutexdb.lock();
    //QSqlQuery query;
    //QString sql;

    //// 查询是否已有该站号的投影记录
    //QString stationNumber = m_mosaicData->SquareFileHead.SiteInfo.StationNumber;
    //sql = QString("SELECT PARAMETERS FROM TBL_SRS WHERE DESCRIPTION='%1'").arg(stationNumber);
    //if (!query.exec(sql))
    //{
    //    QSLOG_ERROR(QString("执行查询语句失败！[%1]").arg(sql));
    //    s_mutexdb.unlock();
    //    return false;
    //}

    //// 查询到该站号的投影记录，再判断需不需要更新记录
    //if (query.next())
    //{
    //    QString selectProj4Str = query.value(0).toString();
    //    if (selectProj4Str.compare(proj4Str, Qt::CaseInsensitive) == 0)
    //    {
    //        // 查询有站号投影相同的记录，不执行插入、更新
    //        QSLOG_INFO(QString("表TBL_SRS中已经存在[description=%1,parameters=%2]的记录，无需插入新纪录！").arg(stationNumber).arg(proj4Str));
    //    }
    //    else
    //    {
    //        // 查询到有站号相同，投影不相同的记录，则更新投影
    //        QSLOG_INFO(QString("表TBL_SRS中已经存在[description=%1,parameters!=%2]的记录，需更新记录！").arg(stationNumber).arg(proj4Str));
    //        sql = QString("UPDATE TBL_SRS SET PARAMETERS='%1' WHERE DESCRIPTION='%2'").arg(proj4Str).arg(stationNumber);
    //        if (!query.exec(sql))
    //        {
    //            QSLOG_ERROR(QString("执行更新语句失败！[%1]").arg(sql));
    //            s_mutexdb.unlock();
    //            return false;
    //        }

    //        QSLOG_INFO(QString("更新一条记录成功！[%1]").arg(sql));
    //    }

    //    s_mutexdb.unlock();
    //    return true;
    //}

    //// 没有查询到该站号的投影，则需要查询最大的坐标系ID
    //int srsId = 0;
    //sql = "SELECT MAX(SRS_ID) FROM TBL_SRS";
    //if (!query.exec(sql))
    //{
    //    QSLOG_ERROR(QString("执行查询语句失败！[%1]").arg(sql));
    //    s_mutexdb.unlock();
    //    return false;
    //}

    //if (query.next())
    //{
    //    srsId = query.value(0).toInt() + 1;
    //}

    //// 插入新的站号投影记录
    //sql = QString("INSERT INTO TBL_SRS "
    //              "VALUES(%1, '%2', 'aeqd', '', '%3', null, 'radar', null, 0, '1')")
    //      .arg(srsId).arg(stationNumber).arg(proj4Str);

    //if (!query.exec(sql))
    //{
    //    QSLOG_ERROR(QString("执行插入数据表失败！[%1]").arg(query.lastError().text()));
    //    s_mutexdb.unlock();
    //    return false;
    //}

    //QSLOG_INFO(QString("插入一条记录成功！[%1]").arg(sql));
    //s_mutexdb.unlock();
    return true;
}

int MosaicDataProvider::fromWindowToColorBar( int Xoffset, int Yoffset )
{
    // 矩形个数
    int nRectNum = m_mosaicData->ProductColor.rgbColorLst.size();
    int nRectHeight = m_legendRect.height() / nRectNum;


    int location = -1;
    location = (Yoffset - m_legendRect.top()) / nRectHeight;

    if (location >= nRectNum)
    {
        return -1;
    }
    return location;
}

float MosaicDataProvider::getValueByPosition( int x,int y,int xLen,int h )
{
    float v = -10000;
    //if ((y * xLen + x) >= xLen * h)
    //{
    //    return v;
    //}

    //if (m_mosaicData->UnChar == 0)
    //{
    //    v = (char) m_mosaicData->SquareBuffer[y * xLen + x];
    //}
    //else if (m_mosaicData->UnChar == 1)
    //{
    //    v = (unsigned char) m_mosaicData->SquareBuffer[y * xLen + x];
    //}
    //else if (m_mosaicData->UnChar == 2)
    //{
    //    short * pShort = (short *) (m_mosaicData->SquareBuffer);
    //    v = (short) pShort[y * xLen + x];
    //}
    //else if (m_mosaicData->UnChar == 3)
    //{
    //    float * pFloat = (float *) (m_mosaicData->SquareBuffer);
    //    v = (float) pFloat[y * xLen + x];
    //}
    return v ;
}

//雷达站东经，北纬，仰角，方位角，实际距离
void MosaicDataProvider::FlatToEarthCoordinate(double e0, double n0, double el, double az, double r,float &Lon,float &Lat)
{
    const double Pi = PI/180;
    const double R = EARTH_RADIUS;  //转成弧度和地球半径常数
    e0 *= Pi;
    n0 *= Pi;
    el = 0; //化成弧度,因为所有产品已为0度平面。
    //计算以地心为原点的直角坐标x y z
    double x = r * cos(el) * sin(az);
    double y = R * cos(n0) - r * sin(n0) * cos(el) * cos(az) + r * cos(n0) * sin(el);
    double z = R * sin(n0) + r * cos(n0) * cos(el) * cos(az) + r * sin(n0) * sin(el);
    //计算经度e、纬度n和高度h //经度
    Lon = (float)((e0 + asin(x / sqrt(x * x + y * y))) / Pi);
    //纬度//float h = (float)(Math.Sqrt(x*x+y*y+z*z) - R);  //距地面高度
    Lat = (float)(acos(sqrt(x * x + y * y) / sqrt(x * x + y * y + z * z)) / Pi);  //纬度//float h = (float)(Math.Sqrt(x*x+y*y+z*z) - R);  //距地面高度
}

void MosaicDataProvider::getLonLatFromPoint(float lon0, float lat0, float posx, float posy, float &lon, float &lat)
{
    //雷达中心点的经纬度
    lon0 = lon0*(3.14159265/180.0);
    lat0 = lat0*(3.14159265/180.0);
    posx = posx/6371.0;
    posy = posy/6371.0;
    float p,c;
    p = (float)sqrt(pow(posx, 2) + pow(posy, 2));
    c = (float)atan(p);

    //所求坐标点的经纬度
    lon = lon0 + atan(posx * sin(c) / (p * cos(lat0) * cos(c) - posy * sin(lat0) * sin(c)));
    lat = asin(cos(c) * sin(lat0) + posy * sin(c) * cos(lat0) / p);
    lat = lat * (180.0/3.14159265);
    lon = lon * (180.0/3.14159265);
}

void MosaicDataProvider::verTopolar( double x, double y, double *r, double *az )
{
    if (x == 0.0 && y>=0.0)
    {
        *r = y/1000.0;
        *az = 0.0;
    }
    else if(x > 0.0 && y>0.0)
    {
        *r = sqrt(x*x+y*y)/1000.0;
        *az =atan(x/y) * 180.0/PI;
    }
    else if (x > 0.0 && y == 0.0)
    {
        *r = x/1000;
        *az = 90.0;
    }
    else if (x > 0.0 && y < 0.0)
    {
        *r = sqrt(x*x+y*y)/1000.0;
        *az = atan(-y/x) * 180.0/PI + 90.0 ;
    }
    else if (x == 0 && y<0)
    {
        *r = -y/1000.0;
        *az = 180.0;
    }
    else if (x <0 && y <0)
    {
        *r = sqrt(x*x+y*y)/1000.0;
        *az = atan(x/y) * 180.0/PI + 180.0 ;
    }
    else if (x <0 && y == 0)
    {
        *r = -x/1000.0;
        *az = 270.0;
    }
    else if (x < 0 && y >0 )
    {
        *r = sqrt(x*x+y*y)/1000.0;
        *az = atan(-y/x) * 180.0/PI + 270.0 ;
    }
}

QgsRectangle MosaicDataProvider::transformExtent(QgsRenderContext *renderCxt, const QgsRectangle &rect)
{
    QgsRectangle newRect = rect;
    if ((renderCxt != NULL)
            && (renderCxt->coordinateTransform() != NULL))
    {
        QgsCoordinateReferenceSystem srcCrs = renderCxt->coordinateTransform()->sourceCrs();
        QgsCoordinateReferenceSystem destCrs = renderCxt->coordinateTransform()->destCRS();
        if (srcCrs.projectionAcronym() == "")
        {
            srcCrs = *m_mosaicData->Crs;
        }

        if (srcCrs.toProj4() != destCrs.toProj4())
        {
            QgsCoordinateTransform coodTransform(srcCrs, destCrs);
            newRect = coodTransform.transformBoundingBox(rect);
        }
    }

    return newRect;
}


void MosaicDataProvider::generateDateTime( int year, int month, int day, int hour, int minute, int second, QString &date, QString &time )
{
    QDateTime gct_dt(QDate(year, month, day), QTime(hour, minute, second));
    date = gct_dt.date().toString("yyyy-MM-dd");
    time = gct_dt.time().toString("hh:mm:ss");
}

void MosaicDataProvider::drawInteractColorBar( QImage &colorImage )
{
    //QPainter painter(&colorImage);
    //painter.setRenderHint(QPainter::Antialiasing);
    //painter.setRenderHint(QPainter::TextAntialiasing);

    //// 矩形个数
    //int nRectNum = m_mosaicData->ProductColor.rgbColorLst.size();

    //// 设置字体
    //QFont font("微软雅黑", m_mosaicData->FontSize);
    //painter.setFont(font);

    //QBrush brush(Qt::transparent);

    //// 矩形距图片上边缘距离
    //int topOffset = m_mosaicData->FontSize;
    //// 字体距图片上边缘距离
    //int fontTopOffset = topOffset;

    //// 矩形距离图片左边缘距离
    //int leftOffset = 50;
    //int space = 5;

    //// 矩形宽度
    //int nRectWidth = 3*m_mosaicData->FontSize;
    //// 矩形高度
    //int nRectHeight = (colorImage.height() - 2*space)/(nRectNum+1);

    //// 如果矩形太矮就不绘制
    //if (nRectHeight<10)
    //{
    //    return;
    //}

    //QRect *colorRect = new QRect[nRectNum];
    //int i = 0;
    //foreach(const RGBCOLOR &rgbColor, m_mosaicData->ProductColor.rgbColorLst)
    //{
    //    // QSLOG_INFO(QString("%1 %2 %3").arg(rgbColor.r).arg(rgbColor.g).arg(rgbColor.b));
    //    brush.setColor(QColor(rgbColor.r, rgbColor.g, rgbColor.b));
    //    colorRect[i].setLeft(leftOffset);
    //    colorRect[i].setWidth(nRectWidth);
    //    colorRect[i].setTop(topOffset + i*nRectHeight);
    //    colorRect[i].setHeight(nRectHeight);

    //    //ND时，色标矩形设为透明色
    //    if(rgbColor.realValue.compare("ND") != 0)
    //    {
    //        painter.fillRect(colorRect[i], brush);
    //    }
    //    painter.drawRect(colorRect[i]);
    //    painter.drawText(colorRect[i].right()+space, colorRect[i].top()+space, rgbColor.realValue);
    //    i++;
    //}

    //if (colorRect != NULL)
    //{
    //    delete[] colorRect;
    //    colorRect = NULL;
    //}

    //int unitTopOffset = topOffset + nRectHeight*nRectNum /*+ 2*m_mosaicData->FontSize*/;
    //painter.drawText(2*leftOffset, unitTopOffset, "单位:");

    //QString productUnit = getUnitByColorName(m_mosaicData->ProductCode.ColorName);
    //painter.drawText(2*leftOffset+4*m_mosaicData->FontSize, unitTopOffset, productUnit);
}

bool MosaicDataProvider::getValueSum( void *data, int count)
{
    //QString points((char*)data);
    //QSLOG_DEBUG(QString("传入的顶点字符串：%1").arg(points));
    //QList<QPointF>PointList;
    //QPolygon Polygon;
    //float sumV = 0.0;
    ////格点数
    //int point_count = 0;

    //if (points.split("_").count() != count)
    //{
    //    QSLOG_ERROR("传入的顶点数据格式有误");
    //    return false;
    //}
    //for (int i=0; i<count; i++)
    //{
    //    QPointF point;
    //    point.setX(points.split("_").at(i).split(" ").at(0).toDouble());
    //    point.setY(points.split("_").at(i).split(" ").at(1).toDouble());
    //    PointList.append(point);
    //}
    //int xLen = m_mosaicData->SquareFileHead.BaseInfo.XLength;
    //int yLen = m_mosaicData->SquareFileHead.BaseInfo.YLength;
    //double addV = m_mosaicData->SquareFileHead.BaseInfo.addV;
    //double mulV = m_mosaicData->SquareFileHead.BaseInfo.mulV;

    ////实际面积与格点面积比
    //double scale = mExtent.width()*mExtent.height()/(xLen*yLen);

    ////for (int i = 0;i<count;i++)
    ////{
    ////	float xOffset = PointList.at(i).x();
    ////	float yOffset = PointList.at(i).y();
    ////	if (qAbs(xOffset)<=(mExtent.width()/2) && qAbs(yOffset)<=(mExtent.width()/2))
    ////	{
    ////		double Xpercent = qAbs(xOffset)* xLen / mExtent.width();
    ////		double Ypercent = qAbs(yOffset)* yLen / mExtent.height();

    ////		float v = m_mosaicData->Zero;

    ////		if (xOffset >=0 && yOffset>= 0)
    ////		{
    ////			Polygon<<QPoint(qRound(xLen/2+Xpercent),qRound(yLen/2-Ypercent));
    ////		}
    ////		else if (xOffset >=0 && yOffset <= 0)
    ////		{
    ////			Polygon<<QPoint(qRound(xLen/2+Xpercent),qRound(yLen/2+Ypercent));
    ////		}
    ////		else if (xOffset <=0 && yOffset <= 0)
    ////		{
    ////			Polygon<<QPoint(qRound(xLen/2-Xpercent),qRound(yLen/2+Ypercent));
    ////		}
    ////		else if (xOffset <=0 && yOffset >= 0)
    ////		{
    ////			Polygon<<QPoint(qRound(xLen/2-Xpercent),qRound(yLen/2-Ypercent));
    ////		}
    ////	}
    ////}

    //// Modify by Fanghua Guo 2016-5-16
    //for (int i = 0; i<count; i++)
    //{
    //    float xOffset = PointList.at(i).x();
    //    float yOffset = PointList.at(i).y();

    //    double Xpercent = xOffset* xLen / mExtent.width();
    //    double Ypercent = yOffset* yLen / mExtent.height();
    //    Polygon<<QPoint(qRound(xLen/2.0 + Xpercent), qRound(yLen/2.0 - Ypercent));
    //}

    //for (int j=0; j<yLen; j++)
    //{
    //    for (int i= 0; i<xLen; i++)
    //    {
    //        if (Polygon.containsPoint(QPoint(i,j),Qt::WindingFill))
    //        {
    //            point_count++;
    //            float v = getValueByPosition(i, j, xLen, yLen);
    //            if (isValidData((int)v, -99999, 99999) && (qAbs(v-m_mosaicData->Zero)>PRECISION))
    //            {
    //                float val = (v + addV)*mulV;
    //                sumV+=scale*val/1000;
    //            }
    //        }
    //    }
    //}

    //double Area = point_count *scale/1000000;
    //QString str = QString("区域累计降水量：%1 m^3\n区域面积：%2 km^2").arg(QString("").sprintf("%.3f",sumV))
    //              .arg(QString("").sprintf("%.3f",Area));
    //QSLOG_INFO(str);
    //memset(data, 0, _msize(data));
    //memcpy((char *)data, str.toStdString().c_str(), _msize(data));
    return true;
}



/**
* Class factory to return a pointer to a newly created
* QgsGdalProvider object
*/
DLL_EXPORT_C_DECL MosaicDataProvider * classFactory( const QString *uri )
{
    return new MosaicDataProvider( *uri );
}
/** Required key function (used to map the plugin to a data store type)
*/
DLL_EXPORT_C_DECL QString providerKey()
{
    return QString("mosaic");
}
/**
* Required description function
*/
DLL_EXPORT_C_DECL QString description()
{
    return QString("mosaic data provider");
}
/**
* Required isProvider function. Used to determine if this shared library
* is a data provider plugin
*/
DLL_EXPORT_C_DECL bool isProvider()
{
    return true;
}

DLL_EXPORT_C_DECL void buildSupportedRasterFileFilter( QString & theFileFiltersString )
{

}

DLL_EXPORT_C_DECL void cleanupProvider()
{
}

PRODUCTCODE MosaicDataProvider::getProductCodeInfoByEnName(QString name)
{
    foreach(const PRODUCTCODE &productCode, m_productCodeLst)
    {
        if(productCode.ProductEnName == name)
        {
            return productCode;
        }
    }

    return PRODUCTCODE();
}
