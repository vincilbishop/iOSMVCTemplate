/**
 * EAGLView.h - Exit Games Photon "realtime iPhone demo"
 * Copyright (C) 2009 Exit Games GmbH
 * http://www.exitgames.com
 *
 * @author developer@exitgames.com
 */

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#import "Texture2D.h"
#import "player.h"


@class Game;

/*
This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
The view content is basically an EAGL surface you render your OpenGL scene into.
Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
*/
@interface EAGLView : UIView
{
    
@private
    /* The pixel dimensions of the backbuffer */
    GLint backingWidth;
    GLint backingHeight;

    EAGLContext *context;

    /* OpenGL names for the renderbuffer and framebuffers used to render to this view */
    GLuint viewRenderbuffer, viewFramebuffer;

    /* OpenGL name for the depth buffer that is attached to viewFramebuffer, if it exists (0 if it does not exist) */
    GLuint depthRenderbuffer;

    NSTimer* animationTimer;
    NSTimeInterval animationInterval;

	// game logic info, needed for drawing and updated from game code class
	Player** player;
	Game* mGame;

	Texture2D* textureArrowLeft;
	Texture2D* textureArrowUp;
	Texture2D* textureArrowRight;
	Texture2D* textureArrowDown;
}

@property NSTimeInterval AnimationInterval;

// draw methods
- (void) startAnimation;
- (void) setPlayerArray:(Game*)game :(Player*[MAXPLAYERS])p;
- (void) stopAnimation;
- (void) drawView;
- (void) showText: (NSString*)text :(float)x :(float)y :(float)width :(float)height :(unsigned int)textureWidth :(unsigned int)textureHeight :(float)fontSize :(int)color;
- (void) activateImageTextureDraw;
- (void) deactivateImageTextureDraw;
- (void) activateColoredVerticesDraw;
- (void) deactivateColoredVerticesDraw;
- (void) activateStringTextureDraw;
- (void) deactivateStringTextureDraw;

// drawing helper methods
- (unsigned char) cut2uByte:(long long)number;

@end