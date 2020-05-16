#ifndef GEOLAYER_H
#define GEOLAYER_H
#include <QMap>
#include <QList>
#include "geoobject.h"

///
/// \brief The GeoLayer class
///
class GeoLayer
{
public:
    GeoLayer();

    ~GeoLayer();

    void addObj(GeoObject *obj);

    void removeObj(int IndexID);

    void addpropertie(QString key,QVariant value);

    QStringList getPropertiesKeys();

    QVariant getPropertiesValue(QString key);

    QString toQString();

    bool PropertiesContains(QString key);

    QList<GeoObject *> getAllObjs();

    int getObjSize();

    GeoObject * getObj(int index);

    void GetLayerScale();

    GeoObject* getObjByPropertie(QString key,QString value);

    void clearObj();
    
    void transfertoXYZ();

    // 投影
    double Xmin,Xmax,Ymin,Ymax;

    bool hasIndex();

    void setSpatialIndexID(int n);


    int getMapIndexID() const;
    void setMapIndexID(int value);

private:
    QMap<QString,QVariant> properties;

    QList<GeoObject *> objs;

    bool isgetscale = false;

    bool hasSpatialIndex = false;

    int mapIndexID = -1;
};

#endif // GEOLAYER_H
