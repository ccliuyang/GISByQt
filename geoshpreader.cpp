#include "geoshpreader.h"
#include "geopoint.h"
#include "geopolygon.h"
#include "geopolyline.h"
#include "geomultipolygon.h"
#include <QtSql/QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
GeoshpReader::GeoshpReader(QString p)
{
    //初始化时，同时初始化文件路径
    this->path = p;
    QStringList ss = p.split("/");
    QString name = ss.last();
    name = name.split(".").first();
    ss.removeLast();
    QString newPath = ss.join("/");
    dbfPath = newPath + "/TEMPGEOGIS.dbf";
    prjPath = newPath + "/TEMPGEOGIS.prj";
    sbnPath = newPath + "/TEMPGEOGIS.sbn";
    sbxPath = newPath + "/TEMPGEOGIS.sbx";
    shpPath = newPath + "/TEMPGEOGIS.shp";
    shxPath = newPath + "/TEMPGEOGIS.shx";
    olddbfPath = newPath + "/" + name + ".dbf";
    oldprjPath = newPath + "/" + name + ".prj";
    oldsbnPath = newPath + "/" + name + ".sbn";
    oldsbxPath = newPath + "/" + name + ".sbx";
    oldshpPath = newPath + "/" + name + ".shp";
    oldshxPath = newPath + "/" + name + ".shx";

    //mout << name;
}
bool GeoshpReader::read(GeoLayer *layer)
{
    //注册
    GDALAllRegister();
    //无shx也可以读
    CPLSetConfigOption("SHAPE_RESTORE_SHX","true");
    GDALDataset* poDS;

    //shp文件名不能为中文
    this->copyTempFile();
    //获得shp
    poDS = static_cast<GDALDataset*>(GDALOpenEx(this->shpPath.toUtf8(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr));

    if (poDS == nullptr)
    {
        mout <<"Open failed.\n";
        OGRCleanupAll();
        return false;
    }
    //开始读
    OGRLayer  *poLayer;
    OGRFeatureDefn *poFDefn;
    OGRFeature *poFeature;
    OGRGeometry *poGeometry;
    //读取每个图层
    for(int ily=0;ily<poDS->GetLayerCount();ily++){
        poLayer = poDS->GetLayer(ily);
        poFDefn = poLayer->GetLayerDefn();
        poLayer->ResetReading();
        //读取每个要素，按类型
        while ((poFeature = poLayer->GetNextFeature()) != nullptr){
            poGeometry = poFeature->GetGeometryRef();
            if(poGeometry!=nullptr){
                OGRwkbGeometryType pGeoType = poGeometry->getGeometryType();
                if(pGeoType == wkbPoint){
                    //点
                    OGRPoint* poPoint = static_cast<OGRPoint*>(poGeometry);
                    GeoObject* obj = new GeoPoint(LonLat(poPoint->getX(),poPoint->getY()));
                    //mout <<"["<< poPoint->getX() <<","<<poPoint->getY()<<"]";
                    this->readProperties(poFDefn,poFeature,obj);
                    obj->addpropertie("type","Point");
                    layer->addObj(obj);
                }else if(pGeoType==wkbLineString){
                    //线
                    GeoObject* obj = new GeoPolyline();
                    OGRLineString* poPolyline = static_cast<OGRLineString*>(poGeometry);
                    for(int ipoint=0;ipoint<poPolyline->getNumPoints();ipoint++){
                        obj->addPoint(LonLat(poPolyline->getX(ipoint),poPolyline->getY(ipoint)));
                        //mout << ipoint <<":["<<poPolyline->getX(ipoint)<<","<<poPolyline->getY(ipoint)<<"]";
                    }
                    this->readProperties(poFDefn,poFeature,obj);
                    obj->addpropertie("type","LineString");
                    layer->addObj(obj);
                }else if(pGeoType==wkbPolygon){
                    //面
                    GeoObject* obj = new GeoPolygon();
                    OGRPolygon* poPolygon = static_cast<OGRPolygon*>(poGeometry);
                    OGRLinearRing* poLinearRing =  poPolygon->getExteriorRing();
                    for(int ipoint=0;ipoint<poLinearRing->getNumPoints();ipoint++){
                        obj->addPoint(LonLat(poLinearRing->getX(ipoint),poLinearRing->getY(ipoint)));
                        //mout << ipoint <<":["<<poLinearRing->getX(ipoint)<<","<<poLinearRing->getY(ipoint)<<"]";
                    }
                    this->readProperties(poFDefn,poFeature,obj);
                    obj->addpropertie("type","Polygon");
                    layer->addObj(obj);
                }else if(pGeoType==wkbMultiPolygon){
                    //多面
                    GeoObject* obj = new GeoMultiPolygon();
                    QList<LonLat> geopolygon;
                    OGRMultiPolygon* poMultiPolygon = static_cast<OGRMultiPolygon*>(poGeometry);
                    OGRPolygon* poPolygon;
                    for(int ipolygon=0;ipolygon<poMultiPolygon->getNumGeometries();ipolygon++){
                        poPolygon = static_cast<OGRPolygon*>(poMultiPolygon->getGeometryRef(ipolygon));
                        OGRLinearRing* poLinearRing =  poPolygon->getExteriorRing();
                        geopolygon.clear();
                        for(int ipoint=0;ipoint<poLinearRing->getNumPoints();ipoint++){
                            geopolygon.append(LonLat(poLinearRing->getX(ipoint),poLinearRing->getY(ipoint)));
                            //mout << ipoint <<":["<<poLinearRing->getX(ipoint)<<","<<poLinearRing->getY(ipoint)<<"]";
                        }
                        static_cast<GeoMultiPolygon*>(obj)->addPolygon(geopolygon);
                    }
                    this->readProperties(poFDefn,poFeature,obj);
                    obj->addpropertie("type","MultiPolygon");
                    layer->addObj(obj);
                }else{

                }
            }
            OGRFeature::DestroyFeature(poFeature);
        }
    }
    //清除
    GDALClose(poDS);
    OGRCleanupAll();
    //删除
    this->deleteTempFile();
    return true;
}
void GeoshpReader::readProperties(OGRFeatureDefn *poFDefn,OGRFeature *poFeature, GeoObject *obj)
{
    //读取属性，按照类型读取
//    OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
    for(int iField = 0; iField < poFDefn->GetFieldCount(); iField++){
        OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn(iField);
        OGRFieldType type = poFieldDefn->GetType();
        if(OFTInteger == type){
            obj->addpropertie(poFieldDefn->GetNameRef(),poFeature->GetFieldAsInteger(iField));
            //mout<<poFeature->GetFieldAsInteger(iField);
        }else if(OFTReal == type){
            obj->addpropertie(poFieldDefn->GetNameRef(),poFeature->GetFieldAsDouble(iField));
            //mout << poFeature->GetFieldAsDouble(iField);
        }else if(OFTString == type){
            obj->addpropertie(poFieldDefn->GetNameRef(),poFeature->GetFieldAsString(iField));
            //mout << poFeature->GetFieldAsString(iField);
        }else{
            obj->addpropertie(poFieldDefn->GetNameRef(),poFeature->GetFieldAsString(iField));
            //mout << poFeature->GetFieldAsString(iField);
        }
    }
}
bool GeoshpReader::read()
{
    mout<<"1111";

    GDALAllRegister();
    CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

    GDALDataset* poDS;
    poDS = static_cast<GDALDataset*>(GDALOpenEx("F:/2.shp", GDAL_OF_VECTOR, nullptr, nullptr, nullptr));

    if (poDS == nullptr)
    {
        mout <<"Open failed.\n";
        return false;
    }

    OGRLayer  *poLayer;
    OGRFeatureDefn *poFDefn;
    OGRFeature *poFeature;
    OGRGeometry *poGeometry;
    for(int ily=0;ily<poDS->GetLayerCount();ily++){
        poLayer = poDS->GetLayer(ily);
        poFDefn = poLayer->GetLayerDefn();
        poLayer->ResetReading();
        while ((poFeature = poLayer->GetNextFeature()) != nullptr){
            poGeometry = poFeature->GetGeometryRef();
            if(poGeometry!=nullptr){
                OGRwkbGeometryType pGeoType = poGeometry->getGeometryType();
                if(pGeoType == wkbPoint){
                    OGRPoint* poPoint = static_cast<OGRPoint*>(poGeometry);
                    mout <<"["<< poPoint->getX() <<","<<poPoint->getY()<<"]";
                    for(int iField = 0; iField < poFDefn->GetFieldCount(); iField++){
                        OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn(iField);
                        OGRFieldType type = poFieldDefn->GetType();
                        if(OFTInteger == type){
                            mout<<poFeature->GetFieldAsInteger(iField);
                        }else if(OFTReal == type){
                            mout << poFeature->GetFieldAsDouble(iField);
                        }else if(OFTString == type){
                            mout << poFeature->GetFieldAsString(iField);
                        }else{
                            mout << poFeature->GetFieldAsString(iField);
                        }
                    }
                }else if(pGeoType==wkbLineString){
                    OGRLineString* poPolyline = static_cast<OGRLineString*>(poGeometry);
                    for(int ipoint=0;ipoint<poPolyline->getNumPoints();ipoint++){
                        mout << ipoint <<":["<<poPolyline->getX(ipoint)<<","<<poPolyline->getY(ipoint)<<"]";
                    }
                    for(int iField = 0; iField < poFDefn->GetFieldCount(); iField++){
                        OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn(iField);
                        OGRFieldType type = poFieldDefn->GetType();
                        if(OFTInteger == type){
                            mout<<poFeature->GetFieldAsInteger(iField);
                        }else if(OFTReal == type){
                            mout << poFeature->GetFieldAsDouble(iField);
                        }else if(OFTString == type){
                            mout << poFeature->GetFieldAsString(iField);
                        }else{
                            mout << poFeature->GetFieldAsString(iField);
                        }
                    }
                }else if(pGeoType==wkbPolygon){
                    OGRPolygon* poPolygon = static_cast<OGRPolygon*>(poGeometry);
                    OGRLinearRing* poLinearRing =  poPolygon->getExteriorRing();
                    for(int ipoint=0;ipoint<poLinearRing->getNumPoints();ipoint++){
                        mout << ipoint <<":["<<poLinearRing->getX(ipoint)<<","<<poLinearRing->getY(ipoint)<<"]";
                    }
                    for(int iField = 0; iField < poFDefn->GetFieldCount(); iField++){
                        OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn(iField);
                        OGRFieldType type = poFieldDefn->GetType();
                        if(OFTInteger == type){
                            mout<<poFeature->GetFieldAsInteger(iField);
                        }else if(OFTReal == type){
                            mout << poFeature->GetFieldAsDouble(iField);
                        }else if(OFTString == type){
                            mout << poFeature->GetFieldAsString(iField);
                        }else{
                            mout << poFeature->GetFieldAsString(iField);
                        }
                    }
                }else if(pGeoType==wkbMultiPolygon){
                    OGRMultiPolygon* poMultiPolygon = static_cast<OGRMultiPolygon*>(poGeometry);
                    OGRPolygon* poPolygon;
                    for(int ipolygon=0;ipolygon<poMultiPolygon->getNumGeometries();ipolygon++){
                        poPolygon = static_cast<OGRPolygon*>(poMultiPolygon->getGeometryRef(ipolygon));
                        OGRLinearRing* poLinearRing =  poPolygon->getExteriorRing();
                        for(int ipoint=0;ipoint<poLinearRing->getNumPoints();ipoint++){
                            mout << ipoint <<":["<<poLinearRing->getX(ipoint)<<","<<poLinearRing->getY(ipoint)<<"]";
                        }
                    }
                }else{

                }
            }
            OGRFeature::DestroyFeature(poFeature);
        }
    }
    GDALClose(poDS);
    OGRCleanupAll();
    return true;
}
bool GeoshpReader::copyTempFile()
{
    //未作检验
    if(false==QFile::copy(olddbfPath,dbfPath)){
        //return false;
    }
    if(false==QFile::copy(oldprjPath,prjPath)){
        //return false;
    }
    if(false==QFile::copy(oldsbnPath,sbnPath)){
        //return false;
    }
    if(false==QFile::copy(oldsbxPath,sbxPath)){
        //return false;
    }
    if(false==QFile::copy(oldshpPath,shpPath)){
        return false;
    }
    if(false==QFile::copy(oldshxPath,shxPath)){
        //return false;
    }
    return true;
}
void GeoshpReader::deleteTempFile()
{
    //未做检验
    QFile::remove(dbfPath);
    QFile::remove(prjPath);
    QFile::remove(sbnPath);
    QFile::remove(sbxPath);
    QFile::remove(shpPath);
    QFile::remove(shxPath);
}
bool GeoshpReader::readPostGisShp()
{
//    QSqlDatabase db;
//    db = QSqlDatabase::addDatabase("QPSQL","wyksb");
//    db.setHostName("localhost");
//    db.setDatabaseName("CHINA");
//    db.setUserName("postgres");
//    db.setPassword("991029610");
//    mout << db.open();


//////    for(int i=0;i<db.tables().count();i++)
//////    {
//////        QSqlRecord re = db.record(db.tables().at(i));
//////        for(int j=0;j<re.count();j++){
//////            mout << re.value(j);
//////        }
//////    }

////    mout << db.tables(QSql::TableType::AllTables);
//////    QSqlRecord record =  db.record("4");
//    QSqlQueryModel* model = new QSqlQueryModel();

//    model->setQuery("SELECT gid, ST_AsBinary(geom) AS geom FROM point",db);

//    int numRows = model->rowCount();
//    mout << numRows;
//    for (int i=0; i<numRows; ++i) {
//        // Read fields
//        qlonglong id = model->record(i).value("gid").toLongLong();
//        mout << id;
//        QByteArray wkb = model->record(i).value("geom").toByteArray();
//        // Process !
//        processRecord(id, wkb);
//    }
//    delete model;

//    const char *database_info = "hostaddr=127.0.0.1 dbname=CHINA user=postgres password=991029610";
//    PGconn *conn = PQconnectdb(database_info);
//    if (PQstatus(conn) == CONNECTION_BAD)
//    {
//        mout<<"connect bad"<<endl;
//        return 0;
//    }
//    else if (PQstatus(conn) == CONNECTION_OK)
//    {
//        mout<<"connect ok"<<endl;
//    }
//    // road为表的名称
//    const char *query = "SELECT gid FROM point";
//    PGresult *roadResults = PQexec(conn,query);
//    int roadNum = PQntuples(roadResults);

//    if (!roadResults||PQresultStatus(roadResults) != PGRES_TUPLES_OK )
//    {
//        mout<<"bad"<<endl;
//        PQfinish(conn);
//        return 0;
//    }
//    else
//    {
//        mout<<"ok, "<<roadNum<<endl;
//        mout<<PQgetvalue(roadResults,0,0);
//    }
//    PQclear(roadResults);
//    PQfinish(conn);
    return true;
}



//void GeoshpReader::processRecord(qlonglong id, QByteArray wkb)
//{
//    OGRSpatialReference osr;
//    OGRGeometry *geom = nullptr;

//    // Parse WKB
//    OGRErr err = OGRGeometryFactory::createFromWkb((unsigned char*)wkb.constData(), &osr, &geom);
//    if (err != OGRERR_NONE){
//        // process error, like emit signal
//    }

//    // Analyse geometry by type and process them as you wish
//    OGRwkbGeometryType type = wkbFlatten(geom->getGeometryType());
//    switch(type) {
//    case wkbLineString: {
//            OGRLineString *poRing = (OGRLineString*)geom;

//            // Access line string nodes for example :
//            int numNode = poRing->getNumPoints();
//            OGRPoint p;
//            for(int i = 0;  i<numNode; i++) {
//                poRing->getPoint(i, &p);
//                mout<< p.getX()<<p.getY();
//            }
//            break;
//    }
//    case wkbPoint:{
//        OGRPoint* poPoint = static_cast<OGRPoint*>(geom);
//        mout <<"["<< poPoint->getX() <<","<<poPoint->getY()<<"]";
//        break;
//    }
//    case wkbMultiLineString:
//    {
//        OGRGeometryCollection *poCol = (OGRGeometryCollection*) geom;
//        int numCol = poCol->getNumGeometries();
//        for(int i=0; i<numCol; i++) {
//            // Access line length for example :
//            mout<<  poCol->getGeometryRef(i)->getGeometryType();
//        }
//        break;
//    }
//    default:
//        break;
//        // process error, like emit signal
//    }
//    // Clean-up
//    OGRGeometryFactory::destroyGeometry(geom);

//}
GeoshpReader::~GeoshpReader()
{
//    db.close();
}
