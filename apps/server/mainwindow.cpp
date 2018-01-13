#include "mainwindow.h"
#include "modulelist.h"
#include "systemlog.h"
#include "threadlog.h"
#include "ctkLog.h"

#include <QSplitter>
#include <QVBoxLayout>
#include <QTimer>

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
    /*********************菜单****************************/
    m_pMenu = new CMenu(this);

    /***************** 中央窗口 **************************/
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

    /************************** 状态栏*****************************************/
    // 初始化状态栏
    m_pRunTime = new QLabel(QStringLiteral("运行时长："));
    m_pSysTime = new QLabel(QStringLiteral("系统时间："));
    m_pSysTime->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui.statusbar->setStyleSheet(QString("QStatusBar::item{border:0px}"));
    ui.statusbar->setSizeGripEnabled(false);
    ui.statusbar->addWidget(m_pRunTime);
    ui.statusbar->addWidget(new QLabel(" "), 1);
    ui.statusbar->addWidget(m_pSysTime);
    m_runTime = QDateTime::currentDateTime();
    QTimer::singleShot(1000, this, SLOT(refreshStatus()));
}

void MainWindow::refreshStatus()
{
    // 获取系统时间
    QDateTime curDT = QDateTime::currentDateTime();
    QString str = curDT.toString("yyyy-MM-dd hh:mm:ss ");
    m_pSysTime->setText(str);
    // 获取运行时间
    calRunTime();
    QTimer::singleShot(1000, this, SLOT(refreshStatus()));
}

void MainWindow::calRunTime()
{
    int tm = QDateTime::currentDateTime().toTime_t() - m_runTime.toTime_t();
    if (tm < 0)
    {
        QSLOG_ERROR("time is overflow.");
    }
    int day = tm / (24 * 60 * 60);
    int hour = (tm - day * (24 * 60 * 60)) / (60 * 60);
    int min = (tm - day * (24 * 60 * 60) - hour * (60 * 60)) / 60;
    int sec = tm - day * (24 * 60 * 60) - hour * (60 * 60) - min * 60;
    QString strTime;
    {
        strTime = QStringLiteral(" 运行时长：%1天 %2时 %3分 %4秒").arg(day).arg(hour, 2, 10, QChar('0')).arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
    }
    m_pRunTime->setText(strTime);
}