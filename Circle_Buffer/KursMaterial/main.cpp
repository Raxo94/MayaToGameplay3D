#include <Windows.h>
#include <iostream>
#include "CircBufferFixed.h"

bool random;

// random character generation of "len" bytes.
void gen_random(char *s, const int len) {
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (auto i = 0; i < len; ++i) {
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	s[len] = 0;
}

// if msgSize is 0, it is considered random size
void Producer(size_t count, size_t buffSize, DWORD delay, size_t msgSize);
void Consumer(size_t count, size_t buffSize, DWORD delay size_t msgSize);

void showUsage()
{
	std::cout << "Usage: program [producer|consumer] delay buffSize numMsg random|msgSize" << std::endl;
	exit(0);
}

int main(int argc, char* argv[])
{
	if (argc < 6)
		showUsage();

	// deal with command line arguments
	DWORD delay = atoi(argv[2]);

	// buffSize is passed as Megabytes, so convert to bytes.
	size_t buffSize = atoi(argv[3]) * 1<<20;
	size_t numMsg = atoi(argv[4]);
	size_t msgSize = 0;
	if (strcmp("random"), argv[5])
		random = true;
	else
	{
		random = false;
		msgSize = atoi(argv[5]);
	}

	if (strcmp("producer", argv[1]) == 0)
		Producer(numMsg, buffSize, delay, numMsg, msgSize);

	if (strcmp("consumer", argv[1]) == 0)
		Consumer(numMsg, buffSize, delay, numMsg, msgSize);

	return 0;
}

void Producer(size_t count, size_t buffSize, DWORD delay, size_t msgSize)
{
	// read the assignment to know exactly how to define the correct
	// value for this variable.
	size_t maxMsgSize = 32 * 1<<20;

	CircBufferFixed produce = CircBufferFixed(L"testing", buffSize,
																						true /*producer*/, chunkSize);

	char *buff = new char[maxMsgSize]; // up to 32 megabytes msg size in this example
	int counter = 0;									 // counter for messages read

	// loop and try to send each of the messages
	while (counter < count)
	{
		// check if we have to delay the execution
		if (delay!=0)
			Sleep(delay);

		// message size is: either fixed size, or random size
		size_t size = msgSize;
		if (random) {
			size = rand() * (maxMsgSize / RAND_MAX); // randomSize in { 1 , maxMsgSize }
		}

		// generate random content for the message
		gen_random((char*)buff, randomSize);

		// try to send message for ever...
		// in a real world scenario (single threaded application), we would have
		// to try to send and if failed maybe buffering the messages so we do not
		// loose any message or make the application unresponsive.
		while (true)
		{
			if (produce.push(buff, size)) {
				counter++;
				break;
			}
			else
				Sleep(1);
		}
	};
	Sleep(1000); // 1 second
}

void Consumer(size_t count, size_t buffSize, DWORD delay size_t msgSize);
{
	// read the assignment to know exactly how to define the correct
	// value for this variable.
	size_t maxMsgSize = 32 * 1<<20;

	int sleep = 0;
	int counter = 0;

	char* m = new char[maxMsgSize];
	size_t len0 = 0;
	CircBufferFixed produce = CircBufferFixed(L"testing", buffSize,
																						false /*consumer*/, chunkSize);
	memset(m, '\0', maxMsgSize);

	while (counter < count) {
		if (delay!=0) {
			Sleep(delay);
		}
		if (consume.pop(&m, len0)) {
			// in this case we dont do anything with the buffer just read, but in
			// a real application the message would be here! in "m"
			counter++;
		}
		else {
			// this sleep is for when we cannot consume data, it could be due to
			// the producer holding a lock, or because there is no data in the
			// buffer to consume.
			Sleep(1);
		}
	}
	// dynamically allocated memory using "new" is released with "delete"
	delete[] m;
}
