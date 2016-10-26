#ifndef ASSEMBLERSYMBOLS_H
#define ASSEMBLERSYMBOLS_H

#include <QObject>
#include <QDataStream>

struct AssemSymbol {
    quint16 address;
    QString name;
};

class AssemblerSymbols : public QObject
{
    Q_OBJECT
public:
    explicit AssemblerSymbols(QObject *parent = 0);

    bool hasAssemSymbolAtAddress(quint16 address);

    const AssemSymbol &at(int location) const { return m_assemblerSymbols.at(location); }
    AssemSymbol &symbolRefAt(int location) { return m_assemblerSymbols[location]; }
    AssemSymbol &operator[](int location) { return m_assemblerSymbols[location]; }

    void editSymbol(int at, AssemSymbol newSymbol);

    QDataStream &read(QDataStream &dataStream);
    QDataStream &write(QDataStream &dataStream) const;

    int numAssemblerSymbols() const { return m_assemblerSymbols.count(); }

    void doTestData();

signals:
    void symbolAdded(AssemSymbol &AssemSymbol, int location);
    void symbolAddedAt(int location);
    void symbolRemovedAt(int location);
    void symbolChangedAt(int location);

public slots:
    void addSymbol(AssemSymbol ep);
    void removeSymbolAt(int location);

protected:
    QList<AssemSymbol> m_assemblerSymbols;
};

QDataStream &operator<<(QDataStream &out, const AssemblerSymbols &model);
QDataStream &operator>>(QDataStream &in, AssemblerSymbols &model);


#endif // ASSEMBLERSYMBOLS_H
