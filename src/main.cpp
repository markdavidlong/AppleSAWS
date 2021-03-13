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

#include <QApplication>
#include <QSettings>
#include <QString>
#include <QFontDatabase>
#include <QDebug>

#include "binaryfile.h"
#include "genericfile.h"
#include "dos33diskimage.h"
#include "catalogsector.h"
#include "applesoftfile.h"
#include "startupdialog.h"


int main(int argc, char** argv)
{


    QCoreApplication::setOrganizationName("LydianScaleSoftware");
    QCoreApplication::setOrganizationDomain("lydianscale.com");
    QCoreApplication::setApplicationName("AppleSAWS");

    QApplication a(argc, argv);

    auto x = QFontDatabase::addApplicationFont(":/fonts/Print_Char_21.ttf");
    auto y = QFontDatabase::addApplicationFont(":/fonts/PR_Number_3.ttf");

    if (x < 0) { qWarning("Could not load Print_Char_21.ttf font."); }
    if (y < 0) { qWarning("Could not load PR_Number_3.ttf font."); }

    StartupDialog w;
    w.show();

    return a.exec();
}

