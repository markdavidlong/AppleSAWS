#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "diskfile.h"
#include "binaryfile.h"

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
    void handleDiskItemSelectedDefaultOpen(DiskFile *disk, FileDescriptiveEntry fde);

signals:
    void diskFileLoading(QString filename, DiskFile *file);
    void diskFileLoaded(QString filename, DiskFile *file);
    void diskFileLoadFailed(QString filename, DiskFile *file);
    void diskFileUnloading(DiskFile *file);
    void diskFileUnloaded();

protected:
    void openInHiresViewWidget(BinaryFile *file, QString filename);
private:
    Ui::MainWindow *ui;

    DiskFile *m_disk;
};

#endif // MAINWINDOW_H
