#pragma once

#include <QWidget>
#include <QString>
#include <QPrinter>
#include <QPrintDialog>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>

class GenericFile;
class QMenu;

class FileViewerInterface : public QWidget
{
    Q_OBJECT

public:
    FileViewerInterface(QWidget *parent = Q_NULLPTR) : QWidget(parent) { setFont(QFont("courier")); }

    virtual bool optionsMenuItems(QMenu *) = 0;

    virtual bool canPrint() const { return false; }
    virtual bool canExport() const { return false; }

    QString title() const { return m_title; }

public slots:
    virtual void setFile(GenericFile *file) = 0;

    virtual void doPrint() { }
    virtual void doExport() { }

signals:

protected:
    QString m_title;
};

