#include "lonlat.h"
#include <QStringList>
#include "math.h"


LonLat::LonLat()
{

}
LonLat::~LonLat()
{

}
LonLat::LonLat(QString s)
{
    //s = [longitude,latitude]
    s.remove("[");
    s.remove("]");
    QStringList ss = s.split(",");
    this->longitude = ss[0].toDouble();
    this->latitude = ss[1].toDouble();
}
LonLat::LonLat(double lon,double lat)
{
    this->longitude = lon;
    this->latitude = lat;
}
QString LonLat::toQString()
{
    return QString("[%1,%2]").arg(this->longitude).arg(this->latitude);
}
bool LonLat::isInArea(GeoRect rect)
{
    if(longitude >= rect.Xmin&&latitude  >= rect.Ymin
     &&longitude <= rect.Xmax&&latitude  <= rect.Ymax)
        return true;
    return false;
}
double LonLat::getDistance(LonLat p)
{
    return sqrt((p.longitude - this->longitude)*(p.longitude - this->longitude)
          +(p.latitude - this->latitude)*(p.latitude - this->latitude));
}
double LonLat::getDistance(QList<LonLat> pts)
{
    double dis=DBL_MAX;
    for(int i=0;i<pts.count();i++){
        double a = getDistance(pts.at(i));
        if(a < dis)
            dis = a;
    }
    return dis;
}
double LonLat::getDistance(QList<QList<LonLat> > polygons)
{
    double dis = DBL_MAX;
    foreach(QList<LonLat> p,polygons){
        double a = getDistance(p);
        if(a < dis)
            dis = a;
    }
    return dis;
}
void LonLat::TransfertoXYZ()
{
    double PI=3.141592653589793238462643383279;
    double A=6378137;
    double B=6356752.314;
    double E=sqrt((A*A-B*B)/(A*A));
    double N=A/(sqrt(1-E*E*(sin(longitude/180.0*PI)*sin(longitude/180.0*PI))));
    X=(N+0)*cos(longitude/180*PI)*cos(latitude/180*PI);
    Y=(N+0)*sin(longitude/180*PI)*cos(latitude/180*PI);
    Z=(N*(1-E*E)+0)*sin(latitude/180*PI);
//    double R=6378137.0;
//    X=R*cos(longitude/180.0*PI)*sin(latitude/180*PI);
//    Y=R*sin(longitude/180.0*PI)*sin(latitude/180*PI);
//    Z=R*cos(latitude/180*PI);

}
