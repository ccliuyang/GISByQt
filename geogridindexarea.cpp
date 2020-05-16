#include "geogridindexarea.h"

GeoGridIndexArea::GeoGridIndexArea()
{

}
GeoGridIndexArea::GeoGridIndexArea(double minx,double miny,double maxx,double maxy)
{
    this->rect = GeoRect(minx,miny,maxx,maxy);
}
void GeoGridIndexArea::addObj(GeoObject *obj)
{
    objs.append(obj);
}
bool GeoGridIndexArea::isInArea(LonLat pt)
{
    return pt.isInArea(this->rect);
}
QList<GeoObject*> GeoGridIndexArea::getObjs()
{
    return objs;
}
GeoRect GeoGridIndexArea::getRect()
{
    return this->rect;
}
QString GeoGridIndexArea::toQString()
{
    return QString("GridArea:%1, obj Num:%2\n")
            .arg(this->rect.toQString()).arg(objs.count());
}
