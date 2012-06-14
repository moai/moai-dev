// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <Box2D/Box2D.h>
#include <moaicore/MOAIBox2DBody.h>
#include <moaicore/MOAIBox2DFixture.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIBox2DWorld.h>
#include <moaicore/MOAITransform.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_BOX2D

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**   @name addChain
@text Create and add a set of collision edges to teh body.
 
@in         MOAIBox2DBody self
@in         table verts Array containing vertex coordinate components ( t[1] = x0, t[2] = y0, t[3] = x1, t[4] = y1... )
@opt    boolean closeChain          Default value is false.
@out  MOAIBox2DFixture fixture      Returns nil on failure.
*/
int MOAIBox2DBody::_addChain ( lua_State* L ) {
    MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
 
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
 
	float unitsToMeters = self->GetUnitsToMeters ();
	u32 totalCoords = lua_objlen ( state, 2 );
	if( totalCoords < 4 || totalCoords % 2 != 0 ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_InvalidVertexCount_D, totalCoords );
		return 0;
	}
 
	u32 totalVerts = totalCoords / 2;                       
	b2Vec2 * verts = (b2Vec2 *)alloca(sizeof(b2Vec2) * totalVerts);
	int numVerts = MOAIBox2DFixture::LoadVerts( state, 2, verts, totalVerts, unitsToMeters );       
	if ( numVerts ) {
		bool closeChain = state.GetValue < bool >( 3, false );
 
		b2ChainShape chainShape;                               
		if( closeChain ) {
			chainShape.CreateLoop(verts, numVerts);
		}
		else {
			chainShape.CreateChain(verts, numVerts);
		}
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &chainShape;
		MOAIBox2DFixture* fixture = new MOAIBox2DFixture ();
		fixture->SetFixture ( self->mBody->CreateFixture ( &fixtureDef ));
		fixture->SetWorld ( self->mWorld );
		self->mWorld->LuaRetain ( fixture );
		fixture->PushLuaUserdata ( state );
		return 1;           
	}       
	return 0;
}

//----------------------------------------------------------------//
/**	@name	addCircle
	@text	Create and add circle fixture to the body.
	
	@in		MOAIBox2DBody self
	@in		number x	in units, world coordinates, converted to meters
	@in		number y	in units, world coordinates, converted to meters
	@in		number radius	in units, converted to meters
	@out	MOAIBox2DFixture fixture
*/
int MOAIBox2DBody::_addCircle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "UNNN" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	b2CircleShape circleShape;

	circleShape.m_p.x		= state.GetValue < float >( 2, 0.0f ) * unitsToMeters;
	circleShape.m_p.y		= state.GetValue < float >( 3, 0.0f ) * unitsToMeters;
	circleShape.m_radius	= state.GetValue < float >( 4, 1.0f ) * unitsToMeters;
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	
	MOAIBox2DFixture* fixture = new MOAIBox2DFixture ();
	fixture->SetFixture ( self->mBody->CreateFixture ( &fixtureDef ));
	fixture->SetWorld ( self->mWorld );
	self->mWorld->LuaRetain ( fixture );

	fixture->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addEdges
 @text	Create and add a polygon fixture to the body.
 
 @in		MOAIBox2DBody self
 @in		table verts Array containing vertex coordinate components in units, world coordinates, converted to meters ( t[1] = x0, t[2] = y0, t[3] = x1, t[4] = y1... )
 @out	    table Array containing MOAIBox2DFixture fixtures. Returns nil on failure.
 */
