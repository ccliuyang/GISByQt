#include "kdewidget.h"
#include "ui_kdewidget.h"
#include <QFileDialog>

KDEWidget::KDEWidget(QList<GeoLayer*>ls,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KDEWidget)
{
    ui->setupUi(this);

    foreach (GeoLayer* l, ls) {
        ui->comboBox_layer->addItem(l->getPropertiesValue("name").toString());
    }
    layers.append(ls);
    this->setWindowTitle("KDE");
}

KDEWidget::~KDEWidget()
{
    delete ui;
}

void KDEWidget::on_pushButton_GetPath_clicked()
{
    QString path = QFileDialog::getSaveFileName(this,"Save Path","../","JPG(*.jpg)");
    if(path==nullptr){
        return;
    }
    ui->lineEdit_Path->setText(path);
}

void KDEWidget::on_pushButton_save_clicked()
{
    if(ui->comboBox_layer->currentIndex()==-1)
        return;
    if(ui->lineEdit_CellSize->text().toDouble()<0)
        return;
    if(ui->lineEdit_Tape->text().toDouble()<0)
        return;
    if(ui->lineEdit_Path->text()==nullptr)
        return;

    GeoLayer* l = layers.at(ui->comboBox_layer->currentIndex());
    l->GetLayerScale();
    double cs = ui->lineEdit_CellSize->text().toDouble();
    double tape = ui->lineEdit_Tape->text().toDouble();
    mout << l->getPropertiesValue("name") << cs << tape;
    KDECells* kdeCells = new KDECells(l,cs,cs,tape);
    bool ok = kdeCells->saveAsJPG(ui->lineEdit_Path->text());
    if(ok == true)
    {
        ui->img_Label->setStyleSheet(QStringLiteral("border-image: url(%1)").arg(ui->lineEdit_Path->text()));
    }
}

void KDEWidget::on_pushButton_cancel_clicked()
{
    this->close();
}
