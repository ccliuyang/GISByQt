#ifndef GEOMAP_H
#define GEOMAP_H
#include<QString>
#include <QList>
#include "geolayer.h"
#include "spatialindexmanager.h"
///
/// \brief The GeoMap class
///
class GeoMap
{
public:
    GeoMap(QString n);

    ~GeoMap();

    void addLayer(GeoLayer *layer);

    void removeLayer(int IndexID);

    void addpropertie(QString key,QVariant value);

    QStringList getPropertiesKeys();

    QVariant getPropertiesValue(QString key);

    bool PropertiesContains(QString key);

    QString toQString();

    QList<GeoLayer *> getAllLayers();

    GeoLayer* getLayer(int index);

    int getLayerSize();

    void setName(QString n);

    void GetMapScale();

    void swapLayer(int index1 , int index2);

    double Xmin,Xmax,Ymin,Ymax;

    ///
    /// \brief 清除所有图层，但未析构
    ///
    void clearLayer();

    void creatIndexByName(QString indexName,int n=8);

    SpatialIndexManager *getIndexManager() const;

    QString hasSpatialIndex();

    void transfertoXYZ();
private:
    QMap<QString,QVariant> properties;

    int mapScale;

    QList<GeoLayer *> layers;

    bool isgetscale = false;

    SpatialIndexManager* indexManager = nullptr;
};

#endif // GEOMAP_H
