#ifndef DISASSEMBLERMETADATADIALOG_H
#define DISASSEMBLERMETADATADIALOG_H

#include "binaryfilemetadata.h"
#include "EntryPoints.h"
#include "EntryPointModel.h"
#include "AssemblerSymbolModel.h"
#include "AssemblerSymbols.h"
#include "LocationInfoDialog.h"

#include <QDialog>

namespace Ui {
class DisassemblerMetadataDialog;
}

class DisassemblerMetadataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DisassemblerMetadataDialog(BinaryFileMetadata *bfm, QWidget *parent = 0);
    ~DisassemblerMetadataDialog();

    void setRelocatable(bool relocatable);

protected:
    void showEvent(QShowEvent *);

protected slots:
    void handleCancelButton();
    void handleExitButton();
    void handleProcessButton();

    void handleAddEntryPointButton();
    void handleRemoveEntryPointButton();

    void handleAddSymbolButton();
    void handleRemoveSymbolButton();


private:
    Ui::DisassemblerMetadataDialog *ui;

    BinaryFileMetadata *m_bfm;

    EntryPointModel *m_epmodel;

    AssemblerSymbolModel *m_asmodel;

    bool m_isRelocatable;
};


#endif // DISASSEMBLERMETADATADIALOG_H
