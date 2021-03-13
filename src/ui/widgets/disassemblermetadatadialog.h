#ifndef DISASSEMBLERMETADATADIALOG_H
#define DISASSEMBLERMETADATADIALOG_H

/*****************************************************************************
*    AppleSAWS - The Apple Software Analysis WorkShop                        *
*    Copyright (C) 2015-2021  Mark D. Long                                   *
*                                                                            *
*    This program is free software: you can redistribute it and/or modify    *
*    it under the terms of the GNU General Public License as published by    *
*    the Free Software Foundation, either version 3 of the License, or       *
*    (at your option) any later version.                                     *
*                                                                            *
*    This program is distributed in the hope that it will be useful,         *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
*    GNU General Public License for more details.                            *
*                                                                            *
*    You should have received a copy of the GNU General Public License       *
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.  *
*****************************************************************************/

#include "binaryfilemetadata.h"
#include "entrypoints.h"
#include "entrypointmodel.h"
#include "assemblersymbolmodel.h"
#include "assemblersymbols.h"
#include "locationinfodialog.h"

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


#endif // DISASSEMBLERMETADATADIALOG_H
