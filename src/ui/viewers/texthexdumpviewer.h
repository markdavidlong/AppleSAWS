#ifndef TEXTHEXDUMPVIEWER_H
#define TEXTHEXDUMPVIEWER_H

#include "genericfile.h"

#include <QString>
#include <QByteArray>
#include <QWidget>


namespace Ui {
class TextHexDumpViewer;
}

class TextHexDumpViewer : public QWidget
{
    Q_OBJECT

public:
    explicit TextHexDumpViewer(QWidget *parent = 0);
    ~TextHexDumpViewer();

    void setFile(GenericFile *file, quint16 offset = 0);
    void setData(QByteArray data);
    void setText(QString text);

protected:
    QString makeTextStr(QByteArray data);
    QString makeHexStr(QByteArray data);
private:
    Ui::TextHexDumpViewer *ui;

    quint16 m_offset;
};

#endif // TEXTHEXDUMPVIEWER_H
