#ifndef _COMMON_H
#define _COMMON_H

#include <zl-util/headers.h>
#include <float.h>

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------

#define KILO	(1024)
#define	MEG		(KILO * KILO)
#define GIG		(KILO * MEG)
#define FILE_SEP_CHAR '/'


//-----------------------------------------------------------------------------
// Helper Functions and Macros
//-----------------------------------------------------------------------------

# define NOINLINE(FunctionDef) __declspec(noinline) FunctionDef

/// Return if _ptr is aligned to _align (which must be a power of 2)
/// @eg
///   ASSERT(IS_ALIGNED(ptr, 128));
/// @endeg
#define IS_ALIGNED(_ptr, _align) (((uintptr_t(_ptr)) & ((_align)-1)) == 0)

/// Return n, constrained to be within the range [min, max].
template <class T> inline T Pin(const T n, const T min,	const T max)
{
	return Min(Max(n, min), max);
}

inline bool Equal(float fA, float fB, float fEpsilon)
{
    return fabsf(fA - fB) <= fEpsilon;
}

inline float FSel(float fComparand, float fValGE, float fValLT)
{
    return (fComparand >= 0.f) ? fValGE : fValLT;
}

template <class T> inline T Square(const T x)
{
    return x * x;
}

inline float Lerp(float fA, float fB, float fT)
{
    return (1.f-fT) * fA + fT * fB;
}

//-----------------------------------------------------------------------------
// Helper Classes
//-----------------------------------------------------------------------------

// Handy class for representing ranges of numbers.
template< typename T >
struct Range
{
    Range( T _min, T _max )
    {
        min = _min;
        max = _max;
        assert( min <= max );
    }

    Range()
    {
        min = 0;
        max = 0;
    }

    static Range<T> FromUnsorted(T first, T second)
    {
        return Range(Min(first,second), Max(first,second));
    }

    bool operator==(const Range<T>& rhs) const
    {
        return (min == rhs.min && max == rhs.max);
    }

    void Set( T _min, T _max )
    {
        assert( _min <= _max );
        min = _min;
        max = _max;
    }

    bool Contains( T arg ) const
    {
        return arg >= min && arg <= max;
    }

    bool ContainsSquared( T arg ) const
    {
        return arg >= Square(min) && arg <= Square(max);
    }

    /// Scales and biases the input so that values within the range are mapped to [0, 1]
    T ToParam( T arg ) const
    {
        return (arg - min) / (max - min);
    }

    bool GetOverlap( const Range<T>& Other, Range<T>* pOutResult = NULL ) const
    {
        float fMin = Max( min, Other.min );
        float fMax = Min( max, Other.max );
        if ( fMin <= fMax )
        {
            if ( pOutResult )
            {
                *pOutResult = Range<T>( fMin, fMax );
            }
            return true;
        }
        else
        {
            return false;
        }
    }

    T Delta() const
    {
        return max - min;
    }

    union {
        struct {
            T	min, max;
        };

        T m[2];
    };
};

#define PROFILE_TIME_REGION(name)
#define PROFILE_TIME_REGION_FAST(name)
//#define Trace( arg1, arg2, msg, ... ) printf( msg, __VA_ARGS__ )
#define Trace( arg1, arg2, msg, ... )

#endif // _Common_H_
