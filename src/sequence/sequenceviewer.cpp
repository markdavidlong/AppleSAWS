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

#include "sequenceviewer.h"
#include "ui_sequenceviewer.h"

SequenceViewer::SequenceViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SequenceViewer)
{
    ui->setupUi(this);
    ui->vsplitter->setStretchFactor(0,1);
    ui->vsplitter->setStretchFactor(1,1);
    ui->vsplitter->setSizes(QList<int>({4000, 4000}));

    ui->hsplitter->setStretchFactor(0,5);
    ui->hsplitter->setStretchFactor(1,1);
    ui->hsplitter->setSizes(QList<int>({40000, 10000}));


}

SequenceViewer::~SequenceViewer()
{
    delete ui;
}
