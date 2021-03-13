#ifndef STARTUPDIALOG_H
#define STARTUPDIALOG_H

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

#include "notesdialog.h"
#include "hrcgcontrolsinfo.h"
#include "hexconverter.h"
#include "hexdumpviewer.h"
#include "asciiinfodialog.h"

#include <QDialog>

namespace Ui {
class StartupDialog;
}

class StartupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartupDialog(QWidget *parent = nullptr);
    ~StartupDialog();

private slots:
    void on_exploreContentsBtn_clicked();

    void on_exploreDiskImageBtn_clicked();

    void on_bootSectorCompareBtn_clicked();

    void on_infoBtn_clicked();

    void on_newStuffButton_clicked();

private:
    Ui::StartupDialog *ui;

    NotesDialog *m_notes;
    HRCGControlsInfo *m_hrcgDialog;
    HexConverter *m_hexConverter;
    AsciiInfoDialog *m_AsciiInfoDialog;
};

#endif // STARTUPDIALOG_H
