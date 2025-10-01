#pragma once

#include <QDialog>
#include <memory>

namespace Ui {
class HexConverter;
}

class HexConverter final : public QDialog
{
    Q_OBJECT

public:
    explicit HexConverter(QWidget *parent = nullptr);
    ~HexConverter() override;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    HexConverter(const HexConverter&) = delete;
    HexConverter& operator=(const HexConverter&) = delete;
    HexConverter(HexConverter&&) = delete;
    HexConverter& operator=(HexConverter&&) = delete;

public slots:
    void calcFromNewUint16(const QString& value);
    void calcFromNewInt16(const QString& value);
    void calcFromNewHex(const QString& value);
    void calcFromNewUint8(const QString& value);
    void calcFromNewInt8(const QString& value);

protected slots:
private:
    std::unique_ptr<Ui::HexConverter> ui;
};

