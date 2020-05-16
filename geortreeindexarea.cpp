#include "geortreeindexarea.h"

GeoRTreeIndexArea::GeoRTreeIndexArea(GeoRect r)
{
    this->rect = r;
}
GeoRTreeIndexArea::~GeoRTreeIndexArea()
{
    for(int i=0;i<subAreas.count();i++){
        delete subAreas[i];
        subAreas[i] = nullptr;
    }
    subAreas.clear();
}

QList<GeoObject *> GeoRTreeIndexArea::getObjs() const
{
    return objs;
}

GeoRect GeoRTreeIndexArea::getRect() const
{
    return rect;
}

QList<GeoRTreeIndexArea *> GeoRTreeIndexArea::getSubAreas() const
{

    return subAreas;
}
void GeoRTreeIndexArea::addObj(GeoObject * o)
{
    objs.append(o);
    this->rect = rect.getMMR(o->mmRect);
}
//void GeoRTreeIndexArea::addObj(QList<GeoObject *>os)
//{
//    objs.append(os);
//}
bool GeoRTreeIndexArea::isInArea(LonLat pt)
{
    return pt.isInArea(this->rect);
}
QString GeoRTreeIndexArea::toQString()
{
    if(objs.count()>0)
        return QString("RTreeArea:%1, obj Num:%2\n").arg(this->rect.toQString())
                .arg(objs.count());
    else
        return QString("QuadTreeArea:%1, NO obj\n").arg(this->rect.toQString());
}
void GeoRTreeIndexArea::addSubArea(GeoRTreeIndexArea *area)
{
    area->setParentArea(this);
    subAreas.append(area);
    this->rect = rect.getMMR(area->getRect());
}
void GeoRTreeIndexArea::addSubArea(QList<GeoRTreeIndexArea *> areas)
{
    foreach (GeoRTreeIndexArea* a, areas) {
        a->setParentArea(this);
        this->rect = rect.getMMR(a->getRect());
    }
    subAreas.append(areas);
}
bool GeoRTreeIndexArea::hasSubArea()
{
//    mout << "zks";
//    if(subAreas.size() == 0)
//        return false;
//    else
//        return true;
    return !subAreas.isEmpty();
}
int GeoRTreeIndexArea::getObjSize()
{
    return objs.size();
}
GeoObject* GeoRTreeIndexArea::takeObjAt(int i)
{
    return objs.takeAt(i);
}

GeoRTreeIndexArea *GeoRTreeIndexArea::getParentArea() const
{
    return parentArea;
}

void GeoRTreeIndexArea::setParentArea(GeoRTreeIndexArea *value)
{
    parentArea = value;
}
bool GeoRTreeIndexArea::hasParent()
{
    if(parentArea == nullptr)
        return false;
    else
        return true;
}
QList<GeoRTreeIndexArea*> GeoRTreeIndexArea::takeAllSubArea()
{
    QList<GeoRTreeIndexArea*>a;
    int size = subAreas.size();
    for(int i=0;i<size;i++){
        a.append(subAreas.takeFirst());
    }
    return a;
}
GeoRTreeIndexArea* GeoRTreeIndexArea::takeAreaAt(int i)
{
    return subAreas.takeAt(i);
}
int GeoRTreeIndexArea::getSubAreaSize()
{
    return subAreas.size();
}
int GeoRTreeIndexArea::removeAll(GeoRTreeIndexArea *a)
{
    return subAreas.removeAll(a);
}
