/**
  * @file   auxfunction.h
  * @brief  公共方法.
  *
  *  这里是详细说明。
  * @author Fanghua Guo
  * @par   Copyright (c):
  *          Fanghua Guo.
  *         All Rights Reserved
  * @date   2016:09:29
  * @note   这里是注意事项。
  * @version <版本号>
  */
#ifndef AUXFUNCTION_H
#define AUXFUNCTION_H
#include <qgsrectangle.h>
#include "commontypedef.h"

#define deg_to_rad(x) (x*PI/180)
#define rad_to_deg(x) (x*180/PI)

//两经纬度之间的直线距离KM
float getDistance(double lon1, double lat1, double lon2, double lat2);

//根据雷达中心点的经纬度，雷达的探测距离，计算最大最小经纬度
QgsRectangle getMapExtents(double lon0, double lat0, int maxl);

// 中点位置X
double getCentralX(QgsRectangle rect);
double getCentralY(QgsRectangle rect);


#endif // AUXFUNCTION_H
