#ifndef CENTRALAPPWINDOW_H
#define CENTRALAPPWINDOW_H

#include <QMainWindow>

#include "mainapptoolbar.h"
class QTreeView;
class QAction;
class QStatusBar;
class SequenceToolBox;

class CentralAppWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CentralAppWindow(QWidget *parent = nullptr);

private:
    void createActions();
    void initMenuBar();
    void initStatusBar();
    void initToolBars();
    void initDockWidgets();

private:
    QAction *m_quitAction;

    MainAppToolbar *m_mainToolBar;
    QTreeView *m_directory_area;
    QWidget *m_project_area;
    QStatusBar *m_status_bar;

    SequenceToolBox *m_toolbox;
};

#endif // CENTRALAPPWINDOW_H
