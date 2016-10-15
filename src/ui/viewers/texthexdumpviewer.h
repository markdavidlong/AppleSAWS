#ifndef TEXTHEXDUMPVIEWER_H
#define TEXTHEXDUMPVIEWER_H

#include "genericfile.h"
#include "fileviewerinterface.h"

#include <QString>
#include <QByteArray>
#include <QWidget>


namespace Ui {
class TextHexDumpViewer;
}

class TextHexDumpViewer : public FileViewerInterface
{
    Q_OBJECT

public:
    explicit TextHexDumpViewer(QWidget *parent = 0);
    ~TextHexDumpViewer();

    void setFile(GenericFile *file) { setFile(file,0); }
    void setFile(GenericFile *file, quint16 offset);
    void setData(QByteArray data);
    void setText(QString text);

    virtual bool optionsMenuItems(QMenu *menu);

public slots:
    void toggleWordWrap(bool enabled);
protected:
    QString makeTextStr(QByteArray data);
    QString makeHexStr(QByteArray data);
private:
    Ui::TextHexDumpViewer *ui;

    quint16 m_offset;
};

#endif // TEXTHEXDUMPVIEWER_H
