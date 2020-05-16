#include "attributetable.h"
#include "ui_attributetable.h"
#include "geoobject.h"
AttributeTable::AttributeTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AttributeTable)
{
    ui->setupUi(this);
    this->setWindowTitle("Attribute Table");
    model = new QStandardItemModel();
}

AttributeTable::~AttributeTable()
{
    delete ui;
    if(model != nullptr)
        delete model;
}
void AttributeTable::setTable(GeoLayer *layer)
{
    QList<GeoObject*> objs = layer->getAllObjs();
    int rowCount = objs.count();
    QStringList keys = objs.at(0)->getPropertiesKeys();
    int colCount = keys.count();
    model->setColumnCount(colCount);
    for (int i=0;i<colCount;i++) {
        model->setHorizontalHeaderItem(i,new QStandardItem(keys.at(i)));
    }
    for (int row=0;row<rowCount;row++) {
        for (int col=0;col<colCount;col++) {
            model->setItem(row,col,new QStandardItem(objs.at(row)->getPropertiesValue(keys.at(col)).toString()));
        }
    }
    ui->tableView->setModel(model);
}
