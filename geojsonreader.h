#ifndef GEOJSONREADER_H
#define GEOJSONREADER_H
#include <QString>
#include "geolayer.h"
///
/// \brief The geoJsonReader class
///
class geoJsonReader
{
public:
    ///
    /// \brief Json读取
    /// \param path：Json文件路径
    ///
    geoJsonReader(QString path);
    ///
    /// \brief 测试函数
    /// \return 是否成功读取
    ///
    bool read();
    ///
    /// \brief 读取函数存入layer
    /// \param layer：数据存放图层
    /// \return 是否成功读取
    ///
    bool read(GeoLayer *layer);
private:
    ///
    /// \brief path
    ///
    QString path;
};

#endif // GEOJSONREADER_H
