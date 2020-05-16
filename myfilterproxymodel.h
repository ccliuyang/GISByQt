#ifndef MYFILTERPROXYMODEL_H
#define MYFILTERPROXYMODEL_H
#include <QSortFilterProxyModel>

///
/// \brief The MyFilterProxyModel class
///
class MyFilterProxyModel : public QSortFilterProxyModel
{
public:
    MyFilterProxyModel();

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

#endif // MYFILTERPROXYMODEL_H
