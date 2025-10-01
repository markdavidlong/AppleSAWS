#pragma once

#include "AttributedMemory.h"
#include "FileViewerInterface.h"
#include "DisassemblerMetadataDialog.h"
#include "../../binaryfile/JumpLineManager.h"

#include <QWidget>
#include <QByteArray>
#include <memory>

class BinaryFile;
class RelocatableFile;

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

    QString getPotentialLabel(quint16 address);
    virtual bool optionsMenuItems(QMenu *);

    bool canPrint() const;
    bool canExport() const;

    QString makeDescriptorStringForVal(quint8 val);

    void disassemble(QList<quint16> entryPoints);
    QStringList getDisassemblyStrings();

public slots:
    void setFile(GenericFile *file);
    void toggleWordWrap(bool enabled);

    void doPrint();
    void doExport();
    void handleDisassembleRequest(QList<quint16> addresses);
protected:
    void setTextFont(const QFont &font);
protected slots:
    void showMetadataDialog();
private:
    std::unique_ptr<Ui::DisassemblerViewer> ui;

    DisassemblerMetadataDialog *m_dmd;
    GenericFile *m_file;

    QAction *m_wordWrapAction;
    QAction *m_showMetadataAction;
    QAction *m_setFontAction;

    BinaryFileMetadata *m_bfm;

    AttributedMemory m_mem;

    bool m_isRelo;

    QStringList m_disassemblyStrings;
    JumpLines m_jumpLines;
};

