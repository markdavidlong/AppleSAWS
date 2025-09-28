#pragma once

#include <QDialog>

namespace Ui {
class HexConverter;
}

class HexConverter : public QDialog
{
    Q_OBJECT

public:
    explicit HexConverter(QWidget *parent = 0);
    ~HexConverter();

public slots:
    void calcFromNewUint16(QString value);
    void calcFromNewInt16(QString value);
    void calcFromNewHex(QString value);
    void calcFromNewUint8(QString value);
    void calcFromNewInt8(QString value);

protected slots:
private:
    Ui::HexConverter *ui;
};

