#ifndef GEOPOLYLINE_H
#define GEOPOLYLINE_H
#include "geoobject.h"
#include "lonlat.h"
#include "geostyle.h"
#include <QList>
///
/// \brief The GeoPolyline class
///
class GeoPolyline : public GeoObject
{
public:
    GeoPolyline();

    ~GeoPolyline() override;

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

    float getR();

    float getG();

    float getB();

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

    GeoStyle* lineStyle = nullptr;
};

#endif // GEOPOLYLINE_H
