#ifndef ASSEMBLERSYMBOLS_H
#define ASSEMBLERSYMBOLS_H

#include <QObject>
#include <QDataStream>

typedef enum {
 //   SizeUnknown = 0,
    SizeByte = 1,
    SizeWord = 2
} SymbolSize;

struct AssemblerSymbol {
    quint16 address;
    QString name;
    SymbolSize symbolsize;
};


class AssemblerSymbols : public QObject
{
    Q_OBJECT
public:
    explicit AssemblerSymbols(QObject *parent = 0);

    bool hasAssemSymbolAtAddress(quint16 address);

    const AssemblerSymbol &at(int location) const { return m_assemblerSymbols.at(location); }
    AssemblerSymbol &symbolRefAt(int location) { return m_assemblerSymbols[location]; }
    AssemblerSymbol &operator[](int location) { return m_assemblerSymbols[location]; }

    void editSymbol(int at, AssemblerSymbol newSymbol);

    QDataStream &read(QDataStream &dataStream);
    QDataStream &write(QDataStream &dataStream) const;

    int numAssemblerSymbols() const { return m_assemblerSymbols.count(); }

    void doTestData();

    int locationOfSymbolAtAddress(quint16 address);
    QString getSymbolAtAddress(quint16 address);
signals:
    void symbolAdded(AssemblerSymbol &AssemblerSymbol, int location);
    void symbolAddedAt(int location);
    void symbolRemovedAt(int location);
    void symbolChangedAt(int location);

public slots:
    void addSymbol(AssemblerSymbol ep);
    void removeSymbolAt(int location);

protected:
    QList<AssemblerSymbol> m_assemblerSymbols;
};

QDataStream &operator<<(QDataStream &out, const AssemblerSymbols &model);
QDataStream &operator>>(QDataStream &in, AssemblerSymbols &model);


#endif // ASSEMBLERSYMBOLS_H
