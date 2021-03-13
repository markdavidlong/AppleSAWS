#ifndef APPLESOFTFILEVIEWER_H
#define APPLESOFTFILEVIEWER_H

/*****************************************************************************
*    AppleSAWS - The Apple Software Analysis WorkShop                        *
*    Copyright (C) 2015-2021  Mark D. Long                                   *
*                                                                            *
*    This program is free software: you can redistribute it and/or modify    *
*    it under the terms of the GNU General Public License as published by    *
*    the Free Software Foundation, either version 3 of the License, or       *
*    (at your option) any later version.                                     *
*                                                                            *
*    This program is distributed in the hope that it will be useful,         *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
*    GNU General Public License for more details.                            *
*                                                                            *
*    You should have received a copy of the GNU General Public License       *
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.  *
*****************************************************************************/

#include "applesoftfile.h"
#include "applesoftformatter.h"
#include "viewerbase.h"
#include "applesoftfiledetailviewer.h"

#include <QWidget>
#include <QMenu>
#include <QAction>


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

#endif // APPLESOFTFILEVIEWER_H
