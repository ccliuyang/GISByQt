#ifndef ATTRIBUTETABLE_H
#define ATTRIBUTETABLE_H

#include <QWidget>
#include <QStandardItemModel>
#include "geolayer.h"

namespace Ui {
class AttributeTable;
}

class AttributeTable : public QWidget
{
    Q_OBJECT

public:
    explicit AttributeTable(QWidget *parent = nullptr);
    ~AttributeTable();
    void setTable(GeoLayer* layer);


private:
    Ui::AttributeTable *ui;
    QStandardItemModel *model = nullptr;

};

#endif // ATTRIBUTETABLE_H
