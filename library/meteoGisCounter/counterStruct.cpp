#include "counterStruct.h"
#include "qcounter.h"
#include <QDateTime>
#pragma region 类成员函数实现

namespace QCounter
{
extern bool IsClockwise(QList<PointD> pointList);

void Polygon::AddHole(QList<PointD> pList)
{
    if (IsClockwise(pList))
    {
        int indexcount = pList.count();
        for (int i = 0; i < indexcount/2; i++)
        {
            pList.swap(i, indexcount-i-1);
        }
    }
    PolyLine line;
    line.ClearPolyLineValue();
    line.PointList = pList;

    HoleLines.push_back(line);

}

PolyLinePoint::PolyLinePoint(float wParam)
{
    _endPointList.clear();
    HoleLines.clear();
    Param=wParam;
}
}

#pragma endregion 类成员函数实现

// void DateTime(std::string strfilename, std::string str, std::string str1);

namespace QCounter
{
//	extern QList<QCounter::PolyLine> SmoothLines(const QList<QCounter::PolyLine> &aLineList);
}

void tracingContourLines(double **gridData, const QList<double> &cValues, const QList<double> &X, const QList<double> &Y, double noData, QList<QCounter::PolyLine> &lines,float param)
{
    QCounter::PolyLinePoint linepoint(param);

    QList<QCounter::Border> border;
    lines = linepoint.TracingContourLines(gridData, cValues, X, Y, noData, border);
    lines = linepoint.SmoothLines(lines);
}

void tracingPolygons(double **gridData, const QList<double> &cValues, const QList<double> &X, const QList<double> &Y, double noData, QList<QCounter::Polygon> &polygon,float param)
{
    QCounter::PolyLinePoint linepoint(param);
    QList<QCounter::Border> border;

    QList<QCounter::PolyLine> lines = linepoint.TracingContourLines(gridData, cValues, X, Y, noData, border);
    lines = linepoint.SmoothLines(lines);
    polygon = linepoint.TracingPolygons(gridData, lines, border, X, Y, cValues);

    linepoint.PolyMaxMin(gridData, X, Y, cValues, noData, polygon);
}

extern QList<QCounter::PolyLine> TracingStreamline(double** U, double** V, const QList<double> & X, const QList<double> & Y, double UNDEF, int density);

void tracingStreamlines(double** U, double** V, const QList<double> & X, const QList<double> & Y,QList<QCounter::PolyLine> & polyLines, double UNDEF, int density)
{
    polyLines = TracingStreamline( U,  V, X, Y, UNDEF, density);
}