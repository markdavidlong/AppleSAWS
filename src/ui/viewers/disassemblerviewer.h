#ifndef DISASSEMBLERVIEWER_H
#define DISASSEMBLERVIEWER_H

#include <QWidget>
#include <QByteArray>

#include "binaryfile.h"
#include "relocatablefile.h"
#include "fileviewerinterface.h"
#include "DisassemblerMetadataDialog.h"

namespace Ui {
class DisassemblerViewer;
}

class DisassemblerViewer : public FileViewerInterface
{
    Q_OBJECT

public:
    explicit DisassemblerViewer(QWidget *parent = 0);
    ~DisassemblerViewer();

    void setFile(BinaryFile *file);
    void setFile(RelocatableFile *file);
    void setData(QByteArray data);
    void setText(QString text);

    QString getPotentialLabel(quint16 address);
    virtual bool optionsMenuItems(QMenu *);
    bool canPrint() const;
    bool canExport() const;

    QString makeDescriptorStringForVal(quint8 val);

    QStringList getDisassemblyStrings(quint16 address);

public slots:
    void setFile(GenericFile *file);
    void toggleWordWrap(bool enabled);

    void doPrint();
    void doExport();
    void handleDisassembleRequest(QList<quint16> addresses);
protected slots:
    void showMetadataDialog();
private:
    Ui::DisassemblerViewer *ui;

    DisassemblerMetadataDialog *m_dmd;
    GenericFile *m_file;

    QAction *m_wordWrapAction;
    QAction *m_showMetadataAction;

    BinaryFileMetadata *m_bfm;


    bool m_isRelo;
};

#endif // DISASSEMBLERVIEWER_H
