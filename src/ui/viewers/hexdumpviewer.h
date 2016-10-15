#ifndef HEXDUMPVIEWER_H
#define HEXDUMPVIEWER_H

#include "genericfile.h"
#include "fileviewerinterface.h"

#include <QString>
#include <QByteArray>
#include <QWidget>


namespace Ui {
class HexDumpViewer;
}

class HexDumpViewer : public FileViewerInterface
{
    Q_OBJECT

public:
    explicit HexDumpViewer(QWidget *parent = 0);
    ~HexDumpViewer();

    void setFile(GenericFile *file) { setFile(file,0); }
    void setFile(GenericFile *file, quint16 offset);

    virtual bool optionsMenuItems(QMenu *) {  return false; }

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
