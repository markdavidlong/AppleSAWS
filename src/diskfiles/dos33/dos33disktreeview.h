#ifndef DOS33DISKTREEVIEW_H
#define DOS33DISKTREEVIEW_H

#include <QTreeView>
#include <QWidget>

#include "filedescriptiveentry.h"

class Dos33DiskTreeView : public QTreeView
{
    Q_OBJECT
public:
    Dos33DiskTreeView(QWidget *parent = nullptr);

protected:


signals:
    void fileClicked(const FileDescriptiveEntry &fde);
    void fileRightClicked(const FileDescriptiveEntry &fde);
    void fileDoubleClicked(const FileDescriptiveEntry &fde);
    void diskClicked(QString name);
    void diskRightClicked(QString name);
    void sectorClicked(TSPair ts);
    void sectorRightClicked(TSPair ts);
    void sectorDoubleClicked(TSPair ts);


private slots:
    void handleClick(const QModelIndex &index);
    void handleDoubleClick(const QModelIndex &index);
    void handlePress(const QModelIndex &index);



};

#endif // DOS33DISKTREEVIEW_H
