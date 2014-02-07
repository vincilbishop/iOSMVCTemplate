#include "AdvancedApplication.h"
#include "Photon_lib.h"
#include "UIListener.h"

UIListenerPtr gUIListener;

void AdvancedApplication::run(UIListener* listener)
{
	gUIListener.reset(listener);

	CPhotonLib photonLib;
	photonLib.run();
	gUIListener->onLibClosed();
	gUIListener.reset();
}