#include "kdecells.h"
#include <QImage>
#include "math.h"
KDECells::KDECells(GeoLayer *l,double Xsize,double Ysize,double tape)

{
    this->tape=tape;
    this->layer=l;
    this->layer->GetLayerScale();
    this->DisDataX=(layer->Xmax)-(layer->Xmin);
    this->DisDataY=(layer->Ymax)-(layer->Ymin);
    this->CellSizeX=Xsize;
    this->CellSizeY=Ysize;
    this->XRatio=(0.5+DisDataX)/CellSizeX;
    this->YRatio=(0.5+DisDataY)/CellSizeY;

    for(int row=0;row<YRatio;row++)
    {
        QList <double> R;

        for(int col=0;col<XRatio;col++)
        {
            R.append(0);
        }
        Cells.append(R);
    }
//    mout << "sss";
    Max=-999999999;
    Min= 999999999;
}
void KDECells::getDensity()
{
//    mout << "sss";
    int size=layer->getObjSize();
    for(int row=0;row<YRatio;row++)
    {
        for(int col=0;col<XRatio;col++)
        {
            density=0;
            double DX,DY;
            DX=(col+0.5)*CellSizeX+layer->Xmin;
            DY=(row+0.5)*CellSizeY+layer->Ymin;
            double K,dis;
            for(int i=0;i<size;i++)
            {

                dis=layer->getObj(i)->getDistance(LonLat(DX,DY));
                dis=dis/tape;
                if(dis<=1.0&&dis>=-1)           K=(0.75*(1.0-dis*dis));
                else K=0;
                if(K<0) mout <<K;
                density=density+K;
            }
            density=density/(size*tape);
            Cells[row][col]=density;
            if(density>=Max) Max=density;
            if(density<=Min) Min=density;

//            mout << density;

        }
    }
    for(int row=0;row<YRatio;row++)
    {
        for(int col=0;col<XRatio;col++)
        {

        }
    }
//    mout<<Max<<"!!!"<<Min;
//    mout << "sss";
}
bool KDECells::saveAsJPG(QString path)
{
    QImage img = QImage (XRatio,YRatio,QImage::Format_RGB32);
//    mout << "sss";
    getDensity();
    QRgb value;
//    uchar* data = new uchar[XRatio*YRatio*4];
    for(int row=0;row<YRatio;row++){
        for(int col=0;col<XRatio;col++){
//            data[row*XRatio + col*4] = static_cast<uchar>(0.0);
//            data[row*XRatio + col*4+1] = static_cast<uchar>(((Cells.at(YRatio-1-row).at(col)-Min)/(Max-Min)*255));
//            data[row*XRatio + col*4+2] = static_cast<uchar>(0.0);
//            data[row*XRatio + col*4+3] = static_cast<uchar>(0.0);
//            if(data[row*XRatio + col]!=0)   mout << data[row*XRatio + col];
            double temp=((Cells.at(YRatio-1-row).at(col)-Min)/(Max-Min))*255;
            if(temp<5)  value=qRgb(255,255,255);
            else if (temp>=5&&temp<=50) value=qRgb(0,191,255);
            else if (temp>50&&temp<=90) value=qRgb(192,255,193);
            else if (temp>90&&temp<=135) value=qRgb(0,255,127);
            else if (temp>135&&temp<=175) value=qRgb(255,246,143);
            else if (temp>175&&temp<=205) value=qRgb(255,215,0);
            else value=qRgb(255,69,0);
            img.setPixel(col,row,value);

//            mout <<temp;
        }

    }
//    mout << XRatio << "|"<<YRatio;

//    mout << data[1];
    mout<<Max<<"!!!"<<Min;
    return img.save(path,"JPG",100);
//    delete[] data;
//    delete img;
//    mout << "sss";
}


