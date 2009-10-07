
#ifndef c33iomem_h
#define c33iomem_h

#include "c33device.h"

#define IOMEM_SIZE 0x10000		/// sizeof io registers, 0x0040000-0x004ffff
#define IOMAP_SIZE 10			/// # of mmapped devices

struct iomap
{
  c33word base_addr;
  c33device* device;
};

class c33iomem: public c33device
{
private:
	c33byte mem[IOMEM_SIZE];
  iomap iomap[IOMAP_SIZE];
public:

  c33word iomem_read(c33word offset, c33word size)
  {
    
  }

  void iomem_write(c33word offset, c33word size, c33word data)
  {
  }
};

#endif // !c33iomem_h