int MOAIBox2DBody::_addEdges ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	float unitsToMeters = self->GetUnitsToMeters ();
	u32 totalVerts = lua_objlen ( state, 2 ) / 2;
	
	if (totalVerts) {
		
		b2Vec2 * verts = (b2Vec2 *)alloca(sizeof(b2Vec2) * totalVerts);
		int numVerts = MOAIBox2DFixture::LoadVerts( state, 2, verts, totalVerts, unitsToMeters );
		
		if ( numVerts ) {
			MOAILuaState retstate ( L );
			lua_createtable ( retstate, numVerts, 0 );
			int idx = 1;
			b2EdgeShape edgeShape;
			for ( u32 i = 0; i < totalVerts; i+=2) {
				edgeShape.Set(verts[i], verts[i+1]);
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &edgeShape;
				MOAIBox2DFixture* fixture = new MOAIBox2DFixture ();
				fixture->SetFixture ( self->mBody->CreateFixture ( &fixtureDef ));
				fixture->SetWorld ( self->mWorld );
				self->mWorld->LuaRetain ( fixture );
				lua_pushnumber ( retstate, idx );
				fixture->PushLuaUserdata ( retstate );				
				lua_settable ( retstate, -3 );
				idx++;
			}
			
			return 1;
		}
	}	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	addPolygon
	@text	Create and add a polygon fixture to the body.
	
	@in		MOAIBox2DBody self
	@in		table verts					Array containg vertex coordinate components in units, world coordinates, converted to meters. ( t[1] = x0, t[2] = y0, t[3] = x1, t[4] = y1... )
	@out	MOAIBox2DFixture fixture	Returns nil on failure.
*/
int MOAIBox2DBody::_addPolygon ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )

	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}

	float unitsToMeters = self->GetUnitsToMeters ();

	b2Vec2 verts [ b2_maxPolygonVertices ];
	
	// TODO: add error checking
	int numVerts = MOAIBox2DFixture::LoadVerts ( state, 2, verts, b2_maxPolygonVertices, unitsToMeters );
	
	if (( numVerts < 3 ) || ( numVerts > b2_maxPolygonVertices )) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_InvalidVertexCount_D, numVerts );
		return 0;
	}
	
	if ( numVerts ) {
		
		b2PolygonShape polyShape;
		polyShape.Set ( verts, numVerts );
		
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &polyShape;
		
		MOAIBox2DFixture* fixture = new MOAIBox2DFixture ();
		fixture->SetFixture ( self->mBody->CreateFixture ( &fixtureDef ));
		fixture->SetWorld ( self->mWorld );
		self->mWorld->LuaRetain ( fixture );

		fixture->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	addRect
	@text	Create and add a rect fixture to the body.
	
	@in		MOAIBox2DBody self
	@in		number xMin	in units, world coordinates, converted to meters
	@in		number yMin	in units, world coordinates, converted to meters
	@in		number xMax	in units, world coordinates, converted to meters
	@in		number yMax	in units, world coordinates, converted to meters
	@in		number angle
	@out	MOAIBox2DFixture fixture
*/
int MOAIBox2DBody::_addRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "UNNNN" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	USRect rect = state.GetRect < float >( 2 );
	rect.Bless ();
	
	float angle = state.GetValue < float >( 6, 0.0f );
	
	float hx = rect.Width () * 0.5f * unitsToMeters;
	float hy = rect.Height () * 0.5f * unitsToMeters;
	
	b2Vec2 center;
	center.x = ( rect.mXMin * unitsToMeters ) + hx;
	center.y = ( rect.mYMin * unitsToMeters ) + hy;
	
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox ( hx, hy, center, angle );
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polygonShape;
	
	MOAIBox2DFixture* fixture = new MOAIBox2DFixture ();
	fixture->SetFixture ( self->mBody->CreateFixture ( &fixtureDef ));
	fixture->SetWorld ( self->mWorld );
	self->mWorld->LuaRetain ( fixture );

	fixture->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	applyAngularImpulse
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@in		number angularImpulse	in kg * units / s, converted to kg * m / s
	@out	nil
*/
int MOAIBox2DBody::_applyAngularImpulse ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "UN" )
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	float impulse = state.GetValue < float >( 2, 0.0f ) * self->GetUnitsToMeters();
	self->mBody->ApplyAngularImpulse ( impulse );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	applyForce
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@in		number forceX	in kg * units / s^2, converted to N [kg * m / s^2]
	@in		number forceY	in kg * units / s^2, converted to N [kg * m / s^2]
	@opt	number pointX	in units, world coordinates, converted to meters
	@opt	number pointY	in units, world coordinates, converted to meters
	@out	nil
