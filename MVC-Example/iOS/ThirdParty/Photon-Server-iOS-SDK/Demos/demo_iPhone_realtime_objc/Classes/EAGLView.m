/**
 * EAGLView.m - Exit Games Photon "realtime iPhone demo"
 * Copyright (C) 2009 Exit Games GmbH
 * http://www.exitgames.com
 *
 * @author developer@exitgames.com
 */

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

#import "EAGLView.h"
#import "game.h"

#define USE_DEPTH_BUFFER 0

extern const char POS_MAX;

// A class extension to declare private methods
@interface EAGLView ()

@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) NSTimer *animationTimer;

- (bool) createFramebuffer;
- (void) destroyFramebuffer;

@end


@implementation EAGLView

@synthesize context;
@synthesize animationTimer;


// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}


//The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (instancetype) initWithCoder:(NSCoder*)coder
{
    if ((self = [super initWithCoder:coder]))
	{
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer*)self.layer;

        eaglLayer.opaque = true;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:false], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];

        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        
        if (!context || ![EAGLContext setCurrentContext:context])
		{
            [self release];
            return nil;
        }
        animationInterval = 1.0 / 60.0;
		textureArrowLeft = [[Texture2D alloc] initWithImage:[UIImage imageNamed:@"arrowLeft.png"]];
		textureArrowUp = [[Texture2D alloc] initWithImage:[UIImage imageNamed:@"arrowUp.png"]];
		textureArrowRight = [[Texture2D alloc] initWithImage:[UIImage imageNamed:@"arrowRight.png"]];
		textureArrowDown = [[Texture2D alloc] initWithImage:[UIImage imageNamed:@"arrowDown.png"]];
    }

    return self;
}


