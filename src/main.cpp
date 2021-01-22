#include <QApplication>
#include <QSettings>
#include <QString>

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

