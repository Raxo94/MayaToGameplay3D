
class CircBufferFixed
{
private:
    // your private stuff,
    // implementation details, etc.
    //
	struct Header
	{
		size_t id;
		size_t length;
		size_t padding;
        // maybe number of consumers here?
	};

public:
    // Constructor
	CircBufferFixed(
            LPCWSTR buffName,          // unique name
            const size_t& buffSize,    // size of the whole filemap
            const bool& isProducer,    // is this buffer going to be used as producer
            const size_t& chunkSize);  // round up messages to multiple of this.

    // Destructor
	~CircBufferFixed();

    // try to send a message through the buffer,
    // if returns true, then it succeeded, otherwise the message has not been sent.
    // it should return false if the buffer does not have enough space.
	bool push(const void* msg, size_t length);
    // try to read a message from the buffer, and the implementation puts the content
    // in the memory. The memory is expected to be allocated by the program that calls
    // this function.
    // The variable length will be changed to reflect the size of the msg just read.
	bool pop(char* msg, size_t& length);
};