*/
int MOAIBox2DBody::_applyForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "UNN" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	b2Vec2 force;
	force.x = state.GetValue < float >( 2, 0.0f ) * unitsToMeters;
	force.y = state.GetValue < float >( 3, 0.0f ) * unitsToMeters;
	
	b2Vec2 point;
	point.x = state.GetValue < float >( 4, 0.0f ) * unitsToMeters;
	point.y = state.GetValue < float >( 5, 0.0f ) * unitsToMeters;
	
	self->mBody->ApplyForce ( force, point );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	applyLinearImpulse
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@in		number impulseX	in kg * units / s, converted to kg * m / s
	@in		number impulseY	in kg * units / s, converted to kg * m / s
	@opt	number pointX	in units, world coordinates, converted to meters
	@opt	number pointY	in units, world coordinates, converted to meters
	@out	nil
*/
int MOAIBox2DBody::_applyLinearImpulse ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "UNN" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	b2Vec2 impulse;
	impulse.x = state.GetValue < float >( 2, 0.0f ) * unitsToMeters;
	impulse.y = state.GetValue < float >( 3, 0.0f ) * unitsToMeters;
	
	b2Vec2 point;
	point.x = state.GetValue < float >( 4, 0.0f ) * unitsToMeters;
	point.y = state.GetValue < float >( 5, 0.0f ) * unitsToMeters;
	
	self->mBody->ApplyLinearImpulse ( impulse, point );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	applyTorque
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@opt	number torque	in (kg * units / s^2) * units, converted to N-m. Default value is 0.
	@out	nil
*/
int MOAIBox2DBody::_applyTorque ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	float unitsToMeters = self->GetUnitsToMeters();
	/* Convert from N-m (kg m / s^2) * m => (kg unit / s^2) * unit */
	float torque = state.GetValue < float >( 2, 0.0f ) * unitsToMeters * unitsToMeters;
	self->mBody->ApplyTorque ( torque );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	destroy
	@text	Schedule body for destruction.
	
	@in		MOAIBox2DBody self
	@out	nil
*/
int MOAIBox2DBody::_destroy ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	if ( self->mWorld ) {
		self->mWorld->ScheduleDestruction ( *self );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getAngle
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@out	number angle		Angle in degrees, converted from radians
*/
int MOAIBox2DBody::_getAngle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	float angle = self->mBody->GetAngle () * ( float )R2D;
	lua_pushnumber ( state, angle );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getAngularVelocity
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@out	number omega		Anglular velocity in degrees/s, converted from radians/s
*/
int MOAIBox2DBody::_getAngularVelocity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	float omega = self->mBody->GetAngularVelocity () * ( float )R2D;
	lua_pushnumber ( state, omega );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getInertia
	@text   See Box2D documentation.

	@in		MOAIBox2DBody self
	@out	number inertia Calculated inertia (based on last call to resetMassData()). In kg * unit/s^s, converted from kg*m/s^2.
*/
int MOAIBox2DBody::_getInertia ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}

	float unitsToMeters = self->GetUnitsToMeters();
	float inertia = self->mBody->GetInertia();
	inertia /= unitsToMeters;
	
	lua_pushnumber ( L, inertia);
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getLinearVelocity
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@out	number velocityX	in unit/s, converted from m/s
	@out	number velocityY	in unit/s, converted from m/s
