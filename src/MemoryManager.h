#ifndef MEMORY_MANAGER_H_INCLUDED
#define MEMORY_MANAGER_H_INCLUDED

#include "MemoryPage.h"
#include "types.h"

class MemoryManager {
public:

MemoryManager(const size_t &start, const size_t &size);
~MemoryManager();

static MemoryManager * p_activeMemoryManager;

void* Malloc(const size_t &size);
void Free(void* ptr);

protected:
MemoryPage * p_first = nullptr;
MemoryPage * p_cachedPage = nullptr; //!< The last accessed memory page.
private:
  uint32_t m_freeSpace = 0;
  uint32_t m_totalSpace = 0;
};

#endif //MEMORY_MANAGER_H_INCLUDED
