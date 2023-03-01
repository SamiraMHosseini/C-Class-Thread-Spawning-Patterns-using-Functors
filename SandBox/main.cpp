#include <thread>

using namespace std::chrono;

class Launch : public BannerBase
{
public:

	Launch(const char* const name) :
		BannerBase(name), thread_()
	{

	}

	Launch() = delete;
	Launch(const Launch&) = default;
	Launch& operator=(const Launch&) = default;

	void operator() (system_clock::time_point tToc)
	{
		START_BANNER;
		auto time{ system_clock::now() };
		int count = 0;
		while (time <= tToc)
		{
			Debug::out("%d \n", count++);
			std::this_thread::sleep_for(100ms);
			time = system_clock::now();
		}
	}
	
	//Different ways to spawn a thread in a class
	void SpawnA()
	{
		auto tToc = system_clock::now() + duration<int>(1);
		/*
		In C++, the this pointer refers to the address of the current object instance.
		It's important to note that 'this' is a pointer, so its value will be the memory address of object.
		In C++, the keyword 'this' also refers to a pointer to the current instance of an object.
		When used inside a class method, 'this' points to the memory address of the object on which the method is called.
		*/
		this->thread_ = std::thread(

			[this, tToc]()->void

			{

				(*this).operator()(tToc);
			}

		);
	}
	void SpawnB()
	{

		auto tToc = system_clock::now() + duration<int>(1);
		std::thread th(
			[tToc, this]()->void

			{

				(*this).operator()(tToc);

			}

		);

		this->thread_ = std::move(th);
	}

	void SpawnC()
	{
		auto tToc = system_clock::now() + duration<int>(1);
		thread_ = std::thread([tToc, this]() { (*this)(tToc); });
	}

	void SpawnD()
	{
		auto tToc = system_clock::now() + duration<int>(1);
		/*
		The following line results in an error because the function's parameter is a reference type,
		and the argument being passed needs to be wrapped with the std::ref function to create a reference wrapper.

		*/
		//this->thread_ = std::thread(&Launch::operator(), this, this->tToc); 

		this->thread_ = std::thread(&Launch::operator(), this,tToc);
	}

	~Launch()
	{


		if (this->thread_.joinable())
		{
			this->thread_.join();
		}
		Debug::out("~Launch() \n");
	}

	std::thread thread_;


};

int main()
{
	START_BANNER_MAIN("Main");


	//the follwoing is causing an error
	/*
	Error: 'std::tuple<void (__thiscall Launch::* )(void),Launch>::tuple': no overloaded function takes 2 arguments.
	Since the error is caused by declaring std::thread as a member variable within the class,
	a possible solution is to add an ampersand (&) in front of the Launch object(launch) to create a reference
	*/
	//Launch launch("---A---");
	//std::thread thLaunch(&Launch::operator(), launch);
	//thLaunch.join();
	

	//Solution 1---------------------------------------------------
	/*auto tToc = system_clock::now() + duration<int>(1);
	Launch launch("---A---");
	std::thread thLaunch(&Launch::operator(), &launch, std::ref(tToc));
	thLaunch.join();*/

	//Solution 2---------------------------------------------------

	Launch launchA("---A---");
	launchA.SpawnA();

	Launch launchB("---B---");
	launchB.SpawnB();

	Launch launchC("---C---");
	launchC.SpawnC();

	Launch launchD("---D---");
	launchD.SpawnD();

}


