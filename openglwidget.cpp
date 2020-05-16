#include "openglwidget.h"
#include <QInputDialog>
double R=0.3;
double G=0.2;
double B=0;
#define PI 3.14159265
OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    Xmin=999999999;
    Xmax=-999999999;
    Ymin=999999999;
    Ymax=-999999999;
    Scale=1;
    DCLeft=0;
    DCBottom=0;
    angleX=0;
    angleY=0;
    high=10000000;
    map = new GeoMap("Draw Map");
}
OpenGLWidget::~OpenGLWidget()
{
    //mout << "openGLWidget析构";
    map->clearLayer();
    delete map;
    for (int i = 0;i<layersTodelete.count();i++) {
        layersTodelete.at(i)->clearObj();
        delete layersTodelete[i];
    }
}
void OpenGLWidget::initializeGL()
{
    //调用内容初始化函数
    initializeOpenGLFunctions();
    //glOrtho(0,900,0,900,1,2);


}
void OpenGLWidget::resizeGL(int w, int h)
{
    //当窗口大小改变时，调整界面坐标显示高度和宽度
    glViewport(0, 0, w, h);
    this->DCWidth=w;
    this->DCHeight=h;

}
void OpenGLWidget::paintGL()
{
    glViewport(DCLeft, DCBottom, DCWidth, DCHeight);

    //初始化map的区域,
    //设置投影方式
    MapPrj();

    //清除之前图形并将背景设置为黑色（设置为黑色纯粹个人爱好！）
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


//    //画map
//    for(int i=0;i<this->maps.size();i++){
//        this->drawMap(this->maps.at(i));
//    }
//    //画layer
//    for(int i=0;i<this->layers.size();i++){
//        this->drawLayer(this->layers.at(i));

//    }
//    //画obj
//    for(int i=0;i<this->objs.size();i++) {
//        this->drawObject(this->objs.at(i));
//    }

    drawMap(map);
    drawSelect();


    //话索引框
    if(map->hasSpatialIndex() != "NULL" && isDrawIndexRect)
        drawGeoRects(map->getIndexManager()->getAllRect());

}
void OpenGLWidget::drawMap(GeoMap *map)
{
//    for(int i=0;i<map->getLayerSize();i++) {
//        this->drawLayer(map->getLayer(i));
//    }
    for (int i=map->getLayerSize()-1;i>=0;i--) {
        this->drawLayer(map->getLayer(i));
    }
}
void OpenGLWidget::drawLayer(GeoLayer *layer)
{
    for(int i=0;i<layer->getObjSize();i++) {
        this->drawObject(layer->getObj(i));
    }

}
void OpenGLWidget::drawObject(GeoObject* obj)
{

    QString type = obj->getPropertiesValue("type").toString();
    if("Point"==type){
        LonLat temp = static_cast<GeoPoint*>(obj)->getData();
        R=static_cast<GeoPoint*>(obj)->getR();
        G=static_cast<GeoPoint*>(obj)->getG();
        B=static_cast<GeoPoint*>(obj)->getB();
        glPointSize(4.0f);
        glBegin(GL_POINTS);


        glColor3d(R, G, B);
        glVertex2d(temp.longitude,temp.latitude);
//        glVertex2d(((temp.longitude-Xmin)/(Xmax-Xmin)*2-1)*0.8,((temp.latitude-Ymin)/(Ymax-Ymin)*2-1)*0.8);
//        glVertex2d(2.4*(temp.longitude)/6000000+0.1, ((temp.latitude)/6000000-0.7)*2.6);
        glEnd();
    }else if("LineString"==type){
        QList<LonLat> temp = static_cast<GeoPolyline*>(obj)->getData();
        R=static_cast<GeoPolyline*>(obj)->getR();
        G=static_cast<GeoPolyline*>(obj)->getG();
        B=static_cast<GeoPolyline*>(obj)->getB();
        glLineWidth(1);
        glBegin(GL_LINE_STRIP);
        glColor3d(R,G,B);
        for (int q=0;q<temp.size();q++)
        {
            double pt1[]={temp.at(q).longitude,temp.at(q).latitude};
            //double pt1[]={2.4*temp.at(q).longitude/6000000+0.1,(temp.at(q).latitude/6000000-0.7)*2.6};
            //double pt2[]={2.4*temp.at(q+1).longitude/6000000+0.1,(temp.at(q+1).latitude/6000000-0.7)*2.6};
            glVertex2dv (pt1);
            //glVertex2dv (pt2);
        }
        glEnd();
    }else if("Polygon"==type){
        if(is3DView==false)
        {
        QList<LonLat> temp = static_cast<GeoPolygon*>(obj)->getData();

//      glPolygonMode(GL_BACK,GL_LINE);
//        //glFrontFace(GL_CCW);
//        glBegin(GL_POLYGON);
//        glPolygonMode(GL_FRONT,GL_FILL);

//        glColor3d(R, G, B);
//        for (int q=0;q<temp.size();q++)
//        {
//            double pt1[]={temp.at(q).longitude,temp.at(q).latitude};
//            //double pt1[]={2.4*temp.at(q).longitude/6000000+0.1,(temp.at(q).latitude/6000000-0.7)*2.6};
//            //double pt2[]={2.4*temp.at(q+1).longitude/6000000+0.1,(temp.at(q+1).latitude/6000000-0.7)*2.6};
//            glVertex2dv (pt1);
//            //glVertex2dv (pt2);
//        }
        R=static_cast<GeoPolygon*>(obj)->getFillR();
        G=static_cast<GeoPolygon*>(obj)->getFillG();
        B=static_cast<GeoPolygon*>(obj)->getFillB();
//        if(R>1) R=R-1;
//        if(G>1) G=G-1;
//        if(B>1) B=B-1;
//        glEnd();
          StartTess(temp);
          R=static_cast<GeoPolygon*>(obj)->getLineR();
          G=static_cast<GeoPolygon*>(obj)->getLineG();
          B=static_cast<GeoPolygon*>(obj)->getLineB();
        glLineWidth(1);
        glBegin(GL_LINE_LOOP);
        glColor3d(R, G, B);
        for (int q=0;q<temp.size();q++)
        {
            double pt1[]={temp.at(q).longitude,temp.at(q).latitude};
            //double pt1[]={2.4*temp.at(q).longitude/6000000+0.1,(temp.at(q).latitude/6000000-0.7)*2.6};
            //double pt2[]={2.4*temp.at(q+1).longitude/6000000+0.1,(temp.at(q+1).latitude/6000000-0.7)*2.6};
            glVertex2dv (pt1);
            //glVertex2dv (pt2);
        }
        glEnd();
        }
        else if(is3DView==true)
        {
            QList<LonLat> temp = static_cast<GeoPolygon*>(obj)->getData();
            R=static_cast<GeoPolygon*>(obj)->getFillR();
            G=static_cast<GeoPolygon*>(obj)->getFillG();
            B=static_cast<GeoPolygon*>(obj)->getFillB();

            StartTess(temp);
//            glColor3d(0.9, 0, 0);
//            glBegin(GL_POINTS);
//            for (int q=0;q<temp.size();q++)
//            {
//                double pt1[]={temp.at(q).X,temp.at(q).Y,temp.at(q).Z};
//                glVertex3dv(pt1);
//            }
//            glEnd();
        }
    }else if("MultiPolygon"==type){
        //glPolygonMode(GL_FRONT,GL_FILL);
        //glPolygonMode(GL_BACK,GL_LINE);
        //glFrontFace(GL_CCW);
        QList<QList<LonLat>> polygons = static_cast<GeoMultiPolygon*>(obj)->getData();
        for(int i=0;i<polygons.size();i++) {
//            glBegin(GL_POLYGON);
                QList<LonLat> temp = polygons.at(i);
            R=static_cast<GeoMultiPolygon*>(obj)->getFillR();
            G=static_cast<GeoMultiPolygon*>(obj)->getFillG();
            B=static_cast<GeoMultiPolygon*>(obj)->getFillB();
            glColor3d(R, G, B);
//            for (int q=0;q<temp.size();q++){

//                 double pt1[]={temp.at(q).longitude,temp.at(q).latitude,0};
//                //double pt1[]={2.4*temp.at(q).longitude/6000000+0.1,(temp.at(q).latitude/6000000-0.7)*2.6};
//                //double pt2[]={2.4*temp.at(q+1).longitude/6000000+0.1,(temp.at(q+1).latitude/6000000-0.7)*2.6};
//                glVertex3dv (pt1);
//                //glVertex2dv (pt2);
//            }
//            glEnd();
            StartTess(temp);
            glLineWidth(1);
            glBegin(GL_LINE_STRIP);
            R=static_cast<GeoMultiPolygon*>(obj)->getLineR();
            G=static_cast<GeoMultiPolygon*>(obj)->getLineG();
            B=static_cast<GeoMultiPolygon*>(obj)->getLineB();
            glColor3d(R, G, B);
            for (int q=0;q<temp.size();q++)
            {
                double pt1[]={temp.at(q).longitude,temp.at(q).latitude};
                //double pt1[]={2.4*temp.at(q).longitude/6000000+0.1,(temp.at(q).latitude/6000000-0.7)*2.6};
                //double pt2[]={2.4*temp.at(q+1).longitude/6000000+0.1,(temp.at(q+1).latitude/6000000-0.7)*2.6};
                glVertex2dv (pt1);
                //glVertex2dv (pt2);
            }
            glEnd();

        }
    }else{

    }
}
void OpenGLWidget::drawSelectObj(GeoObject* obj)
{
    QString type = obj->getPropertiesValue("type").toString();
    if("Point"==type){
        LonLat temp = static_cast<GeoPoint*>(obj)->getData();
        glPointSize(8.0f);
        glBegin(GL_POINTS);

        glColor3d(0, 0.9, 0.9);
        glVertex2d(temp.longitude,temp.latitude);
        glEnd();
    }else if("LineString"==type){
        QList<LonLat> temp = static_cast<GeoPolyline*>(obj)->getData();
        glLineWidth(3);
        glBegin(GL_LINE_STRIP);
        glColor3d(0,0.9,0.9);
        for (int q=0;q<temp.size();q++)
        {
            double pt1[]={temp.at(q).longitude,temp.at(q).latitude};
            glVertex2dv (pt1);
        }
        glEnd();
    }else if("Polygon"==type){
        QList<LonLat> temp = static_cast<GeoPolygon*>(obj)->getData();
        R=static_cast<GeoPolygon*>(obj)->getFillR();
        G=static_cast<GeoPolygon*>(obj)->getFillG();
        B=static_cast<GeoPolygon*>(obj)->getFillB();
        glColor3d(R, G, B);
        StartTess(temp);
        glLineWidth(3);
        glBegin(GL_LINE_LOOP);

        glColor3d(0, 0.9, 0.9);
        for (int q=0;q<temp.size();q++)
        {
            double pt1[]={temp.at(q).longitude,temp.at(q).latitude};
            glVertex2dv (pt1);
        }
        glEnd();
    }else if("MultiPolygon"==type){

        QList<QList<LonLat>> polygons = static_cast<GeoMultiPolygon*>(obj)->getData();
        for(int i=0;i<polygons.size();i++) {

             QList<LonLat> temp = polygons.at(i);
             R=static_cast<GeoMultiPolygon*>(obj)->getFillR();
             G=static_cast<GeoMultiPolygon*>(obj)->getFillG();
             B=static_cast<GeoMultiPolygon*>(obj)->getFillB();


            StartTess(temp);
            glLineWidth(3);
            glBegin(GL_LINE_STRIP);
            glColor3d(0, 0.9, 0.9);
            for (int q=0;q<temp.size();q++)
            {
                double pt1[]={temp.at(q).longitude,temp.at(q).latitude};
                glVertex2dv (pt1);
            }
            glEnd();

        }
    }else{

    }
}

