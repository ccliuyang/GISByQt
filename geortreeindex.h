#ifndef GEORTREEINDEX_H
#define GEORTREEINDEX_H
#include "spatialindex.h"
#include "geortreeindexarea.h"
class GeoRTreeIndex : public SpatialIndex
{
public:
    GeoRTreeIndex();
    GeoRTreeIndex(QList<GeoObject*>objs,GeoRect rect,int n=6);
    ~GeoRTreeIndex() override;
    QString toQString() override;
    GeoObject* getObj(LonLat pt) override;
    void saveIndexFile() override;
    void readIndexFile() override;
    QList<GeoRect> getAllRect() override;
    double getSelectMinDis() const override;
    void setSelectMinDis(double value) override;
private:
    GeoRTreeIndexArea * adjustLeaf(GeoRTreeIndexArea* oriarea);
    void adjustTree(GeoRTreeIndexArea*oriarea);
    void splitNode(GeoRTreeIndexArea* oriarea);
    void pickNext(GeoRTreeIndexArea*oriarea,GeoRTreeIndexArea*area1,GeoRTreeIndexArea*area2);
    void pickSeeds(GeoRTreeIndexArea*oriarea);
    GeoRTreeIndexArea* chooseLeaf(GeoObject* obj);
    GeoRTreeIndexArea* rootArea = nullptr;
    int M=6,m=3;
};

#endif // GEORTREEINDEX_H