- (void)drawView
{
	const int DIMENSION = 2, VERTICES_PER_ELEMENT = 4, WIDTH = POS_MAX+1, HEIGHT = POS_MAX+1, COLOR_DEPTH_BYTES = 4;
	float boardVertices[DIMENSION*VERTICES_PER_ELEMENT*WIDTH*HEIGHT];
	unsigned char boardColors[COLOR_DEPTH_BYTES*VERTICES_PER_ELEMENT*WIDTH*HEIGHT];
	float playerVertices[MAXPLAYERS][8];
	unsigned char playerColors[MAXPLAYERS][16];

	for(int i = 0; i<mGame.CurrentPlayers; i++)
	{
		if(player[i].Number != -1)
		{
			float fPosX = 2.0f*((float)player[i].PosX)/((float)WIDTH)-1.0f+1.0f/WIDTH, fPosY = 2.0f*((float)(POS_MAX-player[i].PosY))/((float)HEIGHT)-1.0f+1.0f/HEIGHT;
			playerVertices[i][0] = -0.8f/WIDTH+fPosX; playerVertices[i][1] = -0.8f/HEIGHT+fPosY;
			playerVertices[i][2] =  0.8f/WIDTH+fPosX; playerVertices[i][3] = -0.8f/HEIGHT+fPosY;
			playerVertices[i][4] = -0.8f/WIDTH+fPosX; playerVertices[i][5] =  0.8f/HEIGHT+fPosY;
			playerVertices[i][6] =  0.8f/WIDTH+fPosX; playerVertices[i][7] =  0.8f/HEIGHT+fPosY;

			unsigned char /*a = (player[i].Color & 0xFF000000)/256/256/256,*/ r =  (player[i].Color & 0x00FF0000)/256/256, g =  (player[i].Color & 0x0000FF00)/256, b =  player[i].Color & 0x000000FF;
			playerColors[i][ 0] = [self cut2uByte:  0+r]; playerColors[i][ 1] = [self cut2uByte:  0+g]; playerColors[i][ 2] = [self cut2uByte:  0+b]; playerColors[i][ 3] = [self cut2uByte:  0];
			playerColors[i][ 4] = [self cut2uByte:127+r]; playerColors[i][ 5] = [self cut2uByte:127+g]; playerColors[i][ 6] = [self cut2uByte:127+b]; playerColors[i][ 7] = [self cut2uByte:  0];
			playerColors[i][ 8] = [self cut2uByte:127+r]; playerColors[i][ 9] = [self cut2uByte:127+g]; playerColors[i][10] = [self cut2uByte:127+b]; playerColors[i][11] = [self cut2uByte:  0];
			playerColors[i][12] = [self cut2uByte:255+r]; playerColors[i][13] = [self cut2uByte:255+g]; playerColors[i][14] = [self cut2uByte:255+b]; playerColors[i][15] = [self cut2uByte:  0];
		}
	}

	for(int j=0; j<HEIGHT; j++)
	{
		for(int i=0; i<WIDTH; i++)
		{
			boardVertices[(j*WIDTH+i)*VERTICES_PER_ELEMENT*DIMENSION+0] = 2.0f*((float)i)/((float)WIDTH)-1.0f;
			boardVertices[(j*WIDTH+i)*VERTICES_PER_ELEMENT*DIMENSION+1] = 2.0f*((float)j)/((float)HEIGHT)-1.0f;

			boardVertices[(j*WIDTH+i)*VERTICES_PER_ELEMENT*DIMENSION+2] = 2.0f*(((float)i)+1.0f)/((float)WIDTH)-1.0f;
			boardVertices[(j*WIDTH+i)*VERTICES_PER_ELEMENT*DIMENSION+3] = 2.0f*((float)j)/((float)HEIGHT)-1.0f;

			boardVertices[(j*WIDTH+i)*VERTICES_PER_ELEMENT*DIMENSION+4] = 2.0f*((float)i)/((float)WIDTH)-1.0f;
			boardVertices[(j*WIDTH+i)*VERTICES_PER_ELEMENT*DIMENSION+5] = 2.0f*(((float)j)+1.0f)/((float)HEIGHT)-1.0f;

			boardVertices[(j*WIDTH+i)*VERTICES_PER_ELEMENT*DIMENSION+6] = 2.0f*(((float)i)+1.0f)/((float)WIDTH)-1.0f;
			boardVertices[(j*WIDTH+i)*VERTICES_PER_ELEMENT*DIMENSION+7] = 2.0f*(((float)j)+1.0f)/((float)HEIGHT)-1.0f;			
		}
	}
	for(int k=0; k<HEIGHT; k++)
		for(int j=0; j<WIDTH; j++)
			for(int i=0; i<COLOR_DEPTH_BYTES*VERTICES_PER_ELEMENT; i++)
				boardColors[(k*WIDTH+j)*COLOR_DEPTH_BYTES*VERTICES_PER_ELEMENT+i] = ((!(WIDTH%2) && ((k*WIDTH+j)/WIDTH)%2)?(k*WIDTH+j)%2:!((k*WIDTH+j)%2))?((j+1<=WIDTH*0.25||j+1>WIDTH*0.75||k+1<=HEIGHT*0.25||k+1>HEIGHT*0.75)?0:30):((j+1<=WIDTH*0.25||j+1>WIDTH*0.75||k+1<=HEIGHT*0.25||k+1>HEIGHT*0.75)?40:70);
    
    [EAGLContext setCurrentContext:context];
    
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
    glViewport(0, 0, backingWidth, backingHeight);

	// project board to a square of the screen instead of whole screen
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(-1.0f, 1.0f, -1.5f, 1.5f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


	
	[self activateColoredVerticesDraw];
	glVertexPointer(2, GL_FLOAT, 0, boardVertices);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, boardColors);
	for(int j=0; j<HEIGHT; j++)
		for(int i=0; i<WIDTH; i++)
    		glDrawArrays(GL_TRIANGLE_STRIP, (j*WIDTH+i)*VERTICES_PER_ELEMENT, VERTICES_PER_ELEMENT);
	[self deactivateColoredVerticesDraw];



	[self activateImageTextureDraw];	
	[textureArrowLeft  drawInRect:CGRectMake(-1, -0.25, 0.5, 0.5)];
	[textureArrowUp    drawInRect:CGRectMake(-0.25, 0.5, 0.5, 0.5)];
	[textureArrowRight drawInRect:CGRectMake(0.5, -0.25, 0.5, 0.5)];
	[textureArrowDown  drawInRect:CGRectMake(-0.25, -1, 0.5, 0.5)];
	[self deactivateImageTextureDraw];



	[self activateColoredVerticesDraw];
	for(int i=0; i<mGame.CurrentPlayers; i++)
	{
		if(player[i].Number != -1)
		{
			glVertexPointer(2, GL_FLOAT, 0, playerVertices[i]);
			glColorPointer(4, GL_UNSIGNED_BYTE, 0, playerColors[i]);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, VERTICES_PER_ELEMENT);
		}
	}
	[self deactivateColoredVerticesDraw];



	for(int i = 0; i<mGame.CurrentPlayers; i++)
		if(player[i].Number != -1)
			[self showText:player[i].Name :2.0f*((float)player[i].PosX)/((float)WIDTH)-1.0f+1.0f/WIDTH+0.1f :2.0f*((float)(POS_MAX-player[i].PosY))/((float)HEIGHT)-1.0f+1.0f/HEIGHT+0.1f :1 :0.08 :256 :32 :24: player[i].Color];

	// back to whole screen projection (a bit less at top because carrier, clock, energy symbols overlay the top rows of pixels)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(-1.0f, 1.0f, -1.0f, 1.125f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);



	/*[self activateImageTextureDraw];
	[textureArrowLeft  drawInRect:CGRectMake(-0.25, -0.9, 0.5, 0.2)];
	[self deactivateImageTextureDraw];
	[self showText:@"Menu" :-0.05 :-1 :0.5 :0.25 :128 :64 :16 :0xFF0000FF];*/
	
	[self showText:[NSString stringWithFormat:@"events sent per second: %i", [mGame SPS]] :-1 :1 :2 :0.04 :512 :16 :16 :0xFFFFFFFF];
	[self showText:[NSString stringWithFormat:@"events received per second: %i", [mGame RPS]] :-1 :0.96 :2 :0.04 :512 :16 :16 :0xFFFFFFFF];
	[self showText:[NSString stringWithFormat:@"roundTripTime in ms: %i", [mGame RoundTripTime]] :-1 :0.92 :2 :0.04 :512 :16 :16 :0xFFFFFFFF];
	[self showText:[NSString stringWithFormat:@"roundTripTimeVaricance in ms: %i", [mGame RoundTripTimeVariance]] :-1 :0.88 :2 :0.04 :512 :16 :16 :0xFFFFFFFF];
	[self showText:[NSString stringWithFormat:@"serverTime in ms: %d", ((int)(long long)round([NSDate timeIntervalSinceReferenceDate]*1000.0))+[mGame ServerTimeOffset]] :-1 :0.84 :2 :0.04 :512 :16 :16 :0xFFFFFFFF];
	

    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];



    // Replace the implementation of this method to do your own custom drawing
    