void OpenGLWidget::drawSelect()
{

    if(highlightMap!=nullptr)
    {
        for(int i=0;i<highlightMap->getLayerSize();i++)
        {
            for(int j=0;j<highlightMap->getLayer(i)->getObjSize();j++)
            {

                drawSelectObj(highlightMap->getLayer(i)->getObj(j));
            }
        }
       highlightMap=nullptr;
    }
    if(highlightLayer!=nullptr)
    {

            for(int j=0;j<highlightLayer->getObjSize();j++)
            {

                drawSelectObj(highlightLayer->getObj(j));
            }

       highlightLayer=nullptr;
    }

    if(highlightObj!=nullptr)
    {
        drawSelectObj(highlightObj);
        highlightObj=nullptr;
    }
}

void OpenGLWidget::clearData()
{
    maps.clear();
    layers.clear();
    objs.clear();
}
void OpenGLWidget::addMapData(GeoMap *map)
{
    maps.append(map);
}
void OpenGLWidget::addMapData(QList<GeoMap *> map)
{
    maps.append(map);
}
void OpenGLWidget::addLayerData(GeoLayer *layer)
{
    layers.append(layer);
}
void OpenGLWidget::addLayerData(QList<GeoLayer *> layer)
{
    layers.append(layer);
}
void OpenGLWidget::addObjectData(GeoObject *obj)
{
    objs.append(obj);
}
void OpenGLWidget::addObjectData(QList<GeoObject *> obj)
{
    objs.append(obj);
}
void OpenGLWidget::MapPrj()
{
    if(is3DView==false)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        double temp1 = PrjRight-PrjLeft;
        double temp2 = PrjTop-PrjBottom;
        double Oriwidth=this->width();
        double OriHeight=this->height();
        double ViewRate = Oriwidth/OriHeight;
        double DataRate = temp1/temp2;


       if(DataRate>=ViewRate)
        {
            double tempBottom=PrjBottom;
            double tempTop=PrjTop;
            PrjBottom=(tempBottom+tempTop)/2-temp1/ViewRate/2;
            PrjTop=(tempBottom+tempTop)/2+temp1/ViewRate/2;
            glOrtho(PrjLeft,PrjRight,PrjBottom,PrjTop,-100,100);
        }
       if(DataRate<ViewRate)
       {
            double tempLeft=PrjLeft;
            double tempRight=PrjRight;
            PrjLeft=(tempLeft+tempRight)/2-ViewRate*temp2/2;
            PrjRight=(tempRight+tempLeft)/2+ViewRate*temp2/2;
            glOrtho(PrjLeft,PrjRight,PrjBottom,PrjTop,-100,100);
       }
//       mout<<"????";
   }
    else if(is3DView==true)
    {

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
//        glOrtho(PrjLeft*10,PrjRight*10,PrjBottom*10,PrjTop*10,-0,1000000000);

        gluPerspective(30,1,high-20000000,high);
         gluLookAt(high*sin(angleX*3.1415926/180)*cos(angleY*3.1415926/180),high*cos(angleX*3.1415926/180)*cos(angleY*3.1415926/180),high*sin(angleY*3.1415926/180),0,0,0,0,0,1);
//        gluLookAt(high*sin(angleX*3.1415926/180),high*cos(angleX*3.1415926/180),high*sin(angleY*3.1415926/180),0,0,0,-cos((90-angleY)*3.1415926/180)*sin(angleX*3.1415926/180),-cos((90-angleY)*3.1415926/180)*cos(angleX*3.1415926/180),sin((90-angleY)*3.1415926/180));
    }
