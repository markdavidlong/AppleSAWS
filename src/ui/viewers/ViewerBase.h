#pragma once

#include "FileViewerInterface.h"
#include "GenericFile.h"

#include <QMainWindow>
#include <QStackedWidget>
#include <QToolBar>
#include <QComboBox>
#include <memory>

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
    void showViewer(int index);

protected:
    void closeEvent(QCloseEvent *event);
    void addViewer(QString descriptor, FileViewerInterface *viewer);

private:
    std::unique_ptr<Ui::ViewerBase> ui;
    QStackedWidget *m_stack;
    QToolBar *m_toolbar;
    QComboBox *m_viewercombo;

    QMap<QString,FileViewerInterface *> m_viewers;
    GenericFile *m_file;
    QMenu *m_optionMenu;

};
