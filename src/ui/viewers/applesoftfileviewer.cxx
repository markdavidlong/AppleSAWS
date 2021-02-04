#include "applesoftfileviewer.h"
#include "ui_applesoftfileviewer.h"
#include "applesoftformatter.h"
#include "util.h"
#include <QDebug>
#include <QSettings>
#include <QPrinter>
#include <QPrintDialog>
#include <QCloseEvent>
#include <QFontDialog>


ApplesoftFileViewer::ApplesoftFileViewer(QWidget *parent) :
    FileViewerInterface(parent),
    ui(new Ui::ApplesoftFileViewer)
{
    ui->setupUi(this);
    m_afdv = Q_NULLPTR;

    QFont textAreaFont("PR Number 3");
  //  QFont textAreaFont("Print Char 21");
   // textAreaFont.setStyleHint(QFont::Monospace);
    textAreaFont.setPointSize(12);

//    qDebug() << "************ " << textAreaFont;

    QSettings settings;
    QString title = QString("Applesoft Viewer");
    m_title = title;
    setWindowTitle(title);

    m_formatter = new ApplesoftFormatter(this);
    m_formatter->setFlags(ApplesoftFormatter::ShowCtrlChars);
    connect(ui->findButton, &QToolButton::clicked, this, &ApplesoftFileViewer::findText);
    m_isFirstFind = true;
    ui->textArea->setUndoRedoEnabled(false);
    ui->textArea->setUndoRedoEnabled(true);

    m_showIntsAction = Q_NULLPTR;
    m_reindentCodeAction = Q_NULLPTR;
    m_blankAfterReturnsAction = Q_NULLPTR;
    m_syntaxHighlightingAction = Q_NULLPTR;
    m_showVarExplorerAction = Q_NULLPTR;
    m_wordWrapAction = Q_NULLPTR;
    m_showCtrlCharsAction = Q_NULLPTR;
    m_setFontAction = Q_NULLPTR;

    toggleWordWrap(settings.value("ASViewer.WordWrap",true).toBool());

    setIndentCode(settings.value("ASViewer.indentCode",false).toBool(), NoReformat);
    setIntsAsHex(settings.value("ASViewer.intsAsHex",false).toBool(), NoReformat);
    setBreakAfterReturn(settings.value("ASViewer.breakAfterReturn",false).toBool(), NoReformat);
    setSyntaxHighlighting(settings.value("ASViewer.syntaxHighlighting",true).toBool(), NoReformat);
    setShowCtrlChars(settings.value("ASViewer.showCtrlChars",true).toBool(), NoReformat);

    setTextFont(fontFromSettings("ASViewer.textFont", textAreaFont), NoReformat);
    setTextFont(textAreaFont, NoReformat);
}

ApplesoftFileViewer::~ApplesoftFileViewer()
{
    if (m_afdv)
    {
        m_afdv->deleteLater();
    }
    delete ui;
}

