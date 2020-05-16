#ifndef SPATIALINDEXMANAGER_H
#define SPATIALINDEXMANAGER_H
#include <QString>
#include "geogridindex.h"
#include "geoquadtreeindex.h"
#include "geortreeindex.h"
#include "geoobject.h"
#include <QList>
class SpatialIndexManager
{
public:
    SpatialIndexManager();
    ~SpatialIndexManager();
    void creatSpatialIndex(QString IndexName,QList<GeoObject*>objs,GeoRect rect,int n=5);
    void setSpatialIndex(SpatialIndex* si);
    SpatialIndex* getSpatialIndex();
    QList<GeoObject*> getObjs(LonLat pt);
    GeoObject* getObj(LonLat pt);
    QString toQString();
    QString getName();
    QList<GeoRect> getAllRect();
    void setSelectMinDis(double min);
    double getSelectMinDis();
private:
    SpatialIndex* spatialIndex = nullptr;
};

#endif // SPATIALINDEXMANAGER_H
