/***************************************************************************
     --------------------------------------
    Date                 : 13-Jan-2018
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

#ifndef _dpc_server_threadlog_h_
#define _dpc_server_threadlog_h_

#include "ui_threadlog.h"
#include <QWidget>

class CThreadLog : public QWidget
{
    Q_OBJECT
public:
    CThreadLog(QWidget *parent = NULL);

    virtual ~CThreadLog();

private:
    Ui::threadLogWidget ui;
};
#endif