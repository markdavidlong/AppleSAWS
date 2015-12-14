#ifndef APPLESOFTFILEVIEWER_H
#define APPLESOFTFILEVIEWER_H

#include <QWidget>

namespace Ui {
class ApplesoftFileViewer;
}

class ApplesoftFileViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ApplesoftFileViewer(QWidget *parent = 0);
    ~ApplesoftFileViewer();

public slots:
    void setData(QByteArray data);
    void setText(QString text);

private:
    Ui::ApplesoftFileViewer *ui;
};

#endif // APPLESOFTFILEVIEWER_H
