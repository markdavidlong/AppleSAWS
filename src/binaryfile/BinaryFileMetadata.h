#pragma once

#include "EntryPoints.h"
#include "AssemblerSymbols.h"
#include "GenericFile.h"

#include <Qt>
#include <QList>
#include <QMap>
#include <QObject>




class BinaryFileMetadata : public QObject
{
    Q_OBJECT
public:
    explicit BinaryFileMetadata(GenericFile &file, quint16 defaultAddress, QObject *parent = nullptr);

    [[nodiscard]] QString filename() const { return m_file.filename(); }

    [[nodiscard]] EntryPoints &entryPoints() noexcept { return m_eps; }
    [[nodiscard]] AssemblerSymbols &assemblerSymbols() noexcept { return m_as; }

signals:
    void doDisassemble(QList<quint16>);

public slots:
    void load();
    void save();

    void requestDisassembly() { emit doDisassemble(m_eps.getEntryPointAddresses()); }

private:
    AssemblerSymbols m_as;
    EntryPoints m_eps;

    GenericFile &m_file;

    quint16 m_defaultAddress;
};

