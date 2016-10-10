
#include <iostream>
#include <windows.h>
using namespace std;

inline void showUsage() //called if program is being used incorrect
{
	std::cout << "Usage: program [producer|consumer] delay buffSize numMsg random|msgSize" << std::endl;
	//getchar();
	exit(0);
}


inline void gen_randomString(char * s, const size_t len)
{
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (auto i = 0; i < len; ++i) {
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	s[len - 1] = 0;
}

inline size_t randomSize(size_t min, size_t max)
{
	if (max - min != 0)
	{
		size_t result = rand() % ((max + 1) - min);
		result += min;
		return result;
	}
	else return 1;

}

inline bool GetIsProducer(char* argv)
{
	bool result;

	if (strcmp("producer", argv) == 0)
	{
		SetConsoleTitle(L"Producer"); result = true;
	}
	else if (strcmp("consumer", argv) == 0)
	{
		SetConsoleTitle(L"Consumer");
		result = false;
	}
	else { showUsage(); }

	return result;
}

inline bool GetIsRandom(char* argv)
{
	if (strcmp("random", argv) == 0)
	{
		return true;
	}
	
	return false;
}