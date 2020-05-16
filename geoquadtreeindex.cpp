#include "geoquadtreeindex.h"

GeoQuadTreeIndex::GeoQuadTreeIndex()
{

}
GeoQuadTreeIndex::~GeoQuadTreeIndex()
{
    if(rootArea != nullptr)
        delete rootArea;
    for(int i=0;i<tempAreas.count();i++){
        delete tempAreas[i];
    }

    tempAreas.clear();
}
GeoQuadTreeIndex::GeoQuadTreeIndex(QList<GeoObject*> objs,GeoRect rect,int n)
{
//    mout << "构造函数";
    this->name = "QuadTree";
    GeoQuadTreeIndexArea* area = new GeoQuadTreeIndexArea(rect);
    rootArea = area;
    area->addObj(objs);
    tempAreas.append(area);
    while(!tempAreas.isEmpty()){
//        mout << "循环开始";
        area = tempAreas.takeFirst();
        area->setAreaObjsSize(n);
        if(area->isOver()){
//            areas.append(area);

            continue;
        }
        QList<GeoRect> r = area->getRect().getSubRect();
        GeoQuadTreeIndexArea* area0 = new GeoQuadTreeIndexArea(r.at(0));
        GeoQuadTreeIndexArea* area1 = new GeoQuadTreeIndexArea(r.at(1));
        GeoQuadTreeIndexArea* area2 = new GeoQuadTreeIndexArea(r.at(2));
        GeoQuadTreeIndexArea* area3 = new GeoQuadTreeIndexArea(r.at(3));
        area->setSubArea(area0,area1,area2,area3);
        tempAreas.append(area0);tempAreas.append(area1);tempAreas.append(area2);tempAreas.append(area3);
        QList<GeoObject*> tempObjs = area->getObjs();
        for(int i=0;i<tempObjs.count();i++){
            if(tempObjs.at(i)->isInArea(r.at(0))){
                area0->addObj(tempObjs.at(i));
            }
            if(tempObjs.at(i)->isInArea(r.at(1))){
                area1->addObj(tempObjs.at(i));
            }
            if(tempObjs.at(i)->isInArea(r.at(2))){
                area2->addObj(tempObjs.at(i));
            }
            if(tempObjs.at(i)->isInArea(r.at(3))){
                area3->addObj(tempObjs.at(i));
            }else{

            }
        }
    }
//    mout <<"创建结束";
}
QString GeoQuadTreeIndex::toQString()
{
//    mout << "四叉树索引toQString";
    QString ss;
    QList<GeoQuadTreeIndexArea*> temp;
    GeoQuadTreeIndexArea* area;
    temp.append(rootArea);
    while(!temp.isEmpty()){
        area = temp.takeFirst();
        ss.append(area->toQString());
        if(false == area->hasSubArea())
            continue;
        temp.append(area->getSubArea());
    }
    return ss;
}
void GeoQuadTreeIndex::saveIndexFile()
{

}
void GeoQuadTreeIndex::readIndexFile()
{

}
GeoObject* GeoQuadTreeIndex::getObj(LonLat pt)
{
    QList<GeoObject*>objstoreturn;
    QList<GeoQuadTreeIndexArea*> temp;
    temp.append(rootArea);
    GeoQuadTreeIndexArea* area;
    while(!temp.isEmpty()){
        area = temp.takeLast();
        if(area->isInArea(pt)){
            if(!area->hasSubArea()){
                if(!area->getObjs().isEmpty())
                    objstoreturn.append(area->getObjs());
                break;
            }else{
                temp.append(area->getSubArea());
            }
        }
    }

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

    foreach(GeoObject* obj, objstoreturn){
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

}
QList<GeoRect> GeoQuadTreeIndex::getAllRect()
{
    QList<GeoRect>r;
    QList<GeoQuadTreeIndexArea*> temp;
    GeoQuadTreeIndexArea* area;
    temp.append(rootArea);
    while(!temp.isEmpty()){
        area = temp.takeFirst();
        r.append(area->getRect());
        if(false == area->hasSubArea())
            continue;
        temp.append(area->getSubArea());
    }
    return r;
}
GeoQuadTreeIndexArea* GeoQuadTreeIndex::getArea(LonLat pt)
{

}

double GeoQuadTreeIndex::getSelectMinDis() const
{
    return selectMinDis;
}

void GeoQuadTreeIndex::setSelectMinDis(double value)
{
    selectMinDis = value;

}