*/
int MOAIBox2DBody::_getLinearVelocity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	b2Vec2 velocity = self->mBody->GetLinearVelocity ();
	lua_pushnumber ( state, velocity.x / unitsToMeters );
	lua_pushnumber ( state, velocity.y / unitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getLocalCenter
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@out	number centerX	in units, local coordinates, converted from meters
	@out	number centerY	in units, local coordinates, converted from meters
*/
int MOAIBox2DBody::_getLocalCenter ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	b2Vec2 center = self->mBody->GetLocalCenter ();
	lua_pushnumber ( state, center.x / unitsToMeters );
	lua_pushnumber ( state, center.y / unitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getMass
	@text   See Box2D documentation.

	@in		MOAIBox2DBody self
	@out	number Mass Calculated mass in kg (based on last call to resetMassData()).
*/
int MOAIBox2DBody::_getMass ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	lua_pushnumber ( L, self->mBody->GetMass ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getPosition
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@out	number positionX	in units, world coordinates, converted from meters
	@out	number positionY	in units, world coordinates, converted from meters
*/
int MOAIBox2DBody::_getPosition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	b2Vec2 position = self->mBody->GetPosition ();
	lua_pushnumber ( state, position.x / unitsToMeters );
	lua_pushnumber ( state, position.y / unitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getWorldCenter
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@out	number worldX	in units, world coordinates, converted from meters
	@out	number worldY	in units, world coordinates, converted from meters
*/
int MOAIBox2DBody::_getWorldCenter ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	b2Vec2 center = self->mBody->GetWorldCenter ();
	lua_pushnumber ( state, center.x / unitsToMeters );
	lua_pushnumber ( state, center.y / unitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	isActive
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@out	boolean isActive
*/
int MOAIBox2DBody::_isActive ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	bool isActive = self->mBody->IsActive ();
	lua_pushboolean ( state, isActive );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isAwake
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@out	boolean isAwake
*/
int MOAIBox2DBody::_isAwake ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	bool isAwake = self->mBody->IsAwake ();
	lua_pushboolean ( state, isAwake );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isBullet
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@out	boolean isBullet
*/
int MOAIBox2DBody::_isBullet ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	bool isBullet = self->mBody->IsBullet ();
	lua_pushboolean ( state, isBullet );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isFixedRotation
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@out	boolean isFixedRotation
*/
int MOAIBox2DBody::_isFixedRotation ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	bool isFixedRotation = self->mBody->IsFixedRotation ();
	lua_pushboolean ( state, isFixedRotation );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	resetMassData
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@out	nil
*/
int MOAIBox2DBody::_resetMassData ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	self->mBody->ResetMassData ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setActive
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@opt	boolean active			Default value is false.
	@out	nil
*/
int MOAIBox2DBody::_setActive ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )

	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}

	bool active = state.GetValue < bool >( 2, false );
	self->mBody->SetActive ( active );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setAngularDamping
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@in		number damping
	@out	nil
*/
int MOAIBox2DBody::_setAngularDamping ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "UN" )
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	float damping = state.GetValue < float >( 2, 0.0f );
	self->mBody->SetAngularDamping ( damping );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setAngularVelocity
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@opt	number omega		Angular velocity in degrees/s, converted to radians/s. Default value is 0.
	@out	nil
*/
int MOAIBox2DBody::_setAngularVelocity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	float omega = state.GetValue < float >( 2, 0.0f ) * ( float )D2R;
	self->mBody->SetAngularVelocity ( omega );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setAwake
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@opt	boolean awake		Default value is true.
	@out	nil
*/
int MOAIBox2DBody::_setAwake ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	bool awake = state.GetValue < bool >( 2, true );
	self->mBody->SetAwake ( awake );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setBullet
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@opt	boolean bullet		Default value is true.
	@out	nil
*/
int MOAIBox2DBody::_setBullet ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	bool bullet = state.GetValue < bool >( 2, true );
	self->mBody->SetBullet ( bullet );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setFixedRotation
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@opt	boolean fixedRotation		Default value is true.
	@out	nil
*/
int MOAIBox2DBody::_setFixedRotation ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	bool fixedRotation = state.GetValue < bool >( 2, true );
	self->mBody->SetFixedRotation ( fixedRotation );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLinearDamping
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@opt	number damping
	@out	nil
*/
int MOAIBox2DBody::_setLinearDamping ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "UN" )
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	float damping = state.GetValue < float >( 2, 0.0f );
	self->mBody->SetLinearDamping ( damping );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLinearVelocity
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@opt	number velocityX	in unit/s, converted to m/s. Default is 0.
	@opt	number velocityY	in unit/s, converted to m/s. Default is 0.
	@out	nil
*/
int MOAIBox2DBody::_setLinearVelocity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	b2Vec2 v;
	v.x		= state.GetValue < float >( 2, 0.0f ) * unitsToMeters;
	v.y		= state.GetValue < float >( 3, 0.0f ) * unitsToMeters;
	
	self->mBody->SetLinearVelocity ( v );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMassData
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@in		number mass			in kg.
	@opt	number I			in kg*units^2, converted to kg * m^2. Default is previous value for I.
	@opt	number centerX		in units, local coordinates, converted to meters. Default is previous value for centerX.
	@opt	number centerY		in units, local coordinates, converted to meters. Default is previous value for centerY.
	@out	nil
*/
int MOAIBox2DBody::_setMassData ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "UN" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	if ( self->mWorld->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	b2MassData massData;
	self->mBody->GetMassData ( &massData );
	
	massData.mass		= state.GetValue < float >( 2, massData.mass );
	massData.I			= state.GetValue < float >( 3, massData.I ) * unitsToMeters * unitsToMeters;
	massData.center.x	= state.GetValue < float >( 4, massData.center.x ) * unitsToMeters;
	massData.center.y	= state.GetValue < float >( 5, massData.center.y ) * unitsToMeters;
	
	self->mBody->SetMassData ( &massData );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTransform
	@text	See Box2D documentation.
	
	@in		MOAIBox2DBody self
	@opt	number positionX		in units, world coordinates, converted to meters. Default is 0.
	@opt	number positionY		in units, world coordinates, converted to meters. Default is 0.
	@opt	number angle			In degrees, converted to radians. Default is 0.
	@out	nil
*/
int MOAIBox2DBody::_setTransform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mBody ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_MissingInstance );
		return 0;
	}
	
	if ( self->mWorld->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	b2Vec2 position;
	position.x		= state.GetValue < float >( 2, 0.0f ) * unitsToMeters;
	position.y		= state.GetValue < float >( 3, 0.0f ) * unitsToMeters;
	float angle		= state.GetValue < float >( 4, 0.0f ) * ( float )D2R;
	
	self->mBody->SetTransform ( position, angle );
	self->ScheduleUpdate ();
	
	return 0;
}

