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

class DisassemblerViewer final : public FileViewerInterface
{
    Q_OBJECT

public:
    explicit DisassemblerViewer(QWidget *parent = nullptr);
    ~DisassemblerViewer() override;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    DisassemblerViewer(const DisassemblerViewer&) = delete;
    DisassemblerViewer& operator=(const DisassemblerViewer&) = delete;
    DisassemblerViewer(DisassemblerViewer&&) = delete;
    DisassemblerViewer& operator=(DisassemblerViewer&&) = delete;

    void setFile(BinaryFile *file);
    void setFile(RelocatableFile *file);
    void setData(QByteArray data);

    [[nodiscard]] QString getPotentialLabel(quint16 address);
    [[nodiscard]] bool optionsMenuItems(QMenu *) override;
    [[nodiscard]] bool canPrint() const override;
    [[nodiscard]] bool canExport() const override;
    [[nodiscard]] QString makeDescriptorStringForVal(quint8 val);

    void disassemble(QList<quint16> entryPoints);
    [[nodiscard]] QStringList getDisassemblyStrings();

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

    DisassemblerMetadataDialog *m_dmd{nullptr};
    GenericFile *m_file{nullptr};

    QAction *m_wordWrapAction{nullptr};
    QAction *m_showMetadataAction{nullptr};
    QAction *m_setFontAction{nullptr};

    BinaryFileMetadata *m_bfm{nullptr};

    AttributedMemory m_mem;

    bool m_isRelo{false};

    QStringList m_disassemblyStrings;
    JumpLines m_jumpLines;
};

