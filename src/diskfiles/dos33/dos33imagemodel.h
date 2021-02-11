#ifndef DOS33IMAGEMODEL_H
#define DOS33IMAGEMODEL_H

#include "dos33treeitem.h"
#include "dos33diskimage.h"

#include <QStandardItemModel>
#include <QObject>

#include <QIcon>

class Dos33ImageModel : public QStandardItemModel
{
    Q_OBJECT

    using ImageMap = QMap<QString, Dos33DiskImage *>;

public:
    Dos33ImageModel(QObject *parent = nullptr);
    ~Dos33ImageModel();

    bool addDiskImage(Dos33DiskImage *image, QString name = "");
    Dos33DiskImage *getDiskImage(QString name);
    Dos33DiskImage *removeDiskImage(QString name);



private:
    QIcon m_icon_A;
    QIcon m_icon_a;
    QIcon m_icon_B;
    QIcon m_icon_b;
    QIcon m_icon_I;
    QIcon m_icon_R;
    QIcon m_icon_S;
    QIcon m_icon_T;
    QIcon m_icon_disk;

    ImageMap m_images;
};

#endif // DOS33IMAGEMODEL_H
