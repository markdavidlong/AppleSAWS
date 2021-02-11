#ifndef DOS33TREEITEM_H
#define DOS33TREEITEM_H

#include <QStandardItem>

class Dos33TreeItem : public QStandardItem
{
public:
    Dos33TreeItem();
    Dos33TreeItem(const QIcon &icon, const QString &text);
    Dos33TreeItem(const QString &text);

    ~Dos33TreeItem() { }

private:
    Dos33TreeItem(int rows, int columns = 1) : QStandardItem(rows,columns) { }

};

#endif // DOS33TREEITEM_H
