#ifndef FILEVIEWERINTERFACE_H
#define FILEVIEWERINTERFACE_H

#include <QWidget>
#include <QString>

class GenericFile;
class QMenu;

class FileViewerInterface : public QWidget
{
    Q_OBJECT

public:
    FileViewerInterface(QWidget *parent = Q_NULLPTR) : QWidget(parent) { }

    virtual bool optionsMenuItems(QMenu *) = 0;

    bool canPrint() const { return false; }

public slots:
    virtual void setFile(GenericFile *file) = 0;

    void doPrint() { }
    QString title() const { return m_title; }

signals:
    void setTitle(QString title);

protected:
    QString m_title;
};

#endif // FILEVIEWERINTERFACE_H
