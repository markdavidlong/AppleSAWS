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

#include "intbasicfileviewer.h"
#include "ui_intbasicfileviewer.h"
#include "fileviewerinterface.h"

IntBasicFileViewer::IntBasicFileViewer(QWidget *parent) :
    FileViewerInterface(parent),
    ui(new Ui::IntBasicFileViewer)
{
    ui->setupUi(this);
}

IntBasicFileViewer::~IntBasicFileViewer()
{
    delete ui;
}

void IntBasicFileViewer::setFile(GenericFile *file) {
    IntBasicFile *ibf = dynamic_cast<IntBasicFile*>(file);
    if (ibf)
    {
      //  setFile(ibf);
        ui->textArea->setText(ibf->data());
    }
}


