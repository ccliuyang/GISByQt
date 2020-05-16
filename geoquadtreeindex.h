#ifndef GEOQUADTREEINDEX_H
#define GEOQUADTREEINDEX_H
#include "spatialindex.h"
#include <QList>
#include "geoquadtreeindexarea.h"

class GeoQuadTreeIndex : public SpatialIndex
{
public:
    GeoQuadTreeIndex();
    GeoQuadTreeIndex(QList<GeoObject*> objs,GeoRect rect,int n=6);
    ~GeoQuadTreeIndex() override;
    QString toQString() override;
    void readIndexFile() override;
    void saveIndexFile() override;
    GeoQuadTreeIndexArea* getArea(LonLat pt);
    GeoObject * getObj(LonLat pt) override;
    QList<GeoRect> getAllRect() override;
    double getSelectMinDis() const override;
    void setSelectMinDis(double value) override;
private:
    GeoQuadTreeIndexArea* rootArea = nullptr;
    QList<GeoQuadTreeIndexArea*>tempAreas;
};

#endif // GEOQUADTREEINDEX_H
