#ifndef APPLESOFTFILEVIEWER_H
#define APPLESOFTFILEVIEWER_H

#include <QWidget>
#include <QMenu>
#include <QAction>

#include "applesoftfile.h"
#include "applesoftformatter.h"
#include "viewerbase.h"

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

   // void setFormatter(ApplesoftFormatter *formatter);

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
    void setIndentCode(bool enabled, ReformatRule reformat = ForceReformat);
    void setIntsAsHex(bool enabled, ReformatRule reformat = ForceReformat);
    void launchVarBrowser();
    void reformatText();

private:
    bool makeMenuOptions(QMenu *menu);

    ApplesoftFile *m_file;
    ApplesoftFormatter *m_formatter;
    bool m_isFirstFind;
    Ui::ApplesoftFileViewer *ui;
};

#endif // APPLESOFTFILEVIEWER_H
