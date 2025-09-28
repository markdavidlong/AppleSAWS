#include "IntBasicFileViewer.h"
#include "ui_IntBasicFileViewer.h"

IntBasicFileViewer::IntBasicFileViewer(QWidget *parent) :
    FileViewerInterface(parent),
    ui(new Ui::IntBasicFileViewer)
{
    ui->setupUi(this);
}

IntBasicFileViewer::~IntBasicFileViewer()
{
    delete ui;
}

void IntBasicFileViewer::setFile(GenericFile *file) {
    IntBasicFile *ibf = dynamic_cast<IntBasicFile*>(file);
    if (ibf)
    {
      //  setFile(ibf);
        ui->textArea->setText(ibf->data());
    }
}


