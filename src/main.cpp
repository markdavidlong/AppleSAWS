#include <QApplication>

#include "mainwindow.h"
#include "binaryfile.h"
#include "genericfile.h"
#include "diskfile.h"
#include "catalogsector.h"
#include "applesoftfile.h"

int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    MainWindow w;
    w.loadDiskFile("~/AppleSAWS/images/missing_ring_good.dsk");
//    w.loadDiskFile("~/AppleSAWS/images/dos.3.3.system.master.dsk");
//    w.loadDiskFile("~/AppleSAWS/images//montezuma_etc.dsk");
    w.show();

    return a.exec();

}

