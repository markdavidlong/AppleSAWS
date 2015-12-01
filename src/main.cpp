#include <QApplication>

#include "mainwindow.h"
#include "binaryfile.h"
#include "genericfile.h"
#include "diskfile.h"
#include "catalogsector.h"
#include "applesoftfile.h"

int main(int argc, char** argv)
{
/*
//    DiskFile df("/home/mlong/Desktop/dos.3.3.system.master.dsk");
     DiskFile df("/home/mlong/Desktop/missing_ring_good.dsk");

    df.getVTOC().dump();

    QList<CatalogSector>  sectors = df.getCatalogSectors();

    FileDescriptiveEntry toLoad;

    foreach (FileDescriptiveEntry fde, df.getAllFDEs()) {
        fde.catalog();
        if (fde.filename.printable().contains("ASM.DATA")) {
            toLoad = fde;
        }
    }

    QByteArray file = df.getFile(toLoad);

    BinaryFile myfile;
    myfile.setData(file);
    myfile.dump();


    //ApplesoftFile af(file);
    //af.list();
    */

    QApplication a(argc, argv);
    MainWindow w;
    w.loadDiskFile("/home/mlong/Desktop/missing_ring_good.dsk");
    w.show();

    return a.exec();

}

