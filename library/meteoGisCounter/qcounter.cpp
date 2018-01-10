#include "qcounter.h"
#include "Dimen.h"

#include <QList>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <QDateTime>
#include <QFile>
#include "ctkLog.h"

// void DateTime(std::string strfilename,std::string str,std::string str1);//测试速率

#pragma region 算法

namespace QCounter
{
    template <class T>
    void ReverveQList(QList<T> &qlist)
    {

        int length1,length2;
        length1=qlist.count()/2;
        length2=qlist.count();
        for(int i=0; i<length1; i++)
        {
            qlist.swap(i,length2-i-1);
        }
    }

    bool IsClockwise(QList<PointD> pointList)
    {
        double y = 0.0;
        int num3 = 0;
        for (int i = 0; i < (pointList.count() - 1); i++)
        {
            PointD td = pointList[i];
            if (i == 0)
            {
                y = td.Y;
                num3 = 0;
            }
            else if (y < td.Y)
            {
                y = td.Y;
                num3 = i;
            }
        }
        int num4 = num3 - 1;
        int num5 = num3;
        int num6 = num3 + 1;
        if (num3 == 0)
        {
            num4 = pointList.count() - 2;
        }
        PointD td2 = pointList[num4];
        PointD td3 = pointList[num5];
        PointD td4 = pointList[num6];
        return ((((td4.X - td2.X) * (td3.Y - td2.Y)) - ((td3.X - td2.X) * (td4.Y - td2.Y))) > 0.0);
    }

    static bool DoubleEquals(double a, double b)
    {
        double num = qAbs(a * 1E-05);
        return qAbs( a - b) < num;
    }

    double GetExtentAndArea(QList<PointD>  pList,  Extent &aExtent)
    {
        PointD td = pList[0];
        double x = td.X;
        double num4 = td.X;
        double y = td.Y;
        double num5 = td.Y;
        for (int i = 1; i < pList.count(); i++)
        {
            td = pList[i];
            if (td.X < x)
            {
                x = td.X;
            }
            if (td.X > num4)
            {
                num4 = td.X;
            }
            if (td.Y < y)
            {
                y = td.Y;
            }
            if (td.Y > num5)
            {
                num5 = td.Y;
            }
        }
        aExtent.xMin = x;
        aExtent.yMin = y;
        aExtent.xMax = num4;
        aExtent.yMax = num5;
        return ((num4 - x) * (num5 - y));
    }
    #pragma endregion 算法

    #pragma region 点

    static bool TraceBorder(C2DimenArray<int> &S1, int i1, int i2, int j1, int j2, int &i3, int &j3)
    {
        int num;
        int num2;
        int num3;
        int num4;
        bool flag = true;
        if (i1 < i2)
        {
            if ((S1.at(i2, j2 - 1) == 1) && (S1.at(i2, j2 + 1) == 1))
            {
                num = S1.at(i2 - 1, j2 - 1);
                num2 = S1.at(i2 + 1, j2);
                num3 = S1.at(i2 + 1, j2 - 1);
                if (((num != 0) && (num2 == 0)) || (((num == 0) && (num2 != 0)) && (num3 != 0)))
                {
                    i3 = i2;
                    j3 = j2 - 1;
                    return flag;
                }
                i3 = i2;
                j3 = j2 + 1;
                return flag;
            }
            if ((S1.at(i2, j2 - 1) == 1) && (S1.at(i2 + 1, j2) == 1))
            {
                num = S1.at(i2 + 1, j2 - 1);
                num2 = S1.at(i2 + 1, j2 + 1);
                num3 = S1.at(i2, j2 - 1);
                num4 = S1.at(i2, j2 + 1);
                if (((num == 0) || (num2 == 0)) || ((num3 == 0) || (num4 == 0)))
                {
                    if (((num == 0) && (num4 == 0)) || ((num2 == 0) && (num3 == 0)))
                    {
                        i3 = i2;
                        j3 = j2 - 1;
                        return flag;
                    }
                    i3 = i2 + 1;
                    j3 = j2;
                    return flag;
                }
                i3 = i2;
                j3 = j2 - 1;
                return flag;
            }
            if ((S1.at(i2, j2 + 1) == 1) && (S1.at(i2 + 1, j2) == 1))
            {
                num = S1.at(i2 + 1, j2 - 1);
                num2 = S1.at(i2 + 1, j2 + 1);
                num3 = S1.at(i2, j2 - 1);
                num4 = S1.at(i2, j2 + 1);
                if (((num == 0) || (num2 == 0)) || ((num3 == 0) || (num4 == 0)))
                {
                    if (((num == 0) && (num4 == 0)) || ((num2 == 0) && (num3 == 0)))
                    {
                        i3 = i2;
                        j3 = j2 + 1;
                        return flag;
                    }
                    i3 = i2 + 1;
                    j3 = j2;
                    return flag;
                }
                i3 = i2;
                j3 = j2 + 1;
                return flag;
            }
            if (S1.at(i2, j2 - 1) == 1)
            {
                i3 = i2;
                j3 = j2 - 1;
                return flag;
            }
            if (S1.at(i2, j2 + 1) == 1)
            {
                i3 = i2;
                j3 = j2 + 1;
                return flag;
            }
            if (S1.at(i2 + 1, j2) == 1)
            {
                i3 = i2 + 1;
                j3 = j2;
                return flag;
            }
            return false;
        }
        if (j1 < j2)
        {
            if ((S1.at(i2 + 1, j2) == 1) && (S1.at(i2 - 1, j2) == 1))
            {
                num = S1.at(i2 + 1, j2 - 1);
                num2 = S1.at(i2, j2 + 1);
                num3 = S1.at(i2 + 1, j2 + 1);
                if (((num != 0) && (num2 == 0)) || (((num == 0) && (num2 != 0)) && (num3 != 0)))
                {
                    i3 = i2 + 1;
                    j3 = j2;
                    return flag;
                }
                i3 = i2 - 1;
                j3 = j2;
                return flag;
            }
            if ((S1.at(i2 + 1, j2) == 1) && (S1.at(i2, j2 + 1) == 1))
            {
                num3 = S1.at(i2 - 1, j2);
                num4 = S1.at(i2 + 1, j2);
                num = S1.at(i2 - 1, j2 + 1);
                num2 = S1.at(i2 + 1, j2 + 1);
                if (((num == 0) || (num2 == 0)) || ((num3 == 0) || (num4 == 0)))
                {
                    if (((num == 0) && (num4 == 0)) || ((num2 == 0) && (num3 == 0)))
                    {
                        i3 = i2 + 1;
                        j3 = j2;
                        return flag;
                    }
                    i3 = i2;
                    j3 = j2 + 1;
                    return flag;
                }
                i3 = i2 + 1;
                j3 = j2;
                return flag;
            }
            if ((S1.at(i2 - 1, j2) == 1) && (S1.at(i2, j2 + 1) == 1))
            {
                num3 = S1.at(i2 - 1, j2);
                num4 = S1.at(i2 + 1, j2);
                num = S1.at(i2 - 1, j2 + 1);
                num2 = S1.at(i2 + 1, j2 + 1);
                if (((num == 0) || (num2 == 0)) || ((num3 == 0) || (num4 == 0)))
                {
                    if (((num == 0) && (num4 == 0)) || ((num2 == 0) && (num3 == 0)))
                    {
                        i3 = i2 - 1;
                        j3 = j2;
                        return flag;
                    }
                    i3 = i2;
                    j3 = j2 + 1;
                    return flag;
                }
                i3 = i2 - 1;
                j3 = j2;
                return flag;
            }
            if (S1.at(i2 + 1, j2) == 1)
            {
                i3 = i2 + 1;
                j3 = j2;
                return flag;
            }
            if (S1.at(i2 - 1, j2) == 1)
            {
                i3 = i2 - 1;
                j3 = j2;
                return flag;
            }
            if (S1.at(i2, j2 + 1) == 1)
            {
                i3 = i2;
                j3 = j2 + 1;
                return flag;
            }
            return false;
        }
        if (i1 > i2)
        {
            if ((S1.at(i2, j2 - 1) == 1) && (S1.at(i2, j2 + 1) == 1))
            {
                num = S1.at(i2 + 1, j2 - 1);
                num2 = S1.at(i2 - 1, j2);
                num3 = S1.at(i2 - 1, j2 + 1);
                if (((num != 0) && (num2 == 0)) || (((num == 0) && (num2 != 0)) && (num3 != 0)))
                {
                    i3 = i2;
                    j3 = j2 - 1;
                    return flag;
                }
                i3 = i2;
                j3 = j2 + 1;
                return flag;
            }
            if ((S1.at(i2, j2 - 1) == 1) && (S1.at(i2 - 1, j2) == 1))
            {
                num = S1.at(i2 - 1, j2 - 1);
                num2 = S1.at(i2 - 1, j2 + 1);
                num3 = S1.at(i2, j2 - 1);
                num4 = S1.at(i2, j2 + 1);
                if (((num == 0) || (num2 == 0)) || ((num3 == 0) || (num4 == 0)))
                {
                    if (((num == 0) && (num4 == 0)) || ((num2 == 0) && (num3 == 0)))
                    {
                        i3 = i2;
                        j3 = j2 - 1;
                        return flag;
                    }
                    i3 = i2 - 1;
                    j3 = j2;
                    return flag;
                }
                i3 = i2;
                j3 = j2 - 1;
                return flag;
            }
            if ((S1.at(i2, j2 + 1) == 1) && (S1.at(i2 - 1, j2) == 1))
            {
                num = S1.at(i2 - 1, j2 - 1);
                num2 = S1.at(i2 - 1, j2 + 1);
                num3 = S1.at(i2, j2 - 1);
                num4 = S1.at(i2, j2 + 1);
                if (((num == 0) || (num2 == 0)) || ((num3 == 0) || (num4 == 0)))
                {
                    if (((num == 0) && (num4 == 0)) || ((num2 == 0) && (num3 == 0)))
                    {
                        i3 = i2;
                        j3 = j2 + 1;
                        return flag;
                    }
                    i3 = i2 - 1;
                    j3 = j2;
                    return flag;
                }
                i3 = i2;
                j3 = j2 + 1;
                return flag;
            }
            if (S1.at(i2, j2 - 1) == 1)
            {
                i3 = i2;
                j3 = j2 - 1;
                return flag;
            }
            if (S1.at(i2, j2 + 1) == 1)
            {
                i3 = i2;
                j3 = j2 + 1;
                return flag;
            }
            if (S1.at(i2 - 1, j2) == 1)
            {
                i3 = i2 - 1;
                j3 = j2;
                return flag;
            }
            return false;
        }
        if (j1 <= j2)
        {
            return flag;
        }
        if ((S1.at(i2 + 1, j2) == 1) && (S1.at(i2 - 1, j2) == 1))
        {
            num = S1.at(i2 + 1, j2 + 1);
            num2 = S1.at(i2, j2 - 1);
            num3 = S1.at(i2 - 1, j2 - 1);
            if (((num != 0) && (num2 == 0)) || (((num == 0) && (num2 != 0)) && (num3 != 0)))
            {
                i3 = i2 + 1;
                j3 = j2;
                return flag;
            }
            i3 = i2 - 1;
            j3 = j2;
            return flag;
        }
        if ((S1.at(i2 + 1, j2) == 1) && (S1.at(i2, j2 - 1) == 1))
        {
            num3 = S1.at(i2 - 1, j2);
            num4 = S1.at(i2 + 1, j2);
            num = S1.at(i2 - 1, j2 - 1);
            num2 = S1.at(i2 + 1, j2 - 1);
            if (((num == 0) || (num2 == 0)) || ((num3 == 0) || (num4 == 0)))
            {
                if (((num == 0) && (num4 == 0)) || ((num2 == 0) && (num3 == 0)))
                {
                    i3 = i2 + 1;
                    j3 = j2;
                    return flag;
                }
                i3 = i2;
                j3 = j2 - 1;
                return flag;
            }
            i3 = i2 + 1;
            j3 = j2;
            return flag;
        }
        if ((S1.at(i2 - 1, j2) == 1) && (S1.at(i2, j2 - 1) == 1))
        {
            num3 = S1.at(i2 - 1, j2);
            num4 = S1.at(i2 + 1, j2);
            num = S1.at(i2 - 1, j2 - 1);
            num2 = S1.at(i2 + 1, j2 - 1);
            if (((num == 0) || (num2 == 0)) || ((num3 == 0) || (num4 == 0)))
            {
                if (((num == 0) && (num4 == 0)) || ((num2 == 0) && (num3 == 0)))
                {
                    i3 = i2 - 1;
                    j3 = j2;
                    return flag;
                }
                i3 = i2;
                j3 = j2 - 1;
                return flag;
            }
            i3 = i2 - 1;
            j3 = j2;
            return flag;
        }
        if (S1.at(i2 + 1, j2) == 1)
        {
            i3 = i2 + 1;
            j3 = j2;
            return flag;
        }
        if (S1.at(i2 - 1, j2) == 1)
        {
            i3 = i2 - 1;
            j3 = j2;
            return flag;
        }
        if (S1.at(i2, j2 - 1) == 1)
        {
            i3 = i2;
            j3 = j2 - 1;
            return flag;
        }

        return false;
    }

    static bool PointInPolygon(const QList<PointD> & poly, const PointD &aPoint)
    {
        bool flag = false;
        int count = poly.count();
        if (count < 3)
        {
            return false;
        }
        double x = poly[count - 1].X;
        double y = poly[count - 1].Y;
        for (int i = 0; i < count; i++)
        {
            double num5;
            double num6;
            double num7;
            double num8;
            double num = poly[i].X;
            double num2 = poly[i].Y;
            if (num > x)
            {
                num5 = x;
                num7 = num;
                num6 = y;
                num8 = num2;
            }
            else
            {
                num5 = num;
                num7 = x;
                num6 = num2;
                num8 = y;
            }
            if (((num < aPoint.X) == (aPoint.X <= x)) && (((aPoint.Y - num6) * (num7 - num5)) < ((num8 - num6) * (aPoint.X - num5))))
            {
                flag = !flag;
            }
            x = num;
            y = num2;
        }
        return flag;
    }


