
#ifndef c33device_h
#define c33device_h

class c33device
{
private:

public:
	c33device() { }
	virtual ~c33device() { }

	virtual void init() { }
	virtual void work() { }
	virtual c33word read(c33word offset, c33word size) = 0;
	virtual void write(c33word offset, c33word size, c33word data) = 0;
	virtual void exit() { }
};

#endif // !c33device_h