//================================================================//
// MOAIBox2DBody
//================================================================//

//----------------------------------------------------------------//
void MOAIBox2DBody::Destroy () {

	if ( this->mBody ) {
		b2World* world = this->mWorld->mWorld;
		world->DestroyBody ( this->mBody );
		this->mBody = 0;
	}
}

//----------------------------------------------------------------//
MOAIBox2DBody::MOAIBox2DBody () :
	mBody ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransformBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DBody::~MOAIBox2DBody () {

	this->Destroy ();
}

//----------------------------------------------------------------//
void MOAIBox2DBody::OnDepNodeUpdate () {

	if ( this->mBody ) {
		
		b2Transform transform = this->mBody->GetTransform ();
		float scale = 1.0f / this->GetUnitsToMeters ();
		
		float* m = this->mLocalToWorldMtx.m;
		
		m [ USAffine3D::C0_R0 ] = ( float )transform.q.GetXAxis().x;
		m [ USAffine3D::C0_R1 ] = ( float )transform.q.GetXAxis().y;

		m [ USAffine3D::C1_R0 ] = ( float )transform.q.GetYAxis().x;
		m [ USAffine3D::C1_R1 ] = ( float )transform.q.GetYAxis().y;

		m [ USAffine3D::C3_R0 ] = ( float )transform.p.x * scale;
		m [ USAffine3D::C3_R1 ] = ( float )transform.p.y * scale;
		
		this->mWorldToLocalMtx.Inverse ( this->mLocalToWorldMtx );
	}
}