//    gluPerspective(60.0,DataRate,-1,1);
//    gluLookAt((PrjLeft+PrjRight)/2,(PrjBottom+PrjTop)/2,3500000,(PrjLeft+PrjRight)/2,(PrjBottom+PrjTop)/2,0,0,1,0);


}

void OpenGLWidget::GetOriScale()
{
    PrjLeft=Xmin;
    PrjRight=Xmax;
    PrjTop=Ymax;
    PrjBottom=Ymin;
}
void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton){
        oldPoint.setX(event->x());
        oldPoint.setY(event->y());
    }else if(event->button() == Qt::LeftButton){
//        QString name = map->hasSpatialIndex();
//        if(name == "NULL"){
//            map->creatIndexByName("Grid");
//        }else if(name == "Grid"){
//            mout << "右键";
//            LonLat pt = GetWorldPoint(event->localPos().x(),event->localPos().y());
//            QList<GeoObject*>objs = map->getIndexManager()->getObjs(pt);
//            GeoLayer* ly = new GeoLayer;
//            foreach (GeoObject* obj,objs) {
//                ly->addObj(obj);
//            }
//            map->addLayer(ly);
//        }
    }

}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{

}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    setMouseTracking(true);
    //平移
    if(event->button() == Qt::RightButton){
        if(is3DView){
            angleX=angleX+5;
        }else{
            double temp1 = PrjRight-PrjLeft;
            double temp2 = PrjTop-PrjBottom;
            double Oriwidth=this->width();
            double OriHeight=this->height();
            double XRate = temp1/Oriwidth;
            double YRate = temp2/OriHeight;
            newPoint.setX(event->x());
            newPoint.setY(event->y());
            int disX=newPoint.x()-oldPoint.x();
            int disY=newPoint.y()-oldPoint.y();
            PrjLeft=PrjLeft-disX*XRate;
            PrjRight=PrjRight-disX*XRate;
            PrjBottom=PrjBottom+disY*YRate;
            PrjTop=PrjTop+disY*YRate;
        }
    }else if(event->button() == Qt::LeftButton){
        if(is3DView){
            angleY=angleY+5;
        }else{
            this->creatSpatialIndex();
            LonLat pt = GetWorldPoint(event->localPos().x(),event->localPos().y());
            GeoObject* objj = map->getIndexManager()->getObj(pt);
            this->highlightObj = objj;
            if(objj!=nullptr)
                emit selectObj(objj);
        }

    }
    this->update();
}

