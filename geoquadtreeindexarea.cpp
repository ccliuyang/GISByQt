#include "geoquadtreeindexarea.h"

GeoQuadTreeIndexArea::GeoQuadTreeIndexArea(GeoRect r)
{
    this->rect = r;
}
GeoQuadTreeIndexArea::~GeoQuadTreeIndexArea()
{
    if(subArea0 != nullptr)
        delete subArea0;
    if(subArea1 != nullptr)
        delete subArea1;
    if(subArea2 != nullptr)
        delete subArea2;
    if(subArea3 != nullptr)
        delete subArea3;
}

QList<GeoObject *> GeoQuadTreeIndexArea::getObjs() const
{
    return objs;
}

GeoRect GeoQuadTreeIndexArea::getRect() const
{
    return rect;
}
bool GeoQuadTreeIndexArea::isInArea(LonLat pt)
{
    return pt.isInArea(this->rect);
}
void GeoQuadTreeIndexArea::addObj(GeoObject *obj)
{
    this->objs.append(obj);
}
void GeoQuadTreeIndexArea::addObj(QList<GeoObject*>obj)
{
    this->objs.append(obj);
}
QString GeoQuadTreeIndexArea::toQString()
{
//    mout <<"Area toQString";
    if(objs.count()>0)
        return QString("QuadTreeArea:%1, obj Num:%2\n").arg(this->rect.toQString())
                .arg(objs.count());
    else
        return QString("QuadTreeArea:%1, NO obj\n").arg(this->rect.toQString());
}
bool GeoQuadTreeIndexArea::isOver()
{
    if(areaObjsSize>=objs.count())
        return true;
    return false;
}

int GeoQuadTreeIndexArea::getAreaObjsSize() const
{
    return areaObjsSize;
}

void GeoQuadTreeIndexArea::setAreaObjsSize(int value)
{
    areaObjsSize = value;
}
void GeoQuadTreeIndexArea::setSubArea(GeoQuadTreeIndexArea *Area0, GeoQuadTreeIndexArea *Area1,
                                      GeoQuadTreeIndexArea *Area2, GeoQuadTreeIndexArea *Area3)
{
    this->subArea0 = Area0;
    this->subArea1 = Area1;
    this->subArea2 = Area2;
    this->subArea3 = Area3;
}
bool GeoQuadTreeIndexArea::hasSubArea()
{
    if(subArea0 != nullptr)
        return true;
    else
        return false;
}
QList<GeoQuadTreeIndexArea*> GeoQuadTreeIndexArea::getSubArea()
{
    QList<GeoQuadTreeIndexArea*> a;
    a.append(subArea0);
    a.append(subArea1);
    a.append(subArea2);
    a.append(subArea3);
    return a;
}
