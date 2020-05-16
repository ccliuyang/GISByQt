#ifndef GEOGRIDINDEXAREA_H
#define GEOGRIDINDEXAREA_H
#include <QList>
#include "geoobject.h"
#include "lonlat.h"
#include "georect.h"

class GeoGridIndexArea
{
public:
    GeoGridIndexArea();
    GeoGridIndexArea(double Xmin,double Ymin,double Xmax,double Ymax);
    void addObj(GeoObject* obj);
    QList<GeoObject*> getObjs();
    bool isInArea(LonLat pt);
    GeoRect getRect();
    QString toQString();
private:
    GeoRect rect;
    QList<GeoObject*> objs;
};

#endif // GEOGRIDINDEXAREA_H
