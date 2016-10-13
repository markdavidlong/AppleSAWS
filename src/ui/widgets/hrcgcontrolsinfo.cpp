#include "hrcgcontrolsinfo.h"
#include "ui_hrcgcontrolsinfo.h"

HRCGControlsInfo::HRCGControlsInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HRCGControlsInfo)
{
    ui->setupUi(this);
    ui->tableWidget->resizeColumnsToContents();
}

HRCGControlsInfo::~HRCGControlsInfo()
{
    delete ui;
}
