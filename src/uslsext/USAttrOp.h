// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USATTROP_H
#define	USATTROP_H

//================================================================//
// USAttrOp
//================================================================//
class USAttrOp  {
protected:

	float mFloat;
	u32 mType;

public:

	enum {
		TYPE_BOOLEAN	= 0x00000001 << 0,
		TYPE_FLOAT		= 0x00000001 << 1,
		TYPE_INT		= 0x00000001 << 2,
		TYPE_INDEX		= 0x00000001 << 3,
	};
	
	//----------------------------------------------------------------//
	virtual bool		Op		( bool value ) = 0;
	virtual float		Op		( float value ) = 0;
	virtual int			Op		( int value ) = 0;
	virtual u32			Op		( u32 value ) = 0;
	
	//----------------------------------------------------------------//
	template < typename TYPE > TYPE	Get	();
	template < typename TYPE > void	Set	( TYPE value );
	
	//----------------------------------------------------------------//
	inline void Clear () {
		this->mType = 0;
	}
	
	//----------------------------------------------------------------//
	inline void Copy ( const USAttrOp& assign ) {
		this->mFloat = assign.mFloat;
		this->mType = assign.mType;
	}
	
	//----------------------------------------------------------------//
	inline bool IsValid () {
		return ( this->mType != 0 );
	}

	//----------------------------------------------------------------//
	USAttrOp () :
		mType ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	virtual ~USAttrOp () {
	}
};

//----------------------------------------------------------------//
template <> bool	USAttrOp::Get < bool >		();
template <> float	USAttrOp::Get < float >		();
template <> int		USAttrOp::Get < int >		();
template <> u32		USAttrOp::Get < u32 >		();
template <> void	USAttrOp::Set < bool >		( bool value );
template <> void	USAttrOp::Set < float >		( float value );
template <> void	USAttrOp::Set < int >		( int value );
template <> void	USAttrOp::Set < u32 >		( u32 value );

//================================================================//
// USAttrAdder
//================================================================//
class USAttrAdder :
	public USAttrOp {
public:

	//----------------------------------------------------------------//
	bool Op ( bool value ) {
		return ( this->mFloat == 1.0f ) ? true : value;
	}
	
	//----------------------------------------------------------------//
	float Op ( float value ) {
		return value + this->mFloat;
	}
	
	//----------------------------------------------------------------//
	int Op ( int value ) {
		return value + ( int )this->mFloat;
	}
	
	//----------------------------------------------------------------//
	u32 Op ( u32 value ) {
		return value + ( u32 )this->mFloat;
	}
};

//================================================================//
// USAttrGetter
//================================================================//
class USAttrGetter :
	public USAttrOp {
public:

	//----------------------------------------------------------------//
	bool Op ( bool value ) {
		this->Set < bool >( value );
		return value;
	}
	
	//----------------------------------------------------------------//
	float Op ( float value ) {
		this->Set < float >( value );
		return value;
	}
	
	//----------------------------------------------------------------//
	int Op ( int value ) {
		this->Set < int >( value );
		return value;
	}
	
	//----------------------------------------------------------------//
	u32 Op ( u32 value ) {
		this->Set < u32 >( value );
		return value;
	}
};

//================================================================//
// USAttrSetter
//================================================================//
class USAttrSetter :
	public USAttrOp {
public:

	//----------------------------------------------------------------//
	bool Op ( bool value ) {
		UNUSED ( value );
		return ( this->mFloat == 1.0f );
	}
	
	//----------------------------------------------------------------//
	float Op ( float value ) {
		UNUSED ( value );
		return this->mFloat;
	}
	
	//----------------------------------------------------------------//
	int Op ( int value ) {
		UNUSED ( value );
		return ( int )this->mFloat;
	}
	
	//----------------------------------------------------------------//
	u32 Op ( u32 value ) {
		UNUSED ( value );
		return ( u32 )this->mFloat;
	}
};

#endif
