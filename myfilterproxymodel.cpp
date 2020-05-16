#include "myfilterproxymodel.h"

MyFilterProxyModel::MyFilterProxyModel()
{

}
bool MyFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    bool filter = QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);

    if (filter)
    {
        return true;
    }
    else
    {
        // check all decendant's
        QModelIndex source_index = sourceModel()->index(source_row, 0, source_parent);
        for (int k=0; k<sourceModel()->rowCount(source_index); k++)
        {
            if (filterAcceptsRow(k, source_index))
            {
                return true;
            }
        }
    }

    return false;
}
