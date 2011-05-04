/*
 Copyright (C) 2011 by Ivan Safrin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/

#include "PolySoundManager.h"

using namespace Polycode;

SoundManager::SoundManager() {
	initAL();
}

void SoundManager::initAL() {
	alGetError();
	if(alcGetCurrentContext() == NULL) {
		Logger::log("AL already initialized\n");
	}
	
	device = alcOpenDevice(NULL);
	if(device == NULL) {
		Logger::log("InitializeAL: Cannot open preferred device\n");
		return;
	}
	
	if (alcGetError(device) != ALC_NO_ERROR) {
		alcCloseDevice(device);
	//	PCCE_THROW("InitializeAL: Could not open device (alc error)");
	}
	
	context = alcCreateContext(device, NULL);
	if (context == NULL) {
		alcCloseDevice(device);
	//	PCCE_THROW("InitializeAL: Could not create context");
	}
	if (alcGetError(device) != ALC_NO_ERROR) {
		alcDestroyContext(context);
		alcCloseDevice(device);
	//	PCCE_THROW("InitializeAL: Could not open device (alc error)");
	}
	
	if (alcMakeContextCurrent(context) != ALC_TRUE) {
		alcDestroyContext(context);
		alcCloseDevice(device);
	//	PCCE_THROW("InitializeAL: Could not make context current");
	}
	if (alcGetError(device) != ALC_NO_ERROR) {
		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
		alcCloseDevice(device);
	//	PCCE_THROW("InitializeAL: Could not make context current (alc error)");
	}
	
	alGetError();
	
	ALfloat listenerPos[] = { 0.0, 0.0, 0.0 };
	ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };	
	ALfloat listenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };
	
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	if (alGetError() != AL_NO_ERROR) {
//		ShutdownAL();
//		PCCE_THROW("InitializeAL: Could not set listener position");
	}
	Logger::log("OpenAL initialized...\n");
}

SoundManager::~SoundManager() {
	alcSuspendContext(context);
	alcDestroyContext(context);
	if (device != NULL) {
		alcCloseDevice(device);
	}
}