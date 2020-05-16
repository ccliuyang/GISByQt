#ifndef GEOQUADTREEINDEXAREA_H
#define GEOQUADTREEINDEXAREA_H

#include "georect.h"
#include "geoobject.h"

class GeoQuadTreeIndexArea
{
public:
    GeoQuadTreeIndexArea(GeoRect r);
    ~GeoQuadTreeIndexArea();
    void addObj(GeoObject*);
    void addObj(QList<GeoObject*>);
    QList<GeoObject *> getObjs() const;
    bool isInArea(LonLat pt);
    GeoRect getRect() const;
    QString toQString();
    void setSubArea(GeoQuadTreeIndexArea*subArea0,GeoQuadTreeIndexArea*subArea1,
                    GeoQuadTreeIndexArea*subArea2,GeoQuadTreeIndexArea*subArea3);
    bool hasSubArea();
    QList<GeoQuadTreeIndexArea*> getSubArea();
    bool isOver();
    int getAreaObjsSize() const;
    void setAreaObjsSize(int value);

private:
    int areaObjsSize = 6;
    GeoRect rect;
    QList<GeoObject*>objs;
    GeoQuadTreeIndexArea* subArea0 = nullptr;
    GeoQuadTreeIndexArea* subArea1 = nullptr;
    GeoQuadTreeIndexArea* subArea2 = nullptr;
    GeoQuadTreeIndexArea* subArea3 = nullptr;
};

#endif // GEOQUADTREEINDEXAREA_H
