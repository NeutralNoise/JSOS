#ifndef MEMORY_MANAGER_H_INCLUDED
#define MEMORY_MANAGER_H_INCLUDED

#include "MemorySegment.h"
#include "tier0/types.h"
#include "tier0/cstddef.h"
#include "tier0/defines.h"

extern "C"  inline uint32_t byte_to_mb(uint32_t b) {
  return b/MB;
}

class MemoryManager {
public:

MemoryManager(const size_t &start, const size_t &size);
~MemoryManager();

static MemoryManager * p_activeMemoryManager;

void* Malloc(const size_t &size);
void Free(void* ptr);

uint32_t TotalMemory() const {
  return m_totalSpace;
}

uint32_t FreeMemory() const {
  return m_freeSpace;
}


protected:
MemorySegment * p_first = NULL;
private:
  uint32_t m_freeSpace = 0;
  uint32_t m_totalSpace = 0;
};

#endif //MEMORY_MANAGER_H_INCLUDED
