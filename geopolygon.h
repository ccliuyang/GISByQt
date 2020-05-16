#ifndef GEOPOLYGON_H
#define GEOPOLYGON_H
#include "geoobject.h"
#include <QList>
#include "lonlat.h"
#include "geostyle.h"
///
/// \brief The GeoPolygon class
///
class GeoPolygon : public GeoObject
{
public:
    GeoPolygon();

    ~GeoPolygon() override;

    QStringList getPropertiesKeys() override;

    QVariant getPropertiesValue(QString key) override;

    bool PropertiesContains(QString key) override;

    void addpropertie(QString key,QVariant value) override;

    void addPoint(LonLat p) override;

    void addPoint(QString p) override;

    QString toQString() override;

    QList<LonLat> getData();

    void GetScale() override;

    float getLineWidth();

    float getFillR();

    float getFillG();

    float getFillB();

    float getLineR();

    float getLineG();

    float getLineB();

    QString getLineJoin();

    QString getLineCap();

    void setStyle(GeoStyle* style) override;

    bool isInArea(GeoRect rect,int n = 0) override;

    double getDistance(LonLat pt) override;

    bool isInPolygon(LonLat pt) override;
    
    void transfertoXYZ() override;


//    bool isInArea(double minx, double miny, double maxx, double maxy) override;
private:

    QMap<QString,QVariant> properties;

    QList<LonLat> pts;

    GeoStyle* polygonStyle = nullptr;
};

#endif // GEOPOLYGON_H
