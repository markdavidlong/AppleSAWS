#pragma once

#include <QDialog>
#include <QLabel>
#include <memory>

namespace Ui {
class LocationInfoDialog;
}

class LocationInfoDialog final : public QDialog
{
    Q_OBJECT

public:
    explicit LocationInfoDialog(QWidget *parent = nullptr);
    ~LocationInfoDialog() override;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    LocationInfoDialog(const LocationInfoDialog&) = delete;
    LocationInfoDialog& operator=(const LocationInfoDialog&) = delete;
    LocationInfoDialog(LocationInfoDialog&&) = delete;
    LocationInfoDialog& operator=(LocationInfoDialog&&) = delete;

    void setInfoLabelString(const QString& label);
    [[nodiscard]] quint16 getAddress() const;
    [[nodiscard]] QString getInfo() const;
    [[nodiscard]] int getSymbolSize() const;

    void showSizeWidgets(bool show);

protected:

private:
    std::unique_ptr<Ui::LocationInfoDialog> ui;
};

