#pragma once

#include "GenericFile.h"
#include "FileViewerInterface.h"

#include <QString>
#include <QByteArray>
#include <QWidget>
#include <memory>


namespace Ui {
class TextHexDumpViewer;
}

class TextHexDumpViewer final : public FileViewerInterface
{
    Q_OBJECT

public:
    explicit TextHexDumpViewer(QWidget *parent = nullptr);
    ~TextHexDumpViewer() override;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    TextHexDumpViewer(const TextHexDumpViewer&) = delete;
    TextHexDumpViewer& operator=(const TextHexDumpViewer&) = delete;
    TextHexDumpViewer(TextHexDumpViewer&&) = delete;
    TextHexDumpViewer& operator=(TextHexDumpViewer&&) = delete;

    void setFile(GenericFile *file) { setFile(file, 0); }
    void setFile(GenericFile *file, quint16 offset);
    void setData(QByteArray data);
    void setText(QString text);

    [[nodiscard]] bool optionsMenuItems(QMenu *menu) override;
    [[nodiscard]] bool canPrint() const override;
    [[nodiscard]] bool canExport() const override;

public slots:
    void toggleWordWrap(bool enabled);
    void doPrint();
    void doExport();

protected:
    [[nodiscard]] QString makeTextStr(QByteArray data);
    [[nodiscard]] QString makeHexStr(QByteArray data);

private:
    std::unique_ptr<Ui::TextHexDumpViewer> ui;

    quint16 m_offset{0};

    GenericFile *m_file{nullptr};
};
