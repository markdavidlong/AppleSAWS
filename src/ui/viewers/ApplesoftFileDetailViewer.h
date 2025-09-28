#pragma once

#include "ApplesoftLine.h"
#include "ApplesoftToken.h"
#include "ApplesoftFile.h"

#include <QWidget>
#include <QDebug>

namespace Ui {
class ApplesoftFileDetailViewer;
}

class ApplesoftFileDetailViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ApplesoftFileDetailViewer(ApplesoftFile *file, QWidget *parent = 0);
    ~ApplesoftFileDetailViewer();

    void setLineData(QVector<ApplesoftLine> lineData);
    void foo() { qDebug() << "AFDV::foo!"; }

    bool save();
    bool load();

protected:
    QString shortenName(QString name);
private:
    void  process();

    Ui::ApplesoftFileDetailViewer *ui;
    QVector<ApplesoftLine> m_lines;

    QMap<QString,QString> m_notes;

    ApplesoftFile *m_file;
};