    static QList<Border> TracingBorders(double ** S0, const QList<double> &X,const QList<double> &Y, C2DimenArray<int> &S1, double undefData)
    {
        int num3;
        int num4;
        int num5;
        int num6;
        int num7;
        int num8;
        int num9;
        int num10;
        int num11;
        int num12;

        PointD td;
        BorderLine line2;
        BorderLine line3;
        QList<BorderLine> list5;
        QList<BorderLine> list;
        int length = Y.count();
        int num2 = X.count();
        /*	S1 = new int[length, num2];*/
        for (num3 = 0; num3 < length; num3++)
        {
            num4 = 0;
            while (num4 < num2)
            {
                if (DoubleEquals(S0[num3] [num4], undefData))
                {
                    S1.at(num3, num4) = 0;
                }
                else
                {
                    S1.at(num3, num4) = 1;
                }
                num4++;
            }
        }
        num3 = 1;
        while (num3 < (length - 1))
        {
            num4 = 1;
            while (num4 < (num2 - 1))
            {
                if (S1.at(num3, num4) == 1)
                {
                    num5 = S1.at(num3, num4 - 1);
                    num6 = S1.at(num3, num4 + 1);
                    num7 = S1.at(num3 - 1, num4);
                    num8 = S1.at(num3 + 1, num4);
                    num9 = S1.at(num3 - 1, num4 - 1);
                    num10 = S1.at(num3 - 1, num4 + 1);
                    num11 = S1.at(num3 + 1, num4 - 1);
                    num12 = S1.at(num3 + 1, num4 + 1);
                    if ((((num5 > 0) && (num6 > 0)) && ((num7 > 0) && (num8 > 0))) && (((num9 > 0) && (num10 > 0)) && ((num11 > 0) && (num12 > 0))))
                    {
                        S1.at(num3, num4) = 2;
                    }
                    if ((((((((num5 + num6) + num7) + num8) + num9) + num10) + num11) + num12) <= 2)
                    {
                        S1.at(num3, num4) = 0;
                    }
                }
                num4++;
            }
            num3++;
        }
        bool flag = false;
        do
        {
            flag = false;
            num3 = 1;
            while (num3 < (length - 1))
            {
                for (num4 = 1; num4 < (num2 - 1); num4++)
                {
                    if (S1.at(num3, num4) == 1)
                    {
                        num5 = S1.at(num3, num4 - 1);
                        num6 = S1.at(num3, num4 + 1);
                        num7 = S1.at(num3 - 1, num4);
                        num8 = S1.at(num3 + 1, num4);
                        num9 = S1.at(num3 - 1, num4 - 1);
                        num10 = S1.at(num3 - 1, num4 + 1);
                        num11 = S1.at(num3 + 1, num4 - 1);
                        num12 = S1.at(num3 + 1, num4 + 1);
                        if (((num5 == 0) && (num6 == 0)) || ((num7 == 0) && (num8 == 0)))
                        {
                            S1.at(num3, num4) = 0;
                            flag = true;
                        }
                        if (((((num11 == 0) && (num6 == 0)) && (num7 == 0)) || (((num12 == 0) && (num5 == 0)) && (num7 == 0))) || ((((num9 == 0) && (num6 == 0)) && (num8 == 0)) || (((num10 == 0) && (num5 == 0)) && (num8 == 0))))
                        {
                            S1.at(num3, num4) = 0;
                            flag = true;
                        }
                    }
                }
                num3++;
            }
        }
        while (flag);
        num4 = 0;
        while (num4 < num2)
        {
            if (S1.at(0, num4) == 1)
            {
                if (S1.at(1, num4) == 0)
                {
                    S1.at(0, num4) = 0;
                }
                else if (num4 == 0)
                {
                    if (S1.at(0, num4 + 1) == 0)
                    {
                        S1.at(0, num4) = 0;
                    }
                }
                else if (num4 == (num2 - 1))
                {
                    if (S1.at(0, num2 - 2) == 0)
                    {
                        S1.at(0, num4) = 0;
                    }
                }
                else if ((S1.at(0, num4 - 1) == 0) && (S1.at(0, num4 + 1) == 0))
                {
                    S1.at(0, num4) = 0;
                }
            }
            if (S1.at(length - 1, num4) == 1)
            {
                if (S1.at(length - 2, num4) == 0)
                {
                    S1.at(length - 1, num4) = 0;
                }
                else if (num4 == 0)
                {
                    if (S1.at(length - 1, num4 + 1) == 0)
                    {
                        S1.at(length - 1, num4) = 0;
                    }
                }
                else if (num4 == (num2 - 1))
                {
                    if (S1.at(length - 1, num2 - 2) == 0)
                    {
                        S1.at(length - 1, num4) = 0;
                    }
                }
                else if ((S1.at(length - 1, num4 - 1) == 0) && (S1.at(length - 1, num4 + 1) == 0))
                {
                    S1.at(length - 1, num4) = 0;
                }
            }
            num4++;
        }
        for (num3 = 0; num3 < length; num3++)
        {
            if (S1.at(num3, 0) == 1)
            {
                if (S1.at(num3, 1) == 0)
                {
                    S1.at(num3, 0)  = 0;
                }
                else if (num3 == 0)
                {
                    if (S1.at(num3 + 1, 0) == 0)
                    {
                        S1.at(num3, 0)  = 0;
                    }
                }
                else if (num3 == (length - 1))
                {
                    if (S1.at(length - 2, 0) == 0)
                    {
                        S1.at(num3, 0)  = 0;
                    }
                }
                else if ((S1.at(num3 - 1, 0) == 0) && (S1.at(num3 + 1, 0) == 0))
                {
                    S1.at(num3, 0)  = 0;
                }
            }
            if (S1.at(num3, num2 - 1) == 1)
            {
                if (S1.at(num3, num2 - 2) == 0)
                {
                    S1.at(num3, num2 - 1) = 0;
                }
                else if (num3 == 0)
                {
                    if (S1.at(num3 + 1, num2 - 1) == 0)
                    {
                        S1.at(num3, num2 - 1) = 0;
                    }
                }
                else if (num3 == (length - 1))
                {
                    if (S1.at(length - 2, num2 - 1) == 0)
                    {
                        S1.at(num3, num2 - 1) = 0;
                    }
                }
                else if ((S1.at(num3 - 1, num2 - 1) == 0) && (S1.at(num3 + 1, num2 - 1) == 0))
                {
                    S1.at(num3, num2 - 1) = 0;
                }
            }
        }

        C2DimenArray<int> numArray(length+2, num2+2);
        //int *pTemp1 =  new int[(length+2) * (num2+2)];
        //int ** numArray = new int *[length + 2];

        //for (int i=0; i<length + 2; i++)
        //{
        //	numArray[i] = pTemp1+i*(num2+2);
        //}

        for (num3 = 0; num3 < (length + 2); num3++)
        {
            num4 = 0;
            while (num4 < (num2 + 2))
            {
                if ((num3 == 0) || (num3 == (length + 1)))
                {
                    numArray.at(num3, num4) = 0;
                }
                else if ((num4 == 0) || (num4 == (num2 + 1)))
                {
                    numArray.at(num3, num4) = 0;
                }
                else
                {
                    numArray.at(num3, num4) = S1.at(num3 - 1, num4 - 1);
                }
                num4++;
            }
        }

        //int *pTemp2 = new int[(length+2) * (num2+2)];
        //int ** numArray2 = new int*[length + 2];
        //for (int i=0;i<length + 2;i++)
        //{
        //	numArray2[i] = pTemp2+i*(num2+2);
        //}
        C2DimenArray<int> numArray2(length+2, num2+2);

        for (num3 = 0; num3 < (length + 2); num3++)
        {
            num4 = 0;
            while (num4 < (num2 + 2))
            {
                if (numArray.at(num3, num4) == 1)
                {
                    num5 = numArray.at(num3, num4 - 1);
                    num6 = numArray.at(num3, num4 + 1);
                    num7 = numArray.at(num3 - 1, num4);
                    num8 = numArray.at(num3 + 1, num4);
                    num9 = numArray.at(num3 - 1, num4 - 1);
                    num10 = numArray.at(num3 - 1, num4 + 1);
                    num11 = numArray.at(num3 + 1, num4 - 1);
                    num12 = numArray.at(num3 + 1, num4 + 1);
                    if ((((num5 == 1) && (num6 == 1)) && ((num7 == 1) && (num8 == 1))) && (((num9 == 0) && (num12 == 0)) || ((num10 == 0) && (num11 == 0))))
                    {
                        numArray2.at(num3, num4) = 2;
                    }
                    else
                    {
                        numArray2.at(num3, num4) = 1;
                    }
                }
                else
                {
                    numArray2.at(num3, num4) = 0;
                }
                num4++;
            }
        }
        //	PointD td;
        //	IJPoint point;
        BorderLine line;
        QList<PointD> pList;
        QList<IJPoint> list3;
        int num19 = 0;
        int num20 = 0;
        for (num3 = 1; num3 < (length + 1); num3++)
        {
            num4 = 1;
            while (num4 < (num2 + 1))
            {
                if (numArray.at(num3, num4) == 1)
                {
                    td.ClearPointDValue();
                    td.PointDValue (X[num4 - 1],Y[num3 - 1]);
                    IJPoint point;
                    point.IJPointValue(num3 - 1,num4 - 1);

                    pList.clear();
                    pList.push_back(td);
                    list3.clear();
                    list3.push_back(point);
                    int num13 = num3;
                    int num14 = num4;
                    int num17 = num3;
                    int num18 = num4;
                    int num15 = num17;
                    int num16 = -1;
                    do
                    {
                        if (!TraceBorder(numArray, num15, num17, num16, num18, num19, num20))
                        {
                            break;
                        }
                        num15 = num17;
                        num16 = num18;
                        num17 = num19;
                        num18 = num20;
                        numArray2.at(num19, num20)--;
                        if (numArray2.at(num19, num20) == 0)
                        {
                            numArray.at(num19, num20) = 3;
                        }

                        td.ClearPointDValue();
                        td.PointDValue(X[num20 - 1],Y[num19 - 1]);
                        point.IJPointValue(num19 - 1,num20 - 1);

                        pList.push_back(td);
                        list3.push_back(point);
                    }
                    while ((num19 != num13) || (num20 != num14));
                    numArray2.at(num3, num4)--;
                    if (numArray2.at(num3, num4) == 0)
                    {
                        numArray.at(num3, num4) = 3;
                    }
                    if (pList.count() > 1)
                    {
                        line.ClearBorderLineValue();
                        line.SetBorderLine(pList, list3);

                        list.push_back(line);
                    }
                }
                num4++;
            }
        }
        QList<Border> list4;
        Border border;
        for (num3 = 1; num3 < list.count(); num3++)
        {
            line2 = list[num3];
            num4 = 0;
            while (num4 < num3)
            {
                line3 = list[num4];
                if (line2.area > line3.area)
                {
                    list.removeAt(num3);
                    list.insert(num4, line2);
                    break;
                }
                num4++;
            }
        }
        if (list.count() == 1)
        {
            line2 = list[0];
            if (!IsClockwise(line2.pointList))
            {

                //			std::list<PointD> templist=line2.pointList.toStdList();
                //			templist.Reverse();
                ReverveQList<PointD>(line2.pointList);
                ReverveQList<IJPoint>(line2.ijPointList);
            }
            line2.isClockwise = true;
            list5.push_back(line2);
            // 		list5 = new QList<BorderLine> {
            // 			line2
            // 		};
            border.ClearBorderValue();
            border.LineList = list5;
            list4.push_back(border);
            return list4;
        }
        for (num3 = 0; num3 < list.count(); num3++)
        {
            if (num3 == list.count())
            {
                return list4;
            }
            line2 = list[num3];
            if (!IsClockwise(line2.pointList))
            {
                //			line2.pointList.Reverse();
                //			line2.ijPointList.Reverse();
                ReverveQList<PointD>(line2.pointList);
                ReverveQList<IJPoint>(line2.ijPointList);
            }
            line2.isClockwise = true;
            /*		list5. = new QList<BorderLine> {
            line2
            };*/
            list5.clear();
            list5.push_back(line2);
            for (num4 = num3 + 1; num4 < list.count(); num4++)
            {
                if (num4 == list.count())
                {
                    break;
                }
                line3 = list[num4];
                if (((line3.extent.xMin > line2.extent.xMin) && (line3.extent.xMax < line2.extent.xMax)) && ((line3.extent.yMin > line2.extent.yMin) && (line3.extent.yMax < line2.extent.yMax)))
                {
                    td = line3.pointList[0];
                    if (PointInPolygon(line2.pointList, td))
                    {
                        line3.isOutLine = false;
                        if (IsClockwise(line3.pointList))
                        {
                            //						pList.Reverse();
                            //						line3.ijPointList.Reverse();
                            ReverveQList<PointD>(pList);
                            ReverveQList<IJPoint>(line3.ijPointList);
                        }
                        line3.isClockwise = false;
                        list5.push_back(line3);//Add(line3);
                        list.removeAt(num4);
                        num4--;
                    }
                }
            }
            // 		border = new Border {
            // 			LineList = list5
            //};

            border.ClearBorderValue();
            border.SetBorderValue(list5);

            list4.push_back(border);//Add(border);

        }


        //delete [](pTemp1);
        //delete [](numArray);
        //delete [](pTemp2);
        //delete [](numArray2);


        return list4;
    }
    #pragma endregion 点;

    #pragma region 线

