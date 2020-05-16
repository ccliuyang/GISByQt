#include "geogridindex.h"

GeoGridIndex::GeoGridIndex()
{

}
GeoGridIndex::~GeoGridIndex()
{
    for (int i=0;i<areas.count();i++) {
        delete areas[i];
    }
    areas.clear();
}
GeoGridIndex::GeoGridIndex(QList<GeoObject*>objs,GeoRect rect,int n2)
{
    this->name = "Grid";
    int size = objs.size();
    double deltx = rect.Xmax - rect.Xmin,
           delty = rect.Ymax - rect.Ymin;
    double nx = deltx/n2+1,
           ny = delty/n2+1;
    for (double i = rect.Ymin; i < rect.Ymax; i += ny)
    {
        for (double j = rect.Xmin; j < rect.Xmax; j += nx)
        {
            GeoGridIndexArea *area = new GeoGridIndexArea(j,i,j+nx,i+ny);
            areas.append(area);
            for(int k=0;k<size;k++){
                if(objs.at(k)->isInArea(area->getRect())){
                    area->addObj(objs.at(k));
                }
            }
//            mout<<area->toQString();
        }
    }

}
GeoGridIndexArea* GeoGridIndex::getArea(LonLat pt)
{
    for (int i=0;i<areas.count();i++) {
        if(areas.at(i)->isInArea(pt))
            return areas.at(i);
    }
    return nullptr;
}
void GeoGridIndex::saveIndexFile()
{

}
void GeoGridIndex::readIndexFile()
{

}
QString GeoGridIndex::toQString()
{
    QString ss;
    for(int i=0;i<areas.count();i++){
        ss.append(QString("%1\n").arg(areas.at(i)->toQString()));
    }
    return ss;
}
GeoObject* GeoGridIndex::getObj(LonLat pt)
{
    if(getArea(pt) == nullptr){
        return nullptr;
    }
    QList<GeoObject*>objs = getArea(pt)->getObjs();
    GeoObject* point = nullptr;
    GeoObject* line = nullptr;
    GeoObject* polygon = nullptr;
    GeoObject* multipolygon = nullptr;
    GeoObject* objj = nullptr;
    double dispoint = selectMinDis;//DBL_MAX;
    double disline = selectMinDis;//DBL_MAX;
    double dispolygon = DBL_MAX;
    double dismultipolygon = DBL_MAX;
    double a;
    foreach (GeoObject* obj, objs) {
        QString type = obj->getPropertiesValue("type").toString();
        if(type == "Point"){
            a = obj->getDistance(pt);
            if(a < dispoint){
                dispoint = a;
                point = obj;
            }
        }else if(type == "LineString"){
            a = obj->getDistance(pt);
            if(a < disline){
                disline = a;
                line = obj;
            }
        }else if(type == "Polygon"){
            if(obj->isInPolygon(pt)){
                a = obj->getDistance(pt);
                if(a < dispolygon){
                    dispolygon = a;
                    polygon = obj;
                }
            }
        }else if(type == "MultiPolygon"){
            if(obj->isInPolygon(pt)){
                a = obj->getDistance(pt);
                if(a < dismultipolygon){
                    dismultipolygon = a;
                    multipolygon = obj;
                }
            }
        }
    }

//    mout << dispoint;
    if(point != nullptr){
        if(line == nullptr)
            return point;
        else
            return dispoint <= disline ? point:line;
    }else{
        if(line != nullptr){
           return line;
        }else{
           if(polygon==nullptr&&multipolygon==nullptr)
               return objj;
           else if(polygon==nullptr&&multipolygon!=nullptr)
               return multipolygon;
           else if(polygon!=nullptr&&multipolygon==nullptr)
               return polygon;
           else if(polygon!=nullptr&&multipolygon!=nullptr)
               return dispolygon <= dismultipolygon ? polygon:multipolygon;
           else
               return objj;
        }
    }


//        QString type = obj->getPropertiesValue("type").toString();
//        if(type == "Point"){
//            double a = pt.getDistance(static_cast<GeoPoint*>(obj)->getData());
//            if(a < dis){
//                dis = a;
//                o = obj;
//            }
//        }else if(type == "LineString"){
//            double a = pt.getDistance(static_cast<GeoPolyline*>(obj)->getData());
//            if(a < dis){
//                dis = a;
//                o = obj;
//            }
//        }else if(type == "Polygon"){
//            double a = pt.getDistance(static_cast<GeoPolygon*>(obj)->getData());
//            if(a < dis){
//                dis = a;
//                o = obj;
//            }
//        }else if(type == "MultiPolygon"){
//            double a = pt.getDistance(static_cast<GeoMultiPolygon*>(obj)->getData());
//            if(a < dis){
//                dis = a;
//                o = obj;
//            }
//        }
}
QList<GeoRect> GeoGridIndex::getAllRect()
{
    QList<GeoRect>r;
    foreach(GeoGridIndexArea* area, this->areas){
        r.append(area->getRect());
    }
    return r;
}

double GeoGridIndex::getSelectMinDis() const
{
    return selectMinDis;
}

void GeoGridIndex::setSelectMinDis(double value)
{
    selectMinDis = value;
}
