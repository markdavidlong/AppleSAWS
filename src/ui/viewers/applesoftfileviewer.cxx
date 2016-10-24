#include "applesoftfileviewer.h"
#include "ui_applesoftfileviewer.h"
#include "applesoftformatter.h"
#include "applesoftfiledetailviewer.h"
#include <QDebug>
#include <QSettings>
#include <QPrinter>
#include <QPrintDialog>


ApplesoftFileViewer::ApplesoftFileViewer(QWidget *parent) :
    FileViewerInterface(parent),
    ui(new Ui::ApplesoftFileViewer)
{
    ui->setupUi(this);

    QSettings settings;
    QString title = QString("AppleSoft Viewer");
    m_title = title;
    setWindowTitle(title);

    m_formatter = new ApplesoftFormatter(this);
    m_formatter->setFlags(ApplesoftFormatter::PrettyFlags);
    connect(ui->findButton,SIGNAL(clicked(bool)), SLOT(findText()));
    m_isFirstFind = true;
    ui->textArea->setUndoRedoEnabled(false);
    ui->textArea->setUndoRedoEnabled(true);

    toggleWordWrap(settings.value("ASViewer.WordWrap",true).toBool());
}

ApplesoftFileViewer::~ApplesoftFileViewer()
{
    delete ui;
}

bool ApplesoftFileViewer::makeMenuOptions(QMenu *menu)
{
    QSettings settings;

    QAction *action = new QAction("Show &Ints as Hex",menu);
    action->setCheckable(true);
    action->setChecked(settings.value("ASViewer.intsAsHex",false).toBool());
    setIntsAsHex(settings.value("ASViewer.intsAsHex",false).toBool());
    connect(action, SIGNAL(toggled(bool)), ui->findText,SLOT(clear()));
    connect(action, SIGNAL(toggled(bool)),SLOT(setIntsAsHex(bool)));
    menu->addAction(action);

    action = new QAction("&Reindent code",menu);
    action->setCheckable(true);
    action->setChecked(settings.value("ASViewer.indentCode",false).toBool());
    setIndentCode(settings.value("ASViewer.indentCode",false).toBool());
    connect(action, SIGNAL(toggled(bool)), ui->findText,SLOT(clear()));
    connect(action, SIGNAL(toggled(bool)),SLOT(setIndentCode(bool)));
    menu->addAction(action);

    menu->addSeparator();

    action = new QAction("Show &Variable Explorer...",menu);
    action->setCheckable(false);
    connect(action, SIGNAL(triggered(bool)), SLOT(launchVarBrowser()));
    menu->addAction(action);

    menu->addSeparator();

    action = new QAction("&Word Wrap");
    action->setCheckable(true);
    action->setChecked(settings.value("ASViewer.WordWrap",true).toBool());
    connect(action, SIGNAL(toggled(bool)), SLOT(toggleWordWrap(bool)));
    menu->addAction(action);

    return true;
}

bool ApplesoftFileViewer::optionsMenuItems(QMenu *menu)
{
    return makeMenuOptions(menu);
}


void ApplesoftFileViewer::toggleWordWrap(bool enabled)
{
    if (enabled)
    {
        ui->textArea->setWordWrapMode(QTextOption::WordWrap);
    }
    else
    {
        ui->textArea->setWordWrapMode(QTextOption::NoWrap);
    }
    QSettings settings;
    settings.setValue("ASViewer.WordWrap",enabled);
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
    QSettings settings;
    settings.setValue("ASViewer.indentCode",enabled);
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
    QSettings settings;
    settings.setValue("ASViewer.intsAsHex",enabled);
    reformatText();
}

void ApplesoftFileViewer::reformatText()
{
    ui->textArea->setText(m_formatter->formatText());
}

void ApplesoftFileViewer::setFile(GenericFile *file) {
    ApplesoftFile *af = dynamic_cast<ApplesoftFile*>(file);
    if (af)
    {
        setFile(af);
    }
}

void ApplesoftFileViewer::setFile(ApplesoftFile *file) {
    m_file = file;
    m_formatter->setFile(file);

    QString title = QString("AppleSoft Viewer: %1").arg(m_file->filename());
    m_title = title;
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
    //TODO: Make this much more robust

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
            highlightCursor.mergeCharFormat(colorFormat);
        }
    }
    QString results = QString("%1 match%2").arg(count).arg(count != 1?"es":"");
    ui->findResults->setText(results);
    cursor.endEditBlock();
    ui->textArea->ensureCursorVisible();

    m_isFirstFind = false;
}

bool ApplesoftFileViewer::canPrint() const { return true; }

void ApplesoftFileViewer::doPrint()
{
    QPrinter printer;

    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle(tr("Print AppleSoft File"));
    if (ui->textArea->textCursor().hasSelection())
        dialog.addEnabledOption(QAbstractPrintDialog::PrintSelection);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    ui->textArea->print(&printer);
}

bool ApplesoftFileViewer::canExport() const { return true; }

void ApplesoftFileViewer::doExport()
{
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QDir savename = QDir(defaultPath).filePath(m_file->filename()+".txt");

    QString saveName = QFileDialog::getSaveFileName(this,
       tr("Export AppleSoft"), savename.path(), tr("Text Files (*.txt)"));

    if (saveName == "") return;  // User cancelled

    qDebug() << "Set filename: " << saveName;

    QFile saveFile(saveName);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,"Save Error","Could not save "+saveName);
        return;
    }

    QTextStream out(&saveFile);
    out << ui->textArea->document()->toPlainText();
    saveFile.close();
}