    static bool TraceIsoline_UndefData(int i1, int i2, C2DimenArray<double> &H, C2DimenArray<double> &S, int j1, int j2,const QList<double>& X,const QList<double>& Y, double nx, double ny, double a2x, int & i3,  int & j3,  double & a3x,  double & a3y, bool & isS)
    {

        if (((j2+1)>=X.count()) || ((i2+1)>=Y.count()) || (j2==0) || (i2==0))
        {
            QSLOG_ERROR(QString("j2=%1,i2=%2,X.count()=%3,Y.count()=%4").arg(j2).arg(i2).arg(X.count()).arg(Y.count()));
            return false;
        }

        bool flag = true;
        if (i1 < i2)
        {
            if ((H.at(i2, j2) != -2.0) && (H.at(i2, j2 + 1) != -2.0))
            {
                if (H.at(i2, j2) < H.at(i2, j2 + 1))
                {
                    a3x = X[j2];
                    a3y = Y[i2] + (H.at(i2, j2) * ny);
                    i3 = i2;
                    j3 = j2;
                    H.at(i3, j3) = -2.0;
                    isS = false;
                    return flag;
                }
                a3x = X[j2 + 1];
                a3y = Y[i2] + (H.at(i2, j2 + 1) * ny);
                i3 = i2;
                j3 = j2 + 1;
                H.at(i3, j3) = -2.0;
                isS = false;
                return flag;
            }
            if ((H.at(i2, j2) != -2.0) && (H.at(i2, j2 + 1) == -2.0))
            {
                a3x = X[j2];
                a3y = Y[i2] + (H.at(i2, j2) * ny);
                i3 = i2;
                j3 = j2;
                H.at(i3, j3) = -2.0;
                isS = false;
                return flag;
            }
            if ((H.at(i2, j2) == -2.0) && (H.at(i2, j2 + 1) != -2.0))
            {
                a3x = X[j2 + 1];
                a3y = Y[i2] + (H.at(i2, j2 + 1) * ny);
                i3 = i2;
                j3 = j2 + 1;
                H.at(i3, j3) = -2.0;
                isS = false;
                return flag;
            }
            if (S.at(i2 + 1, j2) != -2.0)
            {
                a3x = X[j2] + (S.at(i2 + 1, j2) * nx);
                a3y = Y[i2 + 1];
                i3 = i2 + 1;
                j3 = j2;
                S.at(i3, j3) = -2.0;
                isS = true;
                return flag;
            }
            return false;
        }
        if (j1 < j2)
        {
            if ((S.at(i2, j2) != -2.0) && (S.at(i2 + 1, j2) != -2.0))
            {
                if (S.at(i2, j2) < S.at(i2 + 1, j2))
                {
                    a3x = X[j2] + (S.at(i2, j2) * nx);
                    a3y = Y[i2];
                    i3 = i2;
                    j3 = j2;
                    S.at(i3, j3) = -2.0;
                    isS = true;
                    return flag;
                }
                a3x = X[j2] + (S.at(i2 + 1, j2) * nx);
                a3y = Y[i2 + 1];
                i3 = i2 + 1;
                j3 = j2;
                S.at(i3, j3) = -2.0;
                isS = true;
                return flag;
            }
            if ((S.at(i2, j2) != -2.0) && (S.at(i2 + 1, j2) == -2.0))
            {
                a3x = X[j2] + (S.at(i2, j2) * nx);
                a3y = Y[i2];
                i3 = i2;
                j3 = j2;
                S.at(i3, j3) = -2.0;
                isS = true;
                return flag;
            }
            if ((S.at(i2, j2) == -2.0) && (S.at(i2 + 1, j2) != -2.0))
            {
                a3x = X[j2] + (S.at(i2 + 1, j2) * nx);
                a3y = Y[i2 + 1];
                i3 = i2 + 1;
                j3 = j2;
                S.at(i3, j3) = -2.0;
                isS = true;
                return flag;
            }
            if (H.at(i2, j2 + 1) != -2.0)
            {
                a3x = X[j2 + 1];
                a3y = Y[i2] + (H.at(i2, j2 + 1) * ny);
                i3 = i2;
                j3 = j2 + 1;
                H.at(i3, j3) = -2.0;
                isS = false;
                return flag;
            }
            return false;
        }
        if (X[j2] < a2x)
        {
            if ((H.at(i2 - 1, j2) != -2.0) && (H.at(i2 - 1, j2 + 1) != -2.0))
            {
                if (H.at(i2 - 1, j2) > H.at(i2 - 1, j2 + 1))
                {
                    a3x = X[j2];
                    a3y = Y[i2 - 1] + (H.at(i2 - 1, j2) * ny);
                    i3 = i2 - 1;
                    j3 = j2;
                    H.at(i3, j3) = -2.0;
                    isS = false;
                    return flag;
                }
                a3x = X[j2 + 1];
                a3y = Y[i2 - 1] + (H.at(i2 - 1, j2 + 1) * ny);
                i3 = i2 - 1;
                j3 = j2 + 1;
                H.at(i3, j3) = -2.0;
                isS = false;
                return flag;
            }
            if ((H.at(i2 - 1, j2) != -2.0) && (H.at(i2 - 1, j2 + 1) == -2.0))
            {
                a3x = X[j2];
                a3y = Y[i2 - 1] + (H.at(i2 - 1, j2) * ny);
                i3 = i2 - 1;
                j3 = j2;
                H.at(i3, j3) = -2.0;
                isS = false;
                return flag;
            }
            if ((H.at(i2 - 1, j2) == -2.0) && (H.at(i2 - 1, j2 + 1) != -2.0))
            {
                a3x = X[j2 + 1];
                a3y = Y[i2 - 1] + (H.at(i2 - 1, j2 + 1) * ny);
                i3 = i2 - 1;
                j3 = j2 + 1;
                H.at(i3, j3) = -2.0;
                isS = false;
                return flag;
            }
            if (S.at(i2 - 1, j2) != -2.0)
            {
                a3x = X[j2] + (S.at(i2 - 1, j2) * nx);
                a3y = Y[i2 - 1];
                i3 = i2 - 1;
                j3 = j2;
                S.at(i3, j3) = -2.0;
                isS = true;
                return flag;
            }
            return false;
        }
        if ((S.at(i2 + 1, j2 - 1) != -2.0) && (S.at(i2, j2 - 1) != -2.0))
        {
            if (S.at(i2 + 1, j2 - 1) > S.at(i2, j2 - 1))
            {
                a3x = X[j2 - 1] + (S.at(i2 + 1, j2 - 1) * nx);
                a3y = Y[i2 + 1];
                i3 = i2 + 1;
                j3 = j2 - 1;
                S.at(i3, j3) = -2.0;
                isS = true;
                return flag;
            }
            a3x = X[j2 - 1] + (S.at(i2, j2 - 1) * nx);
            a3y = Y[i2];
            i3 = i2;
            j3 = j2 - 1;
            S.at(i3, j3) = -2.0;
            isS = true;
            return flag;
        }
        if ((S.at(i2 + 1, j2 - 1) != -2.0) && (S.at(i2, j2 - 1) == -2.0))
        {
            a3x = X[j2 - 1] + (S.at(i2 + 1, j2 - 1) * nx);
            a3y = Y[i2 + 1];
            i3 = i2 + 1;
            j3 = j2 - 1;
            S.at(i3, j3) = -2.0;
            isS = true;
            return flag;
        }
        if ((S.at(i2 + 1, j2 - 1) == -2.0) && (S.at(i2, j2 - 1) != -2.0))
        {
            a3x = X[j2 - 1] + (S.at(i2, j2 - 1) * nx);
            a3y = Y[i2];
            i3 = i2;
            j3 = j2 - 1;
            S.at(i3, j3) = -2.0;
            isS = true;
            return flag;
        }
        if (H.at(i2,j2 - 1) != -2.0)
        {
            a3x = X[j2 - 1];
            a3y = Y[i2] + (H.at(i2,j2 - 1) * ny);
            i3 = i2;
            j3 = j2 - 1;
            H.at(i3, j3) = -2.0;
            isS = false;
            return flag;
        }

        return false;
    }



//PRIVATE
    QList<PolyLine> PolyLinePoint::Isoline_UndefData(double** S0, const QList<double> &X, const QList<double> &Y, double W, double nx, double ny, C2DimenArray<double> &S,  C2DimenArray<double> &H, C3DimenArray<int> &SB, C3DimenArray<int> &HB, int lineNum)
    {


        int num3;
        int num4;
        int num5;
        int num6;
        int num7;
        int num8;
        double num11;
        double num12;
        double num15;
        double num16;
        PointD td;
        PolyLine line;
        QList<PolyLine> list;// = new QList<PolyLine>();
        //	int length = S0.GetLength(0);
        //	int num2 = S0.GetLength(1);
        int length = Y.count();
        int num2 = X.count();
        int num9 = 0;
        int num10 = 0;
        double num13 = 0.0;
        double num14 = 0.0;
        bool isS = true;
        EndPoint point;// = new EndPoint();
        for (num3 = 0; num3 < length; num3++)
        {
            for (num4 = 0; num4 < num2; num4++)
            {
                QList<PointD> list2;// = new QList<PointD>();

                if (((num4 >= (num2 - 1)) || (SB.at(0, num3, num4) <= -1)) || (S.at(num3, num4) == -2.0))
                {
                    goto Label_033E;
                }

                num6 = num3;
                num8 = num4;
                num11 = X[num8] + (S.at(num6, num8) * nx);
                num12 = Y[num6];
                if (SB.at(1, num3, num4) == 0)
                {
                    num5 = -1;
                    point.sPoint.X = X[num4 + 1];
                    point.sPoint.Y = Y[num3];
                }
                else
                {
                    num5 = num6;
                    point.sPoint.X = X[num4];
                    point.sPoint.Y = Y[num3];
                }
                num7 = num8;
                //td = new PointD {
                //	X = num11,
                //		Y = num12
                //};
                td.PointDValue(num11,num12);

                list2.push_back(td);

                point.Index = lineNum + list.count();
                point.Point = td;
                point.BorderIdx = SB.at(0, num3, num4);
                _endPointList.push_back(point);
                //line = new PolyLine {
                //	Type = "Border",
                //		BorderIdx = SB[0][ num3][ num4]
                //};
                {
                    line.Type="Border";
                    line.BorderIdx=SB.at(0, num3, num4);
                }

Label_0181:
                if (!TraceIsoline_UndefData(num5, num6,  H,  S, num7, num8, X, Y, nx, ny, num11,  num9,  num10,  num13,  num14, isS))
                {
                    goto Label_02BB;
                }
                // 			td = new PointD {
                // 				X = num13,
                // 					Y = num14
                // 			};
                td.PointDValue(num13,num14);
                list2.push_back(td);
                if (isS)
                {
                    if (SB.at(0, num9, num10) <= -1)
                    {
                        goto Label_029E;
                    }
                    if (SB.at(1, num9, num10) == 0)
                    {
                        point.sPoint.X = X[num10 + 1];
                        point.sPoint.Y = Y[num9];
                    }
                    else
                    {
                        point.sPoint.X = X[num10];
                        point.sPoint.Y = Y[num9];
                    }
                    goto Label_02C7;
                }
                if (HB.at(0, num9, num10) > -1)
                {
                    if (HB.at(1, num9, num10) == 0)
                    {
                        point.sPoint.X = X[num10];
                        point.sPoint.Y = Y[num9];
                    }
                    else
                    {
                        point.sPoint.X = X[num10];
                        point.sPoint.Y = Y[num9 + 1];
                    }
                    goto Label_02C7;
                }
Label_029E:
                num11 = num13;
                num12 = num14;
                num5 = num6;
                num7 = num8;
                num6 = num9;
                num8 = num10;
                goto Label_0181;
Label_02BB:
                line.Type = "Error";
Label_02C7:
                S.at(num3, num4) = -2.0;
                if ((list2.count() > 1) && (line.Type != "Error"))
                {
                    point.Point = td;
                    _endPointList.push_back(point);
                    line.Value = W;
                    line.PointList = list2;
                    list.push_back(line);
                }
                else
                {
                    _endPointList.removeAt(_endPointList.count() - 1);
                }
Label_033E:
                if (((num3 >= (length - 1)) || (HB.at(0, num3, num4) <= -1)) || (H.at(num3, num4) == -2.0))
                {
                    continue;
                }
                list2.clear() ;//= new List<PointD>();
                num6 = num3;
                num8 = num4;
                num11 = X[num8];
                num12 = Y[num6] + (H.at(num6, num8) * ny);
                num5 = num6;
                if (HB.at(1, num3, num4) == 0)
                {
                    num7 = -1;
                    point.sPoint.X = X[num4];
                    point.sPoint.Y = Y[num3];
                }
                else
                {
                    num7 = num8;
                    point.sPoint.X = X[num4];
                    point.sPoint.Y = Y[num3 + 1];
                }
                // 			td = new PointD {
                // 				X = num11,
                // 					Y = num12
                // 			};
                td.PointDValue(num11,num12);
                list2.push_back(td);
                point.Index = lineNum + list.count();
                point.Point = td;
                point.BorderIdx = HB.at(0, num3, num4);
                _endPointList.push_back(point);
                //line = new PolyLine {
                //	Type = "Border",
                //		BorderIdx = HB[0][ num3][ num4]
                //};
                {
                    line.Type="Border";
                    line.BorderIdx=HB.at(0, num3, num4);
                }

Label_0473:
                if (!TraceIsoline_UndefData(num5, num6,  H,  S, num7, num8, X, Y, nx, ny, num11,  num9,  num10,  num13,  num14,  isS))
                {
                    goto Label_05AD;
                }
                // 			td = new PointD {
                // 				X = num13,
                // 					Y = num14
                // 			};
                td.PointDValue(num13,num14);
                list2.push_back(td);
                if (isS)
                {
                    if (SB.at(0, num9, num10) <= -1)
                    {
                        goto Label_0590;
                    }
                    if (SB.at(1, num9, num10) == 0)
                    {
                        point.sPoint.X = X[num10 + 1];
                        point.sPoint.Y = Y[num9];
                    }
                    else
                    {
                        point.sPoint.X = X[num10];
                        point.sPoint.Y = Y[num9];
                    }
                    goto Label_05B9;
                }
                if (HB.at(0, num9, num10) > -1)
                {
                    if (HB.at(1, num9, num10) == 0)
                    {
                        point.sPoint.X = X[num10];
                        point.sPoint.Y = Y[num9];
                    }
                    else
                    {
                        point.sPoint.X = X[num10];
                        point.sPoint.Y = Y[num9 + 1];
                    }
                    goto Label_05B9;
                }
Label_0590:
                num11 = num13;
                num12 = num14;
                num5 = num6;
                num7 = num8;
                num6 = num9;
                num8 = num10;
                goto Label_0473;
Label_05AD:
                line.Type = "Error";
Label_05B9:
                H.at(num3, num4) = -2.0;
                if ((list2.count() > 1) && (line.Type != "Error"))
                {
                    point.Point = td;
                    _endPointList.push_back(point);
                    line.Value = W;
                    line.PointList = list2;
                    list.push_back(line);
                }
                else
                {
                    _endPointList.removeAt(_endPointList.count() - 1);
                }
            }
        }
        num4 = 0;
        while (num4 < (num2 - 1))
        {
            if (S.at(0, num4) != -2.0)
            {
                S.at(0, num4) = -2.0;
            }
            if (S.at(length - 1, num4) != -2.0)
            {
                S.at(length - 1, num4) = -2.0;
            }
            num4++;
        }
        for (num3 = 0; num3 < (length - 1); num3++)
        {
            if (H.at(num3, 0) != -2.0)
            {
                H.at(num3, 0) = -2.0;
            }
            if (H.at(num3, num2 - 1) != -2.0)
            {
                H.at(num3, num2 - 1) = -2.0;
            }
        }
        for (num3 = 1; num3 < (length - 2); num3++)
        {
            num4 = 1;
            while (num4 < (num2 - 1))
            {
                QList<PointD> list3;// = new QList<PointD>();
                if (H.at(num3, num4) == -2.0)
                {
                    goto Label_088D;
                }

                num6 = num3;
                num8 = num4;
                num11 = X[num8];
                num12 = Y[num3] + (H.at(num3, num8) * ny);
                num7 = -1;
                num5 = num6;
                num15 = num11;
                num16 = num12;
                // 			td = new PointD {
                // 				X = num11,
                // 					Y = num12
                // 			};
                td.PointDValue(num11,num12);
                list3.push_back(td);
                // 			line = new PolyLine {
                // 				Type = "Close"
                // 			};
                {
                    line.Type="Close";
                }

                while (true)
                {
                    if (!TraceIsoline_UndefData(num5, num6,  H,  S, num7, num8, X, Y, nx, ny, num11,  num9,  num10,  num13,  num14,  isS))
                    {
                        break;
                    }
                    // 				td = new PointD {
                    // 					X = num13,
                    // 						Y = num14
                    // 				};
                    td.PointDValue(num13,num14);
                    list3.push_back(td);
                    if ((abs(num14 - num16) < 1E-06) && (abs(num13 - num15) < 1E-06))
                    {
                        goto Label_0843;
                    }
                    num11 = num13;
                    num12 = num14;
                    num5 = num6;
                    num7 = num8;
                    num6 = num9;
                    num8 = num10;
                }
                line.Type = "Error";
Label_0843:
                H.at(num3, num4) = -2.0;
                if ((list3.count() > 1) && (line.Type != "Error"))
                {
                    line.Value = W;
                    line.PointList = list3;
                    list.push_back(line);
                }
Label_088D:
                num4++;
            }
        }
        for (num3 = 1; num3 < (length - 1); num3++)
        {
            for (num4 = 1; num4 < (num2 - 2); num4++)
            {
                if (S.at(num3, num4) == -2.0)
                {
                    continue;
                }
                QList<PointD> list4;// = new List<PointD>();
                num6 = num3;
                num8 = num4;
                num11 = X[num8] + (S.at(num3, num4) * nx);
                num12 = Y[num3];
                num7 = num8;
                num5 = -1;
                num15 = num11;
                num16 = num12;
                // 			td = new PointD {
                // 				X = num11,
                // 					Y = num12
                // 			};
                td.PointDValue(num11,num12);
                list4.push_back(td);
                // 			line = new PolyLine {
                // 				Type = "Close"
                // 			};
                line.Type="Close";
                while (true)
                {
                    if (!TraceIsoline_UndefData(num5, num6,  H,  S, num7, num8, X, Y, nx, ny, num11,  num9,  num10,  num13,  num14,  isS))
                    {
                        break;
                    }
                    // 				td = new PointD {
                    // 					X = num13,
                    // 						Y = num14
                    // 				};
                    td.PointDValue(num13,num14);
                    list4.push_back(td);
                    if ((abs(num14 - num16) < 1E-06) && (abs(num13 - num15) < 1E-06))
                    {
                        goto Label_09D8;
                    }
                    num11 = num13;
                    num12 = num14;
                    num5 = num6;
                    num7 = num8;
                    num6 = num9;
                    num8 = num10;
                }
                line.Type = "Error";
Label_09D8:
                S.at(num3, num4) = -2.0;
                if ((list4.count() > 1) && (line.Type != "Error"))
                {
                    line.Value = W;
                    line.PointList = list4;
                    list.push_back(line);
                }
            }
        }

        return list;
    }


//private
    QList<PolyLine> PolyLinePoint::CreateContourLines_UndefData(double** S0, const QList<double> &X, const QList<double> &Y, int nc, const QList<double> &contour, double nx, double ny, C2DimenArray<int> &S1, double undefData, QList<Border> &borders)
    {
        if (contour.count() == 0)
        {
            return QList<PolyLine>();
        }

        int num3;
        int num4;
        QList<PolyLine> list ;//= new QList<PolyLine>();
        QList<PolyLine> list2 ;// = new QList<PolyLine>();
        //	int length = S0.GetLength(0);
        //	int num2 = S0.GetLength(1);
        int length = Y.count();
        int num2 = X.count();

        double num5 = contour[0] * 1E-05;
        if (num5 == 0.0)
        {
            num5 = 1E-05;
        }
        for (num3 = 0; num3 < length; num3++)
        {
            num4 = 0;
            while (num4 < num2)
            {
                if (!DoubleEquals(S0[num3][ num4], undefData))
                {
                    S0[num3][ num4] += num5;
                }
                num4++;
            }
        }

        C3DimenArray<int> sB(2, length, num2 - 1);


        C3DimenArray<int> hB(2, length-1, num2);


        for (num3 = 0; num3 < length; num3++)
        {
            num4 = 0;
            while (num4 < num2)
            {
                if (num4 < (num2 - 1))
                {
                    //sB[0][ num3][ num4] = -1;
                    //sB[1][ num3][ num4] = -1;
                    sB.at(0, num3, num4) = -1;
                    sB.at(1, num3, num4) = -1;
                }
                if (num3 < (length - 1))
                {
                    //hB[0][ num3][ num4] = -1;
                    //hB[1][ num3][ num4] = -1;
                    hB.at(0, num3, num4) = -1;
                    hB.at(1, num3, num4) = -1;
                }
                num4++;
            }
        }

        QList<IJPoint> ijPointList;// = new QList<IJPoint>();
        num3 = 0;
        while (num3 < borders.count())
        {
            Border border = borders.at(num3);
            num4 = 0;
            while (num4 < border.getCount())//border.LineNum
            {

                QList<IJPoint> ijPointList = border.LineList.at(num4).ijPointList;
                for (int j = 0; j < (ijPointList.count() - 1); j++)
                {
                    int num7;
                    int num8;
                    IJPoint point = ijPointList.at(j);
                    IJPoint point2 = ijPointList.at(j + 1);
                    if (point.I == point2.I)
                    {
                        num7 = point.I;
                        num8 = qMin(point.J, point2.J);

                        // sB[0][ num7][ num8] = num3;
                        sB.at(0, num7, num8) = num3;
                        if (point2.J > point.J)
                        {
                            // sB[1][num7][ num8] = 1;
                            sB.at(1, num7, num8) = 1;

                        }
                        else
                        {
                            // sB[1][num7][ num8] = 0;
                            sB.at(1, num7, num8) = 0;
                        }
                    }
                    else
                    {
                        num8 = point.J;
                        num7 = qMin(point.I, point2.I);
                        // hB[0][ num7][ num8] = num3;
                        hB.at(0, num7, num8) = num3;
                        // qDebug() << num7 << " " << num8 << endl;
                        if (point2.I > point.I)
                        {
                            // hB[1][num7][ num8] = 0;
                            hB.at(1, num7, num8) = 0;
                        }
                        else
                        {
                            // hB[1][num7][ num8] = 1;
                            hB.at(1, num7, num8) = 1;
                        }
                    }
                }
                num4++;
            }
            num3++;
        }

        C2DimenArray<double> s(length, num2-1);

        C2DimenArray<double> h(length-1, num2);

        for (int i = 0; i < nc; i++)
        {
            double w = contour[i];
            num3 = 0;
            while (num3 < length)
            {
                num4 = 0;
                while (num4 < num2)
                {
                    if (num4 < (num2 - 1))
                    {
                        if ((S1.at(num3, num4) != 0) && (S1.at(num3, num4 + 1) != 0))
                        {
                            if (((S0[num3][ num4] - w) * (S0[num3][ num4 + 1] - w)) < 0.0)
                            {
                                s.at(num3, num4) = (w - S0[num3][ num4]) / (S0[num3][ num4 + 1] - S0[num3][ num4]);
                            }
                            else
                            {
                                s.at(num3, num4) = -2.0;
                            }
                        }
                        else
                        {
                            s.at(num3, num4) = -2.0;
                        }
                    }
                    if (num3 < (length - 1))
                    {
                        if ((S1.at(num3, num4) != 0) && (S1.at(num3 + 1, num4) != 0))
                        {
                            if (((S0[num3][ num4] - w) * (S0[num3 + 1][ num4] - w)) < 0.0)
                            {
                                h.at(num3, num4) = (w - S0[num3][ num4]) / (S0[num3 + 1][ num4] - S0[num3][ num4]);
                            }
                            else
                            {
                                h.at(num3, num4) = -2.0;
                            }
                        }
                        else
                        {
                            h.at(num3, num4) = -2.0;
                        }
                    }
                    num4++;
                }
                num3++;
            }
            list2 = Isoline_UndefData(S0, X, Y, w, nx, ny, s, h, sB, hB, list.count());
            list+=list2;
        }

        //QTime timer;
        //timer.start();
        // #pragma omp parallel for
        for (num3 = 0; num3 < borders.count(); num3++)
        {
            BorderLine line = borders.at(num3).LineList.at(0);
            for (num4 = 0; num4 < list.count(); num4++)
            {
                PolyLine &line2 = list[num4];
                if (line2.Type == "Close")
                {
                    const PointD &aPoint = line2.PointList.at(0);
                    if (PointInPolygon(line.pointList, aPoint))
                    {
                        line2.BorderIdx = num3;
                    }
                }
                //list.removeAt(num4);
                //list.insert(num4, line2);
            }
        }
        // qDebug() << timer.elapsed();

        return list;
    }

