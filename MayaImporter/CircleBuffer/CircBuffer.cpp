#include "CircBuffer.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <iostream>
#include <chrono>

CircBufferFixed::CircBufferFixed(LPCWSTR buffName, const bool & isProducer, const size_t & buffSize, const size_t & chunkSize)
{
	this->buffName = buffName;
	this->buffSize = buffSize;
	this->isProducer = isProducer;
	this->chunkSize = chunkSize;
	this->ClientPosition = 0;
	this->MessageCount = 0;
	
	createMaping();
	UpdateControlBuffer();
	ControlPointer[CLIENTCOUNT] = 1; //temporary set ClientCount to 1; as destructors are unreliable.
}

CircBufferFixed::~CircBufferFixed()
{
	if (isProducer==false)
		ControlPointer[CLIENTCOUNT] -= 1;

	UnmapViewOfFile(MapPointer);
	CloseHandle(MapingFile);

	UnmapViewOfFile(ControlPointer);
	CloseHandle(ControlFile);

}




bool CircBufferFixed::createMaping()
{
	//Message map
	this->MapingFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, (DWORD)buffSize, TEXT("FileMappingObject"));

	if (this->MapingFile == NULL)
	{
		_tprintf(TEXT("Could not create file mapping object (%d).\n"), GetLastError());
		return false;
	}

	this->MapPointer = (char*)MapViewOfFile(this->MapingFile, FILE_MAP_ALL_ACCESS, 0, 0, buffSize);

	if (MapPointer == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"), GetLastError());
		CloseHandle(this->MapingFile);
		return false;
	}

	//Controll Map;
	
	this->ControlFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Control), TEXT("ControlMappingObject"));

	if (this->ControlFile == NULL)
	{
		_tprintf(TEXT("Could not create file Control mapping object (%d).\n"), GetLastError());
		return false;
	}

	this->ControlPointer = (size_t*)MapViewOfFile(this->ControlFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(Control));

	if (ControlPointer == NULL)
	{
		_tprintf(TEXT("Could not map view of Control file (%d).\n"), GetLastError());
		CloseHandle(this->MapingFile);
		return false;
	}

	return true; //DONE
}

void CircBufferFixed::UpdateControlBuffer()
{
	
	if (isProducer)
	{
		ControlPointer[HEAD] = 0;
	}
	else
	{
		ControlPointer[HEAD] = 0;
		ControlPointer[TAIL] = 0;
		ControlPointer[CLIENTCOUNT] += 1;
	}
}

size_t CircBufferFixed::CalculateFreeMemory()
{
	size_t freeMemory;
	if (ControlPointer[HEAD] >= ControlPointer[TAIL])
	{
		return freeMemory = (buffSize - ControlPointer[HEAD]) + ControlPointer[TAIL];
	}

	else if (ControlPointer[HEAD] < ControlPointer[TAIL])
	{
		return freeMemory = ControlPointer[TAIL] - ControlPointer[HEAD];
	}
}




bool CircBufferFixed::push(const char * message, size_t length)
{


	ClientPosition = ControlPointer[HEAD];
	size_t padding = chunkSize - ((ClientPosition + sizeof(Header) + length) % chunkSize); //padding my way through the darkness.
	

	if (CalculateFreeMemory() > (sizeof(Header) + length + padding)) // Header mustn't reach the tail but the tail should be able to reach the head
	{
		if (ClientPosition == buffSize) 
			ClientPosition = 0;

		MessageCount += 1;
		Header messageHeader;
		messageHeader.id = MessageCount;
		messageHeader.length = length;
		messageHeader.ClientRemaining = ControlPointer[CLIENTCOUNT];
		messageHeader.padding = padding;

		memcpy(&MapPointer[ClientPosition], &messageHeader, sizeof(Header)); //WRITE HEADER 
		ClientPosition += sizeof(Header);

		if (buffSize - ClientPosition >= length + padding)
		{
			memcpy(&MapPointer[ClientPosition], message, length);
			ClientPosition += length;
			ClientPosition += messageHeader.padding;
		}
		
		else
		{
			size_t Rest = buffSize - ClientPosition;
			memcpy(&MapPointer[ClientPosition], message, Rest);

			ClientPosition = 0;
			size_t Remaining = length - Rest;
			memcpy(&MapPointer[ClientPosition], message + Rest, Remaining);
			ClientPosition = Remaining;
		}
		
		ControlPointer[HEAD] = ClientPosition;
		
		
		//printf("%d  ", MessageCount);
		//cout << (char*)message << endl;
		
		return true;
	}


	else //NO MEMORY
		return false;
}


bool CircBufferFixed::pop(char * message)
{
	ClientPosition = ControlPointer[TAIL];

	if (this->ClientPosition == ControlPointer[HEAD]) //CONSUMER HAS READ ALL MESSAGES
		return false;

	mutex.lock();
	if (ClientPosition == buffSize) ClientPosition = 0;
	Header* messageHeader= (Header*)&MapPointer[this->ClientPosition]; //read header
	ClientPosition += sizeof(Header); //move pointer forward;


	if (buffSize - ClientPosition >= (messageHeader->length + messageHeader->padding))
	{
		memcpy(message, &MapPointer[this->ClientPosition], messageHeader->length);
		ClientPosition += messageHeader->length;
		ClientPosition += messageHeader->padding;
	}

	else 
	{
		size_t Rest = buffSize - ClientPosition;
		memcpy(message,&MapPointer[ClientPosition], Rest);

		ClientPosition = 0;
		size_t Remaining = messageHeader->length - Rest;
		memcpy(message + Rest,&MapPointer[ClientPosition], Remaining);
		ClientPosition = Remaining;
	}


	messageHeader->ClientRemaining -= 1;

	if (messageHeader->ClientRemaining == 0)
		ControlPointer[TAIL] = ClientPosition;

	mutex.unlock(); //Allow other clients to read
	return true;
	
}






