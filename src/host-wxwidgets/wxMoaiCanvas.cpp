#include <moai-core/headers.h>
#include <host-modules/aku_modules.h>
#include <host-modules/aku_modules_custom.h>
#include <host-wxwidgets/wxMoaiCanvas.h>

IMPLEMENT_CLASS ( wxMoaiCanvas, wxGLCanvas )

BEGIN_EVENT_TABLE ( wxMoaiCanvas, wxGLCanvas )

	EVT_IDLE ( wxMoaiCanvas::OnIdleEvent )
	EVT_KEY_DOWN ( wxMoaiCanvas::OnKeyDownEvent )
	EVT_KEY_UP ( wxMoaiCanvas::OnKeyUpEvent )	
	EVT_PAINT ( wxMoaiCanvas::OnPaintEvent )
	EVT_ERASE_BACKGROUND ( wxMoaiCanvas::OnEraseBackgroundEvent )
	EVT_MOUSE_EVENTS ( wxMoaiCanvas::OnMouseEvent )
	EVT_SIZE ( wxMoaiCanvas::OnSizeEvent )
	
END_EVENT_TABLE ()

namespace InputDeviceID {
	enum {
		DEVICE,
		TOTAL,
	};
}

namespace InputSensorID {
	enum {
		KEYBOARD,
		POINTER,
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT,
		TOUCH,
		TOTAL,
	};
}

//================================================================//
// wxMoaiCanvas
//================================================================//

//----------------------------------------------------------------//
void wxMoaiCanvas::AppFinalize () {

	AKUModulesCustomAppFinalize ();
	AKUModulesAppFinalize ();
	AKUAppFinalize ();
}

//----------------------------------------------------------------//
void wxMoaiCanvas::AppInitialize () {

	_CrtSetDbgFlag ( 0 );

	AKUAppInitialize ();
	AKUModulesAppInitialize ();
	AKUModulesCustomAppInitialize ();
}

//----------------------------------------------------------------//
void wxMoaiCanvas::OnEraseBackgroundEvent ( wxEraseEvent& event ) {
	UNUSED ( event );
}

//----------------------------------------------------------------//
void wxMoaiCanvas::OnIdleEvent ( wxIdleEvent& event ) {
	UNUSED ( event );
	
	this->Refresh ( false );
}

//----------------------------------------------------------------//
void wxMoaiCanvas::OnKeyDownEvent ( wxKeyEvent& event ) {

	event.Skip ();
}

//----------------------------------------------------------------//
void wxMoaiCanvas::OnKeyUpEvent ( wxKeyEvent& event ) {

	event.Skip ();
}

//----------------------------------------------------------------//
void wxMoaiCanvas::OnMouseEvent ( wxMouseEvent& event ) {

	event.Skip ();
}

//----------------------------------------------------------------//
void wxMoaiCanvas::OnPaintEvent ( wxPaintEvent& event ) {
	UNUSED ( event );
	wxPaintDC dc ( this );
	
	if ( this->mGLContext ) {
		this->SetCurrent ( *this->mGLContext );
	}
	else {
		static wxGLContext glContext ( this );
		this->mGLContext = &glContext;
		this->SetCurrent ( *this->mGLContext );
		AKUDetectGfxContext ();
	}
	
	AKUSetContext ( this->mAkuContextID );
	AKUModulesUpdate ();
	AKURender ();
	this->SwapBuffers ();
	
	wxTheApp->ProcessIdle ();
}

//----------------------------------------------------------------//
void wxMoaiCanvas::OnSizeEvent ( wxSizeEvent& event ) {

	wxSize size = event.GetSize ();

	AKUSetScreenSize ( size.x, size.y );
	AKUSetViewSize ( size.x, size.y );
	
	event.Skip();
}

//----------------------------------------------------------------//
void wxMoaiCanvas::RunScript ( const char* filename ) {

	AKURunScript ( filename );
}

//----------------------------------------------------------------//
wxMoaiCanvas::wxMoaiCanvas ( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) :
	wxGLCanvas ( parent, id, 0, pos, size, style, "MoaiCanvas", wxNullPalette ),
	mGLContext ( 0 ) {
	
	this->mAkuContextID = AKUCreateContext ();
	AKUModulesContextInitialize ();
	AKUModulesCustomContextInitialize ();
	AKUModulesRunLuaAPIWrapper ();

	AKUSetInputConfigurationName ( "wxWidgets" );

	AKUReserveInputDevices			( InputDeviceID::TOTAL );
	AKUSetInputDevice				( InputDeviceID::DEVICE, "device" );
	
	AKUReserveInputDeviceSensors	( InputDeviceID::DEVICE, InputSensorID::TOTAL );
	AKUSetInputDeviceKeyboard		( InputDeviceID::DEVICE, InputSensorID::KEYBOARD,		"keyboard" );
	AKUSetInputDevicePointer		( InputDeviceID::DEVICE, InputSensorID::POINTER,		"pointer" );
	AKUSetInputDeviceButton			( InputDeviceID::DEVICE, InputSensorID::MOUSE_LEFT,		"mouseLeft" );
	AKUSetInputDeviceButton			( InputDeviceID::DEVICE, InputSensorID::MOUSE_MIDDLE,	"mouseMiddle" );
	AKUSetInputDeviceButton			( InputDeviceID::DEVICE, InputSensorID::MOUSE_RIGHT,	"mouseRight" );
	
	AKUModulesCustomRunBefore ();
	AKUModulesCustomRunAfter ();
}

//----------------------------------------------------------------//
wxMoaiCanvas::~wxMoaiCanvas () {

	AKUDeleteContext ( this->mAkuContextID );
}
