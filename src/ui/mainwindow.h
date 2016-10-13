#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "diskfile.h"
#include "binaryfile.h"
#include "applesoftfile.h"
//#include "relocatablefile.h"
#include "hexconverter.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void loadDiskFile(QString filename);
    void unloadDiskFile();

    void showLoadDialog();

private slots:
    void handleDiskItemSelectedHexViewOpen(DiskFile *disk, FileDescriptiveEntry fde);
    void handleDiskItemSelectedDefaultOpen(DiskFile *disk, FileDescriptiveEntry fde);

signals:
    void diskFileLoading(QString filename, DiskFile *file);
    void diskFileLoaded(QString filename, DiskFile *file);
    void diskFileLoadFailed(QString filename, DiskFile *file);
    void diskFileUnloading(DiskFile *file);
    void diskFileUnloaded();

protected:
    void openInApplesoftFileViewer(ApplesoftFile *file);
    void openInHiresViewWidget(BinaryFile *file, QString filename);
    void openInDisassemblerViewer(BinaryFile *file);
    void openInMazeViewer(BinaryFile *file);
    void openInCharSetViewer(BinaryFile *file);
private:
    Ui::MainWindow *ui;
    HexConverter *m_hexConverter;

    DiskFile *m_disk;
};

#endif // MAINWINDOW_H
