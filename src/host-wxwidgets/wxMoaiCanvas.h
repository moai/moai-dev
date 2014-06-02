#ifndef WXMOAICANVAS_H
#define WXMOAICANVAS_H

#include <moai-core/host.h>

#include <wx/wx.h>
#include <wx/glcanvas.h>

//================================================================//
// wxMoaiCanvas
//================================================================//
class wxMoaiCanvas :
	public wxGLCanvas {
private:

	DECLARE_CLASS( wxMoaiCanvas )
	DECLARE_EVENT_TABLE ()

	AKUContextID	mAkuContextID;
	wxGLContext*	mGLContext;

	//----------------------------------------------------------------//
	void			AffirmContext				();

public:

	//----------------------------------------------------------------//
	static void		AppFinalize					();
	static void		AppInitialize				();
	void			OnEraseBackgroundEvent		( wxEraseEvent& event );
	void			OnIdleEvent					( wxIdleEvent& event );
	void			OnKeyDownEvent				( wxKeyEvent& event );
	void			OnKeyUpEvent				( wxKeyEvent& event );
	void			OnMouseEvent				( wxMouseEvent& event );
	void			OnPaintEvent				( wxPaintEvent& event );
	void			OnSizeEvent					( wxSizeEvent& event );
	void			RunScript					( const char* filename );
					wxMoaiCanvas				(	wxWindow* parent,
													wxWindowID id = wxID_ANY,
													const wxPoint& pos = wxDefaultPosition,
													const wxSize& size = wxDefaultSize,
													long style = 0
												);
					~wxMoaiCanvas				();
};

#endif
