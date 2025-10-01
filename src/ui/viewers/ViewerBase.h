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

class ViewerBase final : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewerBase(QWidget *parent = nullptr);
    ~ViewerBase() override;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    ViewerBase(const ViewerBase&) = delete;
    ViewerBase& operator=(const ViewerBase&) = delete;
    ViewerBase(ViewerBase&&) = delete;
    ViewerBase& operator=(ViewerBase&&) = delete;

    void setFile(GenericFile *file);

signals:
    void viewerClosing(ViewerBase *me);

public slots:
    void showViewer(int index);

protected:
    void closeEvent(QCloseEvent *event) override;
    void addViewer(QString descriptor, FileViewerInterface *viewer);

private:
    std::unique_ptr<Ui::ViewerBase> ui;
    QStackedWidget *m_stack{nullptr};
    QToolBar *m_toolbar{nullptr};
    QComboBox *m_viewercombo{nullptr};

    QMap<QString,FileViewerInterface *> m_viewers;
    GenericFile *m_file{nullptr};
    QMenu *m_optionMenu{nullptr};

};
