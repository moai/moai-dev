/********************************************************************\
 * app.cpp -- UUID GUI application class                            *
 *                                                                  *
 * Copyright (C) 2009 Kenneth Laskoski                              *
 *                                                                  *
\********************************************************************/
/** @file app.cpp
    @brief graphical user interface to UUIDs defined by RFC 4122
    @author Copyright (C) 2009 Kenneth Laskoski

    Use, modification, and distribution are subject
    to the Boost Software License, Version 1.0.  (See accompanying file
    LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)
*/

#include "app.h"
#include "frame.h"

bool App::OnInit()
{
    Frame *main = new Frame(L"GOOID");
    main->Show();
    return true;
}
