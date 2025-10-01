#pragma once

#include "ApplesoftLine.h"
#include "ApplesoftToken.h"
#include "ApplesoftFile.h"

#include <QWidget>
#include <QDebug>
#include <memory>

namespace Ui {
class ApplesoftFileDetailViewer;
}

class ApplesoftFileDetailViewer final : public QWidget
{
    Q_OBJECT

public:
    explicit ApplesoftFileDetailViewer(ApplesoftFile *file, QWidget *parent = nullptr);
    ~ApplesoftFileDetailViewer() override;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    ApplesoftFileDetailViewer(const ApplesoftFileDetailViewer&) = delete;
    ApplesoftFileDetailViewer& operator=(const ApplesoftFileDetailViewer&) = delete;
    ApplesoftFileDetailViewer(ApplesoftFileDetailViewer&&) = delete;
    ApplesoftFileDetailViewer& operator=(ApplesoftFileDetailViewer&&) = delete;

    void setLineData(QList<ApplesoftLine> lineData);
    void foo() { qDebug() << "AFDV::foo!"; }

    bool save();
    bool load();

protected:
    [[nodiscard]] QString shortenName(QString name);
private:
    void process();

    std::unique_ptr<Ui::ApplesoftFileDetailViewer> ui;
    QList<ApplesoftLine> m_lines;

    QMap<QString,QString> m_notes;

    ApplesoftFile *m_file{nullptr};
};

