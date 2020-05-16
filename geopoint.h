#ifndef GEOPOINT_H
#define GEOPOINT_H
#include "geoobject.h"
#include "lonlat.h"
#include "geostyle.h"
///
/// \brief The GeoPoint class
///
class GeoPoint : public GeoObject
{
public:
    GeoPoint();

    GeoPoint(LonLat p);

    GeoPoint(QString p);

    ~GeoPoint() override;

    QStringList getPropertiesKeys() override;

    QVariant getPropertiesValue(QString key) override;

    bool PropertiesContains(QString key) override;

    void addpropertie(QString key,QVariant value) override;

    void addPoint(LonLat p) override;

    void addPoint(QString p) override;

    QString toQString() override;

    LonLat getData();

    void GetScale() override;

    float getPointSize();

    float getR();

    float getG();

    float getB();

    void setStyle(GeoStyle* style) override;

    bool isInArea(GeoRect rect,int n = 0) override;

    double getDistance(LonLat pt) override;

    bool isInPolygon(LonLat pt) override;
    
    void transfertoXYZ() override;

//    bool isInArea(double minx, double miny, double maxx, double maxy) override;
private:
    QMap<QString,QVariant> properties;

    LonLat pt;

    GeoStyle* pointStyle = nullptr;
};

#endif // GEOPOINT_H
