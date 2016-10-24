#ifndef HIRESVIEWWIDGET_H
#define HIRESVIEWWIDGET_H

#include "binaryfile.h"
#include "fileviewerinterface.h"
#include "HiresScreenWidget.h"

#include <QWidget>
#include <QPixmap>
#include <QByteArray>
#include <QPair>
#include <QMap>
#include <QBitArray>
#include <QAction>
#include <QMenu>



class HiresViewWidget : public FileViewerInterface
{
    Q_OBJECT
public:

     explicit HiresViewWidget(QWidget *parent = 0);
     virtual bool optionsMenuItems(QMenu *);

    bool canPrint() const;
    bool canExport() const;
public slots:
    void setFile(GenericFile *file);
    void setFile(BinaryFile *file);

    void doPrint();
    void doExport();
private:
    HiresScreenWidget *hrsw;

    BinaryFile *m_file;
};


#endif // HIRESVIEWWIDGET_H