    QList<PolyLine> PolyLinePoint::wContour_TracingContourLines(double** S0, const QList<double> &X, const QList<double> &Y, int nc, const QList<double> contour, double undefData, QList<Border> borders, C2DimenArray<int> &S1)
    {
        if (X.length()<2 || Y.length()<2)
        {
            QSLOG_ERROR("等值线至少需要两个点！");
            return QList<PolyLine>();
        }

        double nx = X[1] - X[0];
        double ny = Y[1] - Y[0];
        return CreateContourLines_UndefData(S0, X, Y, nc, contour, nx, ny, S1, undefData, borders);
    }


    QList<PolyLine> PolyLinePoint::TracingContourLines(double **gridData, const QList<double> &cValues, const QList<double> &X, const QList<double> &Y, double noData, QList<Border> &borders)
    {
        int length = cValues.count();

        if (X.length()<2 || Y.length()<2)
        {
            QSLOG_ERROR("等值线至少需要两个点！");
            return QList<PolyLine>();
        }

        double num1 = X[1];
        double num2 = X[0];
        double num3 = Y[1];
        double num4 = Y[0];

        C2DimenArray<int> numArray(Y.count(), X.count());
        borders = TracingBorders(gridData, X, Y, numArray, noData);

        QList<PolyLine> lines;
        lines = wContour_TracingContourLines(gridData,X, Y, length, cValues, noData, borders, numArray);

        return lines;

    }

    #pragma endregion 线

    #pragma region 平滑处理
    static double f0(double t)
    {
        return (((0.16666666666666666 * (-t + 1.0)) * (-t + 1.0)) * (-t + 1.0));
    }

    static double f1(double t)
    {
        return (0.16666666666666666 * (((((3.0 * t) * t) * t) - ((6.0 * t) * t)) + 4.0));
    }

    static double f2(double t)
    {
        return (0.16666666666666666 * ((((((-3.0 * t) * t) * t) + ((3.0 * t) * t)) + (3.0 * t)) + 1.0));
    }

    static double f3(double t)
    {
        return (((0.16666666666666666 * t) * t) * t);
    }

    static void fb(double t, double* fs)
    {
        if (fs==NULL || _msize(fs)/sizeof(double)<4)
        {
            return;
        }

        fs[0] = f0(t);
        fs[1] = f1(t);
        fs[2] = f2(t);
        fs[3] = f3(t);
    }

    static void BSpline(QList<PointD> pointList, double t, int i, double &X, double &Y)
    {
        double * fs = new double[4];
        fb(t, fs);
        X = 0.0;
        Y = 0.0;
        for (int j = 0; j < 4; j++)
        {
            PointD td = pointList.at(i + j);
            X += fs[j] * td.X;
            Y += fs[j] * td.Y;
        }
        delete [] fs;
    }

    static QList<PointD> BSplineScanning(QList<PointD> pointList, int sum,float Param=0.05f)
    {

        double x = 0.0;
        double y = 0.0;
        QList<PointD> list; //= new List<PointD>();
        if (sum < 4)
        {
            //		return NULL;
            return list;
        }
        bool flag = false;
        //	PointD td = pointList.get_Item(0);
        PointD td = pointList.at(0);
        PointD td2 = pointList.at(sum - 1);
        if (DoubleEquals(td.X, td2.X) && DoubleEquals(td.Y, td2.Y))
        {
            pointList.removeAt(0);
            pointList.push_back(pointList.at(0));
            pointList.push_back(pointList.at(1));
            pointList.push_back(pointList.at(2));
            pointList.push_back(pointList.at(3));
            pointList.push_back(pointList.at(4));
            pointList.push_back(pointList.at(5));
            pointList.push_back(pointList.at(6));
            flag = true;
        }
        sum = pointList.count();
        for (int i = 0; i < (sum - 3); i++)
        {
            for (float j = 0.0f; j <= 1.0f; j += Param)
            {
                BSpline(pointList, (double) j, i, x, y);
                if (flag)
                {
                    if (i > 3)
                    {
                        td.ClearPointDValue();
                        td.PointDValue(x,y);

                        list.push_back(td);
                    }
                }
                else
                {
                    td.ClearPointDValue();
                    td.PointDValue(x,y);

                    list.push_back(td);
                }
            }
        }
        if (flag)
        {
            list.push_back(list.at(0));
            return list;
        }
        list.insert(0, pointList.at(0));
        list.push_back(pointList.at(pointList.count() - 1));

        return list;
    }

