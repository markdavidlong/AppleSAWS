#pragma once

#include "FileViewerInterface.h"
#include "IntBasicFile.h"
#include <memory>

namespace Ui {
class IntBasicFileViewer;
}

class IntBasicFileViewer final : public FileViewerInterface
{
    Q_OBJECT

public:
    explicit IntBasicFileViewer(QWidget *parent = nullptr);
    ~IntBasicFileViewer() override;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    IntBasicFileViewer(const IntBasicFileViewer&) = delete;
    IntBasicFileViewer& operator=(const IntBasicFileViewer&) = delete;
    IntBasicFileViewer(IntBasicFileViewer&&) = delete;
    IntBasicFileViewer& operator=(IntBasicFileViewer&&) = delete;

    [[nodiscard]] bool optionsMenuItems(QMenu * /*menu*/) override { return false; }

public slots:
    void setFile(GenericFile *file) override;

private:
    std::unique_ptr<Ui::IntBasicFileViewer> ui;
};

