#ifndef GEOOBJECT_H
#define GEOOBJECT_H
#include <QString>
#include <QtDebug>
#include <lonlat.h>
#include "geostyle.h"
#include "georect.h"
#define mout qDebug() <<"["<<__FILE__<<":"<<__LINE__<<"]"
///
/// \brief The GeoObject class
///
class GeoObject
{
public:
    GeoObject();

    virtual ~GeoObject();

    virtual QStringList getPropertiesKeys()=0;

    virtual QVariant getPropertiesValue(QString key)=0;

    virtual bool PropertiesContains(QString key)=0;

    virtual void addPoint(LonLat p) = 0;

    virtual void addPoint(QString p) = 0;

    virtual void addpropertie(QString key,QVariant value) = 0;

    virtual QString toQString() = 0;

    virtual void GetScale()=0;

    virtual void setStyle(GeoStyle* style) = 0;

    virtual bool isInArea(GeoRect rect,int n = 0) = 0;

    virtual double getDistance(LonLat pt) = 0;

    virtual bool isInPolygon(LonLat pt)=0;
    
    virtual void transfertoXYZ()=0;

    double Xmin,Xmax,Ymin,Ymax;
    GeoRect mmRect;
};

#endif // GEOOBJECT_H
