#include "dos33disktreeview.h"

#include <QGuiApplication>

Dos33DiskTreeView::Dos33DiskTreeView(QWidget *parent) : QTreeView(parent)
{
    connect(this, &Dos33DiskTreeView::clicked,
            this, &Dos33DiskTreeView::handleClick);
    connect(this, &Dos33DiskTreeView::pressed,
            this, &Dos33DiskTreeView::handlePress);
    connect(this, &Dos33DiskTreeView::doubleClicked,
            this, &Dos33DiskTreeView::handleDoubleClick);

}

void Dos33DiskTreeView::handleClick(const QModelIndex &/*index*/)
{

}

void Dos33DiskTreeView::handleDoubleClick(const QModelIndex &/*index*/)
{

}

void Dos33DiskTreeView::handlePress(const QModelIndex &/*index*/)
{
    if (QGuiApplication::mouseButtons() & Qt::RightButton)
    {

    }
}
