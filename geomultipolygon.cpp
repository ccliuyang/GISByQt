#include "geomultipolygon.h"

GeoMultiPolygon::GeoMultiPolygon()
{

}
GeoMultiPolygon::~GeoMultiPolygon()
{
    //mout<<"MultiPolygon析构";
    if(multiPolygonStyle != nullptr)
        delete multiPolygonStyle;
}
void GeoMultiPolygon::addpropertie(QString key, QVariant value)
{
    this->properties.insert(key,value);
}
void GeoMultiPolygon::addPoint(LonLat p)
{
    mout << p.toQString();
    //pts.append(p);
}
void GeoMultiPolygon::addPoint(QString p)
{
    mout << p;
    //pts.append(LonLat(p));
}
QString GeoMultiPolygon::toQString()
{
    //mout << "MultiPolygon.toQString()";
    QString s;
    s.append("MultiPolygon:\n");
    for(int i=0;i<polygon.size();i++)
    {
        for(int j=0;j<polygon.at(i).size();j++)
        {
            s.append(polygon[i][j].toQString());
        }
    }
    s.append("\n\nProperties:\n");
    for(int i=0;i<this->properties.size();i++){
        s.append(QString("%1:%2\n").arg(this->properties.keys().at(i))
                 .arg(this->properties.values().at(i).toString()));
    }
    return s;
}

QStringList GeoMultiPolygon::getPropertiesKeys()
{
    return properties.keys();
}
QVariant GeoMultiPolygon::getPropertiesValue(QString key)
{
    if(properties.contains(key))
    {
        return properties.value(key);
    }else{
        return "NULL";
    }
}
void GeoMultiPolygon::addPolygon(QList<LonLat> p)
{
    polygon.append(p);
}
bool GeoMultiPolygon::PropertiesContains(QString key)
{
    return this->properties.contains(key);
}
QList<QList<LonLat>> GeoMultiPolygon::getData()
{
    return this->polygon;
}
void GeoMultiPolygon::GetScale()
{
    Xmin=polygon.at(0).at(0).longitude;
    Xmax=polygon.at(0).at(0).longitude;
    Ymin=polygon.at(0).at(0).latitude;
    Ymax=polygon.at(0).at(0).latitude;
    for(int i=0;i<polygon.size();i++)
    {
        for(int j=0;j<polygon.at(i).size();j++)
        {
            if(polygon.at(i).at(j).longitude>Xmax)    Xmax=polygon.at(i).at(j).longitude;
            if(polygon.at(i).at(j).longitude<Xmin)    Xmin=polygon.at(i).at(j).longitude;
            if(polygon.at(i).at(j).latitude>Ymax)     Ymax=polygon.at(i).at(j).latitude;
            if(polygon.at(i).at(j).latitude<Ymin)     Ymin=polygon.at(i).at(j).latitude;
        }
    }
    mmRect = GeoRect(Xmin,Ymin,Xmax,Ymax);
}
void GeoMultiPolygon::setStyle(GeoStyle* style)
{
    this->multiPolygonStyle = style;
}
float GeoMultiPolygon::getLineWidth()
{
    if(multiPolygonStyle != nullptr)
        return multiPolygonStyle->stroke_width;
    else
        return 1.0f;
}
float GeoMultiPolygon::getFillB()
{
    if(multiPolygonStyle != nullptr)
        return multiPolygonStyle->fillColor.blue() / 255.0f;
    else
        return 0.0f;
}
float GeoMultiPolygon::getFillG()
{
    if(multiPolygonStyle != nullptr)
        return multiPolygonStyle->fillColor.green() / 255.0f;
    else
        return 0.0f;
}
float GeoMultiPolygon::getFillR()
{
    if(multiPolygonStyle != nullptr)
        return multiPolygonStyle->fillColor.red() / 255.0f;
    else
        return 0.0f;
}
float GeoMultiPolygon::getLineB()
{
    if(multiPolygonStyle != nullptr)
        return multiPolygonStyle->strokeColor.blue() / 255.0f;
    else
        return 1.0f;
}
float GeoMultiPolygon::getLineG()
{
    if(multiPolygonStyle != nullptr)
        return multiPolygonStyle->strokeColor.green() / 255.0f;
    else
        return 1.0f;
}
float GeoMultiPolygon::getLineR()
{
    if(multiPolygonStyle != nullptr)
        return multiPolygonStyle->strokeColor.red() / 255.0f;
    else
        return 1.0f;
}
QString GeoMultiPolygon::getLineJoin()
{
    if(multiPolygonStyle != nullptr)
        return multiPolygonStyle->stroke_linejoin;
    else
        return "bevel";
}
QString GeoMultiPolygon::getLineCap()
{
    if(multiPolygonStyle != nullptr)
        return multiPolygonStyle->stroke_linecap;
    else
        return "square";
}
//bool GeoMultiPolygon::isInArea(double minx, double miny, double maxx, double maxy)
//{
//    for(int i=0;i<polygon.count();i++){
//        QList<LonLat> po = polygon.at(i);
//        for(int j=0;j<po.count();j++){
//            if(po[j].isInArea(minx,miny,maxx,maxy))
//                return true;
//        }
//    }
//    return false;
//}
bool GeoMultiPolygon::isInArea(GeoRect rect,int n)
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
double GeoMultiPolygon::getDistance(LonLat pt)
{
    return pt.getDistance(polygon);
}
bool GeoMultiPolygon::isInPolygon(LonLat p)
{
    foreach (QList<LonLat>pts, polygon) {
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
        if(nCross % 2 == 1)
            return true;
    }

    return false;
}
void GeoMultiPolygon::transfertoXYZ()
{
    for(int i=0;i<polygon.size();i++)
    {
        for(int j=0;j<polygon.at(i).size();j++)
        {
                polygon[i][j].TransfertoXYZ();
        }
    }
}