    QList<PolyLine> PolyLinePoint::SmoothLines(const QList<PolyLine> & aLineList)
    {

        QList<PolyLine> list;// = new QList<PolyLine>();
        QList<PointD> pointList;// = new QList<PointD>();
        for (int i = 0; i < aLineList.count(); i++)
        {
            PolyLine line = aLineList.at(i);
            double num1 = line.Value;
            // std::string type = line.Type;
            pointList = line.PointList;//new List<PointD>(line.PointList);
            if (pointList.count() > 1)
            {
                if (pointList.count() == 2)
                {
                    PointD td;
                    PointD td2 = pointList.at(0);
                    PointD td3 = pointList.at(1);
                    //				PointD td3 = pointList.get_Item(1);
                    td.X = ((td3.X - td2.X) / 4.0) + td2.X;
                    td.Y = ((td3.Y - td2.Y) / 4.0) + td2.Y;
                    pointList.insert(1, td);
                    td.ClearPointDValue();
                    td.PointDValue((((td3.X - td2.X) / 4.0) * 3.0) + td2.X,(((td3.Y - td2.Y) / 4.0) * 3.0) + td2.Y);

                    pointList.insert(2, td);
                }
                if (pointList.count() == 3)
                {
                    PointD td4;// = new PointD();
                    PointD td5 = pointList.at(0);
                    PointD td6 = pointList.at(1);
                    td4.X = ((td6.X - td5.X) / 2.0) + td5.X;
                    td4.Y = ((td6.Y - td5.Y) / 2.0) + td5.Y;
                    pointList.insert(1, td4);
                }
                pointList = BSplineScanning(pointList, pointList.count(),Param);
                line.PointList = pointList;
                list.push_back(line);
            }
        }

        return list;
    }

    #pragma endregion 平滑处理

    #pragma region 等值面

    static Extent GetExtent(QList<PointD> pList)
    {
        PointD td = pList.at(0);
        double x = td.X;
        double num3 = td.X;
        double y = td.Y;
        double num4 = td.Y;
        for (int i = 1; i < pList.count(); i++)
        {
            td = pList.at(i);
            if (td.X < x)
            {
                x = td.X;
            }
            if (td.X > num3)
            {
                num3 = td.X;
            }
            if (td.Y < y)
            {
                y = td.Y;
            }
            if (td.Y > num4)
            {
                num4 = td.Y;
            }
        }
        Extent Temp;
        Temp.xMin = x;
        Temp.yMin = y;
        Temp.xMax = num3;
        Temp.yMax = num4;
        return Temp;
    }

    static void AddHoles_Ring(QList<Polygon> &polygonList, QList<QList<PointD>> holeList)
    {
        for (int i = 0; i < holeList.count(); i++)
        {
            QList<PointD> pList = holeList.at(i);
            Extent bExtent = GetExtent(pList);
            for (int j = 0; j < polygonList.count(); j++)
            {
                Polygon polygon = polygonList.at(j);
                if (polygon.Extent.Include(bExtent))
                {
                    bool flag = true;
                    foreach (PointD td, pList)
                    {
                        if (!PointInPolygon(polygon.OutLine.PointList, td))
                        {
                            flag = false;
                            break;
                        }
                    }
                    if (flag)
                    {
                        polygon.AddHole(pList);
                        break;
                    }
                }
            }
        }
    }


    static QList<Polygon> AddPolygonHoles(QList<Polygon> polygonList)
    {
        int num;
        int num2;
        QList<Polygon> list;// = new List<Polygon>();
        for (num = 0; num < polygonList.count(); num++)
        {
            Polygon &polygon = polygonList[num];
            if (!polygon.IsBorder)
            {
                polygon.HoleIndex = 1;
                list.push_back(polygon);
            }
        }
        if (list.count() == 0)
        {
            return polygonList;
        }
        QList<Polygon> list2;// = new List<Polygon>();
        for (num = 1; num < list.count(); num++)
        {
            Polygon &aPolygon = list[num];
            num2 = num - 1;
            while (num2 >= 0)
            {
                Polygon &polygon3 = list[num2];
                if (polygon3.Extent.Include(aPolygon.Extent) && PointInPolygon(polygon3.OutLine.PointList, aPolygon.OutLine.PointList.at(0)))
                {
                    aPolygon.HoleIndex = polygon3.HoleIndex + 1;
                    polygon3.AddHole(aPolygon);
                    break;
                }
                num2--;
            }
        }
        QList<Polygon> list3;// = new List<Polygon>();
        for (num = 0; num < list.count(); num++)
        {
            if (list.at(num).HoleIndex == 1)
            {
                list3.push_back(list.at(num));
            }
        }
        for (num = 0; num < polygonList.count(); num++)
        {
            Polygon &polygon4 = polygonList[num];
            if (polygon4.IsBorder)
            {
                for (num2 = 0; num2 < list3.count(); num2++)
                {
                    Polygon &polygon5 = list3[num2];
                    if (polygon4.Extent.Include(polygon5.Extent) && PointInPolygon(polygon4.OutLine.PointList, polygon5.OutLine.PointList.at(0)))
                    {
                        polygon4.AddHole(polygon5);
                    }
                }
                list2.push_back(polygon4);
            }
        }
        list2.append(list);
        return list2;
    }


    static QList<Polygon> AddPolygonHoles_Ring(QList<Polygon> & polygonList)
    {
        int num;
        int num2;
        QList<Polygon> list;// = new List<Polygon>();
        for (num = 0; num < polygonList.count(); num++)
        {
            Polygon polygon = polygonList.at(num);
            if (!polygon.IsBorder || polygon.IsInnerBorder)
            {
                polygon.HoleIndex = 1;
                list.push_back(polygon);
            }
        }
        if (list.count() == 0)
        {
            return polygonList;
        }
        QList<Polygon> list2;// = new List<Polygon>();
        for (num = 1; num < list.count(); num++)
        {
            Polygon aPolygon = list.at(num);
            num2 = num - 1;
            while (num2 >= 0)
            {
                Polygon polygon3 = list.at(num2);
                if (polygon3.Extent.Include(aPolygon.Extent) && PointInPolygon(polygon3.OutLine.PointList, aPolygon.OutLine.PointList.at(0)))
                {
                    aPolygon.HoleIndex = polygon3.HoleIndex + 1;
                    polygon3.AddHole(aPolygon);
                    break;
                }
                num2--;
            }
        }
        QList<Polygon> list3;// = new List<Polygon>();
        for (num = 0; num < list.count(); num++)
        {
            if (list.at(num).HoleIndex == 1)
            {
                list3.push_back(list.at(num));
            }
        }
        for (num = 0; num < polygonList.count(); num++)
        {
            Polygon polygon4 = polygonList.at(num);
            if (polygon4.IsBorder && !polygon4.IsInnerBorder)
            {
                for (num2 = 0; num2 < list3.count(); num2++)
                {
                    Polygon polygon5 = list3.at(num2);
                    if (polygon4.Extent.Include(polygon5.Extent) && PointInPolygon(polygon4.OutLine.PointList, polygon5.OutLine.PointList.at(0)))
                    {
                        polygon4.AddHole(polygon5);
                    }
                }
                list2.push_back(polygon4);
            }
        }
        list2.append(list);
        return list2;
    }


    static QList<BorderPoint> InsertPoint2Border_Ring(double ** S0, QList<BorderPoint>  & bPList, Border aBorder, QList<int> &pNums)
    {
        QList<BorderPoint> list;// = new List<BorderPoint>();
        QList<BorderPoint> list2;// = new List<BorderPoint>();
        QList<BorderPoint> list3;// = new List<BorderPoint>();
        //	pNums = new int[aBorder.getCount()];
        for (int i = 0; i < aBorder.getCount(); i++)
        {
            BorderPoint point2;
            BorderLine line = aBorder.LineList.at(i);
            list2.clear();
            int num = 0;
            while (num < line.pointList.count())
            {
                BorderPoint point ;
                {
                    point.Id = -1;
                    point.BorderIdx = i;
                    point.Point = line.pointList.at(num);
                    point.Value = S0[line.ijPointList.at(num).I][ line.ijPointList.at(num).J];
                }
                list2.push_back(point);
                num++;
            }
            num = 0;
            while (num < bPList.count())
            {
                //			point2 = bPList.at(num).Clone();
                // /*point2=*/bPList[num].Clone(point2);
                bPList[num] = point2;
                point2.BorderIdx = i;
                PointD td3 = point2.Point;
                //			PointD td = list2.at(0).Point.Clone();
                PointD td;
                td=list2[0].Point;//.Clone();
                for (int j = 1; j < list2.count(); j++)
                {
                    PointD td2;
                    td2=list2[j].Point;//.Clone();
                    if (((((td3.X - td.X) * (td3.X - td2.X)) <= 0.0) && (((td3.Y - td.Y) * (td3.Y - td2.Y)) <= 0.0)) && ((((td3.X - td.X) * (td2.Y - td.Y)) - ((td2.X - td.X) * (td3.Y - td.Y))) == 0.0))
                    {
                        list2.insert(j, point2);
                        break;
                    }
                    td = td2;
                }
                num++;
            }
            list3.clear();
            for (num = 0; num < list2.count(); num++)
            {
                point2 = list2.at(num);
                point2.BInnerIdx = num;
                list3.push_back(point2);
            }

            //pNums[i] = list3.count();
            pNums.insert(i, list3.count());
            list.append(list3);
        }
        return list;
    }

    static QList<BorderPoint> InsertPoint2Border(QList<BorderPoint> bPList, QList<BorderPoint> aBorderList)
    {
        QList<BorderPoint> list;// = new List<BorderPoint>(aBorderList);
        list=aBorderList;
        for (int i = 0; i < bPList.count(); i++)
        {
            BorderPoint point2 = bPList.at(i);
            PointD point = point2.Point;
            PointD td = list.at(0).Point;
            for (int j = 1; j < list.count(); j++)
            {
                PointD td2 = list.at(j).Point;
                if ((DoubleEquals(point.X, td.X) && DoubleEquals(point.Y, td.Y)) || (DoubleEquals(point.X, td2.X) && DoubleEquals(point.Y, td2.Y)))
                {
                    list.insert(j, point2);
                    break;
                }
                if (((((((point.X - td.X) * (point.X - td2.X)) <= 0.0) || DoubleEquals(point.X, td.X)) || DoubleEquals(point.X, td2.X)) && (((((point.Y - td.Y) * (point.Y - td2.Y)) <= 0.0) || DoubleEquals(point.Y, td.Y)) || DoubleEquals(point.Y, td2.Y))) && ((((point.X - td.X) * (td2.Y - td.Y)) - ((td2.X - td.X) * (point.Y - td.Y))) <= 0.001))
                {
                    list.insert(j, point2);
                    break;
                }
                td = td2;
            }
        }
        return list;
    }

