#include "auxfunction.h"
#include <qmath.h>
#include <QDebug>

float getDistance(double lon1, double lat1, double lon2, double lat2)
{
	lon1 = deg_to_rad(lon1);
	lat1 = deg_to_rad(lat1);
	lon2 = deg_to_rad(lon2);
	lat2 = deg_to_rad(lat2);

	double a = lat1 - lat2;
	double b = lon1 - lon2;

	double s =2*qAsin(sqrt(pow(qSin(a/2),2) + qCos(lat1)*qCos(lat2)*pow(qSin(b/2),2)));
	s = s*EARTH_RADIUS;
	s = qRound(s*10000)/10000;
	return s;
}

QgsRectangle getMapExtents(double lon0, double lat0, int maxl)
{
	//用计算北京南京的实际距离905km验证通过
	double s = maxl/EARTH_RADIUS;
	double a = rad_to_deg(s);
	double b = 2 * qAsin(qSin(s/2.0)/qCos(deg_to_rad(lat0)));
	b = rad_to_deg(b);

	QgsRectangle rect(0, 0, 0, 0);
	rect.set(qMin(lon0+b, lon0-b), qMin(lat0+a, lat0-a), qMax(lon0+b, lon0-b), qMax(lat0+a, lat0-a));
	return rect;
}

double getCentralX(QgsRectangle rect)
{
	return (rect.xMaximum() + rect.xMinimum()) * 0.5;
}

double getCentralY(QgsRectangle rect)
{
	return (rect.yMinimum() + rect.yMaximum()) * 0.5;
}