void OpenGLWidget::mapTransfer()
{

}
void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    //缩放
    int nx=event->x();
    int ny=event->y();
    ny=this->height()-ny;

    double Datawidth = PrjRight-PrjLeft;
    double Dataheight= PrjTop-PrjBottom;
    double Oriwidth=this->width();
    double OriHeight=this->height();
    double XRate = Datawidth/Oriwidth;
    double YRate = Dataheight/OriHeight;
    double XRate1=XRate;
    double YRate1=YRate;
    double tempBottom=PrjBottom;
    double tempTop=PrjTop;
    double tempLeft=PrjLeft;
    double tempRight=PrjRight;

//    GetWorldPoint(nx,ny,0);
//    mout<<tempX<<tempY<<tempZ<<" ";


    int t = event->delta();
    if(t>=0)
    {


        PrjLeft=nx*XRate1+tempLeft-(nx*XRate1+tempLeft-tempLeft)*0.8;
        PrjRight=nx*XRate1+tempLeft-(nx*XRate1+tempLeft-tempRight)*0.8;
        PrjTop=ny*YRate1+tempBottom-(ny*YRate1+tempBottom-tempTop)*0.8;
        PrjBottom=ny*YRate1+tempBottom-(ny*YRate1+tempBottom-tempBottom)*0.8;
        high=high*0.8;

    }
    if(t<0)
    {


        PrjLeft=nx*XRate1+tempLeft-(nx*XRate1+tempLeft-tempLeft)*1.2;
        PrjRight=nx*XRate1+tempLeft-(nx*XRate1+tempLeft-tempRight)*1.2;
        PrjTop=ny*YRate1+tempBottom-(ny*YRate1+tempBottom-tempTop)*1.2;
        PrjBottom=ny*YRate1+tempBottom-(ny*YRate1+tempBottom-tempBottom)*1.2;
        high=high*1.2;

    }

     this->update();
}

