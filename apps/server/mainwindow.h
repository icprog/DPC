/***************************************************************************
     --------------------------------------
    Date                 : 10-Jan-2018
    Copyright            : (C) 2018 by bjliu
    Email                : liubojun@sina.com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _dpc_server_mainwindow_h_
#define _dpc_server_mainwindow_h_

#include "ui_mainwindow.h"
#include "menu.h"

#include <QMainWindow>
#include <QLabel>
#include <QDateTime>

// 前置声明
class CModuleList;
class CSystemLog;
class CThreadLog;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief:构造函数
     * @param: QMainWindow * parent 父窗口
     * @return:无
     */
    MainWindow(QMainWindow *parent = NULL);

    /**
     * @brief:析构函数
     * @return: 无
     */
    virtual ~MainWindow();

protected:
    /**
     * @brief:初始化界面布局
     * @return:void 无
     */
    void initUI();

    /**
     * @brief:计算运行时长
     * @return:void
     */
    void calRunTime();

protected slots:
    /**
     * @brief:状态栏刷新
     * @return:void
     */
    void refreshStatus();

private:
    Ui::MainWindow ui;

    /*********************菜单栏及工具栏*********************/
    friend class CMenu;
    CMenu *m_pMenu;

    /*********************中央区域**************************/
    CModuleList *m_pModuleList; // 模块列表
    CSystemLog *m_pSysLog;		// 系统日志
    CThreadLog *m_pThreadLog;	// 线程日志

    /*********************状态栏**********************/
    QLabel *m_pSysTime;			// 状态栏系统时间
    QLabel *m_pRunTime;			// 系统运行时间
    QDateTime m_runTime;		// 系统运行时间
};

#endif