    QList<Polygon> PolyLinePoint::TracingPolygons_Ring(const QList<PolyLine> & LineList, const QList<BorderPoint> & borderList, const Border &aBorder, const QList<double> &contour, const QList<int> &pNums)
    {
        PolyLine line;
        PointD td;
        Polygon polygon;
        Extent extent;
        QList<Polygon> list;// = new List<Polygon>();
        QList<PolyLine> list2;// = new List<PolyLine>();
        int index = 0;
        int num2 = 0;
        list2+=LineList;
        //	list2 = new List<PolyLine>(LineList);
        QList<PointD> poly;// = new List<PointD>();
        //	int[] numArray = new int[borderList.get_Count()];
        QList<int> numArray;// = new int[borderList.get_Count()];
        for (index = 0; index < numArray.length(); index++)
        {
            numArray[index] = 0;
        }
        int num3 = 0;
        int num4 = 0;
        int num5 = 0;
        double lowValue = 0.0;
        double num7 = 0.0;
        double num8 = 0.0;
        QList<BorderPoint> list5;// = new List<BorderPoint>();
        int borderIdx = 0;
        int num10 = 0;
        int bInnerIdx = 0;
        num4 = borderList.count();
        for (index = 0; index < num4; index++)
        {
            QList<PointD> list3;
            BorderPoint point;
            BorderPoint point2;
            if (borderList.at(index).Id == -1)
            {
                continue;
            }
            num3 = index;
            list5.push_back(borderList.at(index));
            bool flag = false;
            if (numArray[num3] < 2)
            {
                point = borderList.at(num3);
                bInnerIdx = point.BInnerIdx;
                list3.clear();// = new List<PointD>();
                QList<int> list6;// = new List<int>();
                list3.push_back(point.Point);
                list6.push_back(num3);
                borderIdx = point.BorderIdx;
                num10 = borderIdx;
                num3++;
                bInnerIdx++;
                if (bInnerIdx == (pNums[borderIdx] - 1))
                {
                    num3 -= pNums[borderIdx] - 1;
                }
                num5 = 0;
                while (true)
                {
                    point = borderList.at(num3);
                    if (point.Id == -1)
                    {
                        if (numArray[num3] == 1)
                        {
                            goto Label_047F;
                        }
                        num8 = point.Value;
                        list3.push_back(point.Point);
                        numArray[num3]++;
                        list6.push_back(num3);
                    }
                    else
                    {
                        if (numArray[num3] == 2)
                        {
                            goto Label_047F;
                        }
                        numArray[num3]++;
                        list6.push_back(num3);
                        line = list2.at(point.Id);
                        if (num5 == 0)
                        {
                            lowValue = line.Value;
                            num7 = line.Value;
                            num5++;
                        }
                        else if (lowValue == num7)
                        {
                            if (line.Value > lowValue)
                            {
                                num7 = line.Value;
                            }
                            else if (line.Value < lowValue)
                            {
                                lowValue = line.Value;
                            }
                            num5++;
                        }
                        //					poly = new List<PointD>(line.PointList);
                        poly.clear();
                        poly = line.PointList;// = new List<PointD>(line.PointList);
                        td = poly.at(0);
                        if ((point.Point.X != td.X) || (point.Point.Y != td.Y))
                        {
                            //						poly.Reverse();
                            ReverveQList<PointD>(poly);
                        }
                        //					list3.AddRange(poly);
                        list3+=poly;
                        num2 = 0;
                        while (num2 < borderList.count())
                        {
                            if (num2 != num3)
                            {
                                point2 = borderList.at(num2);
                                if (point2.Id == point.Id)
                                {
                                    num3 = num2;
                                    bInnerIdx = point2.BInnerIdx;
                                    numArray[num3]++;
                                    list6.push_back(num3);
                                    num10 = point2.BorderIdx;
                                    if ((point.BorderIdx > 0) && (point.BorderIdx == point2.BorderIdx))
                                    {
                                        flag = true;
                                    }
                                    break;
                                }
                            }
                            num2++;
                        }
                    }
                    if (num3 == index)
                    {
                        if (list3.count() > 0)
                        {
                            if (flag)
                            {
                                bool flag2 = false;
                                int num12 = 0;
                                for (int i = 0; i < point.BorderIdx; i++)
                                {
                                    num12 += pNums[i];
                                }
                                int num14 = num12;
                                int num15 = num12 + pNums[point.BorderIdx];
                                int num16 = num14;
                                for (int j = num14; j < num15; j++)
                                {
                                    if (!list6.contains(j))
                                    {
                                        num16 = j;
                                        break;
                                    }
                                }
                                if (PointInPolygon(list3, borderList.at(num16).Point))
                                {
                                    flag2 = true;
                                }
                                if (flag2)
                                {
                                    break;
                                }
                            }
                            // 						polygon = new Polygon {
                            // 							IsBorder = true,
                            // 								IsInnerBorder = flag,
                            // 								LowValue = lowValue,
                            // 								HighValue = num7
                            // 						};
                            {
                                polygon.ClearPolygon();
                                polygon.IsBorder = true;
                                polygon.IsInnerBorder = flag;
                                polygon.LowValue = lowValue;
                                polygon.HighValue = num7;
                            }
                            extent.ClearExtent();;// = new Extent();
                            polygon.Area = GetExtentAndArea(list3, extent);
                            polygon.IsClockWise = true;
                            polygon.StartPointIdx = list5.count() - 1;
                            polygon.Extent = extent;
                            polygon.OutLine.PointList = list3;
                            polygon.OutLine.Value = lowValue;
                            polygon.IsHighCenter = true;
                            if ((lowValue == num7) && (num8 < lowValue))
                            {
                                polygon.IsHighCenter = false;
                            }
                            polygon.OutLine.Type = "Border";
                            //						polygon.HoleLines = new List<PolyLine>();
                            polygon.HoleLines.clear();
                            list.push_back(polygon);
                        }
                        break;
                    }
                    num3++;
                    bInnerIdx++;
                    if (borderIdx != num10)
                    {
                        borderIdx = num10;
                    }
                    if (bInnerIdx == (pNums[borderIdx] - 1))
                    {
                        num3 -= pNums[borderIdx] - 1;
                        bInnerIdx = 0;
                    }
                }
            }
Label_047F:
            flag = false;
            num3 = index;
            if (numArray[num3] >= 2)
            {
                continue;
            }
            list3.clear();// = new List<PointD>();
            QList<int> list7;// = new List<int>();
            point = borderList.at(num3);
            bInnerIdx = point.BInnerIdx;
            list3.push_back(point.Point);
            list7.push_back(num3);
            borderIdx = point.BorderIdx;
            num10 = borderIdx;
            num3 += -1;
            bInnerIdx += -1;
            if (bInnerIdx == -1)
            {
                num3 += pNums[borderIdx] - 1;
            }
            num5 = 0;
            while (true)
            {
                point = borderList.at(num3);
                if (point.Id == -1)
                {
                    if (numArray[num3] == 1)
                    {
                        goto Label_0844;
                    }
                    num8 = point.Value;
                    list3.push_back(point.Point);
                    numArray[num3]++;
                    list7.push_back(num3);
                }
                else
                {
                    if (numArray[num3] == 2)
                    {
                        goto Label_0844;
                    }
                    numArray[num3]++;
                    list7.push_back(num3);
                    line = list2.at(point.Id);
                    if (num5 == 0)
                    {
                        lowValue = line.Value;
                        num7 = line.Value;
                        num5++;
                    }
                    else if (lowValue == num7)
                    {
                        if (line.Value > lowValue)
                        {
                            num7 = line.Value;
                        }
                        else if (line.Value < lowValue)
                        {
                            lowValue = line.Value;
                        }
                        num5++;
                    }
                    poly.clear(); //= new List<PointD>(line.PointList);
                    poly = line.PointList;
                    td = poly.at(0);
                    if ((point.Point.X != td.X) || (point.Point.Y != td.Y))
                    {
                        //					poly.Reverse();
                        ReverveQList<PointD>(poly);
                    }
                    list3+=poly;
                    num2 = 0;
                    while (num2 < borderList.count())
                    {
                        if (num2 != num3)
                        {
                            point2 = borderList.at(num2);
                            if (point2.Id == point.Id)
                            {
                                num3 = num2;
                                bInnerIdx = point2.BInnerIdx;
                                numArray[num3]++;
                                list7.push_back(num3);
                                num10 = point2.BorderIdx;
                                if ((point.BorderIdx > 0) && (point.BorderIdx == point2.BorderIdx))
                                {
                                    flag = true;
                                }
                                break;
                            }
                        }
                        num2++;
                    }
                }
                if (num3 == index)
                {
                    if (list3.count() <= 0)
                    {
                        continue;
                    }
                    if (flag)
                    {
                        bool flag3 = false;
                        int num18 = 0;
                        for (int k = 0; k < point.BorderIdx; k++)
                        {
                            num18 += pNums[k];
                        }
                        int num20 = num18;
                        int num21 = num18 + pNums[point.BorderIdx];
                        int num22 = num20;
                        for (int m = num20; m < num21; m++)
                        {
                            if (!list7.contains(m))
                            {
                                num22 = m;
                                break;
                            }
                        }
                        if (PointInPolygon(list3, borderList.at(num22).Point))
                        {
                            flag3 = true;
                        }
                        if (flag3)
                        {
                            continue;
                        }
                    }
                    //polygon = new Polygon {
                    //	IsBorder = true,
                    //		IsInnerBorder = flag,
                    //		LowValue = lowValue,
                    //		HighValue = num7
                    //};
                    {
                        polygon.ClearPolygon();
                        polygon.IsBorder = true;
                        polygon.IsInnerBorder = flag;
                        polygon.LowValue = lowValue;
                        polygon.HighValue = num7;
                    }
                    extent.ClearExtent();// = new Extent();
                    polygon.Area = GetExtentAndArea(list3, extent);
                    polygon.IsClockWise = false;
                    polygon.StartPointIdx = list5.count() - 1;
                    polygon.Extent = extent;
                    polygon.OutLine.PointList = list3;
                    polygon.OutLine.Value = lowValue;
                    polygon.IsHighCenter = true;
                    if ((lowValue == num7) && (num8 < lowValue))
                    {
                        polygon.IsHighCenter = false;
                    }
                    polygon.OutLine.Type = "Border";
                    polygon.HoleLines.clear();// = new List<PolyLine>();
                    list.push_back(polygon);
                    continue;
                }
                num3 += -1;
                bInnerIdx += -1;
                if (borderIdx != num10)
                {
                    borderIdx = num10;
                }
                if (bInnerIdx == -1)
                {
                    num3 += pNums[borderIdx];
                    bInnerIdx = pNums[borderIdx] - 1;
                }
            }
Label_0844:
            ;
        }
        QList<Polygon> list8;// = new List<Polygon>();
        bool flag4 = false;
        for (index = 0; index < list2.count(); index++)
        {
            line = list2.at(index);
            if (line.Type == "Close")
            {
                // 			polygon = new Polygon {
                // 				IsBorder = false,
                // 					LowValue = line.Value,
                // 					HighValue = line.Value
                // 			};
                {
                    polygon.ClearPolygon();
                    polygon.IsBorder = false;
                    polygon.LowValue = line.Value;
                    polygon.HighValue = line.Value;
                }
                extent.ClearExtent();// = new Extent();
                polygon.Area = GetExtentAndArea(line.PointList,  extent);
                polygon.IsClockWise = IsClockwise(line.PointList);
                polygon.Extent = extent;
                polygon.OutLine = line;
                polygon.IsHighCenter = true;
                polygon.HoleLines.clear();// = new List<PolyLine>();
                flag4 = false;
                num2 = 0;
                while (num2 < list8.count())
                {
                    if (polygon.Area > list8.at(num2).Area)
                    {
                        list8.insert(num2, polygon);
                        flag4 = true;
                        break;
                    }
                    num2++;
                }
                if (!flag4)
                {
                    list8.push_back(polygon);
                }
            }
        }
        if (list.count() == 0)
        {
            // 		line = new PolyLine {
            // 			Type = "Border",
            // 				Value = contour[0],
            // 				PointList = new List<PointD>(aBorder.LineList.at(0).pointList)
            // 		};
            {
                line.ClearPolyLineValue();
                line.Type="Border";
                line.Value = contour[0];
                line.PointList.clear();
                line.PointList=aBorder.LineList.at(0).pointList;
            }
            if (line.PointList.count() > 0)
            {
                // 			polygon = new Polygon {
                // 				LowValue = line.Value,
                // 					HighValue = line.Value
                // 			};
                {
                    polygon.ClearPolygon();
                    polygon.LowValue = line.Value;
                    polygon.HighValue = line.Value;
                }
                extent.ClearExtent();// = new Extent();
                polygon.Area = GetExtentAndArea(line.PointList, extent);
                polygon.IsClockWise = IsClockwise(line.PointList);
                polygon.Extent = extent;
                polygon.OutLine = line;
                polygon.IsHighCenter = false;
                list.push_back(polygon);
            }
        }

        //	list.AddRange(list8);
        list+=list8;
        for (index = list.count() - 1; index >= 0; index += -1)
        {
            polygon = list.at(index);
            if (polygon.OutLine.Type == "Close")
            {
                Extent extent2 = polygon.Extent;
                lowValue = polygon.LowValue;
                td = polygon.OutLine.PointList.at(0);
                for (num2 = index - 1; num2 >= 0; num2 += -1)
                {
                    Polygon polygon2 = list.at(num2);
                    Extent extent3 = polygon2.Extent;
                    num7 = polygon2.LowValue;
                    {
                        poly.clear();// new List<PointD>(polygon2.OutLine.PointList);
                        poly=polygon2.OutLine.PointList;
                    }
                    if (PointInPolygon(poly, td) && ((((extent2.xMin > extent3.xMin) & (extent2.yMin > extent3.yMin)) & (extent2.xMax < extent3.xMax)) & (extent2.yMax < extent3.yMax)))
                    {
                        if (lowValue < num7)
                        {
                            polygon.IsHighCenter = false;
                        }
                        else if ((lowValue == num7) && polygon2.IsHighCenter)
                        {
                            polygon.IsHighCenter = false;
                        }
                        break;
                    }
                }
            }
        }
        return list;
    }

    static QList<Polygon> JudgePolygonHighCenter(QList<Polygon> &borderPolygons, const QList<Polygon> &closedPolygons,const QList<PolyLine> &aLineList,const QList<BorderPoint> &borderList)
    {
        Polygon polygon;
        QList<PointD> poly;// = new List<PointD>();
        double lowValue = 0.0;
        double num4 = 0.0;
        if (borderPolygons.count() == 0)
        {
            double num5 = aLineList.at(0).Value;
            double num6 = aLineList.at(0).Value;
            foreach (PolyLine line2 ,aLineList)
            {
                if (line2.Value > num5)
                {
                    num5 = line2.Value;
                }
                if (line2.Value < num6)
                {
                    num6 = line2.Value;
                }
            }
            //		polygon = new Polygon();
            polygon.ClearPolygon();
            // 		PolyLine line = new PolyLine {
            // 			Type = "Border",
            // 				Value = num6
            // 		};
            PolyLine line;
            {
                line.Type = "Border";
                line.Value = num6;
            }
            polygon.IsHighCenter = false;
            if ((closedPolygons.count() > 0) && (borderList.at(0).Value >= closedPolygons.at(0).LowValue))
            {
                line.Value = num5;
                polygon.IsHighCenter = true;
            }
            poly.clear();
            foreach (BorderPoint point , borderList)
            {
                poly.push_back(point.Point);
            }
            line.PointList.clear();// = new List<PointD>(poly);
            line.PointList+=poly;
            if (line.PointList.count() > 0)
            {
                polygon.IsBorder = true;
                polygon.LowValue = line.Value;
                polygon.HighValue = line.Value;
                Extent aExtent;
                aExtent.ClearExtent();// = new Extent();
                polygon.Area = GetExtentAndArea(line.PointList,  aExtent);
                polygon.IsClockWise = IsClockwise(line.PointList);
                polygon.Extent = aExtent;
                polygon.OutLine = line;
                polygon.HoleLines.clear();// = new List<PolyLine>();
                borderPolygons.push_back(polygon);
            }
        }
        borderPolygons+=closedPolygons;
        int num7 = borderPolygons.count();

        for (int i = 1; i < num7; i++)
        {
            Polygon &polygon1 = borderPolygons[i];
            if (polygon1.OutLine.Type == "Close")
            {

                Extent & extent = polygon1.Extent;

                lowValue = polygon1.LowValue;

                const PointD & aPoint = polygon1.OutLine.PointList.at(0);
                for (int j = i - 1; j >= 0; j--)
                {
                    Polygon &polygon2 = borderPolygons[j];
                    Extent & extent3 = polygon2.Extent;
                    num4 = polygon2.LowValue;
                    const QList<PointD> &poly1=polygon2.OutLine.PointList;
                    if (((PointInPolygon(poly1, aPoint) && (extent.xMin > extent3.xMin)) && ((extent.yMin > extent3.yMin) && (extent.xMax < extent3.xMax))) && (extent.yMax < extent3.yMax))
                    {
                        if (lowValue < num4)
                        {
                            polygon1.IsHighCenter = false;
                        }
                        else if ((lowValue == num4) && polygon2.IsHighCenter)
                        {
                            polygon1.IsHighCenter = false;
                        }
                        break;
                    }
                }
            }
        }
        return borderPolygons;
    }



