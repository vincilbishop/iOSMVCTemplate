#import "OutputListener.h"
#import "LoadBalancing-objc/inc/EGLoadBalancingClient.h"

typedef enum _State
{
	STATE_INITIALIZED = 0,
	STATE_CONNECTING,
	STATE_CONNECTED,
	STATE_JOINING,
	STATE_JOINED,
	STATE_LEAVING,
	STATE_LEFT,
	STATE_DISCONNECTING,
	STATE_DISCONNECTED
} State;



typedef enum _Input
{
	INPUT_NON = 0,
	INPUT_CREATE_GAME,
	INPUT_JOIN_RANDOM_GAME,
	INPUT_LEAVE_GAME,
	INPUT_EXIT
} Input;



@protocol NetworkLogicListener

- (void) stateUpdate:(State)newState;

@end



@interface StateAccessor : NSObject
{
@private
	State mState;
	EGMutableArray* mStateUpdateListeners;
}

@property (readwrite) State State;

- (void) registerForStateUpdates:(id<NetworkLogicListener>)listener;

@end



@interface NetworkLogic : NSObject <EGLoadBalancingListener>
{
@private
	EGLoadBalancingClient* mLoadBalancingClient;
	EGLogger* mLogger;

	StateAccessor* mStateAccessor;
	Input mLastInput;
	id<OutputListener> mOutputListener;
}

@property (readwrite) Input LastInput;
@property (readonly) State State;

+ (NetworkLogic*) networkLogicWithOutputListener:(id<OutputListener>)listener :(NSString*)appVersion;
- (NetworkLogic*) initWithOutputListener:(id<OutputListener>)listener :(NSString*)appVersion;
- (void) registerForStateUpdates:(id<NetworkLogicListener>)listener;
- (void) run;
- (void) connect;
- (void) opCreateRoom;
- (void) opJoinRandomRoom;
- (void) disconnect;
- (void) sendEvent;

@end