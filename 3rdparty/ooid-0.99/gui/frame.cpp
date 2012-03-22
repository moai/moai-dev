/********************************************************************\
 * frame.cpp -- UUID GUI main window                                *
 *                                                                  *
 * Copyright (C) 2009 Kenneth Laskoski                              *
 *                                                                  *
\********************************************************************/
/** @file frame.cpp
    @brief graphical user interface to UUIDs defined by RFC 4122
    @author Copyright (C) 2009 Kenneth Laskoski

    Use, modification, and distribution are subject
    to the Boost Software License, Version 1.0.  (See accompanying file
    LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)
*/

#include "frame.h"
#include "app.h"

enum
{
    v4uuidgenID = 101,
    niluuidgenID,
};

Frame::Frame(const wxString& title)
    : wxFrame(0, wxID_ANY, title, wxPoint(-1, -1), wxSize(500, -1))
{
    wxPanel *root = new wxPanel(this, wxID_ANY);

    lpanel = new wxPanel(root);
    niluuidgen = new wxButton(lpanel, niluuidgenID, L"Generate nil UUID", wxPoint(-1, -1));
    v4uuidgen = new wxButton(lpanel, v4uuidgenID, L"Generate version 4 UUID", wxPoint(-1, -1));

    rpanel = new wxPanel(root);
    result = new wxTextCtrl(rpanel, wxID_ANY, L"", wxPoint(-1, -1), wxSize(-1, -1), wxTE_MULTILINE);

    v4uuidgen->SetFocus();

    wxBoxSizer *lsizer = new wxBoxSizer(wxVERTICAL);
    lsizer->Add(niluuidgen, 0, normal, border);
    lsizer->Add(v4uuidgen, 0, normal, border);
    lpanel->SetSizer(lsizer);

    wxBoxSizer *rsizer = new wxBoxSizer(wxVERTICAL);
    rsizer->Add(result, 1, normal, border);
    rpanel->SetSizer(rsizer);

    wxBoxSizer *rootbox = new wxBoxSizer(wxHORIZONTAL);
    rootbox->Add(lpanel, 0, versus, border);
    rootbox->Add(rpanel, 1, versus, border);
    root->SetSizer(rootbox);
}

void Frame::generate_nil_uuid(wxCommandEvent&)
{
    static const kashmir::uuid_t nil;

    assert(!nil);

    buffer << nil << '\n';

    result->AppendText(buffer.str());
    buffer.str(L"");
}

void Frame::generate_v4_uuid(wxCommandEvent&)
{
    wxGetApp().DevRandom() >> uuid;
    buffer << uuid << '\n';

    result->AppendText(buffer.str());
    buffer.str(L"");
}

BEGIN_EVENT_TABLE(Frame, wxFrame)
    EVT_BUTTON(niluuidgenID,  Frame::generate_nil_uuid)
    EVT_BUTTON(v4uuidgenID,  Frame::generate_v4_uuid)
END_EVENT_TABLE()
