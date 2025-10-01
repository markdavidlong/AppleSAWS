#include "StartupDialog.h"
#include "ui_StartupDialog.h"
#include "HexConverter.h"
#include "DiskExplorer.h"
#include "HRCGControlsInfo.h"
#include "NotesDialog.h"
#include "AsciiInfoDialog.h"



#include <QFileDialog>

StartupDialog::StartupDialog(QWidget *parent) :
    QDialog(parent),
    ui(std::make_unique<Ui::StartupDialog>())
{
    m_notes = new NotesDialog(this);
    ui->setupUi(this);
}

StartupDialog::~StartupDialog() = default;

void StartupDialog::on_exploreContentsBtn_clicked()
{
    DiskExplorer *w = new DiskExplorer(this);
    w->showLoadDialog(false);



}

void StartupDialog::on_exploreDiskImageBtn_clicked()
{

}

void StartupDialog::on_bootSectorCompareBtn_clicked()
{

}

void StartupDialog::on_infoBtn_clicked()
{
    m_notes->show();
}
