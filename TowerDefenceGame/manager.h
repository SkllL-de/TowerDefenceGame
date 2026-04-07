#ifndef _MANAGER_H_
#define _MANAGER_H_

template<typename T>
class Manager
{
public:
	static T* instance()
	{
		static T instance;
		return &instance;
	}

	

protected:
	Manager() = default;
	~Manager() = default;
	Manager(const Manager&) = delete;
	Manager& operator=(const Manager&) = delete;
};



#endif // !_MANAGER_H_

