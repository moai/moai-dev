/********************************************************************\
 * frame.h -- UUID GUI main window                                  *
 *                                                                  *
 * Copyright (C) 2009 Kenneth Laskoski                              *
 *                                                                  *
\********************************************************************/
/** @file frame.h
    @brief graphical user interface to UUIDs defined by RFC 4122
    @author Copyright (C) 2009 Kenneth Laskoski

    Use, modification, and distribution are subject
    to the Boost Software License, Version 1.0.  (See accompanying file
    LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)
*/

#include "../kashmir/uuid.h"
#include <sstream>

#include <wx/wx.h>

class Frame : public wxFrame
{
    kashmir::uuid_t uuid;
    std::wstringstream buffer;

    wxPanel *lpanel;
    wxButton *niluuidgen;
    wxButton *v4uuidgen;

    wxPanel *rpanel;
    wxTextCtrl *result;

    DECLARE_EVENT_TABLE()

    void generate_nil_uuid(wxCommandEvent& event);
    void generate_v4_uuid(wxCommandEvent& event);

    static const int normal = wxEXPAND | wxTOP | wxLEFT;
    static const int versus = wxEXPAND | wxRIGHT | wxBOTTOM;
    static const int border = 6;
public:
    Frame(const wxString& title);
};
