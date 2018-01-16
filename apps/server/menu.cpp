#include "menu.h"
#include "ctkLog.h"
#include "mainwindow.h"
#include "dllmaper.h"
#include "pathbuilder.h"

#include <QCoreApplication>
#include <QFile>
#include <QStringList>

CMenu::CMenu(MainWindow *parent) :m_pWnd(parent)
{
    CPathBuilder oPathBuilder;
    m_strMenuFilePath = QCoreApplication::applicationDirPath() + "/conf/menu.ini";
    m_strResDirPath = oPathBuilder.getProcpePath();
    init();

    initUI();
}

CMenu::~CMenu()
{
    // 释放所有保存的实例
    foreach(CDllMaper *p, m_oDllMapers)
    {
        delete p;
    }
    m_oDllMapers.clear();
}

void CMenu::init()
{
    QFile file(m_strMenuFilePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QSLOG_ERROR("open menu file failure.");
        return;
    }

    QTextStream stream(&file);
    while (!stream.atEnd())
    {
        QString strLine = stream.readLine().trimmed();
        if (strLine.startsWith("#"))
        {
            continue;
        }
        if (strLine.startsWith(" "))
        {
            continue;
        }

        QStringList key2value = strLine.split("=");
        if (key2value.size() != 2)
        {
            continue;
        }
        MenuDetail oMenu;

        // 菜单/工具栏 显示名称
        oMenu.strName = key2value.at(0);

        QString strSecond = key2value.at(1);

        QStringList detailsList = strSecond.split("|");
        if (detailsList.size() != 5)
        {
            continue;
        }

        // 依次是父菜单ID|图标|处理的动态库|处理的函数兼ID|菜单风格

        oMenu.strFatherId = detailsList.at(0);
        oMenu.strIcon = detailsList.at(1);
        oMenu.strLibrary = detailsList.at(2);
        oMenu.strFunction = detailsList.at(3);
        oMenu.strStyle = detailsList.at(4);
        m_oMenu.append(oMenu);

    }
    file.close();
}

void CMenu::initUI()
{
    foreach(MenuDetail t_oMenu, m_oMenu)
    {
        /**********************菜单父子关系建立***************************/
        if (t_oMenu.strFatherId == "NULL")
        {
            if (t_oMenu.strStyle == "MENU")
            {
                QMenu *pMenu = new QMenu(t_oMenu.strName);
                m_pWnd->ui.menubar->addMenu(pMenu);
                m_oAllMenus.insert(t_oMenu.strFunction, pMenu);
            }
            else if (t_oMenu.strStyle == "MENUSEP")
            {
                m_pWnd->ui.menubar->addSeparator();
            }
        }
        else
        {
            QMap<QString, QMenu*>::iterator iter = m_oAllMenus.find(t_oMenu.strFatherId);
            if (iter != m_oAllMenus.end())
            {
                /*********************样式设置***************************/
                if (t_oMenu.strStyle == "MENU")
                {
                    QMenu *pMenu = new QMenu(t_oMenu.strName);
                    iter.value()->addMenu(pMenu);
                    m_oAllMenus.insert(t_oMenu.strFunction, pMenu);
                }
                else if (t_oMenu.strStyle == "MENUSEP")
                {
                    iter.value()->addSeparator();
                }

            }
        }

        /***************除菜单外，其他样式**************************/
        if (t_oMenu.strStyle == "NULL")
        {
            insertMenuAction(t_oMenu);
        }
        else if (t_oMenu.strStyle == "TOOLBARSEP")
        {
            m_pWnd->ui.toolBar->addSeparator();
        }
        else if (t_oMenu.strStyle == "TOOLBARICON")
        {
            QAction *pAction = m_pWnd->ui.toolBar->addAction(t_oMenu.strName);
            // 此处只实例化一次，未考虑释放
            CDllMaper *pDllMaper = new CDllMaper(t_oMenu.strLibrary, t_oMenu.strFunction);
            m_oDllMapers.append(pDllMaper);
            connect(pAction, SIGNAL(triggered(bool)), pDllMaper, SLOT(trigger(bool)));
        }

    }
}

void CMenu::insertMenuAction(MenuDetail &in_oMenu)
{
    QMap<QString, QMenu*>::iterator iter = m_oAllMenus.find(in_oMenu.strFatherId);
    if (iter != m_oAllMenus.end())
    {
        if (in_oMenu.strStyle == "NULL")
        {
            QAction *pAction = NULL;
            if (in_oMenu.strIcon != "NULL")
            {
                QIcon oIcon(m_strResDirPath + "/" + in_oMenu.strIcon);
                pAction = iter.value()->addAction(oIcon, in_oMenu.strName);
            }
            else
            {
                pAction = iter.value()->addAction(in_oMenu.strName);
            }
            // 此处只实例化一次，未考虑释放
            CDllMaper *pDllMaper = new CDllMaper(in_oMenu.strLibrary, in_oMenu.strFunction);
            m_oDllMapers.append(pDllMaper);
            QObject::connect(pAction, SIGNAL(triggered(bool)), pDllMaper, SLOT(trigger(bool)));
        }
        else if (in_oMenu.strStyle == "MENU")
        {
            QMenu *pMenu = new QMenu(in_oMenu.strName);
            iter.value()->addMenu(pMenu);
            m_oAllMenus.insert(in_oMenu.strFunction, pMenu);
        }
    }
}
