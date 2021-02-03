#include <QApplication>
#include <QSettings>
#include <QString>
#include <QFontDatabase>
#include <QDebug>

#include "binaryfile.h"
#include "genericfile.h"
#include "diskfile.h"
#include "catalogsector.h"
#include "applesoftfile.h"
#include "startupdialog.h"

int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("LydianScaleSoftware");
    QCoreApplication::setOrganizationDomain("lydianscale.com");
    QCoreApplication::setApplicationName("AppleSAWS");

    auto x = QFontDatabase::addApplicationFont(":/fonts/Print_Char_21.ttf");
    auto y = QFontDatabase::addApplicationFont(":/fonts/PR_Number_3.ttf");

    if (x < 0) { qWarning("Could not load Print_Char_21.ttf font."); }
    if (y < 0) { qWarning("Could not load PR_Number_3.ttf font."); }

    StartupDialog w;
    w.show();

    return a.exec();

}

