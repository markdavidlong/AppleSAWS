#ifndef HRCGCONTROLSINFO_H
#define HRCGCONTROLSINFO_H

#include <QDialog>

namespace Ui {
class HRCGControlsInfo;
}

class HRCGControlsInfo : public QDialog
{
    Q_OBJECT

public:
    explicit HRCGControlsInfo(QWidget *parent = 0);
    ~HRCGControlsInfo();

private:
    Ui::HRCGControlsInfo *ui;
};

#endif // HRCGCONTROLSINFO_H
