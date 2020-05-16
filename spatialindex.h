#ifndef SPATIALINDEX_H
#define SPATIALINDEX_H

#include <QString>
#include "geoobject.h"
#include "geopoint.h"
#include "geopolygon.h"
#include "geopolyline.h"
#include "geomultipolygon.h"
class SpatialIndex
{
public:
    SpatialIndex();
    virtual ~SpatialIndex();
    virtual QString getName();
    virtual QString toQString()=0;
    virtual GeoObject* getObj(LonLat pt)=0;
    virtual void saveIndexFile()=0;
    virtual void readIndexFile()=0;
    virtual QList<GeoRect> getAllRect() = 0;
    QString name = "NULL";
    double selectMinDis = 2000000000;
    virtual double getSelectMinDis() const;
    virtual void setSelectMinDis(double value);
};

#endif // SPATIALINDEX_H
