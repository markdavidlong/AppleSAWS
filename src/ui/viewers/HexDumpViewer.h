#pragma once

#include "GenericFile.h"
#include "FileViewerInterface.h"

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
    explicit HexDumpViewer(QWidget *parent = 0, int defaultFontSize = -1);
    ~HexDumpViewer();

    void setFile(GenericFile *file) { setFile(file,0); }
    void setFile(GenericFile *file, quint16 offset);
    void setRawData(QByteArray data, quint16 offset = 0);
    virtual bool optionsMenuItems(QMenu *menu);

    bool canPrint() const;
    bool canExport() const;

public slots:
    void showHexAndAsciiValues();
    void toggleWordWrap(bool enabled);
    void doPrint();
    void doExport();

protected:
    void setTextFont(const QFont &font, int forcedFontSize = -1);
private:
    void setText(QString text);
    void setData(QByteArray data);
    QString valToAppleAscii(quint8 val);

    QAction *m_setFontAction;
    int m_defaultFontSize;
    Ui::HexDumpViewer *ui;

    quint16 m_offset;
    QByteArray m_data;

    GenericFile *m_file;
};

