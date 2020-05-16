#ifndef GEORECT_H
#define GEORECT_H

#include <QString>
#include <QList>
class GeoRect
{
public:
    GeoRect();
    GeoRect(double minx, double miny, double maxx, double maxy);
    QString toQString();
    bool isInArea(double x,double y);
    double Xmin,Ymin,Xmax,Ymax;
    QList<GeoRect> getSubRect();
    bool isCross(GeoRect r);
    double getDistance(GeoRect r);
    GeoRect getMMR(GeoRect r);
    double getExpandAcreage(GeoRect r);
    double getActreage();
};

#endif // GEORECT_H