void OpenGLWidget:: mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton){
        PrjLeft=Xmin;
        PrjRight=Xmax;
        PrjBottom=Ymin;
        PrjTop=Ymax;
    }
}

void OpenGLWidget:: keyPressEvent(QKeyEvent *event)
{

}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    int keyvalue=event->key();
    mout <<keyvalue;
    if(keyvalue==Qt::Key_Left)
    mout<<"fk";
}

LonLat OpenGLWidget::GetWorldPoint(double DCx, double DCy)
{
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    double temp1 = PrjRight-PrjLeft;
//    double temp2 = PrjTop-PrjBottom;
//    double Oriwidth=this->width();
//    double OriHeight=this->height();
//    double ViewRate = Oriwidth/OriHeight;
//    double DataRate = temp1/temp2;


//   if(DataRate>=ViewRate)
//    glOrtho(PrjLeft,PrjRight,(PrjBottom+PrjTop)/2-temp1/ViewRate/2,(PrjBottom+PrjTop)/2+temp1/ViewRate/2,-1,1);
//   if(DataRate<ViewRate)
//    glOrtho((PrjLeft+PrjRight)/2-ViewRate*temp2/2,(PrjRight+PrjLeft)/2+ViewRate*temp2/2,PrjBottom,PrjTop,-1,1);


//        double modelMatrix[16];
//        glGetDoublev( GL_MODELVIEW_MATRIX , modelMatrix );
//        for(int i=0;i<16;i++)   mout<<modelMatrix[i]<<" ";
//        double projMatrix[16];
//        glGetDoublev( GL_PROJECTION_MATRIX , projMatrix );
//        for(int i=0;i<16;i++)     mout<<projMatrix[i]<<" ";
//        int viewport[4];
//        glGetIntegerv( GL_VIEWPORT , viewport );
//        for(int i=0;i<4;i++) mout<<viewport[i]<<" ";
//        gluUnProject(x,viewport[3]-y,z,modelMatrix,projMatrix,viewport,&tempX,&tempY,&tempZ);

    LonLat PT;
    double Datawidth = PrjRight-PrjLeft;
    double Dataheight= PrjTop-PrjBottom;
    double Oriwidth=this->width();
    double OriHeight=this->height();
    double XRate = Datawidth/Oriwidth;
    double YRate = Dataheight/OriHeight;
    DCy = OriHeight - DCy;
    PT.longitude =  DCx * XRate+PrjLeft;
    PT.latitude =  DCy * YRate+PrjBottom;
    return PT;

}