//----------------------------------------------------------------//
void MOAIBox2DBody::RegisterLuaClass ( MOAILuaState& state ) {

	MOAITransformBase::RegisterLuaClass ( state );
	
	state.SetField ( -1, "DYNAMIC",		( u32 )b2_dynamicBody );
	state.SetField ( -1, "KINEMATIC",	( u32 )b2_kinematicBody );
	state.SetField ( -1, "STATIC",		( u32 )b2_staticBody );
}

//----------------------------------------------------------------//
void MOAIBox2DBody::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAITransformBase::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "addChain",				_addChain },
		{ "addCircle",				_addCircle },
		{ "addEdges",               _addEdges },
		{ "addPolygon",				_addPolygon },
		{ "addRect",				_addRect },
		{ "applyAngularImpulse",	_applyAngularImpulse },
		{ "applyForce",				_applyForce },
		{ "applyLinearImpulse",		_applyLinearImpulse },
		{ "applyTorque",			_applyTorque },
		{ "destroy",				_destroy },
		{ "getAngle",				_getAngle },
		{ "getAngularVelocity",		_getAngularVelocity },
		{ "getInertia",				_getInertia },
		{ "getLinearVelocity",		_getLinearVelocity },
		{ "getLocalCenter",			_getLocalCenter },
		{ "getMass",				_getMass },
		{ "getPosition",			_getPosition },
		{ "getWorldCenter",			_getWorldCenter },
		{ "isActive",				_isActive },
		{ "isAwake",				_isAwake },
		{ "isBullet",				_isBullet },
		{ "isFixedRotation",		_isFixedRotation },
		{ "resetMassData",			_resetMassData },
		{ "setActive",				_setActive },
		{ "setAngularDamping",		_setAngularDamping },
		{ "setAngularVelocity",		_setAngularVelocity },
		{ "setAwake",				_setAwake },
		{ "setBullet",				_setBullet },
		{ "setFixedRotation",		_setFixedRotation },
		{ "setLinearDamping",		_setLinearDamping },
		{ "setLinearVelocity",		_setLinearVelocity },
		{ "setMassData",			_setMassData },
		{ "setTransform",			_setTransform },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIBox2DBody::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {
	// TODO: these values may need to be cached for performance reasons
	if ( MOAITransform::MOAITransformAttr::Check ( attrID )) {
		const b2Transform & xform = mBody->GetTransform();
		
		switch ( UNPACK_ATTR ( attrID )) {
			case MOAITransform::ATTR_X_LOC: {
				float x = attrOp.Apply ( xform.p.x, op, MOAIAttrOp::ATTR_READ_WRITE ) * this->GetUnitsToMeters ();
				mBody->SetTransform ( b2Vec2( x, xform.p.y), xform.q.GetAngle() );
				return true;
			}

			case MOAITransform::ATTR_Y_LOC: {
				float y = attrOp.Apply ( xform.p.y, op, MOAIAttrOp::ATTR_READ_WRITE ) * this->GetUnitsToMeters ();
				mBody->SetTransform ( b2Vec2( xform.p.x, y ), xform.q.GetAngle() );
				return true;	
			}

			case MOAITransform::ATTR_Z_ROT: {
				float angle = attrOp.Apply ( xform.q.GetAngle(), op, MOAIAttrOp::ATTR_READ_WRITE );				
				mBody->SetTransform ( xform.p,  ( float )((angle * D2R) + M_PI_4 ));
				return true;	
			}


		}
	}
	return MOAITransformBase::ApplyAttrOp (attrID, attrOp, op );
}

//----------------------------------------------------------------//
void MOAIBox2DBody::SetBody ( b2Body* body ) {

	this->mBody = body;
	body->SetUserData ( this );
}

#endif
