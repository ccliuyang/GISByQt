#include "geopolyline.h"

GeoPolyline::GeoPolyline()
{

}
void GeoPolyline::addpropertie(QString key, QVariant value)
{
    this->properties.insert(key,value);
}
void GeoPolyline::addPoint(LonLat p)
{
    pts.append(p);
}
void GeoPolyline::addPoint(QString p)
{
    pts.append(LonLat(p));
}
QString GeoPolyline::toQString()
{
    //mout << "polyline.toQString()";
    QString s;
    s.append("Polyline:\n");
    for(int i=0;i<pts.size();i++)
    {
        s.append(pts[i].toQString());
    }

    s.append("\n\nProperties:\n");
    for(int i=0;i<this->properties.size();i++){
        s.append(QString("%1:%2\n").arg(this->properties.keys().at(i))
                 .arg(this->properties.values().at(i).toString()));
    }
    if(lineStyle != nullptr){
        s.append(QString("Color:(%1,%2,%3)\n").arg(getR()).arg(getG()).arg(getB()));
    }
    return s;
}
GeoPolyline::~GeoPolyline()
{
    //mout << "Polyline析构";
    if(lineStyle != nullptr)
        delete lineStyle;
}
QStringList GeoPolyline::getPropertiesKeys()
{
    return properties.keys();
}
QVariant GeoPolyline::getPropertiesValue(QString key)
{
    if(properties.contains(key))
    {
        return properties.value(key);
    }else{
        return "NULL";
    }
}
bool GeoPolyline::PropertiesContains(QString key)
{
    return this->properties.contains(key);
}
QList<LonLat> GeoPolyline::getData()
{
    return this->pts;
}
void GeoPolyline::GetScale()
{
    Xmin=pts.at(0).longitude;
    Xmax=pts.at(0).longitude;
    Ymin=pts.at(0).latitude;
    Ymax=pts.at(0).latitude;
    for(int i=0;i<pts.size();i++)
    {
        if(pts.at(i).longitude>Xmax)    Xmax=pts.at(i).longitude;
        if(pts.at(i).longitude<Xmin)    Xmin=pts.at(i).longitude;
        if(pts.at(i).latitude>Ymax)     Ymax=pts.at(i).latitude;
        if(pts.at(i).latitude<Ymin)     Ymin=pts.at(i).latitude;
    }
    mmRect = GeoRect(Xmin,Ymin,Xmax,Ymax);
}
float GeoPolyline::getLineWidth()
{
    if(lineStyle != nullptr)
        return lineStyle->stroke_width;
    else
        return 2.0f;
}
QString GeoPolyline::getLineJoin()
{
    if(lineStyle != nullptr)
        return lineStyle->stroke_linejoin;
    else
        return "bevel";
}
QString GeoPolyline::getLineCap()
{
    if(lineStyle != nullptr)
        return lineStyle->stroke_linecap;
    else
        return "square";
}
float GeoPolyline::getB()
{
    if(lineStyle != nullptr)
        return lineStyle->strokeColor.blue() / 255.0f;
    else
        return 1.0f;
}
float GeoPolyline::getG()
{
    if(lineStyle != nullptr)
        return lineStyle->strokeColor.green() / 255.0f;
    else
        return 1.0f;
}
float GeoPolyline::getR()
{
    if(lineStyle != nullptr)
        return lineStyle->strokeColor.red() / 255.0f;
    else
        return 1.0f;
}
void GeoPolyline::setStyle(GeoStyle *style)
{
    this->lineStyle = style;
}
//bool GeoPolyline::isInArea(double minx, double miny, double maxx, double maxy)
//{
//    for(int i=0;i<pts.count();i++){
//        if(pts[i].isInArea(minx,miny,maxx,maxy))
//            return true;
//    }
//    return false;
//}
bool GeoPolyline::isInArea(GeoRect rect,int n)
{
    switch (n) {
    case 0:
        for(int i=0;i<pts.count();i++){
            if(pts[i].isInArea(rect))
                return true;
        }
        return false;
    case 1:
        return mmRect.isCross(rect);
    default:
        for(int i=0;i<pts.count();i++){
            if(pts[i].isInArea(rect))
                return true;
        }
        return false;
    }

}
double GeoPolyline::getDistance(LonLat pt)
{
    return pt.getDistance(pts);
}
bool GeoPolyline::isInPolygon(LonLat pt)
{
    return false;
}
void GeoPolyline::transfertoXYZ()
{
    for(int i=0;i<pts.size();i++)
    {
        this->pts[i].TransfertoXYZ();
    }
}
