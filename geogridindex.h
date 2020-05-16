#ifndef GEOGRIDINDEX_H
#define GEOGRIDINDEX_H

#include "geogridindexarea.h"
#include "lonlat.h"
#include <QList>
#include "geoobject.h"

#include "spatialindex.h"


class GeoGridIndex:public SpatialIndex
{
public:
    GeoGridIndex();
    ~GeoGridIndex() override;
    GeoGridIndex(QList<GeoObject*> objs,GeoRect rect,int n2);
    GeoGridIndexArea* getArea(LonLat pt);
    GeoObject* getObj(LonLat pt) override;
    QString toQString() override;
    void saveIndexFile() override;
    void readIndexFile() override;
    QList<GeoRect> getAllRect() override;
    double getSelectMinDis() const override;
    void setSelectMinDis(double value) override;
private:
    QList<GeoGridIndexArea*> areas;
};

#endif // GEOGRIDINDEX_H
