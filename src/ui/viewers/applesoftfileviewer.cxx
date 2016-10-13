#include "applesoftfileviewer.h"
#include "ui_applesoftfileviewer.h"
#include "applesoftformatter.h"
#include "applesoftfiledetailviewer.h"
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
    m_formatter->setFlags(ApplesoftFormatter::PrettyFlags);
    connect(ui->findButton,SIGNAL(clicked(bool)), SLOT(findText()));
    m_isFirstFind = true;
    ui->textArea->setUndoRedoEnabled(false);
    ui->textArea->setUndoRedoEnabled(true);

    connect(ui->intHexCB, SIGNAL(toggled(bool)), SLOT(setIntsAsHex(bool)));
    connect(ui->intHexCB, SIGNAL(toggled(bool)), ui->findText,SLOT(clear()));

    connect(ui->indentCode, SIGNAL(toggled(bool)), SLOT(setIndentCode(bool)));
    connect(ui->indentCode, SIGNAL(toggled(bool)), ui->findText,SLOT(clear()));

    connect(ui->varBrowserButton, SIGNAL(clicked(bool)), SLOT(launchVarBrowser()));
}

ApplesoftFileViewer::~ApplesoftFileViewer()
{
    delete ui;
}

void ApplesoftFileViewer::setIndentCode(bool enabled)
{
    if (enabled)
    {
        m_formatter->setFlags(m_formatter->flags() | ApplesoftFormatter::ReindentCode);
    }
    else
    {
        m_formatter->setFlags(m_formatter->flags() & ~ApplesoftFormatter::ReindentCode);
    }
    reformatText();
}

void ApplesoftFileViewer::setIntsAsHex(bool enabled)
{
    if (enabled)
    {
        m_formatter->setFlags(m_formatter->flags() | ApplesoftFormatter::ShowIntsAsHex);
    }
    else
    {
        m_formatter->setFlags(m_formatter->flags() & ~ApplesoftFormatter::ShowIntsAsHex);
    }
    reformatText();
}

void ApplesoftFileViewer::reformatText()
{
    ui->textArea->setText(m_formatter->formatText());
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

void ApplesoftFileViewer::launchVarBrowser()
{
    ApplesoftFileDetailViewer *afdv = new ApplesoftFileDetailViewer();

    afdv->setLineData(m_file->getLines());
    afdv->show();
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
