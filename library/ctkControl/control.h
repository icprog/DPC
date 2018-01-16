/***************************************************************************
     --------------------------------------
    Date                 : 16-Jan-2018
    Copyright            : (C) 2018 by bjliu
    Email                : liu_bojun@sina.com

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _dpc_library_control_h_
#define _dpc_library_control_h_

#include "macro.h"


/**
 * @brief:对应菜单启动项
 * @return:DLL_EXPORT_C_DECL void 无
 */
DLL_EXPORT_C_DECL void start();

/**
 * @brief:对应菜单停止项
 * @return:DLL_EXPORT_C_DECL void 无
 */
DLL_EXPORT_C_DECL void stop();

/**
 * @brief:对应菜单退出项
 * @return:DLL_EXPORT_C_DECL void 无
 */
DLL_EXPORT_C_DECL void quit();

#endif