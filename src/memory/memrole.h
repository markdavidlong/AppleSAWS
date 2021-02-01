#ifndef MEMROLE_H
#define MEMROLE_H

class MemoryCell;

class MemRole
{
public:
    MemRole();
    virtual ~MemRole();

    virtual int id() const = 0;

    void setParent(MemoryCell *parent);
    MemoryCell *parent( ) const  { return m_parent; }

protected:
    MemoryCell *m_parent;

};

#endif // MEMROLE_H
