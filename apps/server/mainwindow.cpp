#include "mainwindow.h"
#include "modulelist.h"
#include "systemlog.h"
#include "threadlog.h"

#include <QSplitter>
#include <QVBoxLayout>

MainWindow::MainWindow(QMainWindow *parent /*= NULL*/) : QMainWindow(parent)
{
    ui.setupUi(this);

    initUI();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initUI()
{
    m_pModuleList = new CModuleList();
    m_pSysLog = new CSystemLog();
    m_pThreadLog = new CThreadLog();

    // 用来装载右侧垂直布局的窗口
    QWidget *VSplitterWnd = new QWidget(this);

    QVBoxLayout *t_pVLayout = new QVBoxLayout();
    VSplitterWnd->setLayout(t_pVLayout);

    // 水平布局器
    QSplitter *m_pHSplitter = new QSplitter(Qt::Horizontal, this);

    t_pVLayout->addWidget(m_pThreadLog);
    t_pVLayout->addWidget(m_pSysLog);

    m_pHSplitter->addWidget(m_pModuleList);
    m_pHSplitter->addWidget(VSplitterWnd);
    this->setCentralWidget(m_pHSplitter);
}
