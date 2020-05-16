#ifndef GEOSTYLE_H
#define GEOSTYLE_H

#include <QColor>

class GeoStyle
{
public:
    GeoStyle();
    float pointSize;
    float stroke_width;
    QString stroke_linejoin;
    QString stroke_linecap;
//    QColor pointColor;
    QColor strokeColor;
    QColor fillColor;

};

#endif // GEOSTYLE_H
