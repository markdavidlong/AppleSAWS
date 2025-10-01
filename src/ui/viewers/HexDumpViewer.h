#pragma once

#include "GenericFile.h"
#include "FileViewerInterface.h"

#include <QString>
#include <QByteArray>
#include <QWidget>
#include <memory>


namespace Ui {
class HexDumpViewer;
}

class HexDumpViewer final : public FileViewerInterface
{
    Q_OBJECT

public:
    explicit HexDumpViewer(QWidget *parent = nullptr, int defaultFontSize = -1);
    ~HexDumpViewer() override;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    HexDumpViewer(const HexDumpViewer&) = delete;
    HexDumpViewer& operator=(const HexDumpViewer&) = delete;
    HexDumpViewer(HexDumpViewer&&) = delete;
    HexDumpViewer& operator=(HexDumpViewer&&) = delete;

    void setFile(GenericFile *file) { setFile(file, 0); }
    void setFile(GenericFile *file, quint16 offset);
    void setRawData(QByteArray data, quint16 offset = 0);
    [[nodiscard]] bool optionsMenuItems(QMenu *menu) override;
    [[nodiscard]] bool canPrint() const override;
    [[nodiscard]] bool canExport() const override;

public slots:
    void showHexAndAsciiValues();
    void toggleWordWrap(bool enabled);
    void doPrint();
    void doExport();

protected:
    void setTextFont(const QFont &font, int forcedFontSize = -1);
private:
    void setText(QString text);
    void setData(QByteArray data);
    [[nodiscard]] QString valToAppleAscii(quint8 val);

    QAction *m_setFontAction{nullptr};
    int m_defaultFontSize;
    std::unique_ptr<Ui::HexDumpViewer> ui;

    quint16 m_offset{0};
    QByteArray m_data;

    GenericFile *m_file{nullptr};
};

