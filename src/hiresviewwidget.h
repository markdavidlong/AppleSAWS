#ifndef HIRESVIEWWIDGET_H
#define HIRESVIEWWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QByteArray>
#include <QPair>
#include <QMap>

class HiresViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HiresViewWidget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);

    void resizeEvent(QResizeEvent *event);
signals:

public slots:
    void setData(QByteArray data);

private:
    QPixmap m_pixmap;
    QByteArray m_data;

    int findRowCol(int offset);
    static QMap<int,int> *m_rowTable;

    void makeOffsetTable();
};


#endif // HIRESVIEWWIDGET_H