void OpenGLWidget::CALLBACK vertexCallback(GLvoid *vertex)
{

     GLdouble *pointer =(GLdouble *)vertex;

//     GLdouble pt[]={pointer[i],pointer[i+1],pointer[i+2]};
     glVertex3dv(pointer);
//     i=i++;
//    mout<<"开始绘图"<<i;

}

void OpenGLWidget::CALLBACK beginCallback(GLenum which)
{
    glBegin(which);

    glColor3d(R,G,B);

//    mout<<which<<" ";
}

void OpenGLWidget::CALLBACK endCallback()
{
    glEnd();
//    mout<<"成功结束"<<" ";
}

void OpenGLWidget::CALLBACK errorCallback(GLenum errorcode)
{
    const GLubyte *estring;
    estring = gluErrorString(errorcode);
    mout<<"wocao?啥玩意"<<" ";
    exit(0);
}

void OpenGLWidget::CALLBACK combineCallback(GLdouble coords[3],GLdouble *vertex_data[4],GLfloat weight[4], GLdouble **dataOut )
{
//    mout<<"发生了吗"<<" ";

    GLdouble *vertex;
    int i;
    vertex = (GLdouble *) malloc(6 * sizeof(GLdouble));
    vertex[0] = coords[0];
    vertex[1] = coords[1];
    vertex[2] = coords[2];
//    vertex[3] = 1;
//    vertex[4] = 0;
//    vertex[5] = 0;
//    vertex[6] = 0;

//    mout<<coords[0]<<" ";
//    mout<<coords[1]<<" ";
//    mout<<coords[2]<<" ";
    for (i = 3; i < 7; i++)
    {
//       vertex[i] = weight[0] * vertex_data[0][i];
//            + weight[1] * vertex_data[1][i]
//            + weight[2] * vertex_data[2][i]
//
    }
    *dataOut = vertex;

}

void OpenGLWidget::StartTess(QList<LonLat> temp)
{
//    QList<LonLat> temp = static_cast<GeoPolygon*>(obj)->getData();
    int Psize=temp.size();
    double **PTS = new double*[Psize];
    for (int i=0;i<Psize;i++) {
        PTS[i] = new double[3];
    }

    for (int q=0;q<temp.size();q++)
    {
        if(is3DView==false)
        {
            PTS[q][0]=temp.at(q).longitude;
            PTS[q][1]=temp.at(q).latitude;
            PTS[q][2]=0;
        }
        else if(is3DView==true)
        {
                PTS[q][0]=temp.at(q).X;
                PTS[q][1]=temp.at(q).Y;
                PTS[q][2]=temp.at(q).Z;
        }

    }
//    mout<<"到这了吗"<<"";
    GLUtesselator *tobj = gluNewTess();

    gluTessCallback(tobj, GLU_TESS_VERTEX,(void(__stdcall*)())glVertex3dv);
    gluTessCallback(tobj, GLU_TESS_BEGIN, (void (CALLBACK *)())beginCallback);
    gluTessCallback(tobj, GLU_TESS_END, (void (CALLBACK *)())endCallback);
    gluTessCallback(tobj, GLU_TESS_ERROR, (void (CALLBACK *)())errorCallback);
    gluTessCallback(tobj, GLU_TESS_COMBINE, (void (CALLBACK *)())combineCallback);

   // gluTessProperty(tobj, GLU_TESS_WINDING_RULE,GLU_TESS_WINDING_ODD);
    gluTessBeginPolygon(tobj, nullptr);
    gluTessBeginContour(tobj);


    for (int q=0;q<temp.size();q++)
    {
//        mout<<PTS[q][0]<<"WT";
//        mout<<PTS[q][1]<<"";
//        mout<<PTS[q][2]<<"";

        gluTessVertex(tobj,PTS[q],PTS[q]);

//        mout<<"传入顶点"<<q;


    }
    gluTessEndContour(tobj);
    gluTessEndPolygon(tobj);
    gluDeleteTess(tobj);


    for (int i=0;i<Psize;i++) {
        delete[] PTS[i];
        PTS[i]=nullptr;
    }
    delete[] PTS;
    PTS=nullptr;

}

