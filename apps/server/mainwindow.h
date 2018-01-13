﻿/***************************************************************************
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

#include <QMainWindow>

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

    /**
     * @brief:初始化界面布局
     * @return:void 无
     */
    void initUI();

private:
    Ui::MainWindow ui;

    CModuleList *m_pModuleList;
    CSystemLog *m_pSysLog;
    CThreadLog *m_pThreadLog;
};

#endif