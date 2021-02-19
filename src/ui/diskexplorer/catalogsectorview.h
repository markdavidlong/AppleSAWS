#ifndef CATALOGSECTORVIEW_H
#define CATALOGSECTORVIEW_H

#include <QWidget>

class Sector;

namespace Ui {
class CatalogSectorView;
}

class CatalogSectorView : public QWidget
{
    Q_OBJECT

public:
    explicit CatalogSectorView(QWidget *parent = nullptr);
    ~CatalogSectorView();

    void setSector(Sector *sec);

private:
    Ui::CatalogSectorView *ui;

    Sector *m_sector;
};

#endif // CATALOGSECTORVIEW_H
