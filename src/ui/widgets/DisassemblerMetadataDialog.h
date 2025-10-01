#pragma once

#include "BinaryFileMetadata.h"
#include "EntryPoints.h"
#include "EntryPointModel.h"
#include "AssemblerSymbolModel.h"
#include "AssemblerSymbols.h"
#include "LocationInfoDialog.h"

#include <QDialog>
#include <QItemSelection>
#include <memory>

namespace Ui {
class DisassemblerMetadataDialog;
}

class DisassemblerMetadataDialog final : public QDialog
{
    Q_OBJECT

public:
    explicit DisassemblerMetadataDialog(BinaryFileMetadata *bfm, QWidget *parent = nullptr);
    ~DisassemblerMetadataDialog() override;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    DisassemblerMetadataDialog(const DisassemblerMetadataDialog&) = delete;
    DisassemblerMetadataDialog& operator=(const DisassemblerMetadataDialog&) = delete;
    DisassemblerMetadataDialog(DisassemblerMetadataDialog&&) = delete;
    DisassemblerMetadataDialog& operator=(DisassemblerMetadataDialog&&) = delete;

protected:
    void showEvent(QShowEvent *);

    void closeEvent(QCloseEvent *);
protected slots:
    void handleExitButton();
    void handleProcessButton();

    void handleAddEntryPointButton();
    void handleRemoveEntryPointButton();

    void handleAddSymbolButton();
    void handleRemoveSymbolButton();

    void handleEntryPointSelectionChanged(QItemSelection selected, QItemSelection deselected);
    void handleSymbolSelectionChanged(QItemSelection selected, QItemSelection deselected);


private:
    std::unique_ptr<Ui::DisassemblerMetadataDialog> ui;
    BinaryFileMetadata *m_bfm{nullptr};
    EntryPointModel *m_epmodel{nullptr};
    AssemblerSymbolModel *m_asmodel{nullptr};
    bool m_isRelocatable{false};
};

