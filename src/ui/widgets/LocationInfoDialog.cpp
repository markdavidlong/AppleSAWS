#include "LocationInfoDialog.h"
#include "ui_LocationInfoDialog.h"

LocationInfoDialog::LocationInfoDialog(QWidget *parent) :
    QDialog(parent,  Qt::CustomizeWindowHint | Qt::WindowTitleHint ),
    ui(new Ui::LocationInfoDialog)
{
    ui->setupUi(this);
    showSizeWidgets(false);
}

LocationInfoDialog::~LocationInfoDialog()
{
    delete ui;
}

void LocationInfoDialog::setInfoLabelString(QString label)
{
    ui->infoLabel->setText(label);
}

quint16 LocationInfoDialog::getAddress()
{
    return ui->addressEdit->text().toInt(Q_NULLPTR,16);
}

QString LocationInfoDialog::getInfo()
{
    return ui->infoEdit->text();
}

int LocationInfoDialog::getSymbolSize()
{
    return ui->sizeCombo->currentIndex();
}

void LocationInfoDialog::showSizeWidgets(bool show)
{
    ui->sizeCombo->setHidden(!show);
    ui->sizeLabel->setHidden(!show);
}
