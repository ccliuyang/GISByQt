#include "geopostgislinker.h"
#include "ui_geopostgislinker.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStandardItem>
#include "geoobject.h"
#include "geopoint.h"
#include "geopolyline.h"
#include "geopolygon.h"
#include "geomultipolygon.h"

GeoPostGISLinker::GeoPostGISLinker(int id,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GeoPostGISLinker)
{
    ui->setupUi(this);
    this->model = new QSqlQueryModel();
    this->propertieModel = new QSqlQueryModel();
    this->setWindowTitle("Link PostGIS");
    this->mapIndexID = id;
    db = QSqlDatabase::addDatabase("QPSQL","WYKNB");
    connect(ui->Link_Button,&QPushButton::clicked,this,&GeoPostGISLinker::slotLink);
    connect(ui->Add_Button,&QPushButton::clicked,this,&GeoPostGISLinker::slotAddToMap);
    connect(ui->Cancel_Button,&QPushButton::clicked,this,&GeoPostGISLinker::close);
}

GeoPostGISLinker::~GeoPostGISLinker()
{
    delete ui;
    delete model;
    if(db.isOpen())
        db.close();
}
void GeoPostGISLinker::slotLink()
{
    if(ui->Username_lineEdit->text().isEmpty()){
        QMessageBox::information(this,"warning","Username 为空！");
        return;
    }
    if(ui->Password_lineEdit->text().isEmpty()){
        QMessageBox::information(this,"warning","Password 为空！");
        return;
    }
    if(ui->Hostname_lineEdit->text().isEmpty()){
        QMessageBox::information(this,"warning","Hostname 为空！");
        return;
    }
    if(ui->Port_lineEdit->text().isEmpty()){
        QMessageBox::information(this,"warning","Port 为空！");
        return;
    }else{
        if(ui->Port_lineEdit->text().toInt()<=0){
            QMessageBox::information(this,"warning","输入正确的Port");
            return;
        }
    }
    if(ui->Database_lineEdit->text().isEmpty()){
        QMessageBox::information(this,"warning","Database 为空！");
        return;
    }
    db.setHostName(ui->Hostname_lineEdit->text());
    db.setPassword(ui->Password_lineEdit->text());
    db.setUserName(ui->Username_lineEdit->text());
    db.setPort(ui->Port_lineEdit->text().toInt());
    db.setDatabaseName(ui->Database_lineEdit->text());
    if(db.open()){
        setTablesList();
    }else{
        QMessageBox::information(this,"warning","连接失败！");
        return;
    }
}
void GeoPostGISLinker::slotAddToMap()
{
    QModelIndexList indexList =  ui->Tables_listView->selectionModel()->selectedIndexes();
    for (int i=0;i<indexList.count();i++) {
        QString tableName = indexList.at(i).data().toString();
        model->setQuery(QString("SELECT gid, ST_AsBinary(geom) AS geom FROM %1")
                        .arg(tableName),db);
        propertieModel->setQuery(QString("SELECT * FROM %1")
                                 .arg(tableName),db);
        this->layer = new GeoLayer();
        layer->addpropertie("name",tableName+".postGIS");
        layer->addpropertie("FilePath",QString("From PostGIS Username: %1,Hostname: %2,Port: %3,Table: %4")
                            .arg(db.userName()).arg(db.hostName()).arg(db.port()).arg(tableName));
        for (int j=0;j<model->rowCount();++j) {
//            qlonglong id = model->record(j).value("gid").toLongLong();
            QByteArray wkb = model->record(j).value("geom").toByteArray();
            processRecord(j,wkb);
        }
        emit addLayerToMap(this->mapIndexID,layer);
    }
    emit this->close();
}
void GeoPostGISLinker::setTablesList()
{
    QStringList ss = db.tables(QSql::TableType::Tables);
    QStringList result = ss.filter("tiger.");
    result.append(ss.filter("topology."));
    result.append(ss.filter("spatial_"));
    result.append(ss.filter("pointcloud_"));
    for (int i=0;i<result.count();i++) {
        ss.removeOne(result.at(i));
    }
    QStandardItemModel *ItemModel = new QStandardItemModel(this);
    for (int i=0;i<ss.count();i++) {
        QStandardItem *item = new QStandardItem(ss.at(i));
        ItemModel->appendRow(item);
    }
    ui->Tables_listView->setModel(ItemModel);
    ui->Tables_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->Tables_listView->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);
}
void GeoPostGISLinker::processRecord(int id,QByteArray wkb)
{
        OGRSpatialReference osr;
        OGRGeometry *geom = nullptr;

        // Parse WKB

        OGRErr err = OGRGeometryFactory::createFromWkb((unsigned char*)wkb.constData(), &osr, &geom);
        if (err != OGRERR_NONE){
            // process error, like emit signal
        }

        // Analyse geometry by type and process them as you wish
        OGRwkbGeometryType type = wkbFlatten(geom->getGeometryType());
//        mout << type;
        switch(type) {
        case wkbLineString: {
            //线
            GeoObject* obj = new GeoPolyline();
            OGRLineString* poPolyline = static_cast<OGRLineString*>(geom);
            for(int ipoint=0;ipoint<poPolyline->getNumPoints();ipoint++){
                obj->addPoint(LonLat(poPolyline->getX(ipoint),poPolyline->getY(ipoint)));
                //mout << ipoint <<":["<<poPolyline->getX(ipoint)<<","<<poPolyline->getY(ipoint)<<"]";
            }
//            this->readProperties(poFDefn,poFeature,obj);
            obj->addpropertie("type","LineString");
            readProperties(obj,id);
            layer->addObj(obj);
                break;
        }
        case wkbPoint:{
            //点
            OGRPoint* poPoint = static_cast<OGRPoint*>(geom);
            GeoObject* obj = new GeoPoint(LonLat(poPoint->getX(),poPoint->getY()));
//            mout <<"["<< poPoint->getX() <<","<<poPoint->getY()<<"]";
//            this->readProperties(poFDefn,poFeature,obj);
            obj->addpropertie("type","Point");
            readProperties(obj,id);
            layer->addObj(obj);
            break;
        }
        case wkbPolygon:{
            //面
            GeoObject* obj = new GeoPolygon();
            OGRPolygon* poPolygon = static_cast<OGRPolygon*>(geom);
            OGRLinearRing* poLinearRing =  poPolygon->getExteriorRing();
            for(int ipoint=0;ipoint<poLinearRing->getNumPoints();ipoint++){
                obj->addPoint(LonLat(poLinearRing->getX(ipoint),poLinearRing->getY(ipoint)));
                //mout << ipoint <<":["<<poLinearRing->getX(ipoint)<<","<<poLinearRing->getY(ipoint)<<"]";
            }
//            this->readProperties(poFDefn,poFeature,obj);
            obj->addpropertie("type","Polygon");
            readProperties(obj,id);
            layer->addObj(obj);
            break;
        }
        case wkbMultiPolygon:{
            //多面
            GeoObject* obj = new GeoMultiPolygon();
            QList<LonLat> geopolygon;
            OGRMultiPolygon* poMultiPolygon = static_cast<OGRMultiPolygon*>(geom);
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
//            this->readProperties(poFDefn,poFeature,obj);
            obj->addpropertie("type","MultiPolygon");
            readProperties(obj,id);
            layer->addObj(obj);

            break;
        }
        case wkbMultiLineString:{
            //线
            GeoObject* obj = new GeoPolyline();
            OGRLineString* poPolyline = static_cast<OGRLineString*>(geom);
            for(int ipoint=0;ipoint<poPolyline->getNumPoints();ipoint++){
                obj->addPoint(LonLat(poPolyline->getX(ipoint),poPolyline->getY(ipoint)));
                //mout << ipoint <<":["<<poPolyline->getX(ipoint)<<","<<poPolyline->getY(ipoint)<<"]";
            }
//            this->readProperties(poFDefn,poFeature,obj);
            obj->addpropertie("type","LineString");
            readProperties(obj,id);
            layer->addObj(obj);
            break;
        }
        default:
            break;
            // process error, like emit signal
        }
        // Clean-up
        OGRGeometryFactory::destroyGeometry(geom);
}
void GeoPostGISLinker::readProperties(GeoObject *obj, int index)
{
    for (int i=0;i<propertieModel->columnCount();i++) {
        QString propertieName = propertieModel->record(index).fieldName(i);
        if(propertieName == "gid"||propertieName == "geom")
            continue;
        obj->addpropertie(propertieName,propertieModel->record(index).value(propertieName));
    }
}
