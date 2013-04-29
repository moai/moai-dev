#ifndef _LINE_CODE_H_
#define _LINE_CODE_H_

#include "Common.h"
#include <vector>
#include <zl-util/pch.h>
#include <zl-util/STLMap.h>
#include <zl-util/STLString.h>
#include <zl-util/ZLHashedString.h>

namespace FMODDesigner
{
    class LineCode
    {
    public:
        typedef STLMap < u32, u32 > StringMap;

        LineCode() : m_index(kInvalidIndex) {}

        LineCode(const LineCode& rhs) : m_index(rhs.m_index) {}

        explicit LineCode(const char* szLineCode)
        {
            if (szLineCode && szLineCode[0])
            {  
				const size_t kMaxLineCodeLength = 32;
				char szNormalizedLineCode [ kMaxLineCodeLength ];

				size_t lineCodeLength = strlen ( szLineCode );
				assert ( lineCodeLength < kMaxLineCodeLength );

				for ( size_t i=0; i<lineCodeLength; i++ ) {
					szNormalizedLineCode [ i ] = toupper ( szLineCode [ i ] );
				}
				szNormalizedLineCode [ lineCodeLength ] = 0;

				const u32 uLineCodeHash = ZLHashedString::GetStringHash ( szNormalizedLineCode, lineCodeLength );
				
                u32 nextIndex = sm_byIndex.size();
                StringMap::iterator it = sm_byString.find( uLineCodeHash );
                if (it == sm_byString.end())
                {
					sm_byIndex.push_back( STLString ( szNormalizedLineCode ) );
                    sm_byString.insert( pair<u32, u32>( uLineCodeHash, nextIndex ) );                
                    m_index = nextIndex;
                }
                else
                {            
                    m_index = it->second;
                }
                assert(sm_byIndex.size() == sm_byString.size());
            }
            else
            {
                m_index = kInvalidIndex;
            }
        }

        const char* c_str() const
        {
            if( m_index != kInvalidIndex )
            {            
                return m_index < sm_byIndex.size() ? sm_byIndex[m_index] : "";
            }
            else
            {
                return "";
            }
        }

        bool IsValid() const { return m_index != kInvalidIndex; }

        bool operator==(const LineCode& rhs) const { return m_index == rhs.m_index; }
        bool operator!=(const LineCode& rhs) const { return m_index != rhs.m_index; }

        u32 GetIndex() const { return m_index; }

        u32 GetHash() const
        {
            u32 hash = m_index;
            hash += hash << 13;
            hash ^= hash >> 7;
            hash += hash << 3;
            hash ^= hash >> 17;
            hash += hash << 5;
            return hash;
        }

        static void FreeAllStrings();

        // NOTE: for serialization help only!
        static const char* _GetStringAtIndex(u32 index)
        {
            return index < sm_byIndex.size() ? sm_byIndex[index] : "";
        }

        static u32 GetNumLineCodes()
        {
            return sm_byIndex.size();
        }    

    protected:    
        static const u32 kInvalidIndex = (u32)-1;    
        static std::vector< STLString > sm_byIndex;
        static StringMap sm_byString;

        u32 m_index;
    };
};

template<> struct std::less<FMODDesigner::LineCode> {
    bool operator()(const FMODDesigner::LineCode& a, const FMODDesigner::LineCode& b) const {
        return a.GetIndex() < b.GetIndex();
    }
};

extern FMODDesigner::LineCode kINVALID_LINECODE;


#endif
