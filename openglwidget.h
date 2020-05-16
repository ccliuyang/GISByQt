#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "geomap.h"
#include "geolayer.h"
#include "geopoint.h"
#include "geopolyline.h"
#include "geopolygon.h"
#include "geomultipolygon.h"
#include "QMouseEvent"
#include <GL/glu.h>
#include <math.h>
#include <QListView>
#include <QStringListModel>
#include <QKeyEvent>



///
/// \brief The OpenGLWidget class
///
class OpenGLWidget : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = nullptr);

    ~OpenGLWidget();
    void moveLayer(int index,int offi);

    void setLayerListView(QListView* listView);

    void addData(GeoMap* map);

    void addData(GeoLayer* layer);

    void addData(GeoObject* obj);
    ///
    /// \brief 废弃
    /// \param layer
    ///
    void addLayer(GeoLayer* layer);
    ///
    /// \brief 添加Map数据至OpenGLWidget，这里未统一函数名，数据管理改变直接废弃
    /// \param map
    ///
    void addMapData(GeoMap* map);
    ///
    /// \brief 添加Layer数据至OpenGLWidget，这里未统一函数名，数据管理改变直接废弃
    /// \param layer
    ///
    void addLayerData(GeoLayer* layer);
    ///
    /// \brief 添加Object数据至OpenGLWidget，这里未统一函数名，数据管理改变直接废弃
    /// \param obj
    ///
    void addObjectData(GeoObject* obj);
    ///
    /// \brief 添加Map数据列表至OpenGLWidget，这里未统一函数名，数据管理改变直接废弃
    /// \param map
    ///
    void addMapData(QList<GeoMap*> map);
    ///
    /// \brief 添加Layer数据列表至OpenGLWidget，这里未统一函数名，数据管理改变直接废弃
    /// \param layer
    ///
    void addLayerData(QList<GeoLayer*> layer);
    ///
    /// \brief 添加Object数据列表至OpenGLWidget这里未统一函数名，数据管理改变直接废弃
    /// \param obj
    ///
    void addObjectData(QList<GeoObject*> obj);
    ///
    /// \brief 清除OpenGLWidget储存数据，废弃
    ///
    void clearData();
    ///
    /// \brief 投影函数
    ///
    void MapPrj();
    ///
    /// \brief 鼠标事件
    /// \param event
    ///
    void mousePressEvent(QMouseEvent *event);
    ///
    /// \brief 鼠标事件
    /// \param event
    ///
    void mouseMoveEvent(QMouseEvent *event);
    ///
    /// \brief 鼠标事件
    /// \param event
    ///
    void mouseReleaseEvent(QMouseEvent *event);

    void mapTransfer();
    ///
    /// \brief 滚轮事件
    /// \param event
    ///
    void wheelEvent(QWheelEvent *event);
    ///
    /// \brief 鼠标事件
    /// \param event
    ///
    void mouseDoubleClickEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void GetOriScale();

    QPoint oldPoint;

    QPoint newPoint;

    double Scale;

    double Xmin,Xmax,Ymin,Ymax;

    double angleX;
    double angleY;
    double high;

    ///
    /// \brief 设置高亮目标，统一函数名
    /// \param map
    ///
    void setHighlightData(GeoMap* map);
    ///
    /// \brief 置高亮目标，统一函数名
    /// \param layer
    ///
    void setHighlightData(GeoLayer* layer);
    ///
    /// \brief 置高亮目标，统一函数名
    /// \param obj
    ///
    void setHighlightData(GeoObject* obj);

    void setHighlightLayer(int i);
signals:
    void selectObj(GeoObject* obj);

public slots:
    void is3D(bool checked);

    void isShowIndexRect(bool checked);
protected:
    void initializeGL();

    void paintGL();

    void resizeGL(int w, int h);
    ///
    /// \brief 绘制Map，会调用Layer绘制函数
    /// \param map
    ///
    void drawMap(GeoMap* map);
    ///
    /// \brief 绘制Layer，会调用Objectr绘制函数
    /// \param layer
    ///
    void drawLayer(GeoLayer* layer);
    ///
    /// \brief 绘制Object，根据type属性分别画图
    /// \param obj
    ///
    void drawObject(GeoObject* obj);

    LonLat GetWorldPoint(double x,double y);

    void drawGeoRects(QList<GeoRect> rs);


private:
    bool is3DView = false;

    bool isDrawIndexRect = false;

    void creatSpatialIndex();

    QList<GeoLayer*> layersTodelete;
    ///
    /// \brief 每个窗口使用一个图来管理
    ///
    GeoMap* map = nullptr;
    ///
    /// \brief 储存需要绘制的map，废弃
    ///
    QList<GeoMap*> maps;
    ///
    /// \brief 储存需要绘制的layer，废弃
    ///
    QList<GeoLayer*> layers;
    ///
    /// \brief 储存需要绘制的Object，废弃
    ///
    QList<GeoObject*> objs;

    ///
    /// \brief 废弃
    ///
    GeoMap* highlightMap = nullptr;

    GeoLayer* highlightLayer = nullptr;

    ///
    /// \brief 废弃
    ///
    GeoObject* highlightObj = nullptr;

        double DCWidth;

        double DCHeight;

        double DCLeft;

        double DCBottom;

        double PrjRight;

        double PrjTop;

        double PrjLeft;

        double PrjBottom;
        double ScaleX;
        double ScaleY;
        double tempX,tempY,tempZ;
        static void CALLBACK vertexCallback(GLvoid *vertex);
        static void CALLBACK beginCallback(GLenum which);
        static void CALLBACK endCallback  ();
        static void CALLBACK errorCallback(GLenum errorCode);
        static void CALLBACK combineCallback(GLdouble coords[3],GLdouble *vertex_data[4],GLfloat weight[4], GLdouble **dataOut );
        void StartTess(QList<LonLat> temp);
        void drawSelect();
        void drawSelectObj(GeoObject* obj);
};

#endif // OPENGLWIDGET_H
