#ifndef MEMORY_SEGMENT_H_INCLUDED
#define MEMORY_SEGMENT_H_INCLUDED

struct MemoryPage;

struct MemorySegment {

  MemorySegment * next = nullptr;
  MemorySegment * perv = nullptr;
  MemoryPage * page; //!< The page this memory segment belongs too.
  unsigned int size; //!< The size of this memory segment.
  bool allocated = false; //!< Is this memory segment in use.

};

#endif //MEMORY_SEGMENT_H_INCLUDED
