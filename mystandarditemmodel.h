#ifndef MYSTANDARDITEMMODEL_H
#define MYSTANDARDITEMMODEL_H

#include <QObject>
#include <QStandardItem>

class MyStandardItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit MyStandardItemModel( *parent = nullptr);
};

#endif // MYSTANDARDITEMMODEL_H
