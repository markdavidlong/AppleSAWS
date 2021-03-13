#ifndef LOCATIONINFODIALOG_H
#define LOCATIONINFODIALOG_H

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
#include <QLabel>

namespace Ui {
class LocationInfoDialog;
}

class LocationInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LocationInfoDialog(QWidget *parent = 0);
    ~LocationInfoDialog();

    void setInfoLabelString(QString label);
    quint16 getAddress();
    QString getInfo();
    int getSymbolSize();

    void showSizeWidgets(bool show);

protected:

private:
    Ui::LocationInfoDialog *ui;
};

#endif // LOCATIONINFODIALOG_H