bool ApplesoftFileViewer::makeMenuOptions(QMenu *menu)
{
    QSettings settings;

    if (!m_showIntsAction)
    {
        m_showIntsAction = new QAction("Show Ints as &Hex",this);
        m_showIntsAction->setCheckable(true);
        m_showIntsAction->setChecked(settings.value("ASViewer.intsAsHex",false).toBool());
        setIntsAsHex(settings.value("ASViewer.intsAsHex",false).toBool(),NoReformat);

        connect(m_showIntsAction, &QAction::toggled, ui->findText, &QLineEdit::clear);
        connect(m_showIntsAction, &QAction::toggled,
                this, static_cast< void (ApplesoftFileViewer::*)(bool)>(&ApplesoftFileViewer::setIntsAsHex));

    }
    menu->addAction(m_showIntsAction);

    if (!m_reindentCodeAction)
    {
    m_reindentCodeAction = new QAction("&Indent code",this);
    m_reindentCodeAction->setCheckable(true);
    m_reindentCodeAction->setChecked(settings.value("ASViewer.indentCode",false).toBool());
    setIndentCode(settings.value("ASViewer.indentCode",false).toBool(),NoReformat);

    connect(m_reindentCodeAction, &QAction::toggled, ui->findText, &QLineEdit::clear);
    connect(m_reindentCodeAction, &QAction::toggled,
            this, static_cast< void (ApplesoftFileViewer::*)(bool)>(&ApplesoftFileViewer::setIndentCode));

    }
    menu->addAction(m_reindentCodeAction);

    if (!m_blankAfterReturnsAction)
    {
    m_blankAfterReturnsAction = new QAction("Blank &Line after RETURNs",this);
    m_blankAfterReturnsAction->setCheckable(true);
    m_blankAfterReturnsAction->setChecked(settings.value("ASViewer.breakAfterReturn",false).toBool());
    setIndentCode(settings.value("ASViewer.breakAfterReturn",false).toBool(),NoReformat);

    connect(m_blankAfterReturnsAction, &QAction::toggled, ui->findText, &QLineEdit::clear);
    connect(m_blankAfterReturnsAction, &QAction::toggled,
            this, static_cast< void (ApplesoftFileViewer::*)(bool)>(&ApplesoftFileViewer::setBreakAfterReturn));
    }
    menu->addAction(m_blankAfterReturnsAction);

    if (!m_showCtrlCharsAction)
    {
    m_showCtrlCharsAction = new QAction("Show &Control Characters",this);
    m_showCtrlCharsAction->setCheckable(true);
    m_showCtrlCharsAction->setChecked(settings.value("ASViewer.showCtrlChars",false).toBool());
    setIndentCode(settings.value("ASViewer.showCtrlChars",false).toBool(),NoReformat);

    connect(m_showCtrlCharsAction, &QAction::toggled, ui->findText, &QLineEdit::clear);
    connect(m_showCtrlCharsAction, &QAction::toggled,
            this, static_cast<void (ApplesoftFileViewer::*)(bool)>(&ApplesoftFileViewer::setShowCtrlChars));
    }
    menu->addAction(m_showCtrlCharsAction);


    menu->addSeparator();

    if (!m_wordWrapAction)
    {
    m_wordWrapAction = new QAction("&Word Wrap",this);
    m_wordWrapAction->setCheckable(true);
    m_wordWrapAction->setChecked(settings.value("ASViewer.WordWrap",true).toBool());
    toggleWordWrap(settings.value("ASViewer.WordWrap",true).toBool());
    connect(m_wordWrapAction, &QAction::triggered, this, &ApplesoftFileViewer::toggleWordWrap);
    }
    menu->addAction(m_wordWrapAction);

    if (!m_syntaxHighlightingAction)
    {
    m_syntaxHighlightingAction = new QAction("&Syntax Highlighting",this);
    m_syntaxHighlightingAction->setCheckable(true);
    m_syntaxHighlightingAction->setChecked(settings.value("ASViewer.syntaxHighlighting",false).toBool());
    setIndentCode(settings.value("ASViewer.syntaxHighlighting",false).toBool(),NoReformat);

    connect(m_syntaxHighlightingAction, &QAction::toggled, ui->findText, &QLineEdit::clear);
    connect(m_syntaxHighlightingAction, &QAction::toggled,
            this, static_cast<void (ApplesoftFileViewer::*)(bool)>(&ApplesoftFileViewer::setSyntaxHighlighting));
    }
    menu->addAction(m_syntaxHighlightingAction);



    menu->addSeparator();

    if (!m_showVarExplorerAction)
    {
        m_showVarExplorerAction = new QAction("Show &Variable Explorer...",this);
        m_showVarExplorerAction->setCheckable(false);
        connect(m_showVarExplorerAction, &QAction::triggered, this, &ApplesoftFileViewer::launchVarBrowser);
    }
    menu->addAction(m_showVarExplorerAction);

    menu->addSeparator();

    if (!m_setFontAction)
    {
        m_setFontAction = new QAction("Set &Font...",this);
        connect(m_setFontAction, &QAction::triggered, this, [this] {
            bool ok;
            QFont font = QFontDialog::getFont(&ok,
                                              ui->textArea->font(),
                                              this, "Set Font"
                                              /*, QFontDialog::MonospacedFonts| QFontDialog::DontUseNativeDialog */);
            if (ok) {
                setTextFont(font,ForceReformat);
                fontToSettings("ASViewer.textFont",font);
            }

        });
    }
    menu->addAction(m_setFontAction);

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

void ApplesoftFileViewer::setIndentCode(bool enabled, ReformatRule reformat)
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
    if (reformat == ForceReformat)
        reformatText();
}

