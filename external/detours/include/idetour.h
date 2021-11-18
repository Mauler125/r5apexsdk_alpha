#include <vector>

#define XREGISTER2(x,y)	static size_t dummy_reg_##y = AddDetour( new x() );
#define XREGISTER(x,y)	XREGISTER2(x, y)
#define REGISTER(x)		XREGISTER(x, __LINE__)

class IDetour
{
public:
	virtual ~IDetour() { ; }
	virtual void attach() = 0;
	virtual void detach() = 0;
	virtual void debugp() = 0;
};

namespace
{
	std::vector<IDetour*> vdetour;
	size_t AddDetour(IDetour* idtr)
	{
		vdetour.push_back(idtr);
		return vdetour.size();
	}
}

class H : public IDetour
{
	virtual void attach()
	{
		//
	}
	virtual void detach()
	{
		//
	}
	virtual void debugp()
	{
		//
	}
};
REGISTER(H);
