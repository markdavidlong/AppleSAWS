#include "LocationInfoDialog.h"
#include "ui_LocationInfoDialog.h"

LocationInfoDialog::LocationInfoDialog(QWidget *parent) :
    QDialog(parent,  Qt::CustomizeWindowHint | Qt::WindowTitleHint ),
    ui(std::make_unique<Ui::LocationInfoDialog>())
{
    ui->setupUi(this);
    showSizeWidgets(false);
}

LocationInfoDialog::~LocationInfoDialog() = default;

void LocationInfoDialog::setInfoLabelString(const QString& label)
{
    ui->infoLabel->setText(label);
}

quint16 LocationInfoDialog::getAddress() const
{
    return ui->addressEdit->text().toInt(Q_NULLPTR,16);
}

QString LocationInfoDialog::getInfo() const
{
    return ui->infoEdit->text();
}

int LocationInfoDialog::getSymbolSize() const
{
    return ui->sizeCombo->currentIndex();
}

void LocationInfoDialog::showSizeWidgets(bool show)
{
    ui->sizeCombo->setHidden(!show);
    ui->sizeLabel->setHidden(!show);
}
