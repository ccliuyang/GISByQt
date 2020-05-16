#ifndef GEORTREEINDEXAREA_H
#define GEORTREEINDEXAREA_H

#include "georect.h"
#include "geoobject.h"
class GeoRTreeIndexArea
{
public:
    GeoRTreeIndexArea(GeoRect r);
    ~GeoRTreeIndexArea();
    void addObj(GeoObject*);
//    void addObj(QList<GeoObject*>);
    QList<GeoObject *> getObjs() const;
    GeoRect getRect() const;
    bool isInArea(LonLat pt);
    QString toQString();
    void addSubArea(GeoRTreeIndexArea* area);
    void addSubArea(QList<GeoRTreeIndexArea*> areas);
    bool hasSubArea();
    QList<GeoRTreeIndexArea *> getSubAreas() const;
    int getObjSize();
    int getSubAreaSize();
    GeoObject* takeObjAt(int i);
    GeoRTreeIndexArea *getParentArea() const;
    void setParentArea(GeoRTreeIndexArea *value);
    bool hasParent();
    QList<GeoRTreeIndexArea*> takeAllSubArea();
    GeoRTreeIndexArea* takeAreaAt(int i);
    int removeAll(GeoRTreeIndexArea* a);
private:
    GeoRect rect;
    QList<GeoObject*> objs;
    QList<GeoRTreeIndexArea*> subAreas;
    GeoRTreeIndexArea* parentArea = nullptr;
};

#endif // GEORTREEINDEXAREA_H
