#include "geopolygon.h"

GeoPolygon::GeoPolygon()
{

}
void GeoPolygon::addpropertie(QString key, QVariant value)
{
    this->properties.insert(key,value);
}
void GeoPolygon::addPoint(LonLat p)
{
    pts.append(p);
}
void GeoPolygon::addPoint(QString p)
{
    pts.append(LonLat(p));
}
QString GeoPolygon::toQString()
{
    //mout << "polygon.toQString()";
    QString s;
    s.append("Polygon:\n");
    for(int i=0;i<pts.size();i++)
    {
        s.append(pts[i].toQString());
    }
    s.append("\n\nProperties:\n");
    for(int i=0;i<this->properties.size();i++){
        s.append(QString("%1:%2\n").arg(this->properties.keys().at(i))
                 .arg(this->properties.values().at(i).toString()));
    }
    if(polygonStyle != nullptr){
        s.append(QString("Fill Color:(%1,%2,%3)\n").arg(getFillR())
                 .arg(getFillG()).arg(getFillB()));
        s.append(QString("Stroke Color:(%1,%2,%3)\n").arg(getLineR())
                 .arg(getLineG()).arg(getLineB()));
    }
    return s;
}
GeoPolygon::~GeoPolygon()
{
    //mout<<"Polygon析构";
    if(polygonStyle != nullptr)
        delete polygonStyle;
}
QStringList GeoPolygon::getPropertiesKeys()
{
    return properties.keys();
}
QVariant GeoPolygon::getPropertiesValue(QString key)
{
    if(properties.contains(key))
    {
        return properties.value(key);
    }else{
        return "NULL";
    }
}
bool GeoPolygon::PropertiesContains(QString key)
{
    return this->properties.contains(key);
}
QList<LonLat> GeoPolygon::getData()
{
    return this->pts;
}
void GeoPolygon::GetScale()
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
float GeoPolygon::getLineWidth()
{
    if(polygonStyle != nullptr)
        return polygonStyle->stroke_width;
    else
        return 1.0f;
}
float GeoPolygon::getFillB()
{
    if(polygonStyle != nullptr)
        return polygonStyle->fillColor.blue() / 255.0f;
    else
        return 0.0f;
}
float GeoPolygon::getFillG()
{
    if(polygonStyle != nullptr)
        return polygonStyle->fillColor.green() / 255.0f;
    else
        return 0.0f;
}
float GeoPolygon::getFillR()
{
    if(polygonStyle != nullptr)
        return polygonStyle->fillColor.red() / 255.0f;
    else
        return 0.0f;
}
float GeoPolygon::getLineB()
{
    if(polygonStyle != nullptr)
        return polygonStyle->strokeColor.blue() / 255.0f;
    else
        return 1.0f;
}
float GeoPolygon::getLineG()
{
    if(polygonStyle != nullptr)
        return polygonStyle->strokeColor.green() / 255.0f;
    else
        return 1.0f;
}
float GeoPolygon::getLineR()
{
    if(polygonStyle != nullptr)
        return polygonStyle->strokeColor.red() / 255.0f;
    else
        return 1.0f;
}
QString GeoPolygon::getLineJoin()
{
    if(polygonStyle != nullptr)
        return polygonStyle->stroke_linejoin;
    else
        return "bevel";
}
QString GeoPolygon::getLineCap()
{
    if(polygonStyle != nullptr)
        return polygonStyle->stroke_linecap;
    else
        return "square";
}
void GeoPolygon::setStyle(GeoStyle *style)
{
    this->polygonStyle = style;
}
//bool GeoPolygon::isInArea(double minx, double miny, double maxx, double maxy)
//{
//    for(int i=0;i<pts.count();i++){
//        if(pts[i].isInArea(minx,miny,maxx,maxy))
//            return true;
//    }
//    return false;
//}
bool GeoPolygon::isInArea(GeoRect rect,int n)
{
//    double zx = abs(rect.Xmax + rect.Xmin - Xmin - Xmax);
//    double x  = rect.Xmax - rect.Xmin + Xmax - Xmin;
//    double zy = abs(rect.Ymax + rect.Ymin - Ymin - Ymax);
//    double y  = rect.Ymax - rect.Ymin + Ymax - Ymin;
//    if(zx < x && zy < y)
//        return true;
//    return false;
    return mmRect.isCross(rect);
}
double GeoPolygon::getDistance(LonLat pt)
{
    return pt.getDistance(this->pts);
}
bool GeoPolygon::isInPolygon(LonLat p)
{
    int nCross = 0;

    for(int i=0;i<pts.count();i++){
        LonLat p1 = pts.at(i);
        LonLat p2 = pts.at((i+1) % pts.count());

        double ymin = p1.latitude<p2.latitude?p1.latitude:p2.latitude;
        double ymax = p1.latitude>p2.latitude?p1.latitude:p2.latitude;

        if(p1.latitude == p2.latitude)
            continue;
        if(p.latitude < ymin)
            continue;
        if(p.latitude >= ymax)
            continue;
        double x= (p.latitude - p1.latitude)*(p2.longitude - p1.longitude)
                /(p2.latitude -p1.latitude) + p1.longitude;
        if(x > p.longitude)
            nCross++;
    }
//    mout<<nCross % 2;
    return (nCross % 2 == 1);
}
void GeoPolygon::transfertoXYZ()
{
    for(int i=0;i<pts.size();i++)
    {
        this->pts[i].TransfertoXYZ();
    }
}
