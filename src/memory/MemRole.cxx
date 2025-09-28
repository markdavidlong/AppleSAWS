#include "MemRole.h"

MemRole::MemRole()
{
    m_parent = nullptr;
}



void MemRole::setParent(MemoryCell *parent)
{
    m_parent = parent;
}
