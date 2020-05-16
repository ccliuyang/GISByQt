#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QFileDialog>
#include <QTreeView>
#include <QStandardItemModel>
#include <QInputDialog>
#include "geojsonreader.h"
#include "geoshpreader.h"
#include "geolayer.h"
#include <QMessageBox>
#include "geosldreader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    GeoMap* map1 = new GeoMap("test");
//    this->maps.append(map1);
//    GeoLayer* layer1 = new GeoLayer();
//    map1->addLayer(layer1);
//    GeoshpReader* shpreader = new GeoshpReader("F:/1.shp");
//    shpreader->read(layer1);
//    delete shpreader;
//    setMapTreeView(maps,ui->mapTreeView);

//    GeoshpReader* shpreader = new GeoshpReader("sss");
//    shpreader->readPostGisShp();
//    delete shpreader;

//    QColor a = QColor("#d7191c");
//    mout<< a.red();
//    mout<< a.green();
//    mout<<a.blue();

//    GeoSLDReader* mySLDReader = new GeoSLDReader("F:/P.sld");
//    mySLDReader->read();
//    delete  mySLDReader;


    //GeoMap* map = new GeoMap("test");
    //设置标题
    this->setWindowTitle("geoGIS");
    //状态栏
    statusLabel = new QLabel("NULL");
    this->statusBar()->addWidget(statusLabel);
    //创建默认图
    GeoMap* map = new GeoMap("Map");
    map->addpropertie("IndexID",maps.size());
    maps.append(map);
    //初始化TMapreeView
    initMapTreeView(ui->mapTreeView);
    setMapTreeView(maps,ui->mapTreeView);
    ui->lineEdit_TreeSearch->setText("Input name to filter...");
    //连接彩蛋消息
    connect(ui->actionNew_Map,&QAction::triggered,this,&MainWindow::createMap);
    connect(ui->actionAdd_json_to_Map,&QAction::triggered,this,&MainWindow::addJson);
    connect(ui->actionAdd_SHP_to_Map,&QAction::triggered,this,&MainWindow::addSHP);
    connect(ui->actionRemove_Map,&QAction::triggered,this,&MainWindow::removeMap);
    connect(ui->actionLink_to_PostGIS,&QAction::triggered,this,&MainWindow::linkToPostGIS);
    connect(ui->actionKDE,&QAction::triggered,this,&MainWindow::showKDEWidget);

    //清楚tabWidget_Draw标签，并关联关闭事件在关闭Tab时析构Widget
    ui->tabWidget_Draw->clear();
    connect(ui->tabWidget_Draw,&QTabWidget::tabCloseRequested,this,&MainWindow::slotDrawTabClose);

    postGISLinker = nullptr;
}
void MainWindow::initMapTreeView(QTreeView* treeView)
{
    treeView->setSelectionBehavior(QTreeView::SelectRows);			//一次选中整行
    //treeView->setSelectionMode(QTreeView::SingleSelection); //单选，配合上面的整行就是一次选单行

    //slotMapTreeMenu定义菜单页
    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeView, &QTreeView::customContextMenuRequested, this, &MainWindow::slotMapTreeMenu);
    //初始化model
    MapTreeModel = new QStandardItemModel(treeView);
    //初始化代理model
    MapTreeProxyModel = new MyFilterProxyModel;
    MapTreeProxyModel->setSourceModel(MapTreeModel);
    MapTreeProxyModel->setFilterKeyColumn(0);
}
void MainWindow::showKDEWidget()
{
    QList<GeoLayer*>layers;
    foreach (GeoMap*m,maps) {
        layers.append(m->getAllLayers());
    }
    KDEWidget* kde =new KDEWidget(layers);
    kde->show();
}
void MainWindow::linkToPostGIS()
{
    QInputDialog dia(this);
    dia.setWindowTitle("Input IndexID");
    dia.setLabelText("Please input Map IndexID：");
    dia.setInputMode(QInputDialog::IntInput);//可选参数：DoubleInput  TextInput
    if(dia.exec() == QInputDialog::Accepted)
    {
        int index = dia.intValue();
        if(index<0||index>maps.size()-1){
            this->showMessage("输入IndexID有误！");
            return;
        }

        if(postGISLinker != nullptr){
            postGISLinker->close();
            disconnect(postGISLinker,&GeoPostGISLinker::addLayerToMap,this,&MainWindow::slotAddLayerToMap);
            delete postGISLinker;
        }
        postGISLinker = new GeoPostGISLinker(index);
        postGISLinker->show();
        connect(postGISLinker,&GeoPostGISLinker::addLayerToMap,this,&MainWindow::slotAddLayerToMap);
    }
//    setMapTreeView(maps,ui->mapTreeView);
}
void MainWindow::slotAddLayerToMap(int IndexId,GeoLayer* layer)
{
    this->maps.at(IndexId)->addLayer(layer);

    setMapTreeView(maps,ui->mapTreeView);
    this->showMessage(QString("添加%1成功！").arg(layer->getPropertiesValue("name").toString()));
}
void MainWindow::slotCreateMap()
{
    //槽函数统一
    this->createMap();
}
void MainWindow::slotDrawTabClose(int index)
{
    //析构widget
    QWidget *pItemWidget = ui->tabWidget_Draw->widget(index);
    if (nullptr != pItemWidget)
    {
        delete pItemWidget;
        pItemWidget = nullptr;
    }
    OpenGLWidget* opengl = static_cast<OpenGLWidget*>(ui->tabWidget_Draw->currentWidget());
    if(opengl != nullptr){
        opengl->setLayerListView(ui->listView_DrawLayer);
    }else{
        ui->listView_DrawLayer->model()->removeRows(0,ui->listView_DrawLayer->model()->rowCount());
        ui->actionShow_3D_View->setEnabled(false);
        ui->actionShow_Index_Rect->setEnabled(false);
    }
    //ui->tabWidget_Draw->removeTab(index);
}
void MainWindow::setMapTreeMenuEnble(QModelIndex curIndex,QMenu &menu)
{
    QModelIndex index = curIndex.sibling(curIndex.row(),2); //同一行第3列元素的index
    QModelIndex indexP1 = index.parent().sibling(index.parent().row(),2);
    QModelIndex indexP2 = indexP1.parent().sibling(indexP1.parent().row(),2);
    //可以通过是否为空判读在哪
    if(index.isValid())
    {
        if(indexP1.isValid()){
            if(indexP2.isValid()){
                //Object层
                for (int i=0;i<menu.actions().size();i++) {
                    switch(i){
                    case 2: menu.actions().at(i)->setEnabled(false);break;
                    case 3: menu.actions().at(i)->setEnabled(false);break;
                    case 4: menu.actions().at(i)->setEnabled(false);break;
                    case 5: menu.actions().at(i)->setEnabled(false);break;
                    case 8: menu.actions().at(i)->setEnabled(false);break;
                    case 10:menu.actions().at(i)->setEnabled(false);break;
                    case 11:menu.actions().at(i)->setEnabled(false);break;
                    default:menu.actions().at(i)->setEnabled(true) ;break;
                    }
                }
            }else{
                //Layer层
                for (int i=0;i<menu.actions().size();i++) {
                    switch(i){
                    case 2: menu.actions().at(i)->setEnabled(false);break;
                    case 3: menu.actions().at(i)->setEnabled(false);break;
                    case 4: menu.actions().at(i)->setEnabled(false);break;
                    case 5: menu.actions().at(i)->setEnabled(true) ;break;
                    case 8: menu.actions().at(i)->setEnabled(true) ;break;
                    case 10:menu.actions().at(i)->setEnabled(false);break;
                    case 11:menu.actions().at(i)->setEnabled(false);break;
                    default:menu.actions().at(i)->setEnabled(true) ;break;
                    }
                }
            }
        }else{
            //Map层
            for (int i=0;i<menu.actions().size();i++) {
                switch(i){
                case 2: menu.actions().at(i)->setEnabled(true) ;break;
                case 3: menu.actions().at(i)->setEnabled(true) ;break;
                case 4: menu.actions().at(i)->setEnabled(true) ;break;
                case 5: menu.actions().at(i)->setEnabled(false);break;
                case 8: menu.actions().at(i)->setEnabled(false);break;
                case 10:menu.actions().at(i)->setEnabled(true) ;break;
                case 11:menu.actions().at(i)->setEnabled(true) ;break;
                default:menu.actions().at(i)->setEnabled(true) ;break;
                }
            }
        }
    }
}
void MainWindow::slotMapTreeMenu(const QPoint &pos)
{
    //否建TreeView右键菜单
    QMenu menu;
    QModelIndex curIndex = ui->mapTreeView->indexAt(pos);      //当前点击的元素的index 
    QModelIndex index = curIndex.sibling(curIndex.row(),0); //该行的第1列元素的index
    if (index.isValid())
    {
        //可获取元素的文本、data,进行其他判断处理
        //QStandardItem* item = mModel->itemFromIndex(index);
        //QString text = item->text();
        //QVariant data = item->data(Qt::UserRole + 1);
        //...
        //添加一行菜单，进行展开
        //0
        menu.addAction(QStringLiteral("New Map"), this, SLOT(slotCreateMap()));
        menu.addSeparator();
        //1+1
        menu.addAction(QStringLiteral("Add json to Map"), this, SLOT(slotMapTreeMenuAddJson()));
        //2+1
        menu.addAction(QStringLiteral("Add SHP to Map"), this, SLOT(slotMapTreeMenuAddSHP()));
        //3+1
        menu.addAction(QStringLiteral("Link PostGIS"),this,SLOT(slotMapTreeMenuLinkPostGIS()));
        //4+1
        menu.addAction(QStringLiteral("Link SLD"),this,SLOT(slotMapTreeMenuLinkSLD()));
        menu.addSeparator();    //添加一个分隔线
        //5+2
        menu.addAction(QStringLiteral("Properties and Data"), this, SLOT(slotMapTreeMenutoQString()));
        //6+2
        menu.addAction(QStringLiteral("Attribute Table"), this, SLOT(slotMapTreeMenuAttribute()));
        menu.addSeparator();
        //7+3
        menu.addAction(QStringLiteral("Creat Spatial Index"), this, SLOT(slotMapTreeMenuIndex()));
        //8+3
        menu.addAction(QStringLiteral("Spatial Index data"), this, SLOT(slotMapTreeMenuShowIndex()));
        menu.addSeparator();
        //9+4
        menu.addAction(QStringLiteral("Draw"), this, SLOT(slotMapTreeMenuDraw()));
        menu.addSeparator();
        //10+5
        menu.addAction(QStringLiteral("Rename"), this, SLOT(slotMapTreeMenuRename()));
        //11+5
        menu.addAction(QStringLiteral("Delete"), this, SLOT(slotMapTreeMenuDelete()));
        setMapTreeMenuEnble(curIndex,menu);
    }
    menu.exec(QCursor::pos());  //显示菜单
}
void MainWindow::slotMapTreeMenuShowIndex()
{
    int mapindex = -1;
    QModelIndex curIndex = ui->mapTreeView->currentIndex();
    QModelIndex index = curIndex.sibling(curIndex.row(),2); //同一行第3列元素的index
    QModelIndex indexP1 = index.parent().sibling(index.parent().row(),2);
    QModelIndex indexP2 = indexP1.parent().sibling(indexP1.parent().row(),2);

    if(index.isValid()&&indexP1.isValid()==false&&indexP2.isValid()==false)
    {
        mapindex = index.data().toInt();
    }else{
        this->showMessage("请选择Map！");
        return;
    }

    if(mapindex<0||mapindex>maps.size()-1){
        this->showMessage("IndexID有误！");
        return;
    }
    QString name = maps.at(mapindex)->hasSpatialIndex();
    if(name == "NULL")
        this->showMessage("该Map未创建索引！");
    else
        this->showMessage(maps.at(mapindex)->getIndexManager()->toQString());
}
void MainWindow::slotMapTreeMenuIndex()
{
    int mapindex = -1;
    QModelIndex curIndex = ui->mapTreeView->currentIndex();
    QModelIndex index = curIndex.sibling(curIndex.row(),2); //同一行第3列元素的index
    QModelIndex indexP1 = index.parent().sibling(index.parent().row(),2);
    QModelIndex indexP2 = indexP1.parent().sibling(indexP1.parent().row(),2);

    if(index.isValid()&&indexP1.isValid()==false&&indexP2.isValid()==false)
    {
        mapindex = index.data().toInt();
    }else{
        this->showMessage("请为Map创建索引!");
        return;
    }

    if(mapindex<0||mapindex>maps.size()-1){
        this->showMessage("IndexID有误！");
        return;
    }

    QStringList items; //ComboBox 列表的内容
    items <<"Grid"<<"QuadTree";
    bool ok=false;
    QString text = QInputDialog::getItem(this,
                                         "Creat Spatial Index",
                                         "Choose Spatial Index",
                                         items,
                                         0,
                                         false,&ok);

    if(ok && !text.isEmpty()){
        maps.at(mapindex)->creatIndexByName(text);
        this->showMessage("创建索引成功！");
    }else{
        this->showMessage("未创建索引！");
    }

}
void MainWindow::slotMapTreeMenuAttribute()
{
    int mapindex = -1;
    int layerindex = -1;
    QModelIndex curIndex = ui->mapTreeView->currentIndex();
    QModelIndex index = curIndex.sibling(curIndex.row(),2); //同一行第3列元素的index
    QModelIndex indexP1 = index.parent().sibling(index.parent().row(),2);
    QModelIndex indexP2 = indexP1.parent().sibling(indexP1.parent().row(),2);
    if(index.isValid()&&indexP1.isValid()&&indexP2.isValid()==false)
    {
        layerindex = index.data().toInt();
        mapindex = indexP1.data().toInt();
    }else{
        this->showMessage("图层才有属性表!");
        return;
    }
    if(mapindex<0||mapindex>maps.size()-1){
        this->showMessage(QString("Map IndexID有误！map:%1").arg(mapindex));
        return;
    }
    if(layerindex<0||layerindex>maps.at(mapindex)->getLayerSize()-1){
        this->showMessage(QString("Layer IndexID有误！map:%1,Layer:%2").arg(mapindex).arg(layerindex));
        return;
    }

    attributeTable = new AttributeTable();
    attributeTable->setTable(maps.at(mapindex)->getLayer(layerindex));
    attributeTable->show();
}
void MainWindow::slotMapTreeMenuRename()
{
    QString newName;
    QInputDialog dia(this);
    dia.setWindowTitle("Input Name");
    dia.setLabelText("Please input name：");
    dia.setInputMode(QInputDialog::TextInput);//可选参数：DoubleInput  TextInput

    QModelIndex curIndex = ui->mapTreeView->currentIndex();
    QModelIndex oldName = curIndex.sibling(curIndex.row(),0);
    dia.setTextValue(oldName.data().toString());
    if(dia.exec() == QInputDialog::Accepted)
    {
       newName = dia.textValue();
    }else{
        return;
    }
    //因为最多三层，所以直接查找三层
//    QModelIndex curIndex = ui->mapTreeView->currentIndex();
    QModelIndex index = curIndex.sibling(curIndex.row(),2); //同一行第3列元素的index
    QModelIndex indexP1 = index.parent().sibling(index.parent().row(),2);
    QModelIndex indexP2 = indexP1.parent().sibling(indexP1.parent().row(),2);
    //可以通过是否为空判读在哪
    if(index.isValid())
    {
        if(indexP1.isValid()){
            if(indexP2.isValid()){
                //Object层
                this->maps.at(indexP2.data().toInt())
                                  ->getLayer(indexP1.data().toInt())
                                  ->getObj(index.data().toInt())->addpropertie("name",newName);
            }else{
                //Layer层
                this->maps.at(indexP1.data().toInt())->getLayer(index.data().toInt())->addpropertie("name",newName);
            }
        }else{
            //Map层
            this->maps.at(index.data().toInt())->addpropertie("name",newName);
        }
    }
    this->showMessage("重命名成功！");
    this->setMapTreeView(maps,ui->mapTreeView);
}
void MainWindow::slotMapTreeMenuLinkSLD()
{
    int mapindex = -1;
    int layerindex = -1;
    QModelIndex curIndex = ui->mapTreeView->currentIndex();
    QModelIndex index = curIndex.sibling(curIndex.row(),2); //同一行第3列元素的index
    QModelIndex indexP1 = index.parent().sibling(index.parent().row(),2);
    QModelIndex indexP2 = indexP1.parent().sibling(indexP1.parent().row(),2);
    if(index.isValid()&&indexP1.isValid()&&indexP2.isValid()==false)
    {
        layerindex = index.data().toInt();
        mapindex = indexP1.data().toInt();
    }else{
        this->showMessage("SLD需连接到Layer!");
        return;
    }
    if(mapindex<0||mapindex>maps.size()-1){
        this->showMessage(QString("Map IndexID有误！map:%1").arg(mapindex));
        return;
    }
    if(layerindex<0||layerindex>maps.at(mapindex)->getLayerSize()-1){
        this->showMessage(QString("Layer IndexID有误！map:%1,Layer:%2").arg(mapindex).arg(layerindex));
        return;
    }
    QString path = QFileDialog::getOpenFileName(this,"","F:/","SLD(*.sld)");
    if(path.isNull()){
        this->showMessage("路径为空！");
        return;
    }

    GeoLayer *layer = maps.at(mapindex)->getLayer(layerindex);

    //读sld
    GeoSLDReader* sldReader = new GeoSLDReader(path);
    if(sldReader->read(layer))
        this->showMessage(QString("成功连接SLD文件至 Layer:%1！").arg(layer->getPropertiesValue("name").toString()));
    else
        this->showMessage("连接SLD文件失败！");
    delete sldReader;

    maps.at(mapindex)->creatIndexByName("QuadTree");
//    mout<<"创建完成";
    this->showMessage(maps.at(mapindex)->getIndexManager()->toQString());
    this->showMessage(maps.at(mapindex)->hasSpatialIndex());
}
void MainWindow::slotMapTreeMenuLinkPostGIS()
{
    int mapindex = -1;
    QModelIndex curIndex = ui->mapTreeView->currentIndex();
    QModelIndex index = curIndex.sibling(curIndex.row(),2); //同一行第3列元素的index
    QModelIndex indexP1 = index.parent().sibling(index.parent().row(),2);
    QModelIndex indexP2 = indexP1.parent().sibling(indexP1.parent().row(),2);

    if(index.isValid()&&indexP1.isValid()==false&&indexP2.isValid()==false)
    {
        mapindex = index.data().toInt();
    }else{
        this->showMessage("请向Map中添加数据!");
        return;
    }

    if(mapindex<0||mapindex>maps.size()-1){
        this->showMessage("IndexID有误！");
        return;
    }
    if(postGISLinker != nullptr){
        postGISLinker->close();
        disconnect(postGISLinker,&GeoPostGISLinker::addLayerToMap,this,&MainWindow::slotAddLayerToMap);
        delete postGISLinker;
    }
    postGISLinker = new GeoPostGISLinker(mapindex);
    postGISLinker->show();
    connect(postGISLinker,&GeoPostGISLinker::addLayerToMap,this,&MainWindow::slotAddLayerToMap);
}
void MainWindow::slotMapTreeMenutoQString()
{
    //因为最多三层，所以直接查找三层
    QModelIndex curIndex = ui->mapTreeView->currentIndex();
    QModelIndex index = curIndex.sibling(curIndex.row(),2); //同一行第3列元素的index
    QModelIndex indexP1 = index.parent().sibling(index.parent().row(),2);
    QModelIndex indexP2 = indexP1.parent().sibling(indexP1.parent().row(),2);
    //可以通过是否为空判读在哪
    if(index.isValid())
    {
        if(indexP1.isValid()){
            if(indexP2.isValid()){
                //Object层
                this->showMessage(this->maps.at(indexP2.data().toInt())
                                  ->getLayer(indexP1.data().toInt())
                                  ->getObj(index.data().toInt())->toQString());
//                 ui->textBrowser->setText(this->maps.at(indexP2.data().toInt())
//                                          ->getLayer(indexP1.data().toInt())
//                                          ->getObj(index.data().toInt())->toQString());
            }else{
                //Layer层
                this->showMessage(this->maps.at(indexP1.data().toInt())->getLayer(index.data().toInt())->toQString());
//                ui->textBrowser->setText(this->maps.at(indexP1.data().toInt())->getLayer(index.data().toInt())->toQString());
            }
        }else{
            //Map层
            this->showMessage(this->maps.at(index.data().toInt())->toQString());
//            ui->textBrowser->setText(this->maps.at(index.data().toInt())->toQString());
        }
    }
}
void MainWindow::slotShowObjData(GeoObject* obj)
{
    if(obj != nullptr)
        this->showMessage(obj->toQString());
}
void MainWindow::slotMapTreeMenuDraw()
{
    //每次画创建一个新Widget
    OpenGLWidget* openGLWidget = initOpenGlWidget();
//    connect(openGLWidget,&OpenGLWidget::selectObj,this,&MainWindow::slotShowObjData);
//    connect(ui->actionShow_3D_View,&QAction::toggled,openGLWidget,&OpenGLWidget::is3D);
//    connect(ui->tabWidget_Draw,&MainWindow::keyPressEvent,openGLWidget,&OpenGLWidget::keyPressEvent);

    int tabIndex = ui->tabWidget_Draw->addTab(openGLWidget,"title");//添加子标签 类型article
    ui->tabWidget_Draw->setCurrentIndex(tabIndex);
    int mapIndexID = 0;
    //openGLWidget->clearData();

    //查找
    QModelIndex curIndex = ui->mapTreeView->currentIndex();
    QModelIndex index = curIndex.sibling(curIndex.row(),2); //同一行第3列元素的index
    QModelIndex indexP1 = index.parent().sibling(index.parent().row(),2);
    QModelIndex indexP2 = indexP1.parent().sibling(indexP1.parent().row(),2);
    if(index.isValid())
    {
        if(indexP1.isValid()){
            if(indexP2.isValid()){
                mapIndexID = indexP2.data().toInt();
                this->MapTreeModel->item(mapIndexID)->child(indexP1.data().toInt())
                        ->child(index.data().toInt())->setCheckState(Qt::Checked);
                GeoObject* obj = this->maps.at(indexP2.data().toInt())
                        ->getLayer(indexP1.data().toInt())
                        ->getObj(index.data().toInt());
                this->showMessage(obj->toQString());

//                openGLWidget->addObjectData(obj);//添加Object数据给Widget
                openGLWidget->addData(obj);

                //名字属性问题
                QString objName = obj->getPropertiesValue("name").toString();
                if(objName == "NULL")
                    objName = obj->getPropertiesValue("NAME").toString();
                if(objName == "NULL")
                    objName = obj->getPropertiesValue("Name").toString();
                //设置标题
                ui->tabWidget_Draw->setTabText(tabIndex,QString("Map%1,Layer%2,Obj%3:%4")
                                               .arg(indexP2.data().toInt())
                                               .arg(indexP1.data().toInt())
                                               .arg(index.data().toInt())
                                               .arg(objName));
            }else{
                mapIndexID = indexP1.data().toInt();
                this->MapTreeModel->item(mapIndexID)->child(index.data().toInt())->setCheckState(Qt::Checked);
                GeoLayer* ly = this->maps.at(indexP1.data().toInt())->getLayer(index.data().toInt());
                this->showMessage(ly->toQString());

//                openGLWidget->addLayerData(ly);//添加数据给Widget
                openGLWidget->addData(ly);

                //设置标题
                ui->tabWidget_Draw->setTabText(tabIndex,QString("Map%1,Layer%2:%3").arg(indexP1.data().toInt()).arg(index.data().toInt())
                                               .arg(ly->getPropertiesValue("name").toString()));
            }
        }else{
            mapIndexID = index.data().toInt();
            this->MapTreeModel->item(mapIndexID)->setCheckState(Qt::Checked);

//            openGLWidget->addMapData(this->maps.at(index.data().toInt()));//添加Map数据
            openGLWidget->addData(this->maps.at(index.data().toInt()));

            this->showMessage(this->maps.at(index.data().toInt())->toQString());
            //设置标题
            ui->tabWidget_Draw->setTabText(tabIndex,QString("Map%1:%2").arg(index.data().toInt())
                                           .arg(this->maps.at(index.data().toInt())->getPropertiesValue("name").toString()));
        }
    }

    //获取Scale以投影坐标
//    for(int i=0;i<this->maps.size();i++)
//    {
        //mout <<"获得scale";
        maps.at(mapIndexID)->GetMapScale();
        if(openGLWidget->Xmax<maps.at(mapIndexID)->Xmax)   openGLWidget->Xmax=maps.at(mapIndexID)->Xmax;
        if(openGLWidget->Xmin>maps.at(mapIndexID)->Xmin)   openGLWidget->Xmin=maps.at(mapIndexID)->Xmin;
        if(openGLWidget->Ymax<maps.at(mapIndexID)->Ymax)   openGLWidget->Ymax=maps.at(mapIndexID)->Ymax;
        if(openGLWidget->Ymin>maps.at(mapIndexID)->Ymin)   openGLWidget->Ymin=maps.at(mapIndexID)->Ymin;
        openGLWidget->GetOriScale();
        //mout <<"获得scale";
//    }
    //重绘
    emit openGLWidget->update();
    openGLWidget->setLayerListView(ui->listView_DrawLayer);

}
void MainWindow::setMapTreeView(QList<GeoMap*> maps,QTreeView* treeView)
{

//    if(0 == maps.size())
//    {
//        treeView->rem;
//        return;
//    }
    //每次新建一个QStandardItemModel，  需要确定是否析构了！！！！！！
    //使用私有变量model解决析构
    //清除旧的
    MapTreeModel->clear();
    MapTreeModel->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("name")<< QStringLiteral("type") << QStringLiteral("IndexID"));
    //三层循环构建三层tree
    for(int k=0;k<maps.size();k++)
    {
        QString name = maps.at(k)->getPropertiesValue("name").toString();
        QStandardItem* mapItem = new QStandardItem(name);
//        mapItem->setDragEnabled(false);
//        mapItem->setDropEnabled(false);
        mapItem->setCheckable(true);
        mapItem->setUserTristate(true);
        MapTreeModel->appendRow(mapItem);
        MapTreeModel->setItem(mapItem->row(),1,new QStandardItem("Map"));
        MapTreeModel->setItem(mapItem->row(),2,new QStandardItem(QString::number(k)));
        for(int i=0;i<maps.at(k)->getLayerSize();i++)
        {
            GeoLayer* layer = maps.at(k)->getLayer(i);
            QStandardItem* layerItem = new QStandardItem(layer->getPropertiesValue("name").toString());
            layerItem->setCheckable(true);
            layerItem->setUserTristate(true);
            mapItem->appendRow(layerItem);
            layerItem->parent()->setChild(layerItem->row(),1,new QStandardItem("Layer"));
            layerItem->parent()->setChild(layerItem->row(),2,new QStandardItem(layer->getPropertiesValue("IndexID").toString()));
            for(int j=0;j<layer->getObjSize();j++)
            {
                GeoObject* obj = layer->getObj(j);
                //名字属性判断
                QString objName = obj->getPropertiesValue("name").toString();
                if(objName == "NULL")
                    objName = obj->getPropertiesValue("NAME").toString();
                if(objName == "NULL")
                    objName = obj->getPropertiesValue("Name").toString();
                QStandardItem* objItem = new QStandardItem(objName);
//                objItem->setDragEnabled(false);
//                objItem->setDropEnabled(false);
                objItem->setCheckable(true);
                layerItem->appendRow(objItem);
                objItem->parent()->setChild(objItem->row(),1,new QStandardItem(obj->getPropertiesValue("type").toString()));
                objItem->parent()->setChild(objItem->row(),2,new QStandardItem(obj->getPropertiesValue("IndexID").toString()));
            }
        }
    }


    //关联项目属性改变的信号和槽
    connect(MapTreeModel,&QStandardItemModel::itemChanged,this,&MainWindow::slotMapTreeItemChanged);
    //设置筛选代理
    treeView->setModel(MapTreeProxyModel);
    //设置第一列宽度
    treeView->setColumnWidth(0,150);
}
//-----MapTreeView 为了实现关联父子节点-------------------------------------
void treeItem_checkAllChild_recursion(QStandardItem * item,bool check)
{
    if(item == nullptr)
        return;
    int rowCount = item->rowCount();
    for(int i=0;i<rowCount;++i)
    {
        QStandardItem* childItems = item->child(i);
        treeItem_checkAllChild_recursion(childItems,check);
    }
    if(item->isCheckable())
        item->setCheckState(check ? Qt::Checked : Qt::Unchecked);
}
void treeItem_checkAllChild(QStandardItem * item, bool check)
{
    if(item == nullptr)
        return;
    int rowCount = item->rowCount();
    for(int i=0;i<rowCount;++i)
    {
        QStandardItem* childItems = item->child(i);
        treeItem_checkAllChild_recursion(childItems,check);
    }
    if(item->isCheckable())
        item->setCheckState(check ? Qt::Checked : Qt::Unchecked);
}
Qt::CheckState treeItem_checkSibling(QStandardItem * item)
{
    //先通过父节点获取兄弟节点
    QStandardItem * parent = item->parent();
    if(nullptr == parent)
        return item->checkState();
    int brotherCount = parent->rowCount();
    int checkedCount(0),unCheckedCount(0);
    Qt::CheckState state;
    for(int i=0;i<brotherCount;++i)
    {
        QStandardItem* siblingItem = parent->child(i);
        state = siblingItem->checkState();
        if(Qt::PartiallyChecked == state)
            return Qt::PartiallyChecked;
        else if(Qt::Unchecked == state)
            ++unCheckedCount;
        else
            ++checkedCount;
        if(checkedCount>0 && unCheckedCount>0)
            return Qt::PartiallyChecked;
    }
    if(unCheckedCount>0)
        return Qt::Unchecked;
    return Qt::Checked;
}
void treeItem_CheckChildChanged(QStandardItem * item)
{
    if(nullptr == item)
        return;
    Qt::CheckState siblingState = treeItem_checkSibling(item);
    QStandardItem * parentItem = item->parent();
    if(nullptr == parentItem)
        return;
    if(Qt::PartiallyChecked == siblingState)
    {
        if(parentItem->isCheckable() && parentItem->isUserTristate())
            parentItem->setCheckState(Qt::PartiallyChecked);
    }
    else if(Qt::Checked == siblingState)
    {
        if(parentItem->isCheckable())
            parentItem->setCheckState(Qt::Checked);
    }
    else
    {
        if(parentItem->isCheckable())
            parentItem->setCheckState(Qt::Unchecked);
    }
    treeItem_CheckChildChanged(parentItem);
}
//----------------------------------------------------------------------
void MainWindow::slotMapTreeItemChanged(QStandardItem * item )
{
    if (item == nullptr)
        return ;
    if (item->isCheckable())
    {
        //如果条目是存在复选框的，那么就进行下面的操作
        Qt::CheckState state = item->checkState(); //获取当前的选择状态
        if (item->isUserTristate())
        {
             //如果条目是三态的，说明可以对子目录进行全选和全不选的设置
            if ( state != Qt::PartiallyChecked )
            {
                //当前是选中状态，需要对其子项目进行全选
                treeItem_checkAllChild(item,state == Qt::Checked?true:false);
            }
        }
        else
        {
            //说明是两态的，两态会对父级的三态有影响
            //判断兄弟节点的情况
            treeItem_CheckChildChanged(item);
        }
    }

}
void MainWindow::createMap()
{

    QInputDialog dia(this);
    dia.setWindowTitle("Input Name");
    dia.setLabelText("Please input Map name：");
    dia.setInputMode(QInputDialog::TextInput);//可选参数：DoubleInput  TextInput
    if(dia.exec() == QInputDialog::Accepted)
    {
       GeoMap* map = new GeoMap(dia.textValue());
       map->addpropertie("IndexID",maps.size());
       maps.append(map);
       this->showMessage("创建Map成功！");
    }
    setMapTreeView(maps,ui->mapTreeView);   
}
void MainWindow::addSHP()
{
    QInputDialog dia(this);
    dia.setWindowTitle("Input IndexID");
    dia.setLabelText("Please input Map IndexID：");
    dia.setInputMode(QInputDialog::IntInput);//可选参数：DoubleInput  TextInput
    if(dia.exec() == QInputDialog::Accepted)
    {
        int index = dia.intValue();
        if(index<0||index>maps.size()-1){
            this->showMessage("输入IndexID有误！");
            return;
        }
        QString path = QFileDialog::getOpenFileName(this,"","F:/文件/学习/GIS软件开发技术/lambert/","SHP(*.shp)");
        if(path.isNull()){
            this->showMessage("路径为空！");
            return;
        }

        GeoLayer *layer = new GeoLayer();
        maps.at(index)->addLayer(layer);

        layer->addpropertie("name",path.split("/").last());
        layer->addpropertie("FilePath",path);

        //移植到GeoshpReader
//        //shp文件名不能为中文
//        QStringList ss = path.split("/");
//        ss.removeLast();
//        QString newPath = ss.join("/");
//        QString shxPath = newPath + "/TEMPGEOGIS.shx";
//        newPath = newPath + "/TEMPGEOGIS.shp";
//        if(false == QFile::copy(path,newPath)){
//            this->showMessage("储存临时文件出错！");
//            return;
//        }
        //读shp
        GeoshpReader* shpReader = new GeoshpReader(path);
        if(shpReader->read(layer)){
            this->showMessage(QString("添加%1成功！").arg(layer->getPropertiesValue("name").toString()));
        }else{
            this->showMessage("读取SHP文件出错！");
        }
        delete shpReader;
//        QFile::remove(newPath);
//        QFile::remove(shxPath);
    }
    setMapTreeView(maps,ui->mapTreeView);
}
void MainWindow::slotMapTreeMenuAddSHP()
{
    int mapindex = -1;
    QModelIndex curIndex = ui->mapTreeView->currentIndex();
    QModelIndex index = curIndex.sibling(curIndex.row(),2); //同一行第3列元素的index
    QModelIndex indexP1 = index.parent().sibling(index.parent().row(),2);
    QModelIndex indexP2 = indexP1.parent().sibling(indexP1.parent().row(),2);

    if(index.isValid()&&indexP1.isValid()==false&&indexP2.isValid()==false)
    {
        mapindex = index.data().toInt();
    }else{
        this->showMessage("请向Map中添加数据!");
        return;
    }

    if(mapindex<0||mapindex>maps.size()-1){
        this->showMessage("IndexID有误！");
        return;
    }

    QString path = QFileDialog::getOpenFileName(this,"","F:/文件/学习/GIS软件开发技术/lambert/","SHP(*.shp)");
    if(path.isNull()){
        this->showMessage("路径为空！");
        return;
    }

    GeoLayer *layer = new GeoLayer();
    maps.at(mapindex)->addLayer(layer);

    layer->addpropertie("name",path.split("/").last());
    layer->addpropertie("FilePath",path);

    //移植到GeoshpReader
//    //shp文件名不能为中文
//    QStringList ss = path.split("/");
//    ss.removeLast();
//    QString newPath = ss.join("/");
//    QString shxPath = newPath + "/TEMPGEOGIS.shx";
//    newPath = newPath + "/TEMPGEOGIS.shp";
//    if(false == QFile::copy(path,newPath)){
//        this->showMessage("储存临时文件出错！");
//        return;
//    }
    //读shp
    GeoshpReader* shpReader = new GeoshpReader(path);
    if(shpReader->read(layer)){
        this->showMessage(QString("添加%1成功！").arg(layer->getPropertiesValue("name").toString()));
    }else{
        this->showMessage("读取SHP文件出错！");
    }
    delete shpReader;
//    QFile::remove(newPath);
//    QFile::remove(shxPath);
    this->setMapTreeView(maps,ui->mapTreeView);
}
void MainWindow::addJson()
{
    QInputDialog dia(this);
    dia.setWindowTitle("Input IndexID");
    dia.setLabelText("Please input Map IndexID：");
    dia.setInputMode(QInputDialog::IntInput);//可选参数：DoubleInput  TextInput
    if(dia.exec() == QInputDialog::Accepted)
    {
        int index = dia.intValue();
        if(index<0||index>maps.size()-1){
            this->showMessage("输入IndexID有误！");
            return;
        }
        QString path = QFileDialog::getOpenFileName(this,"","F:/文件/学习/GIS软件开发技术/geojson/","JSON(*.json)");
        if(path.isNull()){
            this->showMessage("路径为空！");
            return;
        }
        GeoLayer *layer = new GeoLayer();
        maps.at(index)->addLayer(layer);

        layer->addpropertie("name",path.split("/").last());
        layer->addpropertie("FilePath",path);
        //读json
        geoJsonReader *myReader = new geoJsonReader(path);
        if(myReader->read(layer)){
            this->showMessage(QString("添加%1成功！").arg(layer->getPropertiesValue("name").toString()));
        }else {
            this->showMessage("读取GeoJson文件出错！");
        }
        delete myReader;

    }
    setMapTreeView(maps,ui->mapTreeView);

//    maps[0]->getLayer(0)->GetLayerScale();
//    mout << "获得scale";
//    KDECells* ccc = new KDECells(maps[0]->getLayer(0),1000,1000,300000);
//    mout << "sss";
//    ccc->saveAsJPG("D:/ccc.jpg");
//    mout << "sss";
}
void MainWindow::slotMapTreeMenuAddJson()
{
    int mapindex = -1;
    QModelIndex curIndex = ui->mapTreeView->currentIndex();
    QModelIndex index = curIndex.sibling(curIndex.row(),2); //同一行第3列元素的index
    QModelIndex indexP1 = index.parent().sibling(index.parent().row(),2);
    QModelIndex indexP2 = indexP1.parent().sibling(indexP1.parent().row(),2);
    if(index.isValid()&&indexP1.isValid()==false&&indexP2.isValid()==false)
    {
        mapindex = index.data().toInt();
    }else{
        this->showMessage("请向Map中添加数据!");
        return;
    }
    if(mapindex<0||mapindex>maps.size()-1){
        this->showMessage("IndexID有误！");
        return;
    }
    QString path = QFileDialog::getOpenFileName(this,"","F:/文件/学习/GIS软件开发技术/geojson/","JSON(*.json)");
    if(path.isNull()){
        this->showMessage("路径为空！");
        return;
    }

    GeoLayer *layer = new GeoLayer();
    maps.at(mapindex)->addLayer(layer);

    layer->addpropertie("name",path.split("/").last());

    //读json
    geoJsonReader *myReader = new geoJsonReader(path);
    if(myReader->read(layer)){
        this->showMessage(QString("添加%1成功！").arg(layer->getPropertiesValue("name").toString()));
    }else{
        this->showMessage("读取GeoJson文件出错！");

    }
    delete myReader;

    setMapTreeView(maps,ui->mapTreeView);
}
void MainWindow::showMessage(QVariant msg)
{
    ui->textBrowser->append("*-----------------*");
    ui->textBrowser->append(msg.toString());
    ui->textBrowser->append("*-----------------*");
}
void MainWindow::removeMap()
{
    //删除Map
    QInputDialog dia(this);
    dia.setWindowTitle("Input IndexID");
    dia.setLabelText("Please input Map IndexID：");
    dia.setInputMode(QInputDialog::IntInput);//可选参数：DoubleInput  TextInput
    if(dia.exec() == QInputDialog::Accepted)
    {
        int index = dia.intValue();
        if(index<0||index>maps.size()-1){
            this->showMessage("输入IndexID有误！");
            return;
        }
        if(QMessageBox::Yes == QMessageBox::question(this,"Are you sure?",
                                                     QString("\nMap:%1 将被删除！\n").arg(maps.at(index)
                                                     ->getPropertiesValue("name").toString()),
                                                     QMessageBox::Yes|QMessageBox::Cancel,QMessageBox::Cancel))
        {delete this->maps.takeAt(index);}
    }

    setMapTreeView(maps,ui->mapTreeView);
    this->showMessage("删除成功!");
}
void MainWindow::slotMapTreeMenuDelete()
{
    //与查看属性数据一样
    QModelIndex curIndex = ui->mapTreeView->currentIndex();
    QModelIndex index = curIndex.sibling(curIndex.row(),2); //同一行第3列元素的index
    QModelIndex indexP1 = index.parent().sibling(index.parent().row(),2);
    QModelIndex indexP2 = indexP1.parent().sibling(indexP1.parent().row(),2);
    if(index.isValid())
    {
        if(indexP1.isValid()){
            if(indexP2.isValid()){
                GeoObject* obj = this->maps.at(indexP2.data().toInt())
                                  ->getLayer(indexP1.data().toInt())
                                  ->getObj(index.data().toInt());
                QString objName = obj->getPropertiesValue("name").toString();
                if(objName == "NULL")
                    objName = obj->getPropertiesValue("NAME").toString();
                if(objName == "NULL")
                    objName = obj->getPropertiesValue("Name").toString();
                if(QMessageBox::Yes == QMessageBox::question(this,"Are you sure?",
                                                             QString("%1:%2 将被删除！")
                                                             .arg(obj->getPropertiesValue("type").toString())
                                                             .arg(objName),
                                                             QMessageBox::Yes|QMessageBox::Cancel,QMessageBox::Cancel))
                {   ui->tabWidget_Draw->clear();
                    this->maps.at(indexP2.data().toInt())->getLayer(indexP1.data().toInt())
                        ->removeObj(index.data().toInt());
                    this->showMessage("删除成功!");
                }
                    obj = nullptr;
            }else{
                GeoLayer* ly = this->maps.at(indexP1.data().toInt())->getLayer(index.data().toInt());
                if(QMessageBox::Yes == QMessageBox::question(this,"Are you sure?",
                                                             QString("Layer:%1 将被删除！").arg(ly->getPropertiesValue("name").toString()),
                                                             QMessageBox::Yes|QMessageBox::Cancel,QMessageBox::Cancel))
                {   ui->tabWidget_Draw->clear();
                    this->maps.at(indexP1.data().toInt())->removeLayer(index.data().toInt());
                    this->showMessage("删除成功!");
                }
                ly = nullptr;
            }
        }else{
            if(QMessageBox::Yes == QMessageBox::question(this,"Are you sure?",
                                                         QString("Map:%1 将被删除！").arg(maps.at(index.data().toInt())
                                                                                     ->getPropertiesValue("name").toString()),
                                                         QMessageBox::Yes|QMessageBox::Cancel,QMessageBox::Cancel))
            {   ui->tabWidget_Draw->clear();
                delete this->maps.takeAt(index.data().toInt());
                this->showMessage("删除成功!");
            }

        }
    }

    setMapTreeView(maps,ui->mapTreeView);

}
int MainWindow::sentSelectToOpenGl(OpenGLWidget* openglWidget)
{
    //将选中的目标传给openglWidget，点击按钮时调用
    int mapIndexID = -1;
    for(int i = 0;i < MapTreeModel->rowCount() ;i++)
    {
        QStandardItem *mapItem = MapTreeModel->item(i);
        if(Qt::Checked == mapItem->checkState()){

//            openglWidget->addMapData(this->maps.at(i));
            openglWidget->addData(this->maps.at(i));

            mapIndexID = i;
        }else if(Qt::PartiallyChecked == mapItem->checkState()){
            for(int j=0;j<mapItem->rowCount();j++){
                QStandardItem* layerItem = mapItem->child(j);
                if(Qt::Checked == layerItem->checkState()){
                    mapIndexID = i;

//                    openglWidget->addLayerData(this->maps.at(i)->getLayer(j));
                    openglWidget->addData(this->maps.at(i)->getLayer(j));

                }else if(Qt::PartiallyChecked == layerItem->checkState()){
                    for(int k=0;k<layerItem->rowCount();k++){
                        QStandardItem* objItem = layerItem->child(k);
                        if(Qt::Checked == objItem->checkState()){
                            mapIndexID = i;

//                            openglWidget->addObjectData(this->maps.at(i)->getLayer(j)->getObj(k));
                            openglWidget->addData(this->maps.at(i)->getLayer(j)->getObj(k));

                        }else{
                        }
                    }
                }else{
                }
            }
        }else{
        }
    }
    return mapIndexID;
}
MainWindow::~MainWindow()
{
    for (int i=0;i<maps.size();i++) {
        delete maps[i];
    }

    delete ui;
    if(MapTreeModel != nullptr)
        delete MapTreeModel;
    if(MapTreeProxyModel != nullptr)
        delete MapTreeProxyModel;
    if(postGISLinker != nullptr){
        postGISLinker->close();
        delete postGISLinker;
    }
    if(attributeTable != nullptr){
        attributeTable->close();
        delete attributeTable;
    }
}
void MainWindow::on_pushButton_DrawSelect_clicked()
{
    //按钮点击事件
    OpenGLWidget* openGLWidget = initOpenGlWidget();
//    connect(openGLWidget,&OpenGLWidget::selectObj,this,&MainWindow::slotShowObjData);
//    connect(ui->actionShow_3D_View,&QAction::toggled,openGLWidget,&OpenGLWidget::is3D);
//    connect(ui->tabWidget_Draw,&MainWindow::keyPressEvent,openGLWidget,&OpenGLWidget::keyPressEvent);

    int i = this->sentSelectToOpenGl(openGLWidget);
    if(-1 == i)
    {
        //未选中直接析构
        this->showMessage("未选中任何目标！");
        delete openGLWidget;
        return;
    }else{
        int tabIndex = ui->tabWidget_Draw->addTab(openGLWidget,"Select");//添加子标签
        ui->tabWidget_Draw->setCurrentIndex(tabIndex);
        maps.at(i)->GetMapScale();
        if(openGLWidget->Xmax<maps.at(i)->Xmax)   openGLWidget->Xmax=maps.at(i)->Xmax;
        if(openGLWidget->Xmin>maps.at(i)->Xmin)   openGLWidget->Xmin=maps.at(i)->Xmin;
        if(openGLWidget->Ymax<maps.at(i)->Ymax)   openGLWidget->Ymax=maps.at(i)->Ymax;
        if(openGLWidget->Ymin>maps.at(i)->Ymin)   openGLWidget->Ymin=maps.at(i)->Ymin;
        openGLWidget->GetOriScale();
        emit openGLWidget->update();
        openGLWidget->setLayerListView(ui->listView_DrawLayer);
    }
}
void MainWindow::on_lineEdit_TreeSearch_textEdited(const QString &arg1)
{
    //当输入框文本被修改时调用删选器
    QRegExp regExp(arg1, Qt::CaseInsensitive, QRegExp::FixedString);
    MapTreeProxyModel->setFilterRegExp(regExp);
}
void MainWindow::on_mapTreeView_pressed(const QModelIndex &index1)
{
    //获得当前OpenGLWidget
    OpenGLWidget* opengl = static_cast<OpenGLWidget*>(ui->tabWidget_Draw->currentWidget());
    //因为最多三层，所以直接查找三层
    QModelIndex curIndex = index1;
    QModelIndex index = curIndex.sibling(curIndex.row(),2); //同一行第3列元素的index
    QModelIndex indexP1 = index.parent().sibling(index.parent().row(),2);
    QModelIndex indexP2 = indexP1.parent().sibling(indexP1.parent().row(),2);
    //可以通过是否为空判读在哪
    if(index.isValid())
    {
        if(indexP1.isValid()){
            if(indexP2.isValid()){
                //Object层
                GeoObject* obj = this->maps.at(indexP2.data().toInt())->getLayer(indexP1.data().toInt())
                        ->getObj(index.data().toInt());
                //名字属性判断
                QString objName = obj->getPropertiesValue("name").toString();
                if(objName == "NULL")
                    objName = obj->getPropertiesValue("NAME").toString();
                if(objName == "NULL")
                    objName = obj->getPropertiesValue("Name").toString();
                this->statusLabel->setText(QString("%1--%2--%3；Type：%4；IndexID：%5").arg(maps.at(indexP2.data().toInt())->getPropertiesValue("name").toString())
                                           .arg(maps.at(indexP2.data().toInt())->getLayer(indexP1.data().toInt())->getPropertiesValue("name").toString())
                                           .arg(objName).arg(obj->getPropertiesValue("type").toString()).arg(index.data().toInt()));
                if(opengl != nullptr)
                    opengl->setHighlightData(obj);
            }else{
                //Layer层
                GeoLayer* t = this->maps.at(indexP1.data().toInt())->getLayer(index.data().toInt());
                this->statusLabel->setText(QString("%1--%2；目标数：%3；IndexID：%4").arg(maps.at(indexP1.data().toInt())->getPropertiesValue("name").toString())
                                                                                  .arg(t->getPropertiesValue("name").toString())
                                                                                  .arg(t->getObjSize())
                                                                                  .arg(index.data().toInt()));
                if(opengl != nullptr)
                    opengl->setHighlightData(t);
            }
        }else{
            //Map层
            GeoMap* t = this->maps.at(index.data().toInt());
            this->statusLabel->setText(QString("%1；图层数：%2；IndexID：%3").arg(t->getPropertiesValue("name").toString())
                                       .arg(t->getLayerSize()).arg(index.data().toInt()));
            if(opengl != nullptr)
                opengl->setHighlightData(t);
        }
    }
    if(opengl != nullptr)
        emit opengl->update();
}
void MainWindow::on_mapTreeView_doubleClicked(const QModelIndex &curIndex)
{
    //每次画创建一个新Widget
    OpenGLWidget* openGLWidget = initOpenGlWidget();
//    connect(openGLWidget,&OpenGLWidget::selectObj,this,&MainWindow::slotShowObjData);
//    connect(ui->actionShow_3D_View,&QAction::toggled,openGLWidget,&OpenGLWidget::is3D);
//    connect(ui->tabWidget_Draw,&MainWindow::keyPressEvent,openGLWidget,&OpenGLWidget::keyPressEvent);

    int tabIndex = ui->tabWidget_Draw->addTab(openGLWidget,"title");//添加子标签 类型article
    ui->tabWidget_Draw->setCurrentIndex(tabIndex);
    int mapIndexID = 0;
    //openGLWidget->clearData();

    //查找
//    QModelIndex curIndex = ui->mapTreeView->currentIndex();
    QModelIndex index = curIndex.sibling(curIndex.row(),2); //同一行第3列元素的index
    QModelIndex indexP1 = index.parent().sibling(index.parent().row(),2);
    QModelIndex indexP2 = indexP1.parent().sibling(indexP1.parent().row(),2);
    if(index.isValid())
    {
        if(indexP1.isValid()){
            if(indexP2.isValid()){
                mapIndexID = indexP2.data().toInt();
                this->MapTreeModel->item(mapIndexID)->child(indexP1.data().toInt())
                        ->child(index.data().toInt())->setCheckState(Qt::Checked);
                GeoObject* obj = this->maps.at(indexP2.data().toInt())
                        ->getLayer(indexP1.data().toInt())
                        ->getObj(index.data().toInt());
                this->showMessage(obj->toQString());

//                openGLWidget->addObjectData(obj);//添加Object数据给Widget
                openGLWidget->addData(obj);

                //名字属性问题
                QString objName = obj->getPropertiesValue("name").toString();
                if(objName == "NULL")
                    objName = obj->getPropertiesValue("NAME").toString();
                if(objName == "NULL")
                    objName = obj->getPropertiesValue("Name").toString();
                //设置标题
                ui->tabWidget_Draw->setTabText(tabIndex,QString("Map%1,Layer%2,Obj%3:%4")
                                               .arg(indexP2.data().toInt())
                                               .arg(indexP1.data().toInt())
                                               .arg(index.data().toInt())
                                               .arg(objName));
            }else{
                mapIndexID = indexP1.data().toInt();
                this->MapTreeModel->item(mapIndexID)->child(index.data().toInt())->setCheckState(Qt::Checked);
                GeoLayer* ly = this->maps.at(indexP1.data().toInt())->getLayer(index.data().toInt());
                this->showMessage(ly->toQString());

//                openGLWidget->addLayerData(ly);//添加数据给Widget
                openGLWidget->addData(ly);

                //设置标题
                ui->tabWidget_Draw->setTabText(tabIndex,QString("Map%1,Layer%2:%3").arg(indexP1.data().toInt()).arg(index.data().toInt())
                                               .arg(ly->getPropertiesValue("name").toString()));
            }
        }else{
            mapIndexID = index.data().toInt();
            this->MapTreeModel->item(mapIndexID)->setCheckState(Qt::Checked);

//            openGLWidget->addMapData(this->maps.at(index.data().toInt()));//添加Map数据
            openGLWidget->addData(this->maps.at(index.data().toInt()));

            this->showMessage(this->maps.at(index.data().toInt())->toQString());
            //设置标题
            ui->tabWidget_Draw->setTabText(tabIndex,QString("Map%1:%2").arg(index.data().toInt())
                                           .arg(this->maps.at(index.data().toInt())->getPropertiesValue("name").toString()));
        }
    }

    //获取Scale以投影坐标
//    for(int i=0;i<this->maps.size();i++)
//    {
        //mout <<"获得scale";
        maps.at(mapIndexID)->GetMapScale();
        if(openGLWidget->Xmax<maps.at(mapIndexID)->Xmax)   openGLWidget->Xmax=maps.at(mapIndexID)->Xmax;
        if(openGLWidget->Xmin>maps.at(mapIndexID)->Xmin)   openGLWidget->Xmin=maps.at(mapIndexID)->Xmin;
        if(openGLWidget->Ymax<maps.at(mapIndexID)->Ymax)   openGLWidget->Ymax=maps.at(mapIndexID)->Ymax;
        if(openGLWidget->Ymin>maps.at(mapIndexID)->Ymin)   openGLWidget->Ymin=maps.at(mapIndexID)->Ymin;
        openGLWidget->GetOriScale();
        //mout <<"获得scale";
//    }
    //重绘
    emit openGLWidget->update();
    openGLWidget->setLayerListView(ui->listView_DrawLayer);
}
void MainWindow::on_pushButton_Down_clicked()
{
    QModelIndex index =  ui->listView_DrawLayer->currentIndex();
    OpenGLWidget* opengl = static_cast<OpenGLWidget*>(ui->tabWidget_Draw->currentWidget());
    if(opengl != nullptr){
        opengl->moveLayer(index.row(),1);
        emit opengl->update();
        opengl->setLayerListView(ui->listView_DrawLayer);
    }
}
void MainWindow::on_listView_DrawLayer_doubleClicked(const QModelIndex &index)
{
    OpenGLWidget* opengl = static_cast<OpenGLWidget*>(ui->tabWidget_Draw->currentWidget());
    if(opengl != nullptr){
        opengl->setHighlightLayer(index.row());
        emit opengl->update();
    }

}
void MainWindow::on_pushButton_Up_clicked()
{
    QModelIndex index =  ui->listView_DrawLayer->currentIndex();
    OpenGLWidget* opengl = static_cast<OpenGLWidget*>(ui->tabWidget_Draw->currentWidget());
    if(opengl != nullptr){
        opengl->moveLayer(index.row(),-1);
        emit opengl->update();
        opengl->setLayerListView(ui->listView_DrawLayer);
    }
}
void MainWindow::on_tabWidget_Draw_tabBarClicked(int index)
{
     OpenGLWidget* opengl = static_cast<OpenGLWidget*>(ui->tabWidget_Draw->widget(index));
     if(opengl != nullptr){
         opengl->setLayerListView(ui->listView_DrawLayer);
     }
}
OpenGLWidget* MainWindow::initOpenGlWidget()
{
    OpenGLWidget* openGLWidget = new OpenGLWidget();
    connect(openGLWidget,&OpenGLWidget::selectObj,this,&MainWindow::slotShowObjData);
    connect(ui->actionShow_3D_View,&QAction::toggled,openGLWidget,&OpenGLWidget::is3D);
    connect(ui->actionShow_Index_Rect,&QAction::toggled,openGLWidget,&OpenGLWidget::isShowIndexRect);
    connect(ui->tabWidget_Draw,&MainWindow::keyPressEvent,openGLWidget,&OpenGLWidget::keyPressEvent);
    if(!ui->actionShow_3D_View->isEnabled())
        ui->actionShow_3D_View->setEnabled(true);
    if(!ui->actionShow_Index_Rect->isEnabled())
        ui->actionShow_Index_Rect->setEnabled(true);
    return openGLWidget;
}
