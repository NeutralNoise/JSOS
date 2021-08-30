#include "MemoryManager.h"

MemoryManager::MemoryManager(const size_t &start, const size_t &size) {
  if(p_activeMemoryManager == NULL) {
    p_activeMemoryManager = this;
  }

  //work out if we have enough room for a memory chunk.
  if(size < sizeof(MemorySegment)) {
    p_first = NULL;
  }
  else {
    m_freeSpace = size - sizeof(MemorySegment);
    m_totalSpace = size;
    p_first = (MemorySegment*)start;
    p_first->allocated = false;
    p_first->prev = 0;
    p_first->next = 0;
    p_first->size = m_freeSpace;
  }
}

MemoryManager::~MemoryManager() {
  if(p_activeMemoryManager == NULL) {
    p_activeMemoryManager = NULL;
  }
}

MemoryManager * MemoryManager::p_activeMemoryManager = NULL;

void* MemoryManager::Malloc(const size_t &size) {
  MemorySegment * result = NULL;
  MemorySegment * tmp = NULL;
  
  //Check to see if we have enough space in ram.
  if(sizeof(MemorySegment) + size > m_freeSpace) {
    //TODO display message about not enough space.
    return NULL;
  }

  //Check if we have any free space in a deallocated chunk.
  for(MemorySegment * chunk = p_first; chunk != NULL; chunk = chunk->next) {
    if(chunk->size >= size && !chunk->allocated) {
      result = chunk;
    }
  }

  if(result == NULL) {
    return NULL;
  }
  //Give us only the memory we need.
  if(result->size >= size + sizeof(MemorySegment) + 1) {
    //cut off the unused memory
    MemorySegment * tmp = (MemorySegment*)((size_t)result + sizeof(MemorySegment) + size);

    tmp->allocated = false;
    tmp->size = result->size - size - sizeof(MemorySegment);
    tmp->prev = result;
    tmp->next = result->next;
    if(tmp->next != NULL) {
      tmp->next->prev = tmp;
    }
    result->size = size;
    result->next = tmp;
    m_freeSpace -= sizeof(MemorySegment);
  } 
  m_freeSpace -= size;
  result->allocated = true;
  //Allocate the memory here.
  return (void*)((size_t)(result) + sizeof(MemorySegment));
}

void MemoryManager::Free(void * ptr) {
  MemorySegment* chunk = (MemorySegment*)((size_t)ptr - sizeof(MemorySegment));
  chunk->allocated = false;
  m_freeSpace += chunk->size;
  //Merge memory.
  //Check if the Memorysegment behind this one is free.
  if(chunk->prev != 0 && !chunk->prev->allocated)
  {
      chunk->prev->next = chunk->next;
      chunk->prev->size += chunk->size + sizeof(MemorySegment);
      m_freeSpace += chunk->size + sizeof(MemorySegment);
      if(chunk->next != 0)
          chunk->next->prev = chunk->prev;
      
      chunk = chunk->prev;
  }

  if(chunk->next != 0 && !chunk->next->allocated)
  {
      chunk->size += chunk->next->size + sizeof(MemorySegment);
      m_freeSpace += chunk->next->size + sizeof(MemorySegment);
      chunk->next = chunk->next->next;
      if(chunk->next != 0)
          chunk->next->prev = chunk;
  }

}
