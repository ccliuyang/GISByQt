#include "geopoint.h"

GeoPoint::GeoPoint()
{

}
GeoPoint::GeoPoint(LonLat p)
{
    this->pt = p;
}

GeoPoint::GeoPoint(QString p)
{
    this->pt = LonLat(p);
}
void GeoPoint::addpropertie(QString key, QVariant value)
{
    this->properties.insert(key,value);
}
void GeoPoint::addPoint(LonLat p)
{
    this->pt = p;
}
void GeoPoint::addPoint(QString p)
{
    this->pt = LonLat(p);
}
QString GeoPoint::toQString()
{
    //mout << "point.toQString()";
    QString s;
    s.append(QString("point:\n%1").arg(this->pt.toQString()));
    s.append("\n\nProperties:\n");
    for(int i=0;i<this->properties.size();i++){
        s.append(QString("%1:%2\n").arg(this->properties.keys().at(i))
                 .arg(this->properties.values().at(i).toString()));
    }
    if(pointStyle != nullptr){
        s.append(QString("Color:(%1,%2,%3)\n").arg(getR()).arg(getG()).arg(getB()));
    }
    return s;
}
GeoPoint::~GeoPoint()
{
    //mout << "point析构";
    if(pointStyle != nullptr)
        delete pointStyle;
}
QStringList GeoPoint::getPropertiesKeys()
{
    return properties.keys();
}
QVariant GeoPoint::getPropertiesValue(QString key)
{
    if(properties.contains(key))
    {
        return properties.value(key);
    }else{
        return "NULL";
    }
}
bool GeoPoint::PropertiesContains(QString key)
{
    return this->properties.contains(key);
}
LonLat GeoPoint::getData()
{
    return this->pt;
}
void GeoPoint::GetScale()
{
    Xmin=pt.longitude;
    Xmax=pt.longitude;
    Ymin=pt.latitude;
    Ymax=pt.latitude;
    mmRect = GeoRect(Xmin-0.1,Ymin-0.1,Xmax+0.1,Ymax+0.1);
}
float GeoPoint::getPointSize()
{
    if(pointStyle != nullptr)
        return pointStyle->pointSize;
    else
        return 4.0f;
}
float GeoPoint::getR()
{
    if(pointStyle != nullptr)
        return pointStyle->fillColor.red()/255.0f;
    else
        return 1.0f;
}
float GeoPoint::getG()
{
    if(pointStyle != nullptr)
        return pointStyle->fillColor.green()/255.0f;
    else
        return 0.0f;
}
float GeoPoint::getB()
{
    if(pointStyle != nullptr)
        return pointStyle->fillColor.blue()/255.0f;
    else
        return 0.0f;
}
void GeoPoint::setStyle(GeoStyle *style)
{
    this->pointStyle = style;
}
//bool GeoPoint::isInArea(double minx, double miny, double maxx, double maxy)
//{
//    return pt.isInArea(minx,miny,maxx,maxy);
//}
bool GeoPoint::isInArea(GeoRect rect,int n)
{
    switch(n){
    case 0:
        return pt.isInArea(rect);
    case 1:
        return mmRect.isCross(rect);
    default:
        return pt.isInArea(rect);
    }
}
double GeoPoint::getDistance(LonLat p)
{
    return p.getDistance(pt);
}
bool GeoPoint::isInPolygon(LonLat pt)
{
    return false;
}
void GeoPoint::transfertoXYZ()
{
    pt.TransfertoXYZ();
}
