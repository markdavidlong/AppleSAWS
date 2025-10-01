#pragma once

#include "ui_HRCGControlsInfo.h"

#include <QDialog>
#include <memory>

namespace Ui {
class HRCGControlsInfo;
}

class HRCGControlsInfo final : public QDialog
{
    Q_OBJECT

public:
    explicit HRCGControlsInfo(QWidget *parent = nullptr) :
        QDialog(parent),
        ui(std::make_unique<Ui::HRCGControlsInfo>())
    {
        ui->setupUi(this);
        ui->tableWidget->resizeColumnsToContents();
    }
    
    ~HRCGControlsInfo() override = default;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    HRCGControlsInfo(const HRCGControlsInfo&) = delete;
    HRCGControlsInfo& operator=(const HRCGControlsInfo&) = delete;
    HRCGControlsInfo(HRCGControlsInfo&&) = delete;
    HRCGControlsInfo& operator=(HRCGControlsInfo&&) = delete;

private:
    std::unique_ptr<Ui::HRCGControlsInfo> ui;
};
