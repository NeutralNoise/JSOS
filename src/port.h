#ifndef __PORT_H_INCLUDED
#define __PORT_H_INCLUDED

#include "types.h"


class Port {
protected:
  Port(uint16_t port);
  ~Port();
  uint16_t m_portnum;

};

class Port8Bit : public Port {
public:
  Port8Bit(uint16_t port);
  ~Port8Bit();
  virtual void Write(uint8_t data);
  virtual uint8_t Read();
};

class Port8BitSlow : public Port8Bit {
public:
  Port8BitSlow(uint16_t port);
  ~Port8BitSlow();
  virtual void Write(uint8_t data);
};

class Port16Bit : public Port {
public:
  Port16Bit(uint16_t port);
  ~Port16Bit();
  virtual void Write(uint16_t data);
  virtual uint16_t Read();
};

class Port32Bit : public Port {
public:
  Port32Bit(uint16_t port);
  ~Port32Bit();
  virtual void Write(uint32_t data);
  virtual uint32_t Read();
};

#endif //__PORT_H_INCLUDED
