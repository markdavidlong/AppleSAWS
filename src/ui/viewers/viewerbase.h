#ifndef VIEWERBASE_H
#define VIEWERBASE_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QToolBar>
#include <QComboBox>

#include "fileviewerinterface.h"
#include "genericfile.h"

namespace Ui {
class ViewerBase;
}

class ViewerBase : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewerBase(QWidget *parent = 0);
    ~ViewerBase();

    void setFile(GenericFile *file);

signals:
    void viewerClosing(ViewerBase *me);

public slots:
    void showViewer(const QString &descriptor);

protected:
    void closeEvent(QCloseEvent *event);
    void addViewer(QString descriptor, FileViewerInterface *viewer);

private:
    Ui::ViewerBase *ui;
    QStackedWidget *m_stack;
    QToolBar *m_toolbar;
    QComboBox *m_viewercombo;

    QMap<QString,FileViewerInterface *> m_viewers;
    GenericFile *m_file;
    QMenu *m_optionMenu;

};

#endif // VIEWERBASE_H