void OpenGLWidget::setHighlightData(GeoMap *map)
{
    this->highlightMap = map;
}
void OpenGLWidget::setHighlightData(GeoLayer *layer)
{
    this->highlightLayer = layer;
}
void OpenGLWidget::setHighlightData(GeoObject *obj)
{
    this->highlightObj = obj;
}
void OpenGLWidget::addLayer(GeoLayer *layer)
{
    map->addLayer(layer);
}
void OpenGLWidget::addData(GeoLayer *layer)
{
    map->addLayer(layer);
}
void OpenGLWidget::addData(GeoObject *obj)
{
    GeoLayer* ly = new GeoLayer();
    QString name = obj->getPropertiesValue("name").toString();
    if(name == "NULL"){
        name = obj->getPropertiesValue("Name").toString();
    }
    if(name == "NULL"){
        name = obj->getPropertiesValue("NAME").toString();
    }
    ly->addpropertie("name",name);
    ly->addObj(obj);
    map->addLayer(ly);
    layersTodelete.append(ly);
}
void OpenGLWidget::addData(GeoMap *mp)
{
    QString name = mp->getIndexManager()->getName();
    for (int i=0;i<mp->getLayerSize();i++) {
        map->addLayer(mp->getLayer(i));
    }
}

void OpenGLWidget::setLayerListView(QListView *listView)
{
    QStringList ss;
    QStringListModel *ItemModel = new QStringListModel();
    for (int i=0;i<map->getLayerSize();i++) {
        ss << map->getLayer(i)->getPropertiesValue("name").toString();
    }
    ItemModel->setStringList(ss);
    listView->setModel(ItemModel);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    listView->setDragDropMode(QAbstractItemView::DragDropMode::NoDragDrop);

}
void OpenGLWidget::moveLayer(int index, int offi)
{
    map->swapLayer(index,index+offi);
}
void OpenGLWidget::setHighlightLayer(int i)
{
    highlightLayer = map->getLayer(i);
}
void OpenGLWidget::creatSpatialIndex()
{
    QString name = map->hasSpatialIndex();
    int n1 = 6;
    if(name == "NULL"){
        QStringList items; //ComboBox 列表的内容
        items <<"Grid"<<"QuadTree"<<"RTree";
        bool ok=false;
        QString text = QInputDialog::getItem(this,
                                             "Creat Spatial Index",
                                             "Choose Spatial Index",
                                             items,
                                             0,
                                             false,&ok);
        bool ok1=false;
        int n = QInputDialog::getInt(this,
                                     "Creat Spatial Index",
                                     "Input parameter",
                                     6,1,20,1,&ok1);

        if(ok && !text.isEmpty()){
            if(ok1)
                n1 = n;
            map->creatIndexByName(text,n1);
        }else{
            map->creatIndexByName("QuadTree");
        }
    }
    double NowDCwidth=this->width();
    map->getIndexManager()->setSelectMinDis((PrjRight-PrjLeft)/(NowDCwidth)*10);
}
void OpenGLWidget::drawGeoRects(QList<GeoRect> rs)
{
//    glBegin(GL_LINE_LOOP);
//    glColor3d(0,0,1);
    foreach (GeoRect r, rs) {
        glBegin(GL_LINE_LOOP);
        glColor3d(0,0,1);
        glLineWidth(1);
        glVertex2d(r.Xmin,r.Ymin);
        glVertex2d(r.Xmin,r.Ymax);
        glVertex2d(r.Xmax,r.Ymax);
        glVertex2d(r.Xmax,r.Ymin);
        glEnd();
    }
//    glEnd();
}
void OpenGLWidget::is3D(bool checked)
{
    this->is3DView = checked;
    map->transfertoXYZ();
    this->update();
//    mout << is3DView;
}
void OpenGLWidget::isShowIndexRect(bool checked)
{
    this->isDrawIndexRect = checked;
    this->update();
}
