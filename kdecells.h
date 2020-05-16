#ifndef KDECELLS_H
#define KDECELLS_H
#include <QList>
#include "geolayer.h"


class KDECells
{
public:
    KDECells(GeoLayer *l,double Xsize,double Ysize,double tape);
    bool saveAsJPG(QString path);
private:
    GeoLayer *layer;
    QList<QList<double>> Cells;
    void getAllValue();
    int XRatio;
    int YRatio;
    double DisDataX,DisDataY;
    double CellSizeX,CellSizeY;
    double CX,CY;
    double density;
    double tape;
    void getDensity();
    double Max;
    double Min;

};

#endif // KDECELLS_H
