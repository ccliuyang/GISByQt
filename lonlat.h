#ifndef LONLAT_H
#define LONLAT_H
#include <QString>
#include "georect.h"
#include "float.h"
///
/// \brief The LonLat class
///
class LonLat
{
public:
    LonLat();

    LonLat(QString ll);

    LonLat(double lon,double lat);

    ~LonLat();

    QString toQString();

    double longitude;

    double latitude;

    bool isInArea(GeoRect rect);

    double getDistance(LonLat pt);

    double getDistance(QList<LonLat> pts);

    double getDistance(QList<QList<LonLat>> polygons);
    
    double X,Y,Z;
    
    void TransfertoXYZ();
};

#endif // LONLAT_H
