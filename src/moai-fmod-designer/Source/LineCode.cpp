#include "LineCode.h"

using namespace FMODDesigner;

vector<STLString> LineCode::sm_byIndex;
LineCode::StringMap LineCode::sm_byString;

LineCode kINVALID_LINECODE;


void LineCode::FreeAllStrings()
{    
    sm_byString.clear();
    sm_byIndex.clear();
}