void ApplesoftFileViewer::setBreakAfterReturn(bool enabled, ReformatRule reformat)
{
    if (enabled)
    {
        m_formatter->setFlags(m_formatter->flags() | ApplesoftFormatter::BreakAfterReturn);
    }
    else
    {
        m_formatter->setFlags(m_formatter->flags() & ~ApplesoftFormatter::BreakAfterReturn);
    }
    QSettings settings;
    settings.setValue("ASViewer.breakAfterReturn",enabled);
    if (reformat == ForceReformat)
        reformatText();
}

void ApplesoftFileViewer::setShowCtrlChars(bool enabled, ReformatRule reformat)
{
    if (enabled)
    {
        m_formatter->setFlags(m_formatter->flags() | ApplesoftFormatter::ShowCtrlChars);
    }
    else
    {
        m_formatter->setFlags(m_formatter->flags() & ~ApplesoftFormatter::ShowCtrlChars);
    }
    QSettings settings;
    settings.setValue("ASViewer.showCtrlChars",enabled);
    if (reformat == ForceReformat)
        reformatText();
}

void ApplesoftFileViewer::setSyntaxHighlighting(bool enabled)
{
    setSyntaxHighlighting(enabled, ForceReformat);
}

void ApplesoftFileViewer::setSyntaxHighlighting(bool enabled, ReformatRule reformat)
{
    if (enabled)
    {
        m_formatter->setFlags(m_formatter->flags() | ApplesoftFormatter::SyntaxHighlighting);
    }
    else
    {
        m_formatter->setFlags(m_formatter->flags() & ~ApplesoftFormatter::SyntaxHighlighting);
    }
    QSettings settings;
    settings.setValue("ASViewer.syntaxHighlighting",enabled);
    if (reformat == ForceReformat)
        reformatText();
}

void ApplesoftFileViewer::setIntsAsHex(bool enabled, ReformatRule reformat)
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
    if (reformat == ForceReformat)
        reformatText();
}

void ApplesoftFileViewer::setTextFont(const QFont &font, ReformatRule reformat)
{
    ui->textArea->setFont(font);
    if (reformat == ForceReformat)
    {
        reformatText();
    }
}

void ApplesoftFileViewer::reformatText()
{
    QTextDocument *doc = ui->textArea->document();

    doc->clear();
    m_formatter->formatDocument(doc);

    QTextCursor cursor = ui->textArea->textCursor();
    cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
    ui->textArea->setTextCursor(cursor);

    //qDebug() << m_formatter->flowTargets();
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

    QString title = QString("Applesoft Viewer: %1").arg(m_file->filename());
    m_title = title;
    setWindowTitle(title);
    QTextDocument *doc = ui->textArea->document();
    m_formatter->formatDocument(doc);
    QTextCursor cursor = ui->textArea->textCursor();
    cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
    ui->textArea->setTextCursor(cursor);
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
    if (!m_afdv)
    {
        m_afdv = new ApplesoftFileDetailViewer(m_file);
        qDebug() << "m_afdv = " << m_afdv;
        m_afdv->setWindowTitle(QString("Variables - %1").arg(m_file->filename()));
    }
    m_afdv->show();
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
    dialog.setWindowTitle(tr("Print Applesoft File"));
    if (ui->textArea->textCursor().hasSelection())
        dialog.addEnabledOption(QAbstractPrintDialog::PrintSelection);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QTextDocument printDoc;
    QFont printFont;
    printFont.setStyleHint(QFont::Monospace);
    printFont.setPointSize(10);
    printDoc.setDefaultFont(printFont);
    QTextOption printOptions;
    printOptions.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    printDoc.setDefaultTextOption(printOptions);
    m_formatter->formatDocument(&printDoc);
    printDoc.print(&printer);

    //ui->textArea->document()->print(&printer);
}

bool ApplesoftFileViewer::canExport() const { return true; }

void ApplesoftFileViewer::doExport()
{
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QDir savename = QDir(defaultPath).filePath(m_file->filename()+".txt");

    QString saveName = QFileDialog::getSaveFileName(this,
       tr("Export Applesoft"), savename.path(), tr("Text Files (*.txt)"));

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

