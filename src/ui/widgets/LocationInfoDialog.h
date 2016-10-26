#ifndef LOCATIONINFODIALOG_H
#define LOCATIONINFODIALOG_H

#include <QDialog>
#include <QLabel>

namespace Ui {
class LocationInfoDialog;
}

class LocationInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LocationInfoDialog(QWidget *parent = 0);
    ~LocationInfoDialog();

    void setInfoLabelString(QString label);
    quint16 getAddress();
    QString getInfo();

protected:

private:
    Ui::LocationInfoDialog *ui;
};

#endif // LOCATIONINFODIALOG_H
