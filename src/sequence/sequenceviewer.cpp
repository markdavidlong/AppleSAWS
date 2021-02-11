#include "sequenceviewer.h"
#include "ui_sequenceviewer.h"

SequenceViewer::SequenceViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SequenceViewer)
{
    ui->setupUi(this);
    ui->vsplitter->setStretchFactor(0,1);
    ui->vsplitter->setStretchFactor(1,1);
    ui->vsplitter->setSizes(QList<int>({4000, 4000}));

    ui->hsplitter->setStretchFactor(0,5);
    ui->hsplitter->setStretchFactor(1,1);
    ui->hsplitter->setSizes(QList<int>({40000, 10000}));


}

SequenceViewer::~SequenceViewer()
{
    delete ui;
}
