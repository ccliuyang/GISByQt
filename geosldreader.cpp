#include "geosldreader.h"
#include <QtDebug>
#define mout qDebug() <<"["<<__FILE__<<":"<<__LINE__<<"]"
GeoSLDReader::GeoSLDReader(QString p)
{
    this->path = p;
}

bool GeoSLDReader::read()
{
    QFile file("F:/DiJiChengShiZhuDi.sld");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader xmlReader(&file);
    while(!file.atEnd()){
        QXmlStreamReader::TokenType nType = xmlReader.readNext();
        if(nType == QXmlStreamReader::StartElement){
            QString strElementName = xmlReader.name().toString();
//            mout << strElementName;
//            if(strElementName == "SvgParameter")
//                mout << xmlReader.readElementText();
            if(strElementName == "PropertyName"){
                mout << "PropertyName:" << xmlReader.readElementText();
                xmlReader.readNext();
                xmlReader.readNext();
                mout << "Literal:" << xmlReader.readElementText();
            }else if(strElementName == "SvgParameter"){
                QString name = xmlReader.attributes().value("name").toString();
                if(name == "fill"){
                    mout << "fill:" << xmlReader.readElementText();
                }else if("stroke" == name){
                    mout << "stroke:" << xmlReader.readElementText();
                }else if("stroke-width" == name){
                    mout << "stroke-width:" << xmlReader.readElementText();
                }else if("stroke-linejoin" == name){
                    mout << "stroke-linejoin:" << xmlReader.readElementText();
                }else if("stroke-linecap" == name){
                    mout << "stroke-linecap:" << xmlReader.readElementText();
                }
            }
        }
    }
    file.close();
    return true;
}
bool GeoSLDReader::read(GeoLayer *layer)
{
    QFile file(this->path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader xmlReader(&file);
    GeoObject* obj = nullptr;
    GeoStyle* style = nullptr;
//    while(!file.atEnd()){
    while(!xmlReader.isEndDocument()){
        QXmlStreamReader::TokenType nType = xmlReader.readNext();
        if(nType == QXmlStreamReader::StartElement){
            QString strElementName = xmlReader.name().toString();
//            mout << strElementName;
//            if(strElementName == "SvgParameter")
//                mout << xmlReader.readElementText();
            if(strElementName == "PropertyName"){
//                mout << "PropertyName:" << xmlReader.readElementText();
                QString PropertyName = xmlReader.readElementText();
                xmlReader.readNext();
                xmlReader.readNext();
//                mout << "Literal:" << xmlReader.readElementText();
                QString Literal = xmlReader.readElementText();
                obj = layer->getObjByPropertie(PropertyName,Literal);
                if(obj == nullptr){
                    continue;
                }
                else{
                    style = new GeoStyle();
                    obj->setStyle(style);
                }
            }else if(strElementName == "SvgParameter"){
                QString name = xmlReader.attributes().value("name").toString();
                if(name == "fill"){
//                    mout << "fill:" << xmlReader.readElementText();
                    if(obj == nullptr)
                        continue;
                    style->fillColor = QColor(xmlReader.readElementText());
                }else if("stroke" == name){
//                    mout << "stroke:" << xmlReader.readElementText();
                    if(obj == nullptr)
                        continue;
                    style->strokeColor = QColor(xmlReader.readElementText());
                }else if("stroke-width" == name){
//                    mout << "stroke-width:" << xmlReader.readElementText();
                    if(obj == nullptr)
                        continue;
                    style->stroke_width = xmlReader.readElementText().toFloat();
                }else if("stroke-linejoin" == name){
//                    mout << "stroke-linejoin:" << xmlReader.readElementText();
                    if(obj == nullptr)
                        continue;
                    style->stroke_linejoin = xmlReader.readElementText();
                }else if("stroke-linecap" == name){
//                    mout << "stroke-linecap:" << xmlReader.readElementText();
                    if(obj == nullptr)
                        continue;
                    style->stroke_linecap = xmlReader.readElementText();
                }
            }
        }
    }
    file.close();
    return true;
}
