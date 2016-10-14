#ifndef APPLESOFTFILEVIEWER_H
#define APPLESOFTFILEVIEWER_H

#include <QWidget>
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
     ApplesoftFileViewer(QWidget *parent = 0);
    ~ApplesoftFileViewer();

   // void setFormatter(ApplesoftFormatter *formatter);

    virtual QMenu* optionsMenuItems() const;

public slots:
     void setFile(GenericFile *file);
    void setFile(ApplesoftFile *m_file);
    void setData(QByteArray data);
    void setText(QString text);
    void findText();


protected:
protected slots:
    void setIndentCode(bool enabled);
    void setIntsAsHex(bool enabled);
    void launchVarBrowser();
    void reformatText();

private:
    ApplesoftFile *m_file;
    ApplesoftFormatter *m_formatter;
    bool m_isFirstFind;
    Ui::ApplesoftFileViewer *ui;
};

#endif // APPLESOFTFILEVIEWER_H
