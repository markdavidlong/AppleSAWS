#pragma once

#include "GenericFile.h"
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

    bool canPrint() const;
    bool canExport() const;

public slots:
    void toggleWordWrap(bool enabled);
    void doPrint();
    void doExport();

protected:
    QString makeTextStr(QByteArray data);
    QString makeHexStr(QByteArray data);

private:
    Ui::TextHexDumpViewer *ui;

    quint16 m_offset;

    GenericFile *m_file;
};
