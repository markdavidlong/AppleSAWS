#ifndef VTOCVIEW_H
#define VTOCVIEW_H

#include <QWidget>

class Sector;

namespace Ui {
class VTOCView;
}

class VTOCView : public QWidget
{
    Q_OBJECT

public:
    explicit VTOCView(QWidget *parent = nullptr);
    ~VTOCView();

    void setSector(Sector *sec);

protected:
    QString makeUsage();

private:
    Ui::VTOCView *ui;
    Sector *m_sector;

};

#endif // VTOCVIEW_H