/*    const GLfloat squareVertices[] = {
        -0.5f, -0.5f,
        0.5f,  -0.5f,
        -0.5f,  0.5f,
        0.5f,   0.5f,
    };

    const GLubyte squareColors[] = {
        255, 255,   0, 255,
        0,   255, 255, 255,
        0,     0,   0,   0,
        255,   0, 255, 255,
    };
    
    [EAGLContext setCurrentContext:context];
    
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
    glViewport(0, 0, backingWidth, backingHeight);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(-1.0f, 1.0f, -1.5f, 1.5f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glRotatef(3.0f, 0.0f, 0.0f, 1.0f);
    
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glVertexPointer(2, GL_FLOAT, 0, squareVertices);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, squareColors);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];*/
}

- (void)layoutSubviews
{
    [EAGLContext setCurrentContext:context];
    [self destroyFramebuffer];
    [self createFramebuffer];
    [self drawView];
}

- (bool)createFramebuffer
{
    glGenFramebuffersOES(1, &viewFramebuffer);
    glGenRenderbuffersOES(1, &viewRenderbuffer);
    
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
    
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
    
    if (USE_DEPTH_BUFFER) {
        glGenRenderbuffersOES(1, &depthRenderbuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
        glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
    }
    
    if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
	{
        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
        return false;
    }
    
    return true;
}

- (void)destroyFramebuffer
{
    
    glDeleteFramebuffersOES(1, &viewFramebuffer);
    viewFramebuffer = 0;
    glDeleteRenderbuffersOES(1, &viewRenderbuffer);
    viewRenderbuffer = 0;
    
    if(depthRenderbuffer)
	{
        glDeleteRenderbuffersOES(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
}

- (void)startAnimation
{
    self.animationTimer = [NSTimer scheduledTimerWithTimeInterval:animationInterval target:self selector:@selector(drawView) userInfo:nil repeats:true];
}

- (void)stopAnimation
{
    self.animationTimer = nil;
}

- (void)setAnimationTimer:(NSTimer *)newTimer
{
    [animationTimer invalidate];
    animationTimer = newTimer;
}

- (NSTimeInterval) AnimationInterval
{
	return  animationInterval;
}

- (void)setAnimationInterval:(NSTimeInterval)interval
{
    
    animationInterval = interval;
    if(animationTimer)
	{
        [self stopAnimation];
        [self startAnimation];
    }
}

- (void)setPlayerArray:(Game*)game :(Player*[MAXPLAYERS])p
{
	mGame = game;
	player = p;
}

- (void) showText: (NSString*)text :(float)x :(float)y :(float)width :(float)height :(unsigned int)textureWidth :(unsigned int)textureHeight :(float)fontSize :(int)color
{
	[self activateStringTextureDraw];

	unsigned char /*a = (color & 0xFF000000)/256/256/256,*/ r =  (color & 0x00FF0000)/256/256, g =  (color & 0x0000FF00)/256, b = color & 0x000000FF;
	
	unsigned char Color[16];
	for(int i = 0; i<16; i+=4)
	{
		Color[i+0] = r;
		Color[i+1] = g;
		Color[i+2] = b;
		Color[i+3] = 0;
	}
	
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, Color);
	glColor4ub(r, g, b, 0);

	Texture2D* texture = [[Texture2D alloc] initWithString:text dimensions:CGSizeMake(textureWidth, textureHeight) alignment:UITextAlignmentLeft fontName:@"Arial" fontSize:fontSize];

	[texture drawInRect:CGRectMake(x, y, width, height)];
	[texture release];
	
	[self deactivateStringTextureDraw];
}


- (void)dealloc
{
    
    [self stopAnimation];
    
    if ([EAGLContext currentContext] == context)
	{
        [EAGLContext setCurrentContext:nil];
    }
	
	[textureArrowLeft release];
	[textureArrowUp release];
	[textureArrowRight release];
	[textureArrowDown release];
    
    [context release];  
    [super dealloc];
}

- (unsigned char) cut2uByte:(long long)number
{
	return number>=255?255:number;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch *touch in touches)
	{
	 	CGPoint p = [touch locationInView:self];
		if(p.x < backingWidth*0.25)
			[player[0] move:player[0].PosX-1 :player[0].PosY];
		if(p.x > backingWidth*0.75)
			[player[0] move:player[0].PosX+1 :player[0].PosY];
		if(p.y < backingHeight/1.5*0.25+backingHeight/6 && p.y > backingHeight/6)
			[player[0] move:player[0].PosX :player[0].PosY-1];
		if(p.y > backingHeight/1.5*0.75+backingHeight/6 && p.y < backingHeight/1.5+backingHeight/6)
			[player[0] move:player[0].PosX :player[0].PosY+1];
	}
}

- (void) activateImageTextureDraw
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

- (void) deactivateImageTextureDraw
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

- (void) activateColoredVerticesDraw
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
}

- (void) deactivateColoredVerticesDraw
{
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

- (void) activateStringTextureDraw
{
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

- (void) deactivateStringTextureDraw
{
	glColor4ub(255, 255, 255, 255);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_BLEND);
}

@end
