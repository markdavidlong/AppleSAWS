#include "sequenceviewer.h"
#include "ui_sequenceviewer.h"

SequenceViewer::SequenceViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SequenceViewer)
{
    ui->setupUi(this);

}

SequenceViewer::~SequenceViewer()
{
    delete ui;
}
