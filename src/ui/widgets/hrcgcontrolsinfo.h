#ifndef HRCGCONTROLSINFO_H
#define HRCGCONTROLSINFO_H

#include <QDialog>
#include "ui_hrcgcontrolsinfo.h"


namespace Ui {
class HRCGControlsInfo;
}

class HRCGControlsInfo : public QDialog
{
    Q_OBJECT

public:
    explicit HRCGControlsInfo(QWidget *parent = 0) :
        QDialog(parent),
        ui(new Ui::HRCGControlsInfo)
    {
        ui->setupUi(this);
        ui->tableWidget->resizeColumnsToContents();
    }
    ~HRCGControlsInfo()
    {
        delete ui;
    }

private:
    Ui::HRCGControlsInfo *ui;
};

#endif // HRCGCONTROLSINFO_H
