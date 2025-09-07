#include "History.h"

History::History()
{
    m_pHistorys = new std::list<void*>();
}

History::~History()
{
    m_pHistorys->clear();
    delete m_pHistorys;
    m_pHistorys = nullptr;
}

void History::Add(void* ptr)
{
    m_pHistorys->push_back(ptr);
}

bool History::IsAdded(void* ptr)
{
    for (auto& it : *m_pHistorys)
    {
        if (it == ptr)
            return true;
    }

    return false;
}
