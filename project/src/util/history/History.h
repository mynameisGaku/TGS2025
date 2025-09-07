#pragma once
#include <list>

class History
{
public:
    History();
    ~History();

    void Add(void* ptr);
    bool IsAdded(void* ptr);

private:
    std::list<void*>* m_pHistorys{};
};