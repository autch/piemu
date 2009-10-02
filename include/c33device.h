
#ifndef c33device_h
#define c33device_h

#include "types.h"

class c33device
{
private:
	c33word base_addr;

public:
  virtual ~c33device() { ]

	virtual void init() = 0;

	virtual c33word read(c33word offset, c33word size) = 0;
	virtual void write(c33word offset, c33word size, c33word data) = 0;

	virtual void work() = 0;

	// should return area id used in bcu
	virtual c33word area() = 0;
	c33word addr() const { return base_addr; }
};

#endif // !c33device_h
