#include <pch.h>
#include <Sound.h>
#include <AudioLooper.h>
#include <MasterChannel.h>
#include <PsgDeviceChannel.h>

Sound::Sound()
{
	sound2();
}

void Sound::sound2()
{
	
	looper = new AudioLooper(512);
	looper->Init(looper);
	filter = new BiquadFilterChannel();
	looper->SetChannel(filter);

	MasterChannel* master3 = new MasterChannel();
	_psgDevice = new PsgDeviceChannel();
	_psgDevice->SetMode(PsgDeviceChannel::MODE_SIGNED);
	_psgDevice->SetDevice(PsgDeviceChannel::DEVICE_AY_3_8910);
	master3->AddChannel(_psgDevice);

	if(!looper->IsActive())
	{
		looper->Activate();
	}

	//_psgDevice->WriteRegister(9,10);
	//_psgDevice->WriteRegister(7,244);
	//int a = 400;
	//_psgDevice->WriteRegister(2, a&255);
	//_psgDevice->WriteRegister(3, a>>8);

	filter->SetChannel(master3);

	//looper->Start();
}

void Sound::Start()
{
	looper->Start();
}

void Sound::Write(int address, int value)
{
	_psgDevice->WriteRegister(address,value);
}

void Sound::Stop() {
	looper->Stop();
}

bool Sound::Ready() 
{
	return looper->_ready;
}

void Sound::Test(float f) {
	//GenerateSquare(f*3);
}

void Sound::Test2() {
}

void Sound::Test3(float f) {
}
