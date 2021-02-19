#ifndef TSLISTVIEW_H
#define TSLISTVIEW_H

#include <QTextBrowser>

class Sector;

class TSListView : public QTextBrowser
{
    Q_OBJECT
public:
    explicit TSListView(QWidget *parent = nullptr);

    void setSector(Sector *sec);

signals:

private:
    Sector *m_sector;

};

#endif // TSLISTVIEW_H
