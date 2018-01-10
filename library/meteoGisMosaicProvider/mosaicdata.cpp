#include "mosaicdata.h"

MosaicData::MosaicData()
{
    ImgWidth = -1;
    ImgHeight = -1;
    ProductWidth = -1;
    ProductHeight = -1;
    FontSize = 12;
    BufSize = 40;
    StrUri = "";
    Proj4String = "";
    LevelIndex = -1;
    Redraw = -1;
    Blink = -1;
    OldViewExtent = QgsRectangle(0, 0, 0, 0);
    ProductImage = new QImage(ProductWidth, ProductHeight, QImage::Format::Format_ARGB32);
    BlinkImage = new QImage(ProductWidth, ProductHeight, QImage::Format::Format_ARGB32);

}

MosaicData::~MosaicData()
{
    if (ProductImage != NULL)
    {
        delete ProductImage;
        ProductImage = NULL;
    }

    if (BlinkImage != NULL)
    {
        delete BlinkImage;
        BlinkImage = NULL;
    }
}
