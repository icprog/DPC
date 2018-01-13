/***************************************************************************
     --------------------------------------
    Date                 : 10-Jan-2018
    Copyright            : (C) 2018 by bjliu
    Email                : liubojun@sina.com
	气象水文数据处理框架菜单采用配置化方式进行加载
	#扩展菜单配置，服务器框架的大多数功能都是通过扩展菜单配置来实现的
	#扩展菜单配置格式为：菜单显示名=父菜单ID|图标|处理的动态库|处理的函数兼ID|菜单风格。
	#一共五个段，用”|“分割。当该菜单不存在其中一个段的变量时，用NULL表示。
	#父菜单ID为此菜单建立在哪个菜单上，如果为NULL，则为顶层菜单。
	#其中图标为位于procpe目录下的res中的png文件名。
	#处理的动态库位于procpe下sys目录中的动态库。
	#处理的函数为动态库中的导出函数，建立菜单的时候同时会用这个函数名作为菜单的ID，这样其他菜单可以建立在这个菜单上。
	#菜单风格为菜单和工具栏的风格，一共五个关键词：NULL、TOOLBARICON、TOOLBARSEP、MENU、MENUSEP
	#分别为空，即普通菜单、在工具栏放置图标、，普通工具栏分割条，父菜单，即其他菜单可以建立在这个菜单下、菜单栏分割条。
	#
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _dpc_server_menu_h_
#define _dpc_server_menu_h_

#include <QString>
#include <QList>
#include <QMap>
#include <QObject>

typedef struct
{
    QString strName;		// 菜单在界面上显示名称
    QString strFatherId;	// 父菜单ID
    QString strIcon;		// 图标
    QString strLibrary;		// 处理的动态库
    QString strFunction;	// 处理的函数兼ID
    QString strStyle;		// 菜单风格
} MenuDetail;

typedef QList<MenuDetail> Menus;

/**************前置声明***************************/
class MainWindow;
class QMenu;

class CMenu : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief:构造函数
     * @param: MainWindow * parent 父窗口
     * @return:无
     */
    CMenu(MainWindow *parent);

    /**
     * @brief:析构函数
     * @return:无
     */
    virtual ~CMenu();

protected:
    /**
     * @brief:初始化配置文件，完成菜单的加载
     * @return:无
     */
    void init();

    /**
     * @brief:初始化界面
     * @return:无
     */
    void initUI();

    /**
     * @brief:完成子菜单的插入
     * @param: MenuDetail & in_oMenu 菜单信息
     * @return:无
     */
    void insertMenuAction(MenuDetail &in_oMenu);

private:
    QString m_strMenuFilePath;	// 菜单及工具栏配置文件路径
    QString m_strResDirPath;	// 资源文件路径
    MainWindow *m_pWnd;			// 主窗口
    Menus m_oMenu;				// 内部存储解析的menu.ini配置信息
    QMap<QString, QMenu*> m_oAllMenus;	// 菜单全集
};
#endif