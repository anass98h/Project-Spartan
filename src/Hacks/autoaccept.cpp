#include "autoaccept.h"

bool Settings::AutoAccept::enabled = false;

struct CServerConfirmedReservationCheckCallback
{
	char pad[0x2200];
};

void AutoAccept::PlaySound(const char* filename)
{
	if (!Settings::AutoAccept::enabled)
		return;

	if (engine->IsInGame())
		return;

	if (strcmp(filename, XORSTR("!UI/competitive_accept_beep.wav")) != 0) // https://github.com/LWSS/Fuzion/commit/cf6745c61143cbae2bd3ffe18fd3e06b87b7caa2
		return;

	CServerConfirmedReservationCheckCallback empty_callback;
	IsReadyCallback(&empty_callback);
}
