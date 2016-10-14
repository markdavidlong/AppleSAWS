#ifndef APPLESOFTFILEDETAILVIEWER_H
#define APPLESOFTFILEDETAILVIEWER_H

#include <QWidget>
#include "applesoftline.h"
#include "applesofttoken.h"

namespace Ui {
class ApplesoftFileDetailViewer;
}

class ApplesoftFileDetailViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ApplesoftFileDetailViewer(QWidget *parent = 0);
    ~ApplesoftFileDetailViewer();

    void setLineData(QVector<ApplesoftLine> lineData);

private:
    void  process();

    Ui::ApplesoftFileDetailViewer *ui;
    QVector<ApplesoftLine> m_lines;
};

#endif // APPLESOFTFILEDETAILVIEWER_H
