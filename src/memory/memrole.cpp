#include "memrole.h"

MemRole::MemRole()
{
    m_parent = nullptr;
}

MemRole::~MemRole()
{

}

void MemRole::setParent(MemoryCell *parent)
{
    m_parent = parent;
}
