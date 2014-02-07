#import "PhotonLib.h"
#import "UIListener.h"
#import "Photon-objc/inc/EGLitePeer.h"

@interface Listener : NSObject <EGPhotonListener>
{
@private
	CPhotonLib* current;
	id<UIListener> textView;
}

- (void) InitListener:(CPhotonLib*)lib :(id<UIListener>)txtView;

@end