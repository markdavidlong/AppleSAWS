#pragma once

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
#include <QLabel>


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

    void handleNewOffset(quint16 offset);

private:
    HiresScreenWidget *hrsw;
    QLabel *m_offsetLabel;

    BinaryFile *m_file;
};
