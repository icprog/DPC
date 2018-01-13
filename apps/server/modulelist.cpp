#include "modulelist.h"

CModuleList::CModuleList(QWidget *parent /*= NULL*/) : QWidget(parent)
{
    ui.setupUi(this);

    // 左侧模块列表，设置最大宽度
    this->setMaximumWidth(170);
}

CModuleList::~CModuleList()
{

}
