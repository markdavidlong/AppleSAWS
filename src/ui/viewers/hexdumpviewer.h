#ifndef HEXDUMPVIEWER_H
#define HEXDUMPVIEWER_H

#include "genericfile.h"

#include <QString>
#include <QByteArray>
#include <QWidget>


namespace Ui {
class HexDumpViewer;
}

class HexDumpViewer : public QWidget
{
    Q_OBJECT

public:
    explicit HexDumpViewer(QWidget *parent = 0);
    ~HexDumpViewer();

    void setFile(GenericFile *file, quint16 offset = 0);
    void setData(QByteArray data);
    void setText(QString text);

private:
    Ui::HexDumpViewer *ui;

    quint16 m_offset;
};

#endif // HEXDUMPVIEWER_H
