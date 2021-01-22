#ifndef BINARYFILEMETADATA_H
#define BINARYFILEMETADATA_H

#include "EntryPoints.h"
#include "AssemblerSymbols.h"
#include "genericfile.h"
#include "binaryfile.h"

#include <Qt>
#include <QList>
#include <QMap>
#include <QObject>




class BinaryFileMetadata : public QObject
{
    Q_OBJECT
public:
    BinaryFileMetadata(GenericFile *file, quint16 defaultAddress, QObject *parent = 0);

    QString filename() const { return m_file->filename(); }

    EntryPoints *entryPoints() { return m_eps; }
    AssemblerSymbols *assemblerSymbols() { return m_as; }

signals:
    void doDisassemble(QList<quint16>);

public slots:
    void load();
    void save();

    void requestDisassembly() { emit doDisassemble(m_eps->getEntryPointAddresses()); }

private:

    EntryPoints *m_eps;
    AssemblerSymbols *m_as;

    GenericFile *m_file;

    quint16 m_defaultAddress;
};

#endif // BINARYFILEMETADATA_H
