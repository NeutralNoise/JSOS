#include "MemoryManager.h"
#include "MemorySegment.h"
#include "tier0/defines.h"

#define MAX_PAGE_SIZE (10 * MB)

MemoryManager::MemoryManager(const size_t &start, const size_t &size) {
  if(p_activeMemoryManager == nullptr) {
    p_activeMemoryManager = this;
  }

  //work out if we have enough room for a page file.
  if(size < sizeof(MemoryPage) && size < sizeof(MemorySegment)) {
    p_first = nullptr;
  }
  else {
    //TODO allow define of page file size.
    //Set up the first page file.
    p_first = (MemoryPage*)start;
    p_first->total_size = (uint32_t)MAX_PAGE_SIZE;
    p_first->free_space = (uint32_t)MAX_PAGE_SIZE - sizeof(MemoryPage);
    p_first->next = nullptr;
    p_first->prev = nullptr;
    p_first->is_free = true;
    m_freeSpace = size - sizeof(MemoryPage);
    p_cachedPage = p_first;
    m_totalSpace = size;
    m_freeSpace -= p_first->CreateFirstSegment(m_freeSpace);
  }
}

MemoryManager::~MemoryManager() {
  if(p_activeMemoryManager == nullptr) {
    p_activeMemoryManager = nullptr;
  }
}

MemoryManager * MemoryManager::p_activeMemoryManager = nullptr;

void* MemoryManager::Malloc(const size_t &size) {
  MemorySegment * result = nullptr;
  MemorySegment * tmp = nullptr;
  //Check if the cached page has free space.
  if(p_cachedPage) {
    tmp = p_cachedPage->segments;
    if(p_cachedPage->free_space >= size) {
      //see if the page has memory segments that we can use.
      while(tmp->size < size || tmp->allocated) {
        if(tmp->next) {
          tmp = tmp->next;
        }
        else {
          //We are at the end of the segments so create a new one.
          if(m_freeSpace > size + sizeof(MemorySegment)) {
            m_freeSpace -= p_cachedPage->AddSegment(size);
            result = p_cachedPage->lastsegment;
          }
          else {
            //We need to find a page with the same amount of free space as size + sizeof(MemorySegment)
            p_cachedPage = p_first;
            while(p_cachedPage->free_space < size + sizeof(MemorySegment)) {
              if(p_cachedPage->next) {
                p_cachedPage == p_cachedPage->next;
                tmp = p_cachedPage->segments;
              }
              else {
                //Create a new page;
                if(m_freeSpace < sizeof(MemoryPage) + (uint32_t)MAX_PAGE_SIZE) {
                  //TODO error message
                  //We have ran out of memory.
                  return nullptr;
                }
                p_cachedPage = p_cachedPage->CreateNextPage();
                tmp = p_cachedPage->segments;
              }
            }
          }
        }
      }
    }
  }

  //Check if we already have a segment we can use.
  if(!result) {
    if(tmp->size >= size && !tmp->allocated) {
      if(tmp->size > size + sizeof(MemorySegment)) {
        result = p_cachedPage->SplitSegment(tmp, size);
      }
      else {
        result = tmp;
      }
    }
  } 
  else {
    //We have to add a new segment;
    p_cachedPage->free_space -= p_cachedPage->AddSegment(size);
    result = p_cachedPage->lastsegment;
  }

  result->allocated = true;
  //Allocate the memory here.
  return (void*)((size_t)(result) + sizeof(MemorySegment));
}

void MemoryManager::Free(void * ptr) {
  MemorySegment* tmp = (MemorySegment*)((size_t)ptr - sizeof(MemorySegment));
  tmp->allocated = false;
  tmp->page->free_space += tmp->size;
  //Merge memory.
  if(tmp->perv && !tmp->perv->allocated) {
      tmp->page->free_space += sizeof(MemorySegment);
      tmp->perv->size += (tmp->size + sizeof(MemorySegment));
      tmp->perv->next = tmp->next;
      
      if(tmp->next) {
        tmp->next->perv = tmp->perv;
      }

     tmp = tmp->perv; 
  }

  if(tmp->next && !tmp->next->allocated) {
    tmp->page->free_space += sizeof(MemorySegment);
    tmp->size += tmp->next->size + sizeof(MemorySegment);
    tmp->next = tmp->next->next;
    if(tmp->next) {
      tmp->next->perv = tmp;
    }
  }
}
