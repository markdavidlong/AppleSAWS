#pragma once

#include "FileViewerInterface.h"
#include "IntBasicFile.h"
#include <memory>

namespace Ui {
class IntBasicFileViewer;
}

class IntBasicFileViewer : public FileViewerInterface
{
    Q_OBJECT

public:
    explicit IntBasicFileViewer(QWidget *parent = nullptr);
    ~IntBasicFileViewer();

    bool optionsMenuItems(QMenu * /*menu*/) { return false; }

public slots:
    void setFile(GenericFile *file);

private:
    std::unique_ptr<Ui::IntBasicFileViewer> ui;
};

