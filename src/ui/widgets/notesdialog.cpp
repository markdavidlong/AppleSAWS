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
#include "ui_notesdialog.h"

#include <QFile>

NotesDialog::NotesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NotesDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QByteArray text;
    QFile notes(":/notes/notes.txt");
    if (notes.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        text = notes.readAll();
        notes.close();
    }
    else
    {
        text = "No notes available right now! Maybe next version!";
    }
    ui->notesArea->setMarkdown(text);
}

NotesDialog::~NotesDialog()
{
    delete ui;
}
