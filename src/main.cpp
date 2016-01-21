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
    w.loadDiskFile("/home/mlong2/missing_ring_good.dsk");
//    w.loadDiskFile("/home/mlong2/dos.3.3.system.master.dsk");
//    w.loadDiskFile("/home/mlong2/montezuma_etc.dsk");
    w.show();

    return a.exec();

}

