#ifndef GEOSHPREADER_H
#define GEOSHPREADER_H

#include "gdal.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include "geolayer.h"

#include "libpq-fe.h"
#include <QFile>
//#include <QSqlDatabase>
///
/// \brief The GeoshpReader class
///
class GeoshpReader
{
public:
    ~GeoshpReader();
    ///
    /// \brief SHP读取
    /// \param path
    ///
    GeoshpReader(QString path);
    ///
    /// \brief 读取数据存入layer
    /// \param layer
    /// \return 是否成功
    ///
    bool read(GeoLayer* layer);
    ///
    /// \brief 测试函数
    /// \return
    ///
    bool read();
    ///
    /// \brief 测试函数
    /// \return
    ///
    bool readPostGisShp();
    ///
    /// \brief 测试函数
    /// \param id
    /// \param wkb
    ///
    void processRecord(qlonglong id, QByteArray wkb);

private:
//    QSqlDatabase db;

    QString path;

    QString dbfPath,olddbfPath;

    QString prjPath,oldprjPath;

    QString sbnPath,oldsbnPath;

    QString sbxPath,oldsbxPath;

    QString shpPath,oldshpPath;

    QString shxPath,oldshxPath;
    ///
    /// \brief 复制SHP文件及其关联文件
    /// \return
    ///
    bool copyTempFile();

    ///
    /// \brief 删除复制文件
    ///
    void deleteTempFile();
    ///
    /// \brief 读取属性到obj
    /// \param poFDefn
    /// \param poFeature
    /// \param obj
    ///
    void readProperties(OGRFeatureDefn *poFDefn,OGRFeature *poFeature,GeoObject *obj);


};

#endif // GEOSHPREADER_H
