#include "applesoftfileviewer.h"
#include "ui_applesoftfileviewer.h"

ApplesoftFileViewer::ApplesoftFileViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApplesoftFileViewer)
{
    ui->setupUi(this);
}

ApplesoftFileViewer::~ApplesoftFileViewer()
{
    delete ui;
}

void ApplesoftFileViewer::setData(QByteArray data)
{
    ui->textArea->setText(data);
}

void ApplesoftFileViewer::setText(QString text)
{
    ui->textArea->setHtml(text);
}
