#include "geolayer.h"
#include <QDebug>
#define mout qDebug() <<"["<<__FILE__<<":"<<__LINE__<<"]"
GeoLayer::GeoLayer()
{
    Xmin=999999999;
    Xmax=-999999999;
    Ymin=999999999;
    Ymax=-999999999;
}
void GeoLayer::addObj(GeoObject *obj)
{
    obj->addpropertie("IndexID",this->objs.size());
    this->objs.append(obj);
    this->addpropertie("ObjNumber",objs.size());
    isgetscale = false;
}
void GeoLayer::removeObj(int IndexID)
{
    if(IndexID<0||IndexID>=this->objs.size())
        return;
    delete objs.takeAt(IndexID);
}
void GeoLayer::addpropertie(QString key, QVariant value)
{
    this->properties.insert(key,value);
}
QString GeoLayer::toQString()
{
    //mout << "layer.toQString()";
    QString s;
//    for(int i=0;i<objs.size();i++)
//    {
//        s.append(objs.at(i)->toQString());
//        s.append("--\n");
//    }
    s.append("\nProperties:\n");
    for(int i=0;i<this->properties.size();i++){
        s.append(QString("%1:%2\n").arg(this->properties.keys().at(i))
                 .arg(this->properties.values().at(i).toString()));
    }
    s.append(QString("Rect:(%1,%2,%3,%4)\n").arg(Xmin).arg(Ymin).arg(Xmax).arg(Ymax));
    return s;
}
GeoLayer::~GeoLayer()
{
    for(int i=0;i<objs.size();i++)
    {
        delete objs[i];
    }
    objs.clear();
    //mout << "layer:析构";
}
QStringList GeoLayer::getPropertiesKeys()
{
    return properties.keys();
}
QVariant GeoLayer::getPropertiesValue(QString key)
{
    if(properties.contains(key))
    {
        return properties.value(key);
    }else{
        return "NULL";
    }
}
GeoObject* GeoLayer::getObj(int index)
{
      return this->objs.at(index);
}
QList<GeoObject *> GeoLayer::getAllObjs()
{
    return this->objs;
}
int GeoLayer::getObjSize()
{
    return this->objs.size();
}
bool GeoLayer::PropertiesContains(QString key)
{
    return this->properties.contains(key);
}
void GeoLayer::GetLayerScale()
{
    if(isgetscale == true)
        return;
    for(int i=0;i<objs.size();i++)
    {
        objs.at(i)->GetScale();
        if(Xmax<objs.at(i)->Xmax)   Xmax=objs.at(i)->Xmax;
        if(Xmin>objs.at(i)->Xmin)   Xmin=objs.at(i)->Xmin;
        if(Ymax<objs.at(i)->Ymax)   Ymax=objs.at(i)->Ymax;
        if(Ymin>objs.at(i)->Ymin)   Ymin=objs.at(i)->Ymin;
    }
    this->isgetscale = true;
}
GeoObject* GeoLayer::getObjByPropertie(QString key, QString value)
{
    for(int i=0;i<objs.count();i++){
        if(objs.at(i)->getPropertiesValue(key).toString() == value)
            return objs.at(i);
    }
    return nullptr;
}
void GeoLayer::clearObj()
{
    this->objs.clear();
}
bool GeoLayer::hasIndex()
{
    return hasSpatialIndex;
}


int GeoLayer::getMapIndexID() const
{
    return mapIndexID;
}

void GeoLayer::setMapIndexID(int value)
{
    mapIndexID = value;
}
void GeoLayer::transfertoXYZ()
{
    for(int i=0;i<objs.size();i++)
    {
        objs.at(i)->transfertoXYZ();
    }
}
