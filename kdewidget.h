#ifndef KDEWIDGET_H
#define KDEWIDGET_H

#include <QWidget>
#include "kdecells.h"

namespace Ui {
class KDEWidget;
}

class KDEWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KDEWidget(QList<GeoLayer*>ls,QWidget *parent = nullptr);
    ~KDEWidget();

private slots:
    void on_pushButton_GetPath_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::KDEWidget *ui;
    QList<GeoLayer*> layers;
};

#endif // KDEWIDGET_H
