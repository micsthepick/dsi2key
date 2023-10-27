#include <nds.h>
#include <dsiwifi7.h>

void VBlankHandler(void)
{
	Wifi_Update();
	resyncClock();  // Fixes libnds bug for 3DS (apparently needed for DSI too?)
}

void VCountHandler()
{
	inputGetAndSend();
}

void PowerButtonCallback()
{	
	// This will skip the power-off/sleep mode screen when returning to HOME Menu
	i2cWriteRegister(0x4A, 0x70, 0x01);		// Bootflag = Warmboot/SkipHealthSafety
	i2cWriteRegister(0x4A, 0x11, 0x01);		// Reset to DSi/3DS HOME Menu
}

int main()
{
	readUserSettings();

	irqInit();

	initClockIRQ(); // Start the RTC tracking IRQ
	fifoInit();
	touchInit();

	SetYtrigger(80);

	installWifiFIFO();
	installSoundFIFO();
	installSystemFIFO();

	irqSet(IRQ_VCOUNT, VCountHandler);
	irqSet(IRQ_VBLANK, VBlankHandler);

	irqEnable( IRQ_VBLANK | IRQ_VCOUNT | IRQ_NETWORK);

	setPowerButtonCB(PowerButtonCallback);

	// Keep the ARM7 mostly idle
	while(1)
	{
		swiWaitForVBlank();
	}
	return 0;
}
