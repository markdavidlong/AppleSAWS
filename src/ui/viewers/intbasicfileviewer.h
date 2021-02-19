#ifndef INTBASICFILEVIEWER_H
#define INTBASICFILEVIEWER_H

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



#include "fileviewerinterface.h"
#include "IntBasicFile.h"

namespace Ui {
class IntBasicFileViewer;
}

class IntBasicFileViewer : public FileViewerInterface
{
    Q_OBJECT

public:
    explicit IntBasicFileViewer(QWidget *parent = nullptr);
    ~IntBasicFileViewer();

    bool optionsMenuItems(QMenu * /*menu*/) { return false; }

public slots:
    void setFile(GenericFile *file);

private:
    Ui::IntBasicFileViewer *ui;
};

#endif // INTBASICFILEVIEWER_H
