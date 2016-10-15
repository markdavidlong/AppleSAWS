#ifndef CHARSETVIEWER_H
#define CHARSETVIEWER_H

#include "binaryfile.h"
#include "characterwidget.h"
#include "fileviewerinterface.h"

#include <QWidget>

class CharSetViewer : public FileViewerInterface
{
    Q_OBJECT
public:
    explicit CharSetViewer(QWidget *parent = 0);

    void setFile(GenericFile *file);
    void setFile(BinaryFile *file);

    virtual bool optionsMenuItems(QMenu *menu);

public slots:
    void setFgColor(QColor color);
    void setBgColor(QColor color);
    void setGridColor(QColor color);

    void showGrid(bool show);
    void enableBitShift(bool enable);


signals:

protected:
    QList<CharacterWidget*> getChildren();

private:
    BinaryFile *m_file;

    QByteArray m_data;

    CharacterSet m_charset;

};

#endif // CHARSETVIEWER_H
