#ifndef DISASSEMBLERVIEWER_H
#define DISASSEMBLERVIEWER_H

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

#include <QWidget>
#include <QByteArray>

#include "attributedmemory.h"
#include "binaryfile.h"
#include "relocatablefile.h"
#include "fileviewerinterface.h"
#include "disassemblermetadatadialog.h"
#include "jumplinemanager.h"

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
    Ui::DisassemblerViewer *ui;

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

#endif // DISASSEMBLERVIEWER_H
