#include "applesoftfileviewer.h"
#include "ui_applesoftfileviewer.h"
#include "applesoftformatter.h"
#include <QDebug>

ApplesoftFileViewer::ApplesoftFileViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApplesoftFileViewer)
{
    ui->setupUi(this);
    QString title = QString("AppleSoft Viewer");
    setWindowTitle(title);

    m_formatter = new ApplesoftFormatter(this);
    //m_formatter->setFlags(ApplesoftFormatter::PrettyFlags | ApplesoftFormatter::BreakAfterReturn);
    m_formatter->setFlags(ApplesoftFormatter::AllFlags);
    connect(ui->findButton,SIGNAL(clicked(bool)), SLOT(findText()));
    m_isFirstFind = true;
    ui->textArea->setUndoRedoEnabled(false);
    ui->textArea->setUndoRedoEnabled(true);

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

void ApplesoftFileViewer::findText()
{
    QString searchString = ui->findText->text();
    QTextDocument *document = ui->textArea->document();

    if (m_isFirstFind == false)
    {
        document->undo();
    }

    if (searchString.isEmpty()) {
        ui->findResults->setText("");
        return;
    }

    QTextCursor highlightCursor(document);
    QTextCursor cursor(document);

    cursor.beginEditBlock();

    QTextCharFormat plainFormat(highlightCursor.charFormat());
    QTextCharFormat colorFormat = plainFormat;
    colorFormat.setBackground(Qt::yellow);
    int count = 0;
    while(!highlightCursor.isNull() && !highlightCursor.atEnd()) {
        highlightCursor = document->find(searchString, highlightCursor);

        if (!highlightCursor.isNull()) {
            count++;
            int position = highlightCursor.position();
            highlightCursor.setPosition(position,QTextCursor::KeepAnchor);
            //highlightCursor.movePosition(QTextCursor::WordRight,QTextCursor::KeepAnchor);
            highlightCursor.mergeCharFormat(colorFormat);
        }
    }
    QString results = QString("%1 match%2").arg(count).arg(count != 1?"es":"");
    ui->findResults->setText(results);
    cursor.endEditBlock();
    ui->textArea->ensureCursorVisible();

    m_isFirstFind = false;


}
