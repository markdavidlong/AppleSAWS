#include <QApplication>
#include <QSettings>
#include <QString>

#include "mainwindow.h"
#include "binaryfile.h"
#include "genericfile.h"
#include "diskfile.h"
#include "catalogsector.h"
#include "applesoftfile.h"

int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("AppleSAWS");
    QCoreApplication::setOrganizationDomain("ml.com");
    QCoreApplication::setApplicationName("AppleSAWS");
    MainWindow w;
    QSettings settings;
    QString lastOpened = settings.value("lastOpened").toString();
    if (!lastOpened.isEmpty())
    {
        w.loadDiskFile(lastOpened);
    }
    w.show();

    return a.exec();

}

