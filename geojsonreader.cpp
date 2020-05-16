#include "geojsonreader.h"
#include <QFile>
#include <QtDebug>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include "geomap.h"
#include "geolayer.h"
#include "geoobject.h"
#include "geopoint.h"
#include "geopolyline.h"
#include "geopolygon.h"
#include "geomultipolygon.h"

#define mout qDebug() <<"["<<__FILE__<<":"<<__LINE__<<"]"

geoJsonReader::geoJsonReader(QString p)
{
    this->path = p;

}
bool geoJsonReader::read()
{
    QFile jsonFile(this->path);
    jsonFile.open(QIODevice::ReadOnly);
    QByteArray allData = jsonFile.readAll();
    jsonFile.close();

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
            mout << "json error!";
            return false;
    }
    QJsonObject rootObj = jsonDoc.object();
    QStringList keys = rootObj.keys();
    for(int i = 0; i < keys.size(); i++)
    {
        mout << "key" << i << " is:" << keys.at(i);
    }
    QJsonArray features = rootObj.take("features").toArray();
    for(int i = 0;i<features.size();i++)
    {
        QJsonObject obj = features.at(i).toObject();
        for(int j = 0; j < obj.keys().size(); j++)
        {
            mout << "key" << j << " is:" << obj.keys().at(j);
        }
    }
//    GeoMap *map = new GeoMap("test");
//    GeoLayer *layer = new GeoLayer();
//    GeoObject *objpoint = new GeoPoint(LonLat(1.231,1.3123));
//    GeoObject *objpolyline = new GeoPolyline();
//    objpolyline->addPoint(LonLat(2.323,2.45345));
//    objpolyline->addPoint(LonLat(2.73,2.75345));
//    objpolyline->addPoint(LonLat(2.823,2.85345));
//    GeoObject *objpolygon = new GeoPolygon();
//    objpolygon->addPoint(LonLat(3.232,3.123));
//    objpolygon->addPoint(LonLat(3.432,3.423));
//    objpolygon->addPoint(LonLat(3.632,3.623));
//    objpolygon->addPoint(LonLat(3.232,3.123));
//    layer->addObj(objpoint);
//    layer->addObj(objpolyline);
//    layer->addObj(objpolygon);
//    map->addLayer(layer);
//    mout << map->toQString();
//    delete map;
    return true;
}
bool geoJsonReader::read(GeoLayer *layer)
{
    //打开json文件
    QFile jsonFile(this->path);
    jsonFile.open(QIODevice::ReadOnly);
    QByteArray allData = jsonFile.readAll();
    jsonFile.close();
    //利用QJson读取
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
            mout << "json error!";
            return false;
    }
    //跟Object
    QJsonObject rootObj = jsonDoc.object();
    //读属性
    if(true == rootObj.contains("properties"))
    {
        QJsonObject properties = rootObj.take("properties").toObject();
        QMap<QString,QVariant> propertiesmap = properties.toVariantMap();

        QStringList keys = propertiesmap.keys();
        for(int i=0;i<propertiesmap.size();i++)
        {
//            QStringList keys = propertiesmap.keys();
            layer->addpropertie(keys.at(i),propertiesmap.value(keys.at(i)));
        }
    }
    //向下读取
    QJsonArray features = rootObj.take("features").toArray();
    for(int i=0;i<features.size();i++)
    {
        QJsonObject feature = features[i].toObject();
        QJsonObject geometry = feature.take("geometry").toObject();
        GeoObject *obj = nullptr;
        if(geometry.value("type") == "Point")
        {
            //mout << "读取点数据";
            obj = new GeoPoint(LonLat(geometry.value("coordinates").toArray()[0].toDouble(),
                               geometry.value("coordinates").toArray()[1].toDouble()));
            obj->addpropertie("type","Point");
        }else if(geometry.value("type") == "LineString"){
            //mout<< "读取线数据";
            obj = new GeoPolyline();
            QJsonArray coordinates = geometry.take("coordinates").toArray();
            for(int i=0;i<coordinates.size();i++)
            {
                obj->addPoint(LonLat(coordinates.at(i).toArray()[0].toDouble(),
                              coordinates.at(i).toArray()[1].toDouble()));
            }
            obj->addpropertie("type","LineString");
        }else if (geometry.value("type") == "Polygon") {
            //mout << "读取面数据";
            obj = new GeoPolygon();
            QJsonArray pts = geometry.take("coordinates").toArray()[0].toArray();
            for(int i=0;i<pts.size();i++)
            {
                obj->addPoint(LonLat(pts.at(i).toArray()[0].toDouble(),
                              pts.at(i).toArray()[1].toDouble()));
            }
            obj->addpropertie("type","Polygon");
        }else if(geometry.value("type") == "MultiPolygon"){
            //mout << "读取多面数据";
            obj = new GeoMultiPolygon();
            QJsonArray coordinates = geometry.take("coordinates").toArray();
            QList<LonLat> p;
            for(int i=0;i<coordinates.size();i++)
            {
                QJsonArray pts = coordinates.at(i).toArray().at(0).toArray();

                for(int j=0;j<pts.size();j++)
                {
                    p.append(LonLat(pts.at(j).toArray()[0].toDouble(),
                             pts.at(j).toArray()[1].toDouble()));
                }
                static_cast<GeoMultiPolygon *>(obj)->addPolygon(p);
            }
            obj->addpropertie("type","MultiPolygon");
        }else{

        }
        //读取属性
        if(true == feature.contains("properties") && nullptr != obj)
        {
            QJsonObject properties = feature.take("properties").toObject();
            QMap<QString,QVariant> propertiesmap = properties.toVariantMap();
            QStringList keys = propertiesmap.keys();
            for(int i=0;i<propertiesmap.size();i++)
            {
//                QStringList keys = propertiesmap.keys();
                obj->addpropertie(keys.at(i),propertiesmap.value(keys.at(i)));
            }
        }
        //添加Object
        if(nullptr != obj)
            layer->addObj(obj);

    }
    return true;
}
