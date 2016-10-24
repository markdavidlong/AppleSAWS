#ifndef CHARSETVIEWER_H
#define CHARSETVIEWER_H

#include "binaryfile.h"
#include "characterwidget.h"
#include "fileviewerinterface.h"
#include "CharacterSetExplorer.h"

#include <QWidget>

class CharSetViewer : public FileViewerInterface
{
    Q_OBJECT
public:
    explicit CharSetViewer(QWidget *parent = 0);
    virtual ~CharSetViewer();

    void setFile(GenericFile *file);
    void setFile(BinaryFile *file);

    virtual bool optionsMenuItems(QMenu *menu);

public slots:
    void setFgColor(QColor color);
    void setBgColor(QColor color);
    void setGridColor(QColor color);

    void showGrid(bool show);
    void enableBitShift(bool enable);

    void showExplorer();

signals:

protected:
    QList<CharacterWidget*> getChildren();

protected slots:
    void cleanupExplorer();

private:
    BinaryFile *m_file;

    QByteArray m_data;

    CharacterSet m_charset;

    CharacterSetExplorer *m_cse;
};

#endif // CHARSETVIEWER_H
