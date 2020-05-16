#include "geomap.h"
#include <QDebug>
#define mout qDebug() <<"["<<__FILE__<<":"<<__LINE__<<"]"

GeoMap::GeoMap(QString n)
{
    indexManager = new SpatialIndexManager();
    properties.insert("name",n);
    Xmin=999999999;
    Xmax=-999999999;
    Ymin=999999999;
    Ymax=-999999999;
}
void GeoMap::setName(QString n)
{
    properties["name"]  = n;
}
void GeoMap::addLayer(GeoLayer *layer)
{
    int iii = this->getPropertiesValue("IndexID").toInt();
    layer->setMapIndexID(iii);
    layer->addpropertie("IndexID",this->layers.size());
    this->layers.append(layer);
    this->addpropertie("LayerNumber",layers.size());
    isgetscale = false;
}
void GeoMap::removeLayer(int IndexID)
{
    if(IndexID<0||IndexID>=this->layers.size())
        return;
    delete this->layers.takeAt(IndexID);
}
void GeoMap::addpropertie(QString key, QVariant value)
{
    this->properties.insert(key,value);
}
QString GeoMap::toQString()
{
    //mout << "map.toQString()";
    QString s;
//    for(int i=0;i<layers.size();i++)
//    {
//        s.append(layers.at(i)->toQString());
//        s.append("---\n");
//    }
    s.append("\nProperties:\n");
    for(int i=0;i<this->properties.size();i++){
        s.append(QString("%1:%2\n").arg(this->properties.keys().at(i))
                 .arg(this->properties.values().at(i).toString()));
    }
    s.append(QString("Rect:(%1,%2,%3,%4)\n").arg(Xmin).arg(Ymin).arg(Xmax).arg(Ymax));
    return s;
}
GeoMap::~GeoMap()
{
    for(int i=0;i<layers.size();i++)
    {
        delete layers[i];
    }
    layers.clear();
    delete indexManager;
    //mout << "map:"<< properties["name"] <<"析构";
}
QStringList GeoMap::getPropertiesKeys()
{
    return properties.keys();
}
QVariant GeoMap::getPropertiesValue(QString key)
{

    if(properties.contains(key))
    {
        return properties.value(key);
    }else{
        return "NULL";
    }
}
bool GeoMap::PropertiesContains(QString key)
{
    return this->properties.contains(key);
}
QList<GeoLayer*> GeoMap::getAllLayers()
{
    return this->layers;
}
GeoLayer* GeoMap::getLayer(int index)
{
    return this->layers.at(index);
}
int GeoMap::getLayerSize()
{
    return this->layers.size();
}
void GeoMap::GetMapScale()
{
    if(isgetscale == true)
        return;
    for(int i = 0;i <layers.size();i++)
    {
        layers.at(i)->GetLayerScale();
        if(Xmax<layers.at(i)->Xmax)   Xmax=layers.at(i)->Xmax;
        if(Xmin>layers.at(i)->Xmin)   Xmin=layers.at(i)->Xmin;
        if(Ymax<layers.at(i)->Ymax)   Ymax=layers.at(i)->Ymax;
        if(Ymin>layers.at(i)->Ymin)   Ymin=layers.at(i)->Ymin;
    }
    isgetscale = true;
}
void GeoMap::clearLayer()
{
    this->layers.clear();//未析构
}
void GeoMap::swapLayer(int index1, int index2)
{
    if(index1 <0 || index1 >= layers.count())
        return;
    if(index2 <0 || index2 >= layers.count())
        return;
    layers.swapItemsAt(index1,index2);
}
void GeoMap::creatIndexByName(QString indexName, int n)
{
    this->GetMapScale();
    GeoRect rect = GeoRect(this->Xmin,this->Ymin,this->Xmax,this->Ymax);
    QList<GeoObject*> objs;
    for(int i=0;i<layers.count();i++){
        objs.append(layers.at(i)->getAllObjs());
    }
    indexManager->creatSpatialIndex(indexName,objs,rect,n);
}

SpatialIndexManager *GeoMap::getIndexManager() const
{
//    mout << "获得索引管理类";
    return indexManager;
}
QString GeoMap::hasSpatialIndex()
{
    return indexManager->getName();
}
void GeoMap::transfertoXYZ()
{
    for(int i=0;i<layers.size();i++)
    {
        layers[i]->transfertoXYZ();
    }
}
