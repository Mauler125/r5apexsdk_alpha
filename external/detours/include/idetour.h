#define XREGISTER2(x,y)	static size_t dummy_reg_##y = addTest( new x() );
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
	std::int64_t ppadding = 0;

	size_t addTest(IDetour* id)
	{
		vdetour.push_back(id);
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
