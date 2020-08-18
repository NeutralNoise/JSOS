#ifndef MEMORY_MANAGER_H_INCLUDED
#define MEMORY_MANAGER_H_INCLUDED

#include "MemorySegment.h"
#include "tier0/types.h"

class MemoryManager {
public:

MemoryManager(const size_t &start, const size_t &size);
~MemoryManager();

static MemoryManager * p_activeMemoryManager;

void* Malloc(const size_t &size);
void Free(void* ptr);

protected:
MemorySegment * p_first = nullptr;
private:
  uint32_t m_freeSpace = 0;
  uint32_t m_totalSpace = 0;
};

#endif //MEMORY_MANAGER_H_INCLUDED
