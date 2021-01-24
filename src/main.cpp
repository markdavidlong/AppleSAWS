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
#include "DiskExplorer.h"

int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("LydianScaleSoftware");
    QCoreApplication::setOrganizationDomain("lydianscale.com");
    QCoreApplication::setApplicationName("AppleSAWS");

    qDebug("Adding fonts");
    auto x = QFontDatabase::addApplicationFont(":/fonts/A2_40Col.ttf");
    auto y = QFontDatabase::addApplicationFont(":/fonts/A2_80Col.ttf");
    qDebug("40Col: %d 80Col: %d",x,y);

    qDebug() << "40: " << QFontDatabase::applicationFontFamilies(0);
    qDebug() << "80: " << QFontDatabase::applicationFontFamilies(1);

    DiskExplorer w;
    QSettings settings;
    QString lastOpened = settings.value("lastOpened").toString();
    if (!lastOpened.isEmpty())
    {
        w.loadDiskFile(lastOpened);
    }
    w.show();

    return a.exec();

}

