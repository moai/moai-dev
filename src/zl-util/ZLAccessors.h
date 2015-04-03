// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLACCESSORS_H
#define ZLACCESSORS_H

#define GET(type,name,member) \
	inline type Get##name () { return member; }

#define GET_BOOL(name,member) \
	inline bool name () { return ( member == true ); }

#define GET_CONST(type,name,member) \
	inline const type Get##name () const { return member; }

#define SET(type,name,member) \
	inline void Set##name ( type value ) { member = value; }

#define Can(name,member,value) const \
	inline bool Can##name () { return member == value; }

#define HAS(name,member,value) const \
	inline bool Has##name () { return member == value; }

#define IS(name,member,value) const \
	inline bool Is##name () { return member == value; }

#define GET_SET(type,name,member) \
	inline type Get##name () { return member; } \
	inline void Set##name ( type value ) { member = value; }

#define GET_SET_CONST(type,name,member) \
	inline const type Get##name () const { return member; } \
	inline void Set##name ( type value ) { member = value; }

#define GET_BY_NAME(type,name,member) \
	inline type name () { return member; }

#define SET_BY_NAME(type,name,member) \
	inline void name ( type value ) { member = value; }

#define GET_SET_BY_NAME(type,name,member) \
	inline type name () { return member; } \
	inline void name ( type value ) { member = value; }

#endif
