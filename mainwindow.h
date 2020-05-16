#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define mout qDebug() <<"["<<__FILE__<<":"<<__LINE__<<"]"
#include <QMainWindow>
#include <QTreeView>
#include "openglwidget.h"
#include "geomap.h"
#include <QStandardItem>
#include "myfilterproxymodel.h"
#include "geopostgislinker.h"
#include <QLabel>
#include "attributetable.h"
#include "kdewidget.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showKDEWidget();
    ///
    /// \brief 连接到PostGIS
    ///
    void linkToPostGIS();
    ///
    /// \brief 创造一个新Map，储存在maps里面
    ///
    void createMap();
    ///
    /// \brief 移除一个Map，会析构
    ///
    void removeMap();
    ///
    /// \brief 添加Json文件，连接菜单项
    ///
    void addJson();
    ///
    /// \brief 添加SHP文件，连接菜单项
    ///
    void addSHP();
    ///
    /// \brief 初始化treeview，定义菜单，设置代理等等
    /// \param treeView:需要初始化的treeview
    ///
    void initMapTreeView(QTreeView* treeView);
    ///
    /// \brief 在MessageBox显示信息
    /// \param msg:显示的信息
    ///
    void showMessage(QVariant msg);
    ///
    /// \brief 利用maps设置treeView
    /// \param maps
    /// \param treeView
    ///
    void setMapTreeView(QList<GeoMap*> maps,QTreeView* treeView);
    ///
    /// \brief 将选择的目标传给openglWidget
    /// \param openglWidget
    /// \return 范围Map的IndexID索引值，用于投影，若为-1表示未选中目标
    ///
    int sentSelectToOpenGl(OpenGLWidget* openglWidget);

    OpenGLWidget* initOpenGlWidget();

    void setMapTreeMenuEnble(QModelIndex curIndex,QMenu &menu);
signals:

private:
    Ui::MainWindow *ui;
    ///
    /// \brief 状态栏显示信息Label
    ///
    QLabel* statusLabel = nullptr;
    ///
    /// \brief postGISLinker 成员函数便于管理
    ///
    GeoPostGISLinker* postGISLinker= nullptr;
    //QList<OpenGLWidget*> openGLWidgets;
    ///
    /// \brief 储存Map
    ///
    QList<GeoMap *> maps;
    ///
    /// \brief MapTreeModel TreeView
    ///
    QStandardItemModel* MapTreeModel= nullptr;
    ///
    /// \brief MapTreeProxyModel 过滤代理
    ///
    MyFilterProxyModel* MapTreeProxyModel= nullptr;

    AttributeTable* attributeTable = nullptr;
private slots:
    void slotShowObjData(GeoObject* obj);

    void slotMapTreeMenuShowIndex();

    void slotMapTreeMenuIndex();

    void slotMapTreeMenuAttribute();

    void slotMapTreeMenuRename();
    ///
    /// \brief MapTree菜单连接PostGIS，自动获得目标索引IndexID
    ///
    void slotMapTreeMenuLinkPostGIS();
    ///
    /// \brief 为了好组织PostGIS而新添加的添加图层到图的槽函数
    /// \param IndexId
    /// \param layer
    ///
    void slotAddLayerToMap(int IndexId,GeoLayer* layer);
    ///
    /// \brief MapTree菜单构建槽函数
    /// \param pos
    ///
    void slotMapTreeMenu(const QPoint &pos);
    ///
    /// \brief MapTree菜单获得属性和数据
    ///
    void slotMapTreeMenutoQString();
    ///
    /// \brief MapTree菜单画图，自动获得目标索引IndexID
    ///
    void slotMapTreeMenuDraw();
    ///
    /// \brief MapTree菜单添加Json，自动获得目标索引IndexID
    ///
    void slotMapTreeMenuAddJson();
    ///
    /// \brief MapTree菜单添加shp，自动获得目标索引IndexID
    ///
    void slotMapTreeMenuAddSHP();
    ///
    /// \brief MapTree菜单删除目标，自动获得目标索引IndexID
    ///
    void slotMapTreeMenuDelete();
    ///
    /// \brief MapTree菜单连接SLD，自动获得目标索引IndexID
    ///
    void slotMapTreeMenuLinkSLD();
    ///
    /// \brief OpenGL画图窗口关闭槽函数
    /// \param index：关闭Tab索引
    ///
    void slotDrawTabClose(int index);
    ///
    /// \brief 创建地图槽函数
    ///
    void slotCreateMap();
    ///
    /// \brief 实现TreeView父节点与子节点的关联
    /// \param item
    ///
    void slotMapTreeItemChanged(QStandardItem * item );
    ///
    /// \brief 点击画选择目标
    ///
    void on_pushButton_DrawSelect_clicked();
    ///
    /// \brief 当输入框文本被修改时调用删选器
    /// \param arg1:文本框内容
    ///
    void on_lineEdit_TreeSearch_textEdited(const QString &arg1);


    void on_mapTreeView_pressed(const QModelIndex &index);
    void on_mapTreeView_doubleClicked(const QModelIndex &index);
    void on_pushButton_Down_clicked();
    void on_listView_DrawLayer_doubleClicked(const QModelIndex &index);
    void on_pushButton_Up_clicked();

    void on_tabWidget_Draw_tabBarClicked(int index);
};
#endif // MAINWINDOW_H
