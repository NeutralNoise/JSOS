#ifndef MEMORY_PAGE_H_INCLUDED
#define MEMORY_PAGE_H_INCLUDED

#include "types.h"
#include "MemorySegment.h"

// This memory paging is wrong to the point of being so wrong that its not even in this galexy any more.
// #TODO Rewrite this to use the MMU.

struct MemoryPage {
  MemoryPage * next = nullptr; //!< The next memory page.
  MemoryPage * prev = nullptr; //!< The memory page before this one.
  MemoryPage * nextVirtual = nullptr; //!< unused
  MemoryPage * pervVirtual = nullptr; //!< unused

  MemorySegment * segments; //!< The used memory of this page.
  MemorySegment * lastsegment; //!< The last memory segment in this list.

  size_t total_size; //!< The total size of the memory page.
  size_t free_space; //!< The amount of free space in this page.

  bool is_free; //!< Does this page have enough free space for sizeof(MemorySegment) + 1(byte);

  MemoryPage* CreateNextPage() {
    next = (MemoryPage*)((size_t)(this) + total_size);
    next->total_size = total_size;
    next->free_space = total_size;
    next->prev = this;
    next->CreateFirstSegment();
    return next;
  }

  size_t CreateFirstSegment() {
    if(!segments) {
      segments = (MemorySegment*)(size_t)(this) + sizeof(MemoryPage);
      segments->page = this;
      free_space -= sizeof(MemorySegment);
      lastsegment = segments;
      lastsegment->page = this;
      return sizeof(MemorySegment);
    }
    //We didn't need to do anything so no space was taken.
    return 0;
  }

  size_t CreateFirstSegment(const size_t &size) {
    if(!segments) {
      segments = (MemorySegment*)(size_t)(this) + sizeof(MemoryPage);
      segments->page = this;
      segments->size = size;
      free_space -= sizeof(MemorySegment);
      lastsegment = segments;
      lastsegment->page = this;
      lastsegment->size = size - sizeof(MemorySegment);
      return sizeof(MemorySegment);
    }
    //We didn't need to do anything so no space was taken.
    return 0;
  }

/**
 * @brief Adds a memory segment to the page.
 * 
 * @param size The size of the memory segment.
 * @return unsigned int The amount of memory that was used. 0 if no memory was used.
 */
  unsigned int AddSegment(const unsigned int &size) {
    //You a dumb dumb
    if(size == 0) {
      return 0;
    }
    if(!segments) {
     return CreateFirstSegment();
    }

    if(free_space < size + sizeof(MemorySegment)) {
      return 0;
    }
    else {
      lastsegment->next = (MemorySegment*)((size_t)(lastsegment + lastsegment->size + sizeof(MemorySegment)));
      lastsegment->next->perv = lastsegment;
      lastsegment = lastsegment->next;
      lastsegment->page = this;
    }
    
    return size + sizeof(MemorySegment);
  }

  /**
   * @brief Splits a larger segment into a smaller one.
   * 
   * @param seg the segment to split.
   * @param size The size of the segment.
   * @return MemorySegment* 
   */

  MemorySegment* SplitSegment(MemorySegment* seg, const size_t &size) {
    if(seg->size > size + sizeof(MemorySegment)) {
      MemorySegment* tmp = (MemorySegment*)(size + sizeof(MemorySegment));
      tmp->size = seg->size - (size + sizeof(MemorySegment));
      seg->size = size;
      if(seg->next) {
        seg->next->perv = tmp;
        tmp->next = seg->next;
        seg->next = tmp;
        tmp->perv = seg;
      }
      else {
        seg->next = tmp;
        tmp->perv = seg;
      }
        return seg;
    }
    return nullptr;
  }
};

#endif //MEMORY_PAGE_H_INCLUDED
