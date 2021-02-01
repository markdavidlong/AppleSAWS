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

    qDebug("Adding fonts");
    auto x = QFontDatabase::addApplicationFont(":/fonts/A2_40Col.ttf");
    auto y = QFontDatabase::addApplicationFont(":/fonts/A2_80Col.ttf");

    if (x < 0) { qWarning("Could not load A2_40Col.ttf font."); }
    if (y < 0) { qWarning("Could not load A2_80Col.ttf font."); }

    StartupDialog w;
    w.show();

    return a.exec();

}

