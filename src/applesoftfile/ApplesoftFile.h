#pragma once

#include "GenericFile.h"
#include "ApplesoftLine.h"

#include <QByteArray>
#include <QStringList>
#include <QList>

#include <memory>

class ApplesoftToken;
class ApplesoftRetokenizer;

class ApplesoftFile : public GenericFile
{
public:
    explicit ApplesoftFile(const QByteArray& data = QByteArray());
    ~ApplesoftFile() override = default;
    
    void setData(const QByteArray& data) override;
    QByteArray extraData() const;
    QStringList extraDataHexValues() const;

    const QList<ApplesoftLine>& getLines() const { return m_lines; }

    quint16 length() const override { return m_length; }

    QByteArray rawData() const override;

private:

    QList<ApplesoftLine> m_lines;
    int m_data_end{0};
    quint16 m_length{0};

    std::unique_ptr<ApplesoftRetokenizer> m_retokenizer{nullptr};

    QList<QPair<quint16, quint16>> m_flowTargets;
};
