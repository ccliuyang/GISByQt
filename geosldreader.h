#ifndef GEOSLDREADER_H
#define GEOSLDREADER_H
#include <QString>
#include <QXmlStreamReader>
#include <QFile>
#include "geolayer.h"
class GeoSLDReader
{
public:
    GeoSLDReader(QString path);
    ///
    /// \brief 测试函数
    /// \return
    ///
    bool read();
    bool read(GeoLayer* layer);
private:
    QString path;
};

#endif // GEOSLDREADER_H
