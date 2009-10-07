
#ifndef c33bcu_h
#define c33bcu_h

class c33bcu
{
private:
	struct AREA
	{
		c33int	area;		/// c33 bcu area number
		c33word	addr;		/// base address of device
		c33device* device;
	};

	AREA area[18];
public:

};

#endif // !c33bcu_h