    static QList<Polygon> wPolyContour_TracingPolygons(const QList<PolyLine> &LineList, const QList<BorderPoint> & borderList)
    {
        PolyLine line;
        Polygon polygon;
        Extent extent;
        int num;
        int num2;
        if (LineList.count() == 0)
        {
            QList<Polygon> returnqlist;
            return returnqlist;
            //		return new QList<Polygon>();
        }
        QList<Polygon> borderPolygons;// = new List<Polygon>();
        QList<PolyLine> aLineList;// = new List<PolyLine>();
        aLineList.clear();// = new List<PolyLine>(LineList);
        aLineList=LineList;
        QList<PointD> list4;// = new List<PointD>();
        QList<int> numArray;
        for (int k = 0; k < borderList.count() - 1; ++k)
        {
            numArray.push_back(0);
        }
        // numArray.reserve(borderList.count() - 1);// = new int[borderList.count() - 1];
        // 	for (num = 0; num < numArray.Length; num++)
        // 	{
        // 		numArray[num] = 0;
        // 	}
        double num6 = 0.0;
        double num7 = 0.0;
        double num8 = 0.0;
        QList<BorderPoint> list5;// = new List<BorderPoint>();
        int num4 = borderList.count() - 1;
        for (num = 0; num < num4; num++)
        {
            PointD td;
            td.ClearPointDValue();
            BorderPoint point;
            point.ClearBorderPoint();
            if (borderList.at(num).Id == -1)
            {
                continue;
            }
            int index = num;
            QList<PointD> pList;// = new List<PointD>();
            list5.push_back(borderList.at(num));
            if (numArray[index] >= 2)
            {
                goto Label_0338;
            }
            pList.push_back(borderList.at(index).Point);
            index++;
            if (index == num4)
            {
                index = 0;
            }
            int num5 = 0;
Label_00E8:
            point = borderList.at(index);
            if (point.Id == -1)
            {
                if (numArray[index] == 1)
                {
                    goto Label_0338;
                }
                num8 = point.Value;
                pList.push_back(point.Point);
                numArray[index]++;
            }
            else
            {
                if (numArray[index] == 2)
                {
                    goto Label_0338;
                }
                numArray[index]++;
                line = aLineList.at(point.Id);
                if (num5 == 0)
                {
                    num6 = line.Value;
                    num7 = line.Value;
                    num5++;
                }
                else if (num6 == num7)
                {
                    if (line.Value > num6)
                    {
                        num7 = line.Value;
                    }
                    else if (line.Value < num6)
                    {
                        num6 = line.Value;
                    }
                    num5++;
                }
                list4.clear();// = new List<PointD>(line.PointList);
                list4=line.PointList;
                td = list4.at(0);
                if ((point.Point.X != td.X) || (point.Point.Y != td.Y))
                {
                    //				list4.Reverse();
                    ReverveQList<PointD>(list4);
                }
                pList += list4;
                num2 = 0;
                while (num2 < (borderList.count() - 1))
                {
                    if ((num2 != index) && (borderList.at(num2).Id == point.Id))
                    {
                        index = num2;
                        numArray[index]++;
                        break;
                    }
                    num2++;
                }
            }
            if (index == num)
            {
                if (pList.count() > 0)
                {
                    // 				polygon = new Polygon {
                    // 					IsBorder = true,
                    // 						LowValue = num6,
                    // 						HighValue = num7
                    // 				};
                    {
                        polygon.ClearPolygon();
                        polygon.IsBorder = true;
                        polygon.LowValue = num6;
                        polygon.HighValue = num7;
                    }
                    extent.ClearExtent();// = new Extent();
                    polygon.Area = GetExtentAndArea(pList,  extent);
                    polygon.IsClockWise = true;
                    polygon.StartPointIdx = list5.count() - 1;
                    polygon.Extent = extent;
                    polygon.OutLine.PointList = pList;
                    polygon.OutLine.Value = num6;
                    polygon.IsHighCenter = true;
                    polygon.HoleLines.clear();// = new List<PolyLine>();
                    if ((num6 == num7) && (num8 < num6))
                    {
                        polygon.IsHighCenter = false;
                    }
                    polygon.OutLine.Type = "Border";
                    borderPolygons.push_back(polygon);
                }
            }
            else
            {
                index++;
                if (index == num4)
                {
                    index = 0;
                }
                goto Label_00E8;
            }
Label_0338:
            index = num;
            if (numArray[index] >= 2)
            {
                continue;
            }
            pList.clear();// = new List<PointD>();
            pList.push_back(borderList.at(index).Point);
            index += -1;
            if (index == -1)
            {
                index = num4 - 1;
            }
            num5 = 0;
Label_0376:
            point = borderList.at(index);
            if (point.Id == -1)
            {
                if (numArray[index] == 1)
                {
                    goto Label_05C8;
                }
                num8 = point.Value;
                pList.push_back(point.Point);
                numArray[index]++;
            }
            else
            {
                if (numArray[index] == 2)
                {
                    goto Label_05C8;
                }
                numArray[index]++;
                line = aLineList.at(point.Id);
                if (num5 == 0)
                {
                    num6 = line.Value;
                    num7 = line.Value;
                    num5++;
                }
                else if (num6 == num7)
                {
                    if (line.Value > num6)
                    {
                        num7 = line.Value;
                    }
                    else if (line.Value < num6)
                    {
                        num6 = line.Value;
                    }
                    num5++;
                }
                list4.clear();// = new List<PointD>(line.PointList);
                list4=line.PointList;
                td = list4.at(0);
                if ((point.Point.X != td.X) || (point.Point.Y != td.Y))
                {
                    //				list4.Reverse();
                    ReverveQList<PointD>(list4);
                }
                pList += list4;
                num2 = 0;
                while (num2 < (borderList.count() - 1))
                {
                    if ((num2 != index) && (borderList.at(num2).Id == point.Id))
                    {
                        index = num2;
                        numArray[index]++;
                        break;
                    }
                    num2++;
                }
            }
            if (index == num)
            {
                if (pList.count() > 0)
                {
                    // 				polygon = new Polygon {
                    // 					IsBorder = true,
                    // 						LowValue = num6,
                    // 						HighValue = num7
                    // 				};
                    {
                        polygon.ClearPolygon();
                        polygon.IsBorder = true;
                        polygon.LowValue = num6;
                        polygon.HighValue = num7;
                    }
                    extent.ClearExtent();// = new Extent();
                    polygon.Area = GetExtentAndArea(pList,  extent);
                    polygon.IsClockWise = false;
                    polygon.StartPointIdx = list5.count() - 1;
                    polygon.Extent = extent;
                    polygon.OutLine.PointList = pList;
                    polygon.OutLine.Value = num6;
                    polygon.IsHighCenter = true;
                    polygon.HoleLines.clear();// = new List<PolyLine>();
                    if ((num6 == num7) && (num8 < num6))
                    {
                        polygon.IsHighCenter = false;
                    }
                    polygon.OutLine.Type = "Border";
                    borderPolygons.push_back(polygon);
                }
            }
            else
            {
                index += -1;
                if (index == -1)
                {
                    index = num4 - 1;
                }
                goto Label_0376;
            }
Label_05C8:
            ;
        }
        QList<Polygon> closedPolygons;// = new List<Polygon>();
        for (num = 0; num < aLineList.count(); num++)
        {
            line = aLineList.at(num);
            if ((line.Type == "Close") && (line.PointList.count() > 0))
            {
                // 			polygon = new Polygon {
                // 				IsBorder = false,
                // 					LowValue = line.Value,
                // 					HighValue = line.Value
                // 			};
                {
                    polygon.ClearPolygon();
                    polygon.IsBorder = false;
                    polygon.LowValue = line.Value;
                    polygon.HighValue = line.Value;
                }
                extent.ClearExtent();// = new Extent();
                polygon.Area = GetExtentAndArea(line.PointList,  extent);
                polygon.IsClockWise = IsClockwise(line.PointList);
                polygon.Extent = extent;
                polygon.OutLine = line;
                polygon.IsHighCenter = true;
                polygon.HoleLines.clear();// = new List<PolyLine>();
                bool flag = false;
                for (num2 = 0; num2 < closedPolygons.count(); num2++)
                {
                    if (polygon.Area > closedPolygons.at(num2).Area)
                    {
                        closedPolygons.insert(num2, polygon);
                        flag = true;
                        break;
                    }
                }
                if (!flag)
                {
                    closedPolygons.push_back(polygon);
                }
            }
        }
        return JudgePolygonHighCenter(borderPolygons, closedPolygons, aLineList, borderList);
    }


    QList<Polygon> PolyLinePoint::wContour_TracingPolygons(double** &S0, const QList<PolyLine> &cLineList, QList<Border> &borderList, const QList<double> &contour)
    {
        QList<Polygon> polygonList;
        //	= new List<Polygon>();
        QList<Polygon> list2;
        //	= new List<Polygon>();
        QList<BorderPoint> list3;
        //	= new List<BorderPoint>();
        QList<BorderPoint> bPList;
        //	= new List<BorderPoint>();
        QList<PointD> pointList;
        //	= new List<PointD>();
        QList<PolyLine> lineList;
        //	= new List<PolyLine>();
        QList<BorderPoint> aBorderList;
        //	= new List<BorderPoint>();
        double num3 = 0.0;
        for (int i = 0; i < borderList.count(); i++)
        {
            PointD td;
            BorderPoint point;
            int num2;
            PolyLine line2;
            Polygon polygon;
            IJPoint point2;
            aBorderList.clear();
            bPList.clear();
            lineList.clear();
            polygonList.clear();
            Border aBorder = borderList.at(i);
            BorderLine line = aBorder.LineList.at(0);
            pointList = line.pointList;
            if (!IsClockwise(pointList))
            {
                ReverveQList<PointD>(pointList);
            }
            if (aBorder.getCount() == 1)
            {
                num2 = 0;
                while (num2 < pointList.count())
                {
                    td.ClearPointDValue();
                    td = pointList.at(num2);
                    point.ClearBorderPoint();

                    point.Id = -1;
                    point.Point = td;
                    point.Value = S0[line.ijPointList.at(num2).I][line.ijPointList.at(num2).J];

                    aBorderList.push_back(point);
                    num2++;
                }
                num2 = 0;
                while (num2 < cLineList.count())
                {
                    line2 = cLineList.at(num2);
                    if (line2.BorderIdx == i)
                    {
                        lineList.push_back(line2);
                        if (line2.Type == "Border")
                        {
                            td.ClearPointDValue();
                            td = line2.PointList.at(0);

                            point.ClearBorderPoint();
                            {
                                point.Id = lineList.count() - 1;
                                point.Point = td;
                                point.Value = line2.Value;
                            }

                            bPList.push_back(point);

                            td.ClearPointDValue();
                            td = line2.PointList.at(line2.PointList.count() - 1);

                            point.ClearBorderPoint();
                            {
                                point.Id = lineList.count() - 1;
                                point.Point = td;
                                point.Value = line2.Value;
                            };
                            bPList.push_back(point);
                        }
                    }
                    num2++;
                }


                if (lineList.count() == 0)
                {
                    point2 = line.ijPointList.at(0);
                    polygon.ClearPolygon();
                    //				polygon = new Polygon();
                    if (S0[point2.I][ point2.J] < contour[0])
                    {
                        num3 = contour[0];
                        polygon.IsHighCenter = false;
                    }
                    else
                    {
                        num2 = contour.length() - 1;
                        while (num2 >= 0)
                        {
                            if (S0[point2.I][ point2.J] > contour[num2])
                            {
                                num3 = contour[num2];
                                break;
                            }
                            num2--;
                        }
                        polygon.IsHighCenter = true;
                    }
                    if (pointList.count() > 0)
                    {
                        polygon.IsBorder = true;
                        polygon.HighValue = num3;
                        polygon.LowValue = num3;
                        polygon.Extent.InitExtent();
                        polygon.Area = GetExtentAndArea(pointList, polygon.Extent);
                        polygon.StartPointIdx = 0;
                        polygon.IsClockWise = true;
                        polygon.OutLine.Type = "Border";
                        polygon.OutLine.Value = num3;
                        polygon.OutLine.BorderIdx = i;
                        polygon.OutLine.PointList = pointList;
                        polygon.HoleLines;
                        //= new List<PolyLine>();
                        polygonList.push_back(polygon);
                    }
                }
                else
                {
                    list3 = InsertPoint2Border(bPList, aBorderList);
                    polygonList = wPolyContour_TracingPolygons(lineList, list3);
                }
                polygonList = AddPolygonHoles(polygonList);
            }
            else
            {
                line = aBorder.LineList.at(0);
                num2 = 0;
                while (num2 < cLineList.count())
                {
                    line2 = cLineList.at(num2);
                    if (line2.BorderIdx == i)
                    {
                        lineList.push_back(line2);
                        if (line2.Type == "Border")
                        {
                            td.ClearPointDValue();
                            td = line2.PointList.at(0);

                            point.ClearBorderPoint();
                            {
                                point.Id = lineList.count() - 1;
                                point.Point = td;
                                point.Value = line2.Value;
                            }
                            bPList.push_back(point);
                            td = line2.PointList.at(line2.PointList.count() - 1);
                            point.ClearBorderPoint();
                            point.Id = lineList.count() - 1;
                            point.Point = td;
                            point.Value = line2.Value;

                            bPList.push_back(point);
                        }
                    }
                    num2++;
                }
                if (lineList.count() == 0)
                {
                    point2 = line.ijPointList.at(0);
                    polygon.ClearPolygon();
                    //				polygon = new Polygon();
                    if (S0[point2.I][ point2.J] < contour[0])
                    {
                        num3 = contour[0];
                        polygon.IsHighCenter = false;
                    }
                    else
                    {
                        num2 = contour.length() - 1;
                        while (num2 >= 0)
                        {
                            if (S0[point2.I][ point2.J] > contour[num2])
                            {
                                num3 = contour[num2];
                                break;
                            }
                            num2--;
                        }
                        polygon.IsHighCenter = true;
                    }
                    if (pointList.count() > 0)
                    {
                        polygon.IsBorder = true;
                        polygon.HighValue = num3;
                        polygon.LowValue = num3;
                        polygon.Area = GetExtentAndArea(pointList,  polygon.Extent);
                        polygon.StartPointIdx = 0;
                        polygon.IsClockWise = true;
                        polygon.OutLine.Type = "Border";
                        polygon.OutLine.Value = num3;
                        polygon.OutLine.BorderIdx = i;
                        polygon.OutLine.PointList = pointList;
                        polygon.HoleLines ;//= new List<PolyLine>();
                        polygonList.push_back(polygon);
                    }
                }
                else
                {
                    QList<int> pNums;// = new int[aBorder.getCount()];
                    list3 = InsertPoint2Border_Ring(S0, bPList, aBorder,  pNums);
                    polygonList = TracingPolygons_Ring(lineList, list3, aBorder, contour, pNums);

                    QList<Polygon> list8;
                    while (polygonList.count() > 0)
                    {
                        bool flag = false;
                        num2 = 0;
                        while (num2 < list8.count())
                        {
                            if (polygonList.at(0).Area > list8.at(num2).Area)
                            {
                                list8.push_back(polygonList.at(0));
                                flag = true;
                                break;
                            }
                            num2++;
                        }
                        if (!flag)
                        {
                            list8.push_back(polygonList.at(0));
                        }
                        polygonList.removeAt(0);
                    }
                    polygonList = list8;
                }
                QList<QList<PointD>> holeList;// = new List<List<PointD>>();
                for (num2 = 0; num2 < aBorder.getCount(); num2++)
                {
                    holeList.push_back(aBorder.LineList.at(num2).pointList);
                }
                if (holeList.count() > 0)
                {
                    AddHoles_Ring(polygonList, holeList);
                }
                polygonList = AddPolygonHoles_Ring(polygonList);
            }
            list2.append(polygonList);
            //		list2.AddRange(polygonList);
        }
        foreach (Polygon polygon2 , list2)
        {
            if (!IsClockwise(polygon2.OutLine.PointList))
            {
                //			polygon2.OutLine.PointList.Reverse();
                ReverveQList(polygon2.OutLine.PointList);
            }
        }
        return list2;
    }

}
void QCounter::PolyLinePoint::PolyMaxMin(double ** S, const QList<double> &X, const QList<double> &Y,const QList<double> &cValues,double noData,QList<QCounter::Polygon> &polygon)
{

    double min = 0.0;
    double max = 0.0;

    bool flag = false;
    int num3 = 0;
    for (int i = 0; i < Y.count(); i++)
    {
        for (int j = 0; j < X.length(); j++)
        {
            if (abs((double) ((S[i][j] / noData) - 1.0)) >= 0.01)
            {
                num3++;
                if (num3 == 1)
                {
                    min = S[i][j];
                    max = min;
                }
                else
                {
                    if (S[i][j] < min)
                    {
                        min = S[i][j];
                    }
                    if (S[i][j] > max)
                    {
                        max = S[i][j];
                    }
                }
            }
            else
            {
                flag = true;
            }
        }
    }


// 	for (int i = 0; i < polygon.count(); i++)
// 	{
// 		QCounter::Polygon &ply = polygon[i];
// 		double &lowValue = ply.LowValue;
//
// 		int index = cValues.indexOf(lowValue, 0);
// 		if (index == (cValues.count() - 1))
// 		{
// 			ply.HighValue = max;
// 		}
// 		else
// 		{
// 			ply.HighValue = cValues[index + 1];
// 		}
// 		if (!ply.IsHighCenter)
// 		{
// 			ply.HighValue = lowValue;
// 			if (index == 0)
// 			{
// 				lowValue = min;
// 			}
// 			else
// 			{
// 				lowValue = cValues[index - 1];
// 			}
// 		}
// 	}

    for (int i = 0; i < polygon.count(); i++)
    {
        QCounter::Polygon &ply = polygon[i];
        double lowValue = ply.LowValue;
        //double highValue=0;
        int index = cValues.indexOf(lowValue, 0);
        if (index == (cValues.count() - 1))
        {
            ply.HighValue = max;
        }
        else
        {
            ply.HighValue= cValues[index + 1];
        }
        if (!ply.IsHighCenter)
        {
            ply.HighValue = lowValue;
            if (index == 0)
            {
                lowValue = min;
            }
            else
            {
                lowValue = cValues[index - 1];
            }
        }
        //ply.HighValue=highValue;
        ply.LowValue=lowValue;
    }
}
#pragma endregion 等值面

