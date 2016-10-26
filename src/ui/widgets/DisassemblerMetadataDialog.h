#ifndef DISASSEMBLERMETADATADIALOG_H
#define DISASSEMBLERMETADATADIALOG_H

#include "binaryfilemetadata.h"
#include "EntryPointModel.h"
#include "LocationInfoDialog.h"

#include <QDialog>

namespace Ui {
class DisassemblerMetadataDialog;
}

class DisassemblerMetadataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DisassemblerMetadataDialog(QWidget *parent = 0);
    ~DisassemblerMetadataDialog();

    void setRelocatable(bool relocatable);

    void processSymbols();
    void processEntryPoints();

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

    bool m_isRelocatable;
};


#endif // DISASSEMBLERMETADATADIALOG_H
