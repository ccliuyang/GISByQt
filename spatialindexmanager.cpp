#include "spatialindexmanager.h"

SpatialIndexManager::SpatialIndexManager()
{

}
SpatialIndexManager::~SpatialIndexManager()
{
    if(spatialIndex != nullptr)
        delete spatialIndex;
}
void SpatialIndexManager::creatSpatialIndex(QString IndexName, QList<GeoObject*>objs,GeoRect rect,int n)
{
    if(spatialIndex != nullptr){
        delete spatialIndex;
        spatialIndex = nullptr;
    }
    if(IndexName == "Grid"){
        spatialIndex = new GeoGridIndex(objs,rect,n);
    }else if(IndexName == "QuadTree"){
//        mout<<"开始创建";
        spatialIndex = new GeoQuadTreeIndex(objs,rect,n);
    }else if(IndexName == "RTree"){
        spatialIndex = new GeoRTreeIndex(objs,rect,n);
    }else{

    }
}
QString SpatialIndexManager::toQString()
{
//    mout << "管理类toQString";
    return spatialIndex->toQString();
}
QString SpatialIndexManager::getName()
{
    if(spatialIndex != nullptr)
        return spatialIndex->getName();
    else
        return "NULL";
}
void SpatialIndexManager::setSpatialIndex(SpatialIndex *si)
{
    this->spatialIndex = si;
}
SpatialIndex* SpatialIndexManager::getSpatialIndex()
{
    return this->spatialIndex;
}
QList<GeoObject*> SpatialIndexManager::getObjs(LonLat pt)
{
    QList<GeoObject*> objs;
    if(spatialIndex==nullptr)
        return objs;
    QString name = spatialIndex->getName();
    if(name == "Grid"){
        return static_cast<GeoGridIndex*>(spatialIndex)->getArea(pt)->getObjs();
    }else if(name == "QuadTree"){
        return static_cast<GeoQuadTreeIndex*>(spatialIndex)->getArea(pt)->getObjs();
    }else if(name == "RTree"){
//        return static_cast<GeoRTreeIndex*>(spatialIndex)->getArea(pt)->getObjs();
    }
    return objs;
}
GeoObject* SpatialIndexManager::getObj(LonLat pt)
{
    return spatialIndex->getObj(pt);
}
QList<GeoRect> SpatialIndexManager::getAllRect()
{
    if(spatialIndex!=nullptr)
        return spatialIndex->getAllRect();
    QList<GeoRect>rs;
    return rs;
}
void SpatialIndexManager::setSelectMinDis(double min)
{
    spatialIndex->setSelectMinDis(min);
}
double SpatialIndexManager::getSelectMinDis()
{
    return spatialIndex->getSelectMinDis();
}
