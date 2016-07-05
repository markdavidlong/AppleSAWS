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

public slots:
    void showHexAndAsciiValues();

private:
    void setText(QString text);
    void setData(QByteArray data);
    QString valToAppleAscii(quint8 val);

    Ui::HexDumpViewer *ui;

    quint16 m_offset;
    QByteArray m_data;
};

#endif // HEXDUMPVIEWER_H
