#pragma once

#include "ApplesoftFile.h"
#include "ApplesoftFormatter.h"
#include "ViewerBase.h"
#include "ApplesoftFileDetailViewer.h"

#include <QAction>
#include <QWidget>
#include <QMenu>
#include <memory>

namespace Ui {
class ApplesoftFileViewer;
}

class ApplesoftFileViewer final : public FileViewerInterface
{
    Q_OBJECT

public:
    enum class ReformatRule : bool {
        ForceReformat = true,
        NoReformat = false
    };

    explicit ApplesoftFileViewer(QWidget *parent = nullptr);
    ~ApplesoftFileViewer() override;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    ApplesoftFileViewer(const ApplesoftFileViewer&) = delete;
    ApplesoftFileViewer& operator=(const ApplesoftFileViewer&) = delete;
    ApplesoftFileViewer(ApplesoftFileViewer&&) = delete;
    ApplesoftFileViewer& operator=(ApplesoftFileViewer&&) = delete;

    [[nodiscard]] bool optionsMenuItems(QMenu *menu) override;
    [[nodiscard]] bool canPrint() const override;
    [[nodiscard]] bool canExport() const override;

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

    void setIndentCode(bool enabled) { setIndentCode(enabled, ReformatRule::ForceReformat); }
    void setIndentCode(bool enabled, ReformatRule reformat);

    void setIntsAsHex(bool enabled) { setIntsAsHex(enabled, ReformatRule::ForceReformat); }
    void setIntsAsHex(bool enabled, ReformatRule reformat);

    void setBreakAfterReturn(bool enabled) { setBreakAfterReturn(enabled, ReformatRule::ForceReformat); }
    void setBreakAfterReturn(bool enabled, ReformatRule reformat);

    void setShowCtrlChars(bool enabled) { setShowCtrlChars(enabled, ReformatRule::ForceReformat); }
    void setShowCtrlChars(bool enabled, ReformatRule reformat);

    void setTextFont(const QFont &font, ReformatRule reformat);
    void launchVarBrowser();
    void reformatText();

private:
    [[nodiscard]] bool makeMenuOptions(QMenu *menu);

    ApplesoftFile *m_file{nullptr};
    ApplesoftFormatter *m_formatter{nullptr};
    bool m_isFirstFind{true};
    std::unique_ptr<Ui::ApplesoftFileViewer> ui;
    ApplesoftFileDetailViewer *m_afdv{nullptr};
    QAction *m_showIntsAction{nullptr};
    QAction *m_reindentCodeAction{nullptr};
    QAction *m_blankAfterReturnsAction{nullptr};
    QAction *m_syntaxHighlightingAction{nullptr};
    QAction *m_showVarExplorerAction{nullptr};
    QAction *m_wordWrapAction{nullptr};
    QAction *m_showCtrlCharsAction{nullptr};
    QAction *m_setFontAction{nullptr};
};

