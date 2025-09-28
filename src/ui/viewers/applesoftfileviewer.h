#pragma once

#include "ApplesoftFile.h"
#include "ApplesoftFormatter.h"
#include "viewerbase.h"
#include "ApplesoftFileDetailViewer.h"

#include <QAction>
#include <QWidget>
#include <QMenu>

namespace Ui {
class ApplesoftFileViewer;
}

class ApplesoftFileViewer : public FileViewerInterface
{
    Q_OBJECT

public:

    typedef enum {
        ForceReformat,
        NoReformat
    } ReformatRule;

     ApplesoftFileViewer(QWidget *parent = 0);
    ~ApplesoftFileViewer();

    virtual bool optionsMenuItems(QMenu *menu);

     virtual bool canPrint() const;

     bool canExport() const;

public slots:
    void setFile(GenericFile *file);
    void setFile(ApplesoftFile *m_file);
    void setData(QByteArray data);
    void setText(QString text);
    void findText();
    void doPrint();
    void doExport();

protected slots:
    void toggleWordWrap(bool enabled);
    void setSyntaxHighlighting(bool enabled);
    void setSyntaxHighlighting(bool enabled, ReformatRule reformat);

    void setIndentCode(bool enabled) { setIndentCode(enabled, ForceReformat); }
    void setIndentCode(bool enabled, ReformatRule reformat);

    void setIntsAsHex(bool enabled) { setIntsAsHex(enabled, ForceReformat); }
    void setIntsAsHex(bool enabled, ReformatRule reformat);

    void setBreakAfterReturn(bool enabled) { setBreakAfterReturn(enabled,ForceReformat); }
    void setBreakAfterReturn(bool enabled, ReformatRule reformat);

    void setShowCtrlChars(bool enabled) { setShowCtrlChars(enabled,ForceReformat); }
    void setShowCtrlChars(bool enabled, ReformatRule reformat);

    void setTextFont(const QFont &font, ReformatRule reformat);
    void launchVarBrowser();
    void reformatText();

private:
    bool makeMenuOptions(QMenu *menu);

    ApplesoftFile *m_file;
    ApplesoftFormatter *m_formatter;
    bool m_isFirstFind;
    Ui::ApplesoftFileViewer *ui;
    ApplesoftFileDetailViewer *m_afdv;
    QAction *m_showIntsAction;
    QAction *m_reindentCodeAction;
    QAction *m_blankAfterReturnsAction;
    QAction *m_syntaxHighlightingAction;
    QAction *m_showVarExplorerAction;
    QAction *m_wordWrapAction;
    QAction *m_showCtrlCharsAction;
    QAction *m_setFontAction;
};

