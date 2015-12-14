#include "applesoftfileviewer.h"
#include "ui_applesoftfileviewer.h"
#include "applesoftformatter.h"

ApplesoftFileViewer::ApplesoftFileViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApplesoftFileViewer)
{
    ui->setupUi(this);
    QString title = QString("AppleSoft Viewer");
    setWindowTitle(title);

    m_formatter = new ApplesoftFormatter(this);
    m_formatter->setFlags(ApplesoftFormatter::PrettyFlags);
}

ApplesoftFileViewer::~ApplesoftFileViewer()
{
    delete ui;
}

void ApplesoftFileViewer::setFile(ApplesoftFile *file) {
    m_file = file;
    m_formatter->setFile(file);

    QString title = QString("AppleSoft Viewer: %1").arg(m_file->filename());
    setWindowTitle(title);

    ui->textArea->setText(m_formatter->formatText());
}

void ApplesoftFileViewer::setData(QByteArray data)
{
    ui->textArea->setText(data);
}

void ApplesoftFileViewer::setText(QString text)
{
    ui->textArea->setHtml(text);
}
