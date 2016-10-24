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

    virtual bool optionsMenuItems(QMenu *menu);

    bool canPrint() const;
    bool canExport() const;

public slots:
    void showHexAndAsciiValues();
    void toggleWordWrap(bool enabled);
    void doPrint();
    void doExport();

private:
    void setText(QString text);
    void setData(QByteArray data);
    QString valToAppleAscii(quint8 val);

    Ui::HexDumpViewer *ui;

    quint16 m_offset;
    QByteArray m_data;

    GenericFile *m_file;
};

#endif // HEXDUMPVIEWER_H
