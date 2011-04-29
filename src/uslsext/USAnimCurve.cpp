// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USInterpolate.h>
#include <uslsext/USAnimCurve.h>

//================================================================//
// USAnimCurve
//================================================================//

//----------------------------------------------------------------//
u32 USAnimCurve::FindKeyID ( float time ) {

	u32 keyID = 0;
	for ( u32 i = 0; i < this->Size (); ++i ) {
		if (( *this )[ i ].mTime > time ) break;
		keyID = i;
	}
	return keyID;
}

//----------------------------------------------------------------//
bool USAnimCurve::GetBoolValue ( float time ) {

	float value = this->GetFloatValue ( time );
	return ( value > 0.5f ) ? true : false;
}

//----------------------------------------------------------------//
float USAnimCurve::GetFloatDelta ( float t0, float t1 ) {

	u32 total = this->Size ();
	if ( total < 2 ) return 0.0f;
	if ( t0 == t1 ) return 0.0f;

	float length = this->GetLength ();

	float step = t1 - t0;	
	float delta = 0.0f;	
	if ( step > 0.0f ) {
		
		u32 endID = total - 1;
		u32 keyID = this->FindKeyID ( t0 );
		
		bool more = true;
		while ( more ) {
			
			if ( keyID == endID ) {
				keyID = 0;
				t0 -= length;
				t1 -= length;
			}
			
			USAnimKey k0 = ( *this )[ keyID ];
			USAnimKey k1 = ( *this )[ keyID + 1 ];
			
			float v0 = k0.mValue;
			float v1 = k1.mValue;
			
			float span = k1.mTime - k0.mTime;
		
			if ( span == 0.0f ) {
				continue;
			}
			
			float r0 = v0;
			float r1 = v1;
			
			if ( t0 > k0.mTime ) {
				r0 = USInterpolate::Interpolate ( k0.mMode, v0, v1, ( t0 - k0.mTime ) / span, k0.mWeight );
			}
			
			if ( t1 <= k1.mTime ) {
				r1 = USInterpolate::Interpolate ( k0.mMode, v0, v1, ( t1 - k0.mTime ) / span, k0.mWeight );
				more = false;
			}
			
			delta += r1 - r0;
			keyID++;
		}
	}
	else {
		
		step = -step;
		
		u32 endID = total - 1;
		u32 keyID = this->FindKeyID ( t0 ) + 1;
		if ( keyID > endID ) {
			keyID = endID;
		}
		
		bool more = true;
		while ( more ) {
			
			if ( keyID == 0 ) {
				keyID = endID;
				t0 += length;
				t1 += length;
			}
			
			USAnimKey k0 = ( *this )[ keyID - 1 ];
			USAnimKey k1 = ( *this )[ keyID ];
			
			float v0 = k0.mValue;
			float v1 = k1.mValue;
			
			float span = k1.mTime - k0.mTime;
		
			if ( span == 0.0f ) {
				continue;
			}
			
			float r0 = v0;
			float r1 = v1;
			
			if ( t0 < k1.mTime ) {
				r1 = USInterpolate::Interpolate ( k0.mMode, v0, v1, ( t0 - k0.mTime ) / span, k0.mWeight );
			}
			
			if ( t1 >= k0.mTime ) {
				r0 = USInterpolate::Interpolate ( k0.mMode, v0, v1, ( t1 - k0.mTime ) / span, k0.mWeight );
				more = false;
			}
			
			delta -= r1 - r0;
			keyID--;
		}
	}

	return delta;
}

//----------------------------------------------------------------//
float USAnimCurve::GetFloatValue ( float time ) {

	u32 total = this->Size ();
	if ( total == 0 ) return 0.0f;
	u32 endID = total - 1;
	
	u32 keyID = this->FindKeyID ( time );
	USAnimKey k0 = ( *this )[ keyID ];
	
	if ( keyID == endID ) {
		return k0.mValue;
	}
	
	if ( k0.mMode == USInterpolate::kFlat ) {
		return k0.mValue;
	}

	if ( k0.mTime == time ) {
		return k0.mValue;
	}

	USAnimKey k1 = ( *this )[ keyID + 1 ];

	float v0 = k0.mValue;
	float v1 = k1.mValue;

	if ( v0 == v1 ) {
		return v0;
	}
	
	float span = k1.mTime - k0.mTime;
	
	if ( span == 0.0f ) {
		return v0;
	}
	
	float t = ( time - k0.mTime ) / span;
	return USInterpolate::Interpolate ( k0.mMode, v0, v1, t, k0.mWeight );
}

//----------------------------------------------------------------//
u32 USAnimCurve::GetIndexValue ( float time ) {

	float value = this->GetFloatValue ( time );
	return ( value < 0.0f ) ? 0 : ( u32 )value;
}

//----------------------------------------------------------------//
int USAnimCurve::GetIntValue ( float time ) {

	float value = this->GetFloatValue ( time );
	return ( int ) value;
}

//----------------------------------------------------------------//
float USAnimCurve::GetLength () {

	u32 total = this->Size ();
	if ( total == 0 ) return 0.0f;
	return ( *this )[ total - 1 ].mTime - ( *this )[ 0 ].mTime;
}

//----------------------------------------------------------------//
void USAnimCurve::SetKey ( u32 id, float time, float value, u32 mode, float weight ) {

	if ( id < this->Size ()) {
		( *this )[ id ].mTime = time;
		( *this )[ id ].mValue = value;
		( *this )[ id ].mMode = mode;
		( *this )[ id ].mWeight = weight;
	}
}

//----------------------------------------------------------------//
USAnimCurve::USAnimCurve () {
}

//----------------------------------------------------------------//
USAnimCurve::~USAnimCurve () {
}