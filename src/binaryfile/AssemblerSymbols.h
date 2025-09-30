#pragma once

#include <QObject>
#include <QDataStream>

enum class SymbolSize : quint8 {
    Byte = 1,
    Word = 2
};

struct AssemblerSymbol {
    quint16 address{0};
    QString name{};
    SymbolSize symbolsize{SymbolSize::Byte};
};


class AssemblerSymbols : public QObject
{
    Q_OBJECT
public:
    explicit AssemblerSymbols(QObject *parent = nullptr);

    [[nodiscard]] bool hasAssemSymbolAtAddress(quint16 address) const;

    [[nodiscard]] const AssemblerSymbol &at(int location) const { return m_assemblerSymbols.at(location); }
    [[nodiscard]] AssemblerSymbol &symbolRefAt(int location) { return m_assemblerSymbols[location]; }
    [[nodiscard]] AssemblerSymbol &operator[](int location) { return m_assemblerSymbols[location]; }

    void editSymbol(int at, AssemblerSymbol newSymbol);

    [[nodiscard]] QDataStream &read(QDataStream &dataStream);
    [[nodiscard]] QDataStream &write(QDataStream &dataStream) const;

    [[nodiscard]] int numAssemblerSymbols() const noexcept { return m_assemblerSymbols.count(); }

    void doTestData();

    [[nodiscard]] int locationOfSymbolAtAddress(quint16 address) const;
    [[nodiscard]] QString getSymbolAtAddress(quint16 address) const;

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

// Serialization operators for AssemblerSymbol struct
QDataStream &operator<<(QDataStream &out, const AssemblerSymbol &symbol);
QDataStream &operator>>(QDataStream &in, AssemblerSymbol &symbol);

