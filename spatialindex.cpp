#include "spatialindex.h"

SpatialIndex::SpatialIndex()
{

}
SpatialIndex::~SpatialIndex()
{

}
QString SpatialIndex::getName()
{
    return name;
}

double SpatialIndex::getSelectMinDis() const
{
    return selectMinDis;
}

void SpatialIndex::setSelectMinDis(double value)
{
    selectMinDis = value;
}
