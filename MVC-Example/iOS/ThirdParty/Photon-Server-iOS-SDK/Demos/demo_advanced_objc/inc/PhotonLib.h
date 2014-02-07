#import "demo_photon_CustomType_objc.h"
#import "UIListener.h"
#import "Photon-objc/inc/EGLitePeer.h"

@class EGLitePeer;

typedef enum
{
	SampleStarted = 0,
	PhotonPeerCreated,
	Connecting,
	Connected,
	EstablishingEncryption,
	ErrorEstablishingEncryption,
	EncryptionEstablished,
	Joining,
	ErrorJoining,
	Joined,
	Leaving,
	ErrorLeaving,
	Left,
	ErrorConnecting,
	Receiving,
	Disconnecting,
	Disconnected
} States;

@interface CPhotonLib : NSObject
{
	States currentState;
	EGLitePeer* mpLitePeer;
	id<UIListener> textView;
	SampleCustomType* mpCustomType;
}

@property (readwrite) States State;

- (void) InitCPhotonLib:(id<UIListener>) textView;
- (int) InitLib:(id<EGPhotonListener>) listener;
- (int) Run;
- (int) CreateConnection;
- (int) CloseConnection;
- (short) Join:(NSString*)gameId;
- (short) Leave;
- (void) sendData;
- (void) Stop;

@end