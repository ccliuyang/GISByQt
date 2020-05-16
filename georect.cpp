#include "georect.h"

GeoRect::GeoRect()
{

}
GeoRect::GeoRect(double minx, double miny, double maxx, double maxy)
{
    Xmin = minx;Ymin = miny;Xmax = maxx;Ymax = maxy;
}
QString GeoRect::toQString()
{
    return QString("(%1, %2, %3, %4)").arg(Xmin).arg(Ymin).arg(Xmax).arg(Ymax);
}
bool GeoRect::isInArea(double x, double y)
{
    if(x >= Xmin&& y  >= Ymin
     &&x <= Xmax&& y  <= Ymax)
        return true;
    return false;
}
QList<GeoRect> GeoRect::getSubRect()
{
    double Xmid = (Xmin + Xmax) / 2,
           Ymid = (Ymin + Ymax) / 2;
    GeoRect rect1 = GeoRect(Xmin,Ymin,Xmid,Ymid);
    GeoRect rect2 = GeoRect(Xmid,Ymin,Xmax,Ymid);
    GeoRect rect3 = GeoRect(Xmin,Ymid,Xmid,Ymax);
    GeoRect rect4 = GeoRect(Xmid,Ymid,Xmax,Ymax);
    QList<GeoRect> r;
    r.append(rect1);
    r.append(rect2);
    r.append(rect3);
    r.append(rect4);
    return r;
}
bool GeoRect::isCross(GeoRect rect)
{
    double zx = abs(rect.Xmax + rect.Xmin - Xmin - Xmax);
    double x  = rect.Xmax - rect.Xmin + Xmax - Xmin;
    double zy = abs(rect.Ymax + rect.Ymin - Ymin - Ymax);
    double y  = rect.Ymax - rect.Ymin + Ymax - Ymin;
    if(zx <= x && zy <= y)
        return true;
    return false;
}
double GeoRect::getDistance(GeoRect rect)
{
    double zx = abs(rect.Xmax + rect.Xmin - Xmin - Xmax);
    double x  = rect.Xmax - rect.Xmin + Xmax - Xmin;
    double zy = abs(rect.Ymax + rect.Ymin - Ymin - Ymax);
    double y  = rect.Ymax - rect.Ymin + Ymax - Ymin;
    if(zx <= x && zy <= y)
        return 0.0;
    else
        return zx-x<zy-y?zx-x:zy-y;
}
GeoRect GeoRect::getMMR(GeoRect r)
{
    double xmin = r.Xmin < Xmin ? r.Xmin:Xmin;
    double xmax = r.Xmax > Xmax ? r.Xmax:Xmax;
    double ymin = r.Ymin < Ymin ? r.Ymin:Ymin;
    double ymax = r.Ymax > Ymax ? r.Ymax:Ymax;
    return GeoRect(xmin,ymin,xmax,ymax);
}
double GeoRect::getExpandAcreage(GeoRect r)
{
    return abs(getMMR(r).getActreage() - getActreage());
}
double GeoRect::getActreage()
{
    return (Xmax - Xmin)*(Ymax - Ymin);
}
