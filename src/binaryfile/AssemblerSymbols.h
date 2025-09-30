#pragma once

#include <QObject>
#include <QDataStream>

class AssemblerSymbols : public QObject
{
    Q_OBJECT
public:
    enum class SymbolSize : quint8 {
        Byte = 1,
        Word = 2
    };

    struct Symbol {
        quint16 address{0};
        QString name{};
        SymbolSize symbolsize{SymbolSize::Byte};
    };

    explicit AssemblerSymbols(QObject *parent = nullptr);

    [[nodiscard]] bool hasAssemSymbolAtAddress(quint16 address) const;

    [[nodiscard]] const Symbol &at(int location) const { return m_assemblerSymbols.at(location); }
    [[nodiscard]] Symbol &symbolRefAt(int location) { return m_assemblerSymbols[location]; }
    [[nodiscard]] Symbol &operator[](int location) { return m_assemblerSymbols[location]; }

    void editSymbol(int at, Symbol newSymbol);

    [[nodiscard]] QDataStream &read(QDataStream &dataStream);
    [[nodiscard]] QDataStream &write(QDataStream &dataStream) const;

    [[nodiscard]] int numAssemblerSymbols() const noexcept { return m_assemblerSymbols.count(); }

    void doTestData();

    [[nodiscard]] int locationOfSymbolAtAddress(quint16 address) const;
    [[nodiscard]] QString getSymbolAtAddress(quint16 address) const;

signals:
    void symbolAdded(Symbol &symbol, int location);
    void symbolAddedAt(int location);
    void symbolRemovedAt(int location);
    void symbolChangedAt(int location);

public slots:
    void addSymbol(Symbol ep);
    void removeSymbolAt(int location);

protected:
    QList<Symbol> m_assemblerSymbols;
};

QDataStream &operator<<(QDataStream &out, const AssemblerSymbols &model);
QDataStream &operator>>(QDataStream &in, AssemblerSymbols &model);

// Serialization operators for Symbol struct
QDataStream &operator<<(QDataStream &out, const AssemblerSymbols::Symbol &symbol);
QDataStream &operator>>(QDataStream &in, AssemblerSymbols::Symbol &symbol);

