#ifndef ASCIIINFODIALOG_H
#define ASCIIINFODIALOG_H

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



#include <QDialog>
#include "ui_asciiinfodialog.h"

namespace Ui {
class AsciiInfoDialog;
}

class AsciiInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AsciiInfoDialog(QWidget *parent = 0) :
        QDialog(parent),
        ui(new Ui::AsciiInfoDialog)
    {
        ui->setupUi(this);
        QFont font = ui->tableWidget->itemAt(0,0)->font();
        font.setBold(true);
        for (int idx = 0; idx < ui->tableWidget->rowCount(); idx++)
        {
//            ui->tableWidget->item(idx,5 )->setFont(font);
//            ui->tableWidget->item(idx,6 )->setFont(font);
//            ui->tableWidget->item(idx,7 )->setFont(font);
//            ui->tableWidget->item(idx,8 )->setFont(font);
//            ui->tableWidget->item(idx,9 )->setFont(font);
        }


        for (int idx = 0; idx < ui->tableWidget->rowCount(); idx++)
        {
            for (int jdx = 0; jdx < ui->tableWidget->columnCount(); jdx++)
            {
                ui->tableWidget->item(idx,jdx )->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

                if (jdx < 5 || jdx > 9)
                {
                    ui->tableWidget->item(idx,jdx )->setBackgroundColor(QColor(235,235,235));
                }
            }
        }

        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->resizeRowsToContents();
        ui->tableWidget->verticalHeader()->setVisible(false);
        ui->tableWidget->repaint();
    }

    ~AsciiInfoDialog()
    {
        delete ui;
    }

private:
    Ui::AsciiInfoDialog *ui;
};

#endif // ASCIIINFODIALOG_H
