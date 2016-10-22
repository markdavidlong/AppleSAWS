#include "CharacterSetExplorer.h"
#include "ui_CharacterSetExplorer.h"

CharacterSetExplorer::CharacterSetExplorer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CharacterSetExplorer)
{
    ui->setupUi(this);
}

CharacterSetExplorer::~CharacterSetExplorer()
{
    delete ui;
}
