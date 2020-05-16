#include "geortreeindex.h"
GeoRTreeIndex::GeoRTreeIndex()
{

}
GeoRTreeIndex::~GeoRTreeIndex()
{
    if(rootArea != nullptr)
        delete rootArea;
}
GeoRTreeIndex::GeoRTreeIndex(QList<GeoObject*>objs,GeoRect rect,int n)
{
    this->name = "RTree";
    M = n;m = n/2;
//    rootArea = new GeoRTreeIndexArea(rect);
    while (!objs.isEmpty()) {
        GeoObject* obj = objs.takeFirst();
        GeoRTreeIndexArea* area = chooseLeaf(obj);
        if(area->getObjSize()>M){
            splitNode(area);
            adjustTree(adjustLeaf(area));
        }
    }
    mout << "构造chengg";
}
void GeoRTreeIndex::adjustTree(GeoRTreeIndexArea *oriarea)
{
    if(oriarea->getSubAreaSize()>M){
        if(oriarea->hasParent()){
            GeoRTreeIndexArea* newarea = new GeoRTreeIndexArea(oriarea->getSubAreas().at(0)->getRect());
            newarea->addSubArea(oriarea->takeAreaAt(0));
            for(int i=0;i<m;i++){
                newarea->addSubArea(oriarea->takeAreaAt(0));
            }
            oriarea->getParentArea()->addSubArea(newarea);
            adjustTree(oriarea->getParentArea());
        }else{
            GeoRTreeIndexArea* newarea = new GeoRTreeIndexArea(oriarea->getSubAreas().at(0)->getRect());
            newarea->addSubArea(oriarea->takeAreaAt(0));
            for(int i=0;i<m;i++){
                newarea->addSubArea(oriarea->takeAreaAt(0));
            }
            rootArea = new GeoRTreeIndexArea(newarea->getRect());
            rootArea->addSubArea(newarea);
            rootArea->addSubArea(oriarea);
        }
    }
}
GeoRTreeIndexArea* GeoRTreeIndex::adjustLeaf(GeoRTreeIndexArea *oriarea)
{
    GeoRTreeIndexArea* parent = nullptr;
    if(oriarea->hasParent())
        parent = oriarea->getParentArea();
    else
        return nullptr;
    parent->addSubArea(oriarea->takeAllSubArea());
    parent->removeAll(oriarea);
    if(!oriarea->hasSubArea()) delete oriarea;
    return parent;
}
void GeoRTreeIndex::splitNode(GeoRTreeIndexArea *oriarea)
{
    pickSeeds(oriarea);
    GeoRTreeIndexArea* area1 = oriarea->getSubAreas().at(0);
    GeoRTreeIndexArea* area2 = oriarea->getSubAreas().at(1);
    int n1,n2,n;
    while(oriarea->getObjSize()!=0){
        n1 = area1->getObjSize();n2 = area2->getObjSize();n = oriarea->getObjSize();
        if(n1 - n2 >= n){
            while(oriarea->getObjSize()!=0){
                area2->addObj(oriarea->takeObjAt(0));
            }
            return;
        }else if(n2 - n1 >= n){
            while(oriarea->getObjSize()!=0){
                area1->addObj(oriarea->takeObjAt(0));
            }
            return;
        }
        pickNext(oriarea,area1,area2);
    }
}
void GeoRTreeIndex::pickNext(GeoRTreeIndexArea *oriarea, GeoRTreeIndexArea *area1, GeoRTreeIndexArea *area2)
{
    QList<GeoObject*>os = oriarea->getObjs();
    if(os.count()==0) return;
    double dd=-1,td,d1,d2;
    int o = 0;
    GeoRTreeIndexArea* area = nullptr;
    for(int i=0;i<os.count();i++){
        d1 = area1->getRect().getExpandAcreage(os.at(i)->mmRect);
        d2 = area2->getRect().getExpandAcreage(os.at(i)->mmRect);
        td = abs(d1 - d2);
        if(td>dd){
            dd = td;
            o=i;
            area = d1 < d2 ? area1:area2;
        }
    }
    area->addObj(oriarea->takeObjAt(o));
}
void GeoRTreeIndex::pickSeeds(GeoRTreeIndexArea *oriarea)
{
    QList<GeoObject*>os = oriarea->getObjs();
    double dmax = -1,d;
    int obj1 = 0;
    int obj2 = 0;
    for(int i=0;i<os.count()-1;i++){
        for(int j=i+1;j<os.count();j++){
            d = os.at(i)->mmRect.getMMR(os.at(j)->mmRect).getActreage()
                    -os.at(i)->mmRect.getActreage()-os.at(j)->mmRect.getActreage();
            if(d>dmax){
                dmax = d;
                obj1 = i;
                obj2 = j;
            }
        }
    }
    GeoRTreeIndexArea* area1 = new GeoRTreeIndexArea(os.at(obj1)->mmRect);
    GeoRTreeIndexArea* area2 = new GeoRTreeIndexArea(os.at(obj2)->mmRect);
    area1->addObj(oriarea->takeObjAt(obj1));
    area2->addObj(oriarea->takeObjAt(obj2-1));
    oriarea->addSubArea(area1);oriarea->addSubArea(area2);
}
GeoRTreeIndexArea* GeoRTreeIndex::chooseLeaf(GeoObject* obj)
{
    if(rootArea == nullptr)
        rootArea = new GeoRTreeIndexArea(obj->mmRect);
    if(!rootArea->hasSubArea()){
        GeoRTreeIndexArea* area = new GeoRTreeIndexArea(obj->mmRect);
        area->addObj(obj);
        rootArea->addSubArea(area);
        return area;
    }else{
        QList<GeoRTreeIndexArea*> temp;
        temp.append(rootArea->getSubAreas());
        double expandAcreage=DBL_MAX,ta;
        GeoRTreeIndexArea* area=nullptr;
        while (!temp.isEmpty()) {
            GeoRTreeIndexArea* a = temp.takeFirst();
//            if(a == nullptr)
//            mout << "zks";
            if(a->hasSubArea()){
//                mout << "zks";
                temp.append(a->getSubAreas());
            }else{
                ta = a->getRect().getExpandAcreage(obj->mmRect);
                if(ta<expandAcreage){
                    expandAcreage = ta;
                    area = a;
                }
            }
        }
        area->addObj(obj);
        return area;
    }
}
QString GeoRTreeIndex::toQString()
{
    return QString("null");
}
GeoObject* GeoRTreeIndex::getObj(LonLat pt)
{
    QList<GeoObject*>objstoreturn;
    QList<GeoRTreeIndexArea*>temp;
    temp.append(rootArea);
    GeoRTreeIndexArea* area;
    while (!temp.isEmpty()) {
        area = temp.takeLast();
        if(area->isInArea(pt)){
            if(area->hasSubArea()){
                temp.append(area->getSubAreas());
            }else{
                if(area->getObjSize()!=0){
                    objstoreturn.append(area->getObjs());
                }
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
    return nullptr;
}
void GeoRTreeIndex::readIndexFile()
{

}
void GeoRTreeIndex::saveIndexFile()
{

}
QList<GeoRect> GeoRTreeIndex::getAllRect()
{
//    mout<<"zks";
    QList<GeoRect>rs;
    QList<GeoRTreeIndexArea*> temp;
    temp.append(rootArea->getSubAreas());
    rs.append(rootArea->getRect());
    while(!temp.isEmpty()){
        GeoRTreeIndexArea* area = temp.takeFirst();
        if(area->hasSubArea()){
            temp.append(area->getSubAreas());
            foreach (GeoRTreeIndexArea* a, area->getSubAreas()) {
                rs.append(a->getRect());
            }
        }else{
            rs.append(area->getRect());
        }
    }
//    mout<<"zks"<<rs.first().toQString();
    return rs;
}
double GeoRTreeIndex::getSelectMinDis() const
{
    return selectMinDis;
}
void GeoRTreeIndex::setSelectMinDis(double value)
{
    this->selectMinDis = value;
}
