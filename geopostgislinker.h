#ifndef GEOPOSTGISLINKER_H
#define GEOPOSTGISLINKER_H

#include <QWidget>
#include "geolayer.h"
#include "gdal.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
namespace Ui {
class GeoPostGISLinker;
}

class GeoPostGISLinker : public QWidget
{
    Q_OBJECT

public:
    explicit GeoPostGISLinker(int IndexID,QWidget *parent = nullptr);

    ~GeoPostGISLinker();
signals:
    void addLayerToMap(int IndexID, GeoLayer* ly);

private slots:
    void slotLink();
    void slotAddToMap();
    void processRecord(int id,QByteArray wkb);

private:
    Ui::GeoPostGISLinker *ui;
    QSqlQueryModel* propertieModel = nullptr;
    QSqlQueryModel* model = nullptr;
    GeoLayer* layer = nullptr;
    int mapIndexID;
    QSqlDatabase db;
    void setTablesList();
    void readProperties(GeoObject* obj,int index);

};

#endif // GEOPOSTGISLINKER_H
