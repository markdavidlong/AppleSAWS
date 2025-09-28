#pragma once

#include "binaryfilemetadata.h"
#include "EntryPoints.h"
#include "EntryPointModel.h"
#include "AssemblerSymbolModel.h"
#include "AssemblerSymbols.h"
#include "LocationInfoDialog.h"

#include <QDialog>
#include <QItemSelection>

namespace Ui {
class DisassemblerMetadataDialog;
}

class DisassemblerMetadataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DisassemblerMetadataDialog(BinaryFileMetadata *bfm, QWidget *parent = 0);
    ~DisassemblerMetadataDialog();

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
    Ui::DisassemblerMetadataDialog *ui;

    BinaryFileMetadata *m_bfm;

    EntryPointModel *m_epmodel;

    AssemblerSymbolModel *m_asmodel;

    bool m_isRelocatable;
};

