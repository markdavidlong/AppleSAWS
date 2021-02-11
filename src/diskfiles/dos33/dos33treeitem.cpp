#include "dos33treeitem.h"


Dos33TreeItem::Dos33TreeItem() : QStandardItem()
{
    setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

Dos33TreeItem::Dos33TreeItem(const QIcon &icon, const QString &text)
    : QStandardItem(icon,text)
{
    setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

Dos33TreeItem::Dos33TreeItem(const QString &text) : QStandardItem(text)
{
    setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}