#pragma region 流线

using namespace QCounter;

static bool TracingStreamlinePoint(PointD & aPoint, C2DimenArray<double> & Dx, C2DimenArray<double> & Dy, const QList<double> & X, const QList<double> & Y, int & ii, int & jj, bool isForward)
{
    int length = X.length();
    int num10 = Y.length();
    double num11 = X[1] - X[0];
    double num12 = Y[1] - Y[0];
    double num = Dx.at(ii,jj);
    double num2 = Dx.at(ii, jj + 1);
    double num3 = Dx.at(ii + 1, jj);
    double num4 = Dx.at(ii + 1, jj + 1);
    double num5 = num + ((num3 - num) * ((aPoint.Y - Y[ii]) / num12));
    double num6 = num2 + ((num4 - num2) * ((aPoint.Y - Y[ii]) / num12));
    double num7 = num5 + ((num6 - num5) * ((aPoint.X - X[jj]) / num11));
    num = Dy.at(ii, jj);
    num2 = Dy.at(ii, jj + 1);
    num3 = Dy.at(ii + 1, jj);
    num4 = Dy.at(ii + 1, jj + 1);
    num5 = num + ((num3 - num) * ((aPoint.Y - Y[ii]) / num12));
    num6 = num2 + ((num4 - num2) * ((aPoint.Y - Y[ii]) / num12));
    double num8 = num5 + ((num6 - num5) * ((aPoint.X - X[jj]) / num11));
    if (isForward)
    {
        aPoint.X += num7;
        aPoint.Y += num8;
    }
    else
    {
        aPoint.X -= num7;
        aPoint.Y -= num8;
    }
    if (((aPoint.X < X[jj]) || (aPoint.X > X[jj + 1])) || ((aPoint.Y < Y[ii]) || (aPoint.Y > Y[ii + 1])))
    {
        if (((aPoint.X < X[0]) || (aPoint.X > X[X.length() - 1])) || ((aPoint.Y < Y[0]) || (aPoint.Y > Y[Y.length() - 1])))
        {
            return false;
        }
        for (int i = ii - 2; i < (ii + 3); i++)
        {
            if (((i >= 0) && (i < num10)) && ((aPoint.Y >= Y[i]) && (aPoint.Y <= Y[i + 1])))
            {
                ii = i;
                for (int j = jj - 2; j < (jj + 3); j++)
                {
                    if (((j >= 0) && (j < length)) && ((aPoint.X >= X[j]) && (aPoint.X <= X[j + 1])))
                    {
                        jj = j;
                        break;
                    }
                }
                break;
            }
        }
    }
    return true;
}

QList<PolyLine> TracingStreamline(double** U, double** V, const QList<double> & X, const QList<double> & Y, double UNDEF, int density)
{
    int num6;
    int num7;
    QList<PolyLine> list;// = new List<PolyLine>();
    int length = X.length();
    int num2 = Y.length();
    C2DimenArray<double> dx(num2, length);
    C2DimenArray<double> dy(num2, length);
//	double[,] dx = new double[num2, length];
//	double[,] dy = new double[num2, length];
    double num3 = X[1] - X[0];
    double num4 = Y[1] - Y[0];
    if (density == 0)
    {
        density = 1;
    }
    double num5 = num3 / ((double) (density * density));
    for (num6 = 0; num6 < num2; num6++)
    {
        num7 = 0;
        while (num7 < length)
        {
            if (abs((U[num6][num7] / UNDEF) - 1.0) < 0.01)
            {
                dx.at(num6, num7) = 0.1;
                dy.at(num6, num7) = 0.1;
            }
            else
            {
                double num8 = sqrt((U[num6][num7] * U[num6][num7]) + (V[num6] [num7] * V[num6][num7]));
                if (num8 == 0.0)
                {
                    num8 = 1.0;
                }
                dx.at(num6,num7) = ((U[num6][ num7] / num8) * num3) / ((double) density);
                dy.at(num6, num7) = ((V[num6][ num7] / num8) * num4) / ((double) density);
            }
            num7++;
        }
    }
    C2DimenArray<QList<PointD>> listArray(num2 - 1,length - 1) ;//= new List<PointD>[num2 - 1, length - 1];
//	int[,] numArray3 = new int[num2 - 1, length - 1];
    C2DimenArray<int> numArray3(num2 - 1,length - 1);
    for (num6 = 0; num6 < (num2 - 1); num6++)
    {
        num7 = 0;
        while (num7 < (length - 1))
        {
            if (((num6 % 2) == 0) && ((num7 % 2) == 0))
            {
                numArray3.at(num6, num7) = 0;
            }
            else
            {
                numArray3.at(num6, num7) = 1;
            }
            listArray.at(num6, num7).clear();
//	listArray.at(num6, num7) = new List<PointD>();
            num7++;
        }
    }
    for (num6 = 0; num6 < (num2 - 1); num6++)
    {
        for (num7 = 0; num7 < (length - 1); num7++)
        {
            if (numArray3.at(num6, num7)!= 0)
            {
                continue;
            }
            QList<PointD> list2 ;//= new List<PointD>();
            PointD aPoint;// = new PointD();
            PolyLine line;// = new PolyLine();
            aPoint.X = X[num7] + (num3 / 2.0);
            aPoint.Y = Y[num6] + (num4 / 2.0);
            list2.push_back(aPoint);//;Add((PointD) aPoint.Clone());
            listArray.at(num6, num7).push_back(aPoint);//Add((PointD) aPoint.Clone());
            numArray3.at(num6, num7) = 1;
            int ii = num6;
            int jj = num7;
            int num12 = 500;
            int num11 = 0;
            while (num11 < num12)
            {
                if (((!TracingStreamlinePoint(aPoint, dx, dy, X, Y, ii, jj, true) || (abs((U[ii][jj] / UNDEF) - 1.0) < 0.01)) || ((abs((U[ii] [jj + 1] / UNDEF) - 1.0) < 0.01) || (abs((U[ii + 1][jj] / UNDEF) - 1.0) < 0.01))) || (abs((U[ii + 1][jj + 1] / UNDEF) - 1.0) < 0.01))
                {
                    break;
                }
                bool flag2 = false;
                foreach (PointD td2 ,listArray.at(ii, jj))
                {
                    if (sqrt(((aPoint.X - td2.X) * (aPoint.X - td2.X)) + ((aPoint.Y - td2.Y) * (aPoint.Y - td2.Y))) < num5)
                    {
                        flag2 = true;
                        break;
                    }
                }
                if (flag2)
                {
                    break;
                }
                list2.push_back(aPoint);//Add((PointD) aPoint.Clone());
                listArray.at(ii, jj).push_back(aPoint);//Add((PointD) aPoint.Clone());
                numArray3.at(ii, jj) = 1;
                num11++;
            }
            aPoint.X = X[num7] + (num3 / 2.0);
            aPoint.Y = Y[num6] + (num4 / 2.0);
            ii = num6;
            jj = num7;
            for (num11 = 0; num11 < num12; num11++)
            {
                if (((!TracingStreamlinePoint(aPoint, dx, dy, X, Y, ii, jj, false) || (abs((U[ii] [jj] / UNDEF) - 1.0) < 0.01)) || ((abs((U[ii] [jj + 1] / UNDEF) - 1.0) < 0.01) || (abs((U[ii + 1][jj] / UNDEF) - 1.0) < 0.01))) || (abs((U[ii + 1][jj + 1] / UNDEF) - 1.0) < 0.01))
                {
                    break;
                }
                bool flag4 = false;
                foreach (PointD td3 , listArray.at(ii, jj))
                {
                    if (sqrt(((aPoint.X - td3.X) * (aPoint.X - td3.X)) + ((aPoint.Y - td3.Y) * (aPoint.Y - td3.Y))) < num5)
                    {
                        flag4 = true;
                        break;
                    }
                }
                if (flag4)
                {
                    break;
                }
                list2.insert(0, aPoint);
                listArray.at(ii, jj).push_back(aPoint);//Add((PointD) aPoint.Clone());
                numArray3.at(ii, jj) = 1;
            }
            if (list2.count() > 1)
            {
                line.PointList = list2;
                list.push_back(line);
            }
        }
    }
    return list;
}

bool serializePolyLine(const QString &filePath, const QList<QCounter::PolyLine> &data)
{
	if (data.size() == 0)
		return false;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QDataStream dStream(&file);
    int dSize = data.size();
    dStream << dSize;
    QString strTmp;
    for (int i=0; i<dSize; i++)
    {
        dStream << data.at(i).BorderIdx;
        strTmp = QString::fromStdString(data.at(i).Type);
        dStream << strTmp;
        dStream << data.at(i).Value;
        int tSize = data.at(i).PointList.size();
        dStream << tSize;
        for (int j=0; j<tSize; j++)
        {
            dStream << data.at(i).PointList.at(j).X;
            dStream << data.at(i).PointList.at(j).Y;
        }
    }

    file.close();

    return true;
}

bool deserializePolyLine(const QString &filePath, QList<QCounter::PolyLine> &data)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QDataStream dStream(&file);
    QString strTmp;
    int dSize = data.size();
    dStream >> dSize;
    for (int i=0; i<dSize; i++)
    {
        QCounter::PolyLine elem;
        dStream >> elem.BorderIdx;
        dStream >> strTmp;
        elem.Type = strTmp.toStdString();
        dStream >> elem.Value;
        int tSize = 0;
        dStream >> tSize;
        for (int j=0; j<tSize; j++)
        {
            PointD pd;
            dStream >> pd.X;
            dStream >> pd.Y;
            elem.PointList.push_back(pd);
        }
        data.push_back(elem);
    }

    file.close();

    return true;
}

bool deserializePolygons(const QString &filePath, QList<QCounter::Polygon> &data)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QDataStream dStream(&file);
    QString strTmp;
    int dSize = 0;
    dStream >> dSize;
    dStream >> data;

    file.close();
    return true;
}

bool serializePolygons(const QString &filePath, const QList<QCounter::Polygon> &data)
{
	if (data.size() == 0)
		return false;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QDataStream dStream(&file);
    int dSize = data.size();
    dStream << dSize;
    dStream << data;

    file.close();

    return true;
}

#pragma endregion 流线
//void DateTime(std::string strfilename,std::string str,std::string str1)//测试速率
//{
//
//	std::string strfilename1;
//	strfilename1 += "..\\";
//	strfilename1 += strfilename;
//	strfilename1 += ".txt";
//
//	char *buf1;
//
//	buf1=(char *)strfilename1.c_str();
//
//	FILE *f;
//	f=fopen(buf1,"a");
//
//	fwrite("\n",1,1,f);
//
//	char buf[20];
//
//	//	strcpy_s(buf,str.c_str());
//
//	fwrite(str.c_str(),str.length(),1,f);
//
//	fwrite("\n",1,1,f);
//
//	fwrite(str1.c_str(),str1.length(),1,f);
//	fclose(f);
//}
