#ifndef FEATURE_H
#define FEATURE_H
#include <QString>
#include "lonlat.h"
class Feature
{
public:
    Feature();
    QString idcode;
    QString name;
    LonLat center;
    QString level;
};

#endif // FEATURE_H
