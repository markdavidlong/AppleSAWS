#pragma once

#include "DiskFile.h"

#include <QByteArray>
#include <QString>


class GenericFile
{
public:
    // Compile-time constants for better type safety
    static constexpr quint16 MAX_LENGTH = 0xFFFF;
    
    explicit GenericFile(const QByteArray& data = QByteArray());
    
    // Modern C++ Rule of Five with move semantics
    GenericFile(const GenericFile& other) = default;
    GenericFile& operator=(const GenericFile& other) = default;
    GenericFile(GenericFile&& other) noexcept = default;
    GenericFile& operator=(GenericFile&& other) noexcept = default;
    virtual ~GenericFile() noexcept = default;
  
    virtual void setData(const QByteArray& data);
    virtual void setData(QByteArray&& data);
    [[nodiscard]] virtual QByteArray data() const noexcept { return m_data; }
    
    inline void setFilename(const QString& filename) { m_filename = filename; }
    [[nodiscard]] inline QString filename() const noexcept { return m_filename; }
    
    virtual void setAddress(quint16 location) noexcept { m_address = location; }
    [[nodiscard]] virtual quint16 address() const noexcept { return m_address; }
   
    [[nodiscard]] virtual QByteArray rawData() const noexcept { return m_data; }

    virtual void setLength(quint16 length) noexcept { m_length = length; }
    [[nodiscard]] virtual quint16 length() const noexcept { return m_length; }

    [[nodiscard]] inline DiskFile *diskFile() const noexcept { return m_diskfile; }
    inline void setDiskFile(DiskFile *diskfile) noexcept { m_diskfile = diskfile; }

    // Utility methods for modern C++
    [[nodiscard]] bool isEmpty() const noexcept { return m_data.isEmpty(); }
    [[nodiscard]] constexpr bool hasValidLength() const noexcept { return m_length > 0 && m_length <= MAX_LENGTH; }

protected:
    QByteArray m_data{};
    QString m_filename{};
    quint16 m_address{0};
    quint16 m_length{0};
    DiskFile *m_diskfile{nullptr};

};
