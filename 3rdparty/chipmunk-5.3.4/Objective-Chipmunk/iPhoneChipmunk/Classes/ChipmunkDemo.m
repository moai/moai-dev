#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

#import "ChipmunkDemo.h"
#import "drawSpace.h"

#import <mach/mach_time.h>
double getDoubleTime(void)
{
	mach_timebase_info_data_t base;
	mach_timebase_info(&base);
	
	return (double)mach_absolute_time()*((double)base.numer/(double)base.denom*1.0e-9);
}

@implementation ChipmunkDemo

+ (Class)layerClass
{
	return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)frame
{
	if(self = [super initWithFrame:frame]){
		CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
		eaglLayer.contentsScale = [[UIScreen mainScreen] scale];

		eaglLayer.opaque = YES;
		eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
			[NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking,
			kEAGLColorFormatRGB565, kEAGLDrawablePropertyColorFormat, nil
		];

		context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];

		if(!context || ![EAGLContext setCurrentContext:context]){
			[self release];
			return nil;
		}
		
		mouseBody = [[ChipmunkBody alloc] initStaticBody];
	}
	
	return self;
}

- (BOOL)createFramebuffer {
	glGenFramebuffersOES(1, &viewFramebuffer);
	glGenRenderbuffersOES(1, &viewRenderbuffer);

	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	[context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);

	if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
		NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
		return NO;
	}

	return YES;
}

- (void)destroyFramebuffer
{
	glDeleteFramebuffersOES(1, &viewFramebuffer);
	viewFramebuffer = 0;

	glDeleteRenderbuffersOES(1, &viewRenderbuffer);
	viewRenderbuffer = 0;
}

- (void)setupSpace; {}

- (void)layoutSubviews {
	[self setupSpace];
	
	[EAGLContext setCurrentContext:context];
	[self destroyFramebuffer];
	[self createFramebuffer];
	
	timer = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
	[timer addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
	
	refTime = getDoubleTime();
	refTicks = 0;
	timeStep = 1.0/60.0;
}

- (void)update
{
	cpVect newPoint = cpvlerp(mousePoint_last, mousePoint, 0.25);
	mouseBody.pos = newPoint;
	mouseBody.vel = cpvmult(cpvsub(newPoint, mousePoint_last), 1.0/timeStep);
	mousePoint_last = newPoint;
	
	[space step:timeStep];
}

static float hw = 639.0/2.0, hh = 479.0/2.0;

- (void)draw
{
	GLint width, height;
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &width);
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &height);
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(-hh, hh, -hw, hw, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	drawSpace(space.space);
}

- (void)render:(CADisplayLink *)sender
{
	[EAGLContext setCurrentContext:context];
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	
	double curTime = getDoubleTime();
	double overtime = (curTime - refTime) - (refTicks + 10)*timeStep;
	if(overtime > 0.0f)
		refTime += overtime;
	
	while((curTime - refTime) > refTicks*timeStep){
		refTicks += 1;
		[self update];
	}
	
	[self draw];
	
	[context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

static cpVect
mouseToSpace(cpVect pos, CGSize size)
{
	cpVect v = cpvsub(cpvmult(cpv(pos.y/size.height, pos.x/size.width), 2.0f), cpv(1.0f, 1.0f));
	v = cpv(v.x*hw, v.y*hh);
//	printf("%s\n", cpvstr(v));
	return v;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch *touch = [[event touchesForView:self] anyObject];
	cpVect point = mouseToSpace([touch locationInView:self], self.bounds.size);
//	NSLog(@"down %s", cpvstr(point));
	
	mousePoint = point;

	ChipmunkShape *shape = [space pointQueryFirst:point layers:GRABABLE_MASK_BIT group:nil];
	if(shape){
		ChipmunkBody *body = shape.body;
		
		mousePoint_last = mousePoint;
		mouseBody.pos = mousePoint;
		
		mouseJoint = [space add:[ChipmunkPivotJoint pivotJointWithBodyA:mouseBody bodyB:body anchr1:cpvzero anchr2:[body world2local:point]]];
		mouseJoint.maxForce = 50000.0f;
		mouseJoint.biasCoef = 0.15f;
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch *touch = [[event touchesForView:self] anyObject];
	mousePoint = mouseToSpace([touch locationInView:self], self.bounds.size);
//	NSLog(@"moved %s", cpvstr(mousePoint));
}

- (void)removeFromSuperview
{
	[timer invalidate];
	[super removeFromSuperview];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
//	UITouch *touch = [[event touchesForView:self] anyObject];
//	CGPoint point = [touch locationInView:self];
//	NSLog(@"up");

//	[gamestateDelegate touchUp];
	[space remove:mouseJoint];
	mouseJoint = nil;
}

- (void)dealloc
{
	if([EAGLContext currentContext] == context){
		[EAGLContext setCurrentContext:nil];
	}
	
	[space release];
	[mouseBody release];
	[mouseJoint release];
	
	[context release];
	
	[super dealloc];
}

- (BOOL)paused {return timer.paused;}
- (void)setPaused:(BOOL)value {timer.paused = value;}

@end
