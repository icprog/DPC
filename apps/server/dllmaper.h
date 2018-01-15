/***************************************************************************
     --------------------------------------
    Date                 : 15-Jan-2018
    Copyright            : (C) 2018 by bjliu
    Email                : liu_bojun@sina.com
	通过该类，将动态库函数以槽函数的方式封装起来
	主要是绑定菜单以及工具栏的点击事件
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _dpc_server_dllmaper_h_
#define _dpc_server_dllmaper_h_

#include <QObject>

typedef void(*DllFunction)();

class CDllMaper : public QObject
{
    Q_OBJECT
public:


    /**
     * @brief:构造函数
     * @param: const QString &dllname 动态库名称
     * @param: const QString &functionname 动态库中导出的C语言函数
     * @return:无
     */
    CDllMaper(const QString &dllname, const QString &functionname);

    /**
     * @brief:构造函数
     * @return:无
     */
    virtual ~CDllMaper();

public slots:

    /**
     * @brief:点击按钮的相应函数
     * @param: bool
     * @return:无
     */
    void trigger(bool);

protected:
    /**
     * @brief:构造函数
     * @return:无
     */
    CDllMaper();

    // 动态库中导出的函数
    DllFunction m_pFunctioner;
};
#endif