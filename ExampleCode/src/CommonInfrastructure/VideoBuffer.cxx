/* --------------------------------------------
Copyright (c) 2013, University of Granada
Copyright (c) 2013, Real-Time Innovations, Inc.
Copyright (c) 2013, Javier Povedano-Molina
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notices, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notices,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the copyright holders or contributors.

This work was partially completed at the University of Granada under funding by
Real-Time Innovations, Inc. (RTI).  The above license is granted with
 the permission of RTI.

-------------------------------------------- */
#include <iostream>
#include <sstream>
#include "VideoBuffer.h"

#ifdef WIN32
#include <string>
#else
#include <string.h>
#endif



// ----------------------------------------------------------------------------
// Initialize the video buffer
void EMDSBuffer::Initialize()
{
	data = NULL;
	size = 0;
	seqn = 0;
	timestamp = 0.0;
}


// ----------------------------------------------------------------------------
// Constructor
// Construct and initialize a video buffer with an empty buffer size
EMDSBuffer::EMDSBuffer()
{
	std::cout << "*** ! Empty video buffer (no size) is ready " << std::endl;
	Initialize();
}

// ----------------------------------------------------------------------------
// Constructor
// Construct and initialize a video buffer with a buffer size of allocationSize
EMDSBuffer::EMDSBuffer(int allocationSize)
{

	Initialize();
	data = new unsigned char[allocationSize];

	if (data != NULL)
	{
		memset(data, 0, allocationSize);
		size = allocationSize;
		std::cout << "*** Will be initializing video buffer with the size allocation " << size << std::endl;
	}
}

// ----------------------------------------------------------------------------
// Copy Constructor
EMDSBuffer::EMDSBuffer(EMDSBuffer &copyFrom)
{
	if (data != NULL)
	{
		data = new unsigned char[copyFrom.size];
		std::cout << "***copying data to buffer  " << data <<std::endl;
	}

	if (data == NULL)
	{
		std::cout << "***NO data to copy to buffer " << std::endl;
	}

	memcpy(data, copyFrom.data, copyFrom.size);

	seqn = copyFrom.seqn;
	flags = copyFrom.flags;
	timestamp = copyFrom.timestamp;
}

// ----------------------------------------------------------------------------
// Destructor
EMDSBuffer::~EMDSBuffer()
{
	if (data != NULL)
	{
		delete[] data;
	}
}

// ----------------------------------------------------------------------------
// Getter
unsigned char *EMDSBuffer::GetData()
{
	return data;
}

// ----------------------------------------------------------------------------
// Setter
void EMDSBuffer::SetData(unsigned char *copyFrom, unsigned int sizeIn)
{
	if (sizeIn > size)
	{
		std::stringstream errss;
		errss << "Trying to copy too-large data into buffer";
		throw errss.str();
	}
	memcpy(data, copyFrom, sizeIn);
}

// ----------------------------------------------------------------------------
// Getter
int EMDSBuffer::GetSize()
{
	//std::cout << "*** ! EMDSBuffer contains sized video buffer like Metadata --getting size" << std::endl;
	return size;
}

// ----------------------------------------------------------------------------
// Setter
void EMDSBuffer::SetSeqn(unsigned int seqNumIn)
{
	seqn = seqNumIn;
	std::cout << "***setter set sequence" << seqn << std::endl;
}

// ----------------------------------------------------------------------------
// Getter
unsigned int EMDSBuffer::GetSeqn()
{
	//std::cout << "***getter get sequence" << std::endl;
	return seqn;
}

// ----------------------------------------------------------------------------
// Getter
EMDSBufferFlags EMDSBuffer::GetFlags()
{
	//std::cout << "***getter get flags" << std::endl;
	return flags;
}

// ----------------------------------------------------------------------------
// Setter
void EMDSBuffer::SetFlags(EMDSBufferFlags flagsIn)
{
	flags = flagsIn;
	std::cout << "***setter set flag  " << flags << std::endl;
}

// ----------------------------------------------------------------------------
// Setter
void EMDSBuffer::SetTimestamp(EMDSTimestamp timestampIn)
{
	timestamp = timestampIn;
	std::cout << "***setter set timestamp" << timestamp << std::endl;
}

// ----------------------------------------------------------------------------
// Getter
EMDSTimestamp EMDSBuffer::GetTimestamp()
{
	//std::cout << "***getter timestamp" << std::endl;
	return timestamp;
}

// ----------------------------------------------------------------------------
// Constructor
EMDSBufferQueue::EMDSBufferQueue()
{
	std::cout<< "*** ! EMDS buffer queue initialised" << std::endl;
	Initialize();
}

// ----------------------------------------------------------------------------
// Initialization
void EMDSBufferQueue::Initialize()
{
	// std::cout<< "***EMDS buffer queue list_init" << std::endl;
	list_init(&_fifo);
}

// ----------------------------------------------------------------------------
// Is the Buffer Queue empty?
bool EMDSBufferQueue::IsEmpty()
{
	std::cout<< "***buffer queue is empty? " << std::endl;
	return (bool)list_empty(&_fifo);
}

// ----------------------------------------------------------------------------
// Enqueue a buffer
void EMDSBufferQueue::Queue(EMDSBuffer *data)
{
	std::cout<< "***enqueue " << std::endl;
	list_append(&_fifo,data);
}

// ----------------------------------------------------------------------------
// Dequeue a buffer
EMDSBuffer *EMDSBufferQueue::Dequeue()
{
	std::cout<< "***dequeue " << std::endl;

	return (EMDSBuffer *)list_fetch(&_fifo);
}
