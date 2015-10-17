// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <float.h>
#include <moai-spine/MOAISpineSkeleton.h>
#include <moai-spine/MOAISpineBone.h>
#include <moai-spine/MOAISpineSkeletonData.h>
#include <moai-spine/MOAISpineSlot.h>

//================================================================//
// Spine event listener
//================================================================//
static void callback (spAnimationState* state, int trackIndex, spEventType type, spEvent* event, int loopCount) {
	((MOAISpineSkeleton*) state->rendererObject )->OnAnimationEvent ( trackIndex, type, event, loopCount );
}

//================================================================//
// lua
//================================================================//


//----------------------------------------------------------------//
/**	@name	addAnimation
    @text   Append another animation to animation track.
 
	@in		MOAISpineSkeleton self
	@in		number	track Id
	@in		string	animation name
	@in		bool	loop
	@in		number	delay
	@out	nil
 */
int MOAISpineSkeleton::_addAnimation ( lua_State *L ) {
	MOAI_LUA_SETUP( MOAISpineSkeleton, "UNS" )
	
	int trackId = state.GetValue < int >( 2, 0 ) - 1;
	cc8* name = state.GetValue < cc8* >( 3, "");
	bool loop = state.GetValue < bool >( 4, false );
	float delay = state.GetValue < float >( 5, 0.f );
	
	if ( !self->mSkeleton || !self->mAnimationState ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "MOAISpineSkeleton not initialized \n" );
		return 0;
	}
	if ( trackId < 0 ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "Track id is less than 1. Indexing starts with 1 \n" );
		return 0;
	}
	
	self->AddAnimation ( trackId, name, loop, delay );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	clearAllTracks

	@in		MOAISpineSkeleton self
	@out	nil
*/
int MOAISpineSkeleton::_clearAllTracks ( lua_State *L ) {
	MOAI_LUA_SETUP( MOAISpineSkeleton, "U" )
	
	if ( !self->mSkeleton || !self->mAnimationState ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "MOAISpineSkeleton not initialized \n" );
		return 0;
	}
	self->ClearAllTracks ();
	return 0;
}

//----------------------------------------------------------------//
/**	@name	clearTrack

	@in		MOAISpineSkeleton self
	@in		number track
	@out	nil
*/
int MOAISpineSkeleton::_clearTrack ( lua_State *L ) {
	MOAI_LUA_SETUP( MOAISpineSkeleton, "UN" )
	
	int trackId = state.GetValue < int >( 2, 1 ) - 1;
	
	if ( !self->mSkeleton || !self->mAnimationState ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "MOAISpineSkeleton not initialized \n" );
		return 0;
	}
	if ( trackId < 0 ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "Track id is less than 1. Indexing starts with 1 \n" );
		return 0;
	}
	
	self->ClearTrack ( trackId );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getAttachmentVertices
	@text	Return all attachment vertices in MOAISkeleton coordinate space.
	
	@in		MOAISpineSkeleton self
	@in		string	slot name
	@in		string	attachment name
	@out	table	vertices: {x1, y1, x2, y2, x3, y3, ...}
*/
int MOAISpineSkeleton::_getAttachmentVertices ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAISpineSkeleton, "USS" );
	
	if ( !self->mSkeleton || !self->mAnimationState ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "MOAISpineSkeleton not initialized \n" );
		return 0;
	}
	
	cc8* slotName	= state.GetValue < cc8* >( 2, "" );
	cc8* attachName = state.GetValue < cc8* >( 3, "" );
	
	spSlot* slot = spSkeleton_findSlot ( self->mSkeleton, slotName );
	if ( slot == 0 ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "[getAttachmentVertices]: Slot not found - %s\n", slotName );
		return 0;
	}
	
	spAttachment* attach = spSkeleton_getAttachmentForSlotName ( self->mSkeleton, slotName, attachName );
	if ( attach == 0 ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "[getAttachmentVertices]: Attachment not found - %s\n", attachName );
		return 0;
	}
	
	switch ( attach->type ) {
		case SP_ATTACHMENT_REGION:
			self->mVertices.SetTop ( 8 );
			spRegionAttachment_computeWorldVertices (( spRegionAttachment* ) attach, slot->bone, self->mVertices );
			break;
			
		case SP_ATTACHMENT_BOUNDING_BOX: {
			
			spBoundingBoxAttachment* bb = ( spBoundingBoxAttachment* ) attach;
			self->mVertices.SetTop ( bb->verticesCount );
			spBoundingBoxAttachment_computeWorldVertices ( bb, slot->bone, self->mVertices );
			break;
		}
		
		case SP_ATTACHMENT_MESH: {
			
			spMeshAttachment* mesh = ( spMeshAttachment* ) attach;
			self->mVertices.SetTop ( mesh->verticesCount );
			spMeshAttachment_computeWorldVertices ( mesh, slot, self->mVertices );
			break;
		}

		case SP_ATTACHMENT_SKINNED_MESH: {
			
			spSkinnedMeshAttachment* mesh = ( spSkinnedMeshAttachment* ) attach;
			self->mVertices.SetTop ( mesh->uvsCount );
			spSkinnedMeshAttachment_computeWorldVertices ( mesh, slot, self->mVertices );
			break;
		}
	}
	
	lua_newtable ( L );
	state.WriteArray < float >( self->mVertices.GetTop(), self->mVertices );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getBone
	@text	Return MOAITransform that is bound to skeleton bone. 
			On first call it will create full hierarchy of MOAISpineBones
			from requested bone to the root bone. It is needed for proper
			transform inheritance.

	@in		MOAISpineSkeleton self
	@in		string	skeleton bone name
	@out	MOAISpineBone	bone
*/
int MOAISpineSkeleton::_getBone ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAISpineSkeleton, "US" );
	
	cc8* boneName = state.GetValue < cc8* >( 2, 0 );
	
	spBone* bone = spSkeleton_findBone ( self->mSkeleton, boneName );
	if ( !bone ) {
		return 0;
	}
	
	self->AffirmBoneHierarchy ( bone );
	self->mBoneTransformMap [ boneName ]->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getDuration
    @text   Returns animation duration
 
    @in		MOAISpineSkeleton self
    @in		string	animation name
    @out	number	duration
*/
int MOAISpineSkeleton::_getDuration ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAISpineSkeleton, "US" );
    
    cc8* animationName = state.GetValue < cc8* >( 2, "" );
    if ( !self->mSkeleton ) {
        MOAILogF ( state, ZLLog::LOG_ERROR, "MOAISpineSkeleton not initialized \n" );
		return 0;
    }
    
    spAnimation* animation = spSkeletonData_findAnimation ( self->mSkeleton->data, animationName );
    if ( !animation ) {
        MOAILogF ( state, ZLLog::LOG_ERROR, "MOAISpineSkeleton: animation not found \n" );
		return 0;
    }
    state.Push ( animation->duration );
    
    return 1;
}

//----------------------------------------------------------------//
/**	@name	getSlot
	@text	Return MOAIColor that is bound to skeleton slot.
			MOAISpineSlots are lazily initialized.

	@in		MOAISpineSkeleton self
	@in		string	skeleton slot name
	@out	MOAISpineSlot	slot
 */
int MOAISpineSkeleton::_getSlot ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAISpineSkeleton, "US" );
	
	cc8* slotName = state.GetValue < cc8* >( 2, 0 );
	
	spSlot* slot = spSkeleton_findSlot ( self->mSkeleton, slotName );
	if ( !slot ) {
		return 0;
	}
	
	if ( self->mSlotColorMap.contains( slotName ) ) {
		self->mSlotColorMap [ slotName ]->PushLuaUserdata ( state );
		return 1;
	}
	
	MOAISpineSlot* spineSlot = new MOAISpineSlot();
	spineSlot->SetSlot ( slot );
	self->LuaRetain ( spineSlot );
	self->mSlotColorMap [ slotName ] = spineSlot;
	spineSlot->PushLuaUserdata ( state );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	init

	@in		MOAISpineSkeleton self
	@in		MOAISpineSkeletonData skeleton data
	@out	nil
 
*/
int MOAISpineSkeleton::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpineSkeleton, "UU" )
	
	MOAISpineSkeletonData* data = state.GetLuaObject < MOAISpineSkeletonData >( 2, true );
	
	if ( !data->mSkeletonData ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "Empty skeleton data \n" );
		return 0;
	}
	self->mSkeletonData.Set ( *self, data );
	self->Init ( data->mSkeletonData );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	initAnimationState

	@in		MOAISpineSkeleton self
	@out	nil
*/
int MOAISpineSkeleton::_initAnimationState ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAISpineSkeleton, "U" )
	
	if ( !self->mSkeleton ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "MOAISpineSkeleton not initialized \n" );
		return 0;
	}
	spAnimationStateData* data = spAnimationStateData_create ( self->mSkeleton->data );
	self->InitAnimationState ( data );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setAnimation

	@in		MOAISpineSkeleton self
	@in		number	track Id (indexing starting with 1)
	@in		string	animation name
	@in		bool	loop. Default is false
	@out	nil
*/
int MOAISpineSkeleton::_setAnimation ( lua_State *L ) {
	MOAI_LUA_SETUP( MOAISpineSkeleton, "UNS" )
	
	int trackId = state.GetValue < int >( 2, 1 ) - 1;
	cc8* name = state.GetValue < cc8* >( 3, "");
	bool loop = state.GetValue < bool >( 4, false );
	
	if ( !self->mSkeleton || !self->mAnimationState ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "MOAISpineSkeleton not initialized (ensure that initAnimationState was called) \n" );
		return 0;
	}
	if ( trackId < 0 ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "Track id is less than 1. Indexing starts with 1 \n" );
		return 0;
	}
	
	self->SetAnimation ( trackId, name, loop );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setAttachment
 
	@in		MOAISpineSkeleton self
	@in		string	slotName
	@in		string	attachmentName or nil to disable attachment
	@out	bool	success
*/
int MOAISpineSkeleton::_setAttachment ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpineSkeleton, "US" );
	
	cc8* slotName = state.GetValue < cc8* >( 2, 0 );
	cc8* attachmentName = state.GetValue < cc8* >( 3, 0 );
	
	if ( !self->mSkeleton ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "MOAISpineSkeleton not initialized \n" );
		state.Push ( false );
		return 1;
	}
	state.Push (( bool ) spSkeleton_setAttachment ( self->mSkeleton, slotName, attachmentName ));
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setBonesToSetupPose

	@in		MOAISpineSkeleton self
	@out	nil
*/
int MOAISpineSkeleton::_setBonesToSetupPose	( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpineSkeleton, "U" );
	
	if ( !self->mSkeleton ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "MOAISpineSkeleton not initialized \n" );
		return 0;
	}
	spSkeleton_setBonesToSetupPose ( self->mSkeleton );
	self->UpdateSkeleton ();
	self->UpdateBounds ();
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setComputeBounds
	@text   Flag controlling whether to recompute bounds each frame.
			When disabled, bounds will be initialized to setup pose.
			Default is false.

	@in		MOAISpineSkeleton self
	@in		bool	computeBounds
	@out	nil
*/
int MOAISpineSkeleton::_setComputeBounds ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAISpineSkeleton, "U" );
	
	self->mComputeBounds = state.GetValue < bool >( 2, false );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setFlip
 
	@in		MOAISpineSkeleton self
	@in		bool	flipX
	@in		bool	flipY
	@out	nil
 */
int MOAISpineSkeleton::_setFlip ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAISpineSkeleton, "U" );
	
	bool flipX = state.GetValue < bool >( 2, false );
	bool flipY = state.GetValue < bool >( 3, false );
	
	if ( !self->mSkeleton ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "MOAISpineSkeleton not initialized \n" );
		return 0;
	}
	self->mSkeleton->flipX = flipX;
	self->mSkeleton->flipY = flipY;
    
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMix
 
	@in		MOAISpineSkeleton self
	@in		string	from
	@in		string	to
	@in		number	duration
	@out	nil
*/
int MOAISpineSkeleton::_setMix ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAISpineSkeleton, "USS" );
	
	cc8* fromName = state.GetValue < cc8* >( 2, "" );
	cc8* toName   = state.GetValue < cc8* >( 3, "" );
	float duration  = state.GetValue < float >( 4, 0.1 );
	
	if ( !self->mSkeleton || !self->mAnimationState ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "MOAISpineSkeleton not initialized (ensure that initAnimationState was called) \n" );
		return 0;
	}
	self->SetMix ( fromName, toName, duration );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSkin

	@in		MOAISpineSkeleton self
	@in		string skin name
	@out	nil
*/
int MOAISpineSkeleton::_setSkin	( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpineSkeleton, "US" )
	
	cc8* skinName = state.GetValue < cc8* >( 2, 0 );
	
	if ( !self->mSkeleton ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "MOAISpineSkeleton not initialized \n" );
		return 0;
	}
	spSkeleton_setSkinByName ( self->mSkeleton, skinName );
	spSkeleton_setSlotsToSetupPose ( self->mSkeleton );
	self->UpdateSkeleton ();
	self->UpdateBounds ();
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSlotsToSetupPose

	@in		MOAISpineSkeleton self
	@out	nil
*/
int MOAISpineSkeleton::_setSlotsToSetupPose	( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpineSkeleton, "U" );
	
	if ( !self->mSkeleton ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "MOAISpineSkeleton not initialized \n" );
		return 0;
	}
	spSkeleton_setSlotsToSetupPose ( self->mSkeleton );
	self->UpdateSkeleton ();
	self->UpdateBounds ();
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSlotColor

	@in		MOAISpineSkeleton self
	@in		string	slotName
	@in		number	r
	@in		number	g
	@in		number	b
	@in		number	a
	@out	nil
*/
int MOAISpineSkeleton::_setSlotColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpineSkeleton, "U" );
	
	if ( !self->mSkeleton ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "MOAISpineSkeleton not initialized \n" );
		return 0;
	}
	
	cc8* slotName = state.GetValue < cc8* >( 2, 0 );
	
	spSlot* slot = spSkeleton_findSlot ( self->mSkeleton, slotName );
	if ( !slot ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "Slot %s not found \n", slotName );
		return 0;
	}
	
	slot->r = state.GetValue < float >( 3, 1.0f );
	slot->g = state.GetValue < float >( 4, 1.0f );
	slot->b = state.GetValue < float >( 5, 1.0f );
	slot->a = state.GetValue < float >( 6, 1.0f );
	
	return 0;
}


//----------------------------------------------------------------//
/**	@name	setTime
	@text   Set time for given animation track
	
	@in		MOAISpineSkeleton self
	@in     number  track id
	@in     number  time
	@out	nil
*/
int MOAISpineSkeleton::_setTime ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAISpineSkeleton, "UNN" );
	
	int trackId = state.GetValue ( 2, 1 ) - 1;
	float time = state.GetValue < float >( 3, 0.0f );
	
	if ( !self->mSkeleton || !self->mAnimationState ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "MOAISpineSkeleton not initialized (ensure that initAnimationState was called) \n" );
		return 0;
	}
	if ( trackId < 0 ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "Track id is less than 1. Indexing starts with 1 \n" );
		return 0;
	}
	
	spTrackEntry* trackEntry = spAnimationState_getCurrent ( self->mAnimationState, trackId );
	if ( trackEntry ) {
		trackEntry->time = time;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setToSetupPose
	
	@in		MOAISpineSkeleton self
	@out	nil
*/
int MOAISpineSkeleton::_setToSetupPose ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpineSkeleton, "U" );
	
	if ( !self->mSkeleton ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, "MOAISpineSkeleton not initialized \n" );
		return 0;
	}
	spSkeleton_setToSetupPose ( self->mSkeleton );
	self->UpdateSkeleton ();
	self->UpdateBounds ();
	return 0;
}

//================================================================//
// MOAISpineSkeleton
//================================================================//

//----------------------------------------------------------------//
void MOAISpineSkeleton::AddAnimation ( int trackId, cc8* name, bool loop, float delay ) {
	spAnimation* anim = spSkeletonData_findAnimation ( mSkeleton->data, name );
	
	if ( anim ) {
		spAnimationState_addAnimation ( mAnimationState, trackId, anim, loop, delay );
	}
}

//----------------------------------------------------------------//
void MOAISpineSkeleton::AffirmBoneHierarchy ( spBone* bone ) {
	// create all missing MOAISpineBones first
	for ( spBone* boneIt = bone ; boneIt; boneIt = boneIt->parent ) {
		if ( mBoneTransformMap.contains ( boneIt->data->name )) {
			continue;
		}
		
		MOAISpineBone* luaBone = new MOAISpineBone();
		luaBone->SetBone ( boneIt );
		this->LuaRetain ( luaBone );
		mBoneTransformMap [ boneIt->data->name ] = luaBone;
		
		if ( boneIt == mSkeleton->bones [ 0 ] ) {
			mRootBone = luaBone;
			luaBone->SetAsRootBone ( this );
		}
	}
	
	// create attr links
	for ( spBone* boneIt = bone ; boneIt->parent; boneIt = boneIt->parent ) {
		MOAISpineBone* curBone = mBoneTransformMap [ boneIt->data->name ];
		MOAISpineBone* parent = mBoneTransformMap [ boneIt->parent->data->name ];
		curBone->SetAttrLink ( PACK_ATTR ( MOAITransform, INHERIT_TRANSFORM ), parent, PACK_ATTR ( MOAITransformBase, TRANSFORM_TRAIT ));
	}
}

//----------------------------------------------------------------//
void MOAISpineSkeleton::ClearAllTracks () {
	spAnimationState_clearTracks ( mAnimationState );
}

//----------------------------------------------------------------//
void MOAISpineSkeleton::ClearTrack ( int trackId ) {
	spAnimationState_clearTrack ( mAnimationState, trackId );
}

//----------------------------------------------------------------//
void MOAISpineSkeleton::Draw ( int subPrimID, float lod ) {
	UNUSED ( subPrimID );
	
	if ( !this->IsVisible ( lod )) return;
	if ( !this->mSkeleton ) return;
    
    this->LoadGfxState ();
    this->LoadVertexTransform ();
    this->LoadUVTransform ();
    
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
    
	MOAIShader* shader = this->mMaterialBatch ? this->mMaterialBatch->RawGetShader ( 0 ) : 0;
	if ( !shader ) {
		shader = MOAIShaderMgr::Get ().GetShader ( MOAIShaderMgr::DECK2D_SHADER );
	}
	
	gfxDevice.SetShader ( shader );	
    gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
    gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
    
	MOAIBlendMode normal;
	MOAIBlendMode additive;
	normal.SetBlend ( MOAIBlendMode::BLEND_NORMAL );
	additive.SetBlend ( MOAIBlendMode::BLEND_ADD );
	
	MOAIQuadBrush brush;
	brush.BindVertexFormat ( gfxDevice );
	
	for ( u32 i = 0; i < mSkeleton->slotsCount; ++i ) {
		spSlot* slot = mSkeleton->drawOrder [ i ];
		if ( !slot->attachment ) {
			continue;
		}
		
		MOAITexture* texture;
		float*  uvs = 0;
		int     trianglesCount = 0;
		int*    triangles = 0;
		float r, g, b, a;
		
		switch ( slot->attachment->type ) {
			case SP_ATTACHMENT_REGION: {
				spRegionAttachment *attachment = ( spRegionAttachment* ) slot->attachment;
				texture = ( MOAITexture* ) (( spAtlasRegion* ) attachment->rendererObject )->page->rendererObject;
				uvs = attachment->uvs;
				
				r = attachment->r;
				g = attachment->g;
				b = attachment->b;
				a = attachment->a;
				
				mVertices.SetTop ( 8 );
				spRegionAttachment_computeWorldVertices ( attachment, slot->bone, mVertices );
				break;
			}
				
			case SP_ATTACHMENT_MESH: {
				spMeshAttachment *attachment = ( spMeshAttachment* ) slot->attachment;
				texture = ( MOAITexture* ) (( spAtlasRegion* ) attachment->rendererObject )->page->rendererObject;
				uvs = attachment->uvs;
				triangles = attachment->triangles;
				trianglesCount = attachment->trianglesCount;

				r = attachment->r;
				g = attachment->g;
				b = attachment->b;
				a = attachment->a;
				
				mVertices.SetTop ( attachment->verticesCount );
				spMeshAttachment_computeWorldVertices ( attachment, slot, mVertices );
				break;
			}
				
			case SP_ATTACHMENT_SKINNED_MESH: {
				spSkinnedMeshAttachment *attachment = ( spSkinnedMeshAttachment* ) slot->attachment;
				texture = ( MOAITexture* ) (( spAtlasRegion* ) attachment->rendererObject )->page->rendererObject;
				uvs = attachment->uvs;
				triangles = attachment->triangles;
				trianglesCount = attachment->trianglesCount;

				r = attachment->r;
				g = attachment->g;
				b = attachment->b;
				a = attachment->a;
				
				mVertices.SetTop ( attachment->uvsCount );
				spSkinnedMeshAttachment_computeWorldVertices ( attachment, slot, mVertices );
				break;
			}
				
			default:
				continue;
				break;
		}
		
		gfxDevice.SetTexture ( texture );
		
		if ( slot->data->additiveBlending ) {
			gfxDevice.SetBlendMode ( additive );
		}
		else {
			gfxDevice.SetBlendMode ( normal );
		}
		
		a = mSkeleton->a * slot->a * a;
		r = mSkeleton->r * slot->r * r;
		g = mSkeleton->g * slot->g * g;
		b = mSkeleton->b * slot->b * b;
		
        // premultiply alpha
		ZLColorVec slotColor (r * a, g * a, b * a, a);
		ZLColorVec baseColor = this->mColor;
		slotColor.Modulate ( baseColor );
		gfxDevice.SetPenColor ( slotColor );
		
		if ( trianglesCount > 0 ) {
			
			u32 vtxCount = ( u32 )( mVertices.GetTop () / 2 );
			
			gfxDevice.BeginPrimIndexed ( ZGL_PRIM_TRIANGLES, vtxCount, trianglesCount );

			for ( u32 i = 0; i < mVertices.GetTop (); i += 2 ) {
				gfxDevice.WriteVtx ( mVertices [ i ], mVertices [ i + 1 ], 0 );
				gfxDevice.WriteUV ( uvs [ i ], uvs [ i + 1 ] );
				gfxDevice.WriteFinalColor4b ();
			}

			for ( u32 i = 0; i < trianglesCount; ++i ) {
				gfxDevice.WriteIndex ( triangles [ i ]);
			}

			gfxDevice.EndPrimIndexed ();
		}
		else {
			brush.SetVerts ( mVertices );
			brush.SetUVs ( uvs );
			brush.Draw ();
		}
	}
}

//----------------------------------------------------------------//
void MOAISpineSkeleton::DrawDebug ( int subPrimID, float lod ) {
	MOAIGraphicsProp::DrawDebug ( subPrimID, lod );
	
	
}

//----------------------------------------------------------------//
void MOAISpineSkeleton::Init ( spSkeletonData *skeletonData ) {
	
	mSkeleton = spSkeleton_create ( skeletonData );
		
	this->UpdateSkeleton ();
}

//----------------------------------------------------------------//
void MOAISpineSkeleton::InitAnimationState ( spAnimationStateData *animData ) {
	
	mAnimationState = spAnimationState_create ( animData );
	mAnimationState->rendererObject = this;
	mAnimationState->listener = callback;
}

//----------------------------------------------------------------//
bool MOAISpineSkeleton::IsDone () {
	return false;
}

//----------------------------------------------------------------//
MOAISpineSkeleton::MOAISpineSkeleton ():
	mComputeBounds ( false ),
	mSkeleton ( 0 ),
	mAnimationState ( 0 ),
	mRootBone ( 0 ) {
		
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGraphicsProp )
		RTTI_EXTEND ( MOAIAction )
	RTTI_END
}

//----------------------------------------------------------------//
MOAISpineSkeleton::~MOAISpineSkeleton () {
	if ( mRootBone ) {
		mRootBone->SetAsRootBone ( 0 );
	}
	
	for ( BoneTransformIt it = mBoneTransformMap.begin (); it != mBoneTransformMap.end (); ++it ) {
		it->second->SetBone ( 0 );
		this->LuaRelease( it->second );
	}
	mBoneTransformMap.clear ();
	
	for ( SlotColorIt it = mSlotColorMap.begin (); it != mSlotColorMap.end (); ++it ) {
		it->second->SetSlot ( 0 );
		this->LuaRelease( it->second );
	}
	mSlotColorMap.clear ();
	
	if ( mAnimationState ) {
		spAnimationStateData_dispose ( mAnimationState->data );
		spAnimationState_dispose ( mAnimationState );
	}
	
	if ( mSkeleton ) {
		spSkeleton_dispose ( mSkeleton );
	}
	
	mSkeletonData.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAISpineSkeleton::OnAnimationEvent ( int trackIndex, spEventType type, spEvent* event, int loopCount ) {
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	switch ( type ) {
		case SP_ANIMATION_START:
			if ( this->PushListenerAndSelf ( EVENT_ANIMATION_START, state) ) {
				state.Push ( trackIndex );
				state.DebugCall ( 2, 0 );
			}
			break;
		
		case SP_ANIMATION_END:
			if ( this->PushListenerAndSelf ( EVENT_ANIMATION_END, state) ) {
				state.Push ( trackIndex );
				state.DebugCall ( 2, 0 );
			}
			break;
			
		case SP_ANIMATION_COMPLETE:
			if ( this->PushListenerAndSelf ( EVENT_ANIMATION_COMPLETE, state) ) {
				state.Push ( trackIndex );
				state.Push ( loopCount );
				state.DebugCall ( 3, 0 );
			}
			break;
			
		case SP_ANIMATION_EVENT:
			if ( this->PushListenerAndSelf ( EVENT_ANIMATION_EVENT, state) ) {
				state.Push ( trackIndex );
				state.Push ( event->data->name );
				state.Push ( event->intValue );
				state.Push ( event->floatValue );
				state.Push ( event->stringValue );
				state.DebugCall ( 6, 0 );
			}
			break;
	}
}

//----------------------------------------------------------------//
void MOAISpineSkeleton::OnDepNodeUpdate () {
	
	// Skeleton should be updated before prop for correct bounds
	this->UpdateSkeleton ();
	
	MOAIGraphicsProp::OnDepNodeUpdate ();
}

//----------------------------------------------------------------//
u32 MOAISpineSkeleton::OnGetModelBounds ( ZLBox &bounds ) {
	
    u32 size = mSkeleton->slotsCount;
    
    if ( size == 0) {
        return MOAIProp::BOUNDS_EMPTY;
    }
    
    if ( this->mComputeBounds ) {
        this->UpdateBounds ();
    }
    bounds.Init ( mSkeletonBounds );
    return MOAIProp::BOUNDS_OK;
}

//----------------------------------------------------------------//
void MOAISpineSkeleton::OnUpdate ( double step ) {
	if ( mSkeleton ) {
		spSkeleton_update ( mSkeleton, step );
		
		if ( mAnimationState ) {
			spAnimationState_update ( mAnimationState, step );
			spAnimationState_apply ( mAnimationState, mSkeleton );
		}
		
		if ( mRootBone ) {
			mRootBone->ScheduleUpdate ();
		}
		
		for ( SlotColorIt it = mSlotColorMap.begin (); it != mSlotColorMap.end (); ++it ) {
			it->second->ScheduleUpdate ();
		}
		
		this->ScheduleUpdate ();
	}
}

//----------------------------------------------------------------//
void MOAISpineSkeleton::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIGraphicsProp::RegisterLuaClass ( state );
	MOAIAction::RegisterLuaClass ( state );
	
	state.SetField ( -1, "EVENT_ANIMATION_START", ( u32 )EVENT_ANIMATION_START );
	state.SetField ( -1, "EVENT_ANIMATION_END", ( u32 )EVENT_ANIMATION_END );
	state.SetField ( -1, "EVENT_ANIMATION_COMPLETE", ( u32 )EVENT_ANIMATION_COMPLETE );
	state.SetField ( -1, "EVENT_ANIMATION_EVENT", ( u32 )EVENT_ANIMATION_EVENT );
}

//----------------------------------------------------------------//
void MOAISpineSkeleton::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIGraphicsProp::RegisterLuaFuncs ( state );
	MOAIAction::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "addAnimation", 			_addAnimation },
		{ "clearAllTracks", 		_clearAllTracks },
		{ "clearTrack", 			_clearTrack },
		{ "getAttachmentVertices",	_getAttachmentVertices },
		{ "getBone",				_getBone },
        { "getDuration",            _getDuration },
		{ "getSlot",				_getSlot },
		{ "init", 					_init },
		{ "initAnimationState", 	_initAnimationState },
		{ "setAnimation", 			_setAnimation },
		{ "setAttachment", 			_setAttachment },
		{ "setBonesToSetupPose", 	_setBonesToSetupPose },
		{ "setComputeBounds",		_setComputeBounds },
		{ "setFlip", 				_setFlip },
		{ "setMix", 				_setMix },
		{ "setSkin", 				_setSkin },
		{ "setSlotsToSetupPose", 	_setSlotsToSetupPose },
		{ "setSlotColor",			_setSlotColor },
		{ "setTime",                _setTime },
		{ "setToSetupPose", 		_setToSetupPose },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAISpineSkeleton::SetAnimation ( int trackId, cc8* name, bool loop ) {
	spAnimation* anim = spSkeletonData_findAnimation ( mSkeleton->data, name );
	
	if ( anim != 0 ) {
		spAnimationState_setAnimation ( mAnimationState, trackId, anim, loop);
	}
}

//----------------------------------------------------------------//
void MOAISpineSkeleton::SetMix ( cc8* fromName, cc8* toName, float duration ) {
	spAnimation* fromAnim = spSkeletonData_findAnimation ( mSkeleton->data, fromName );
	spAnimation* toAnim = spSkeletonData_findAnimation ( mSkeleton->data, toName );
	
	if ( fromAnim != 0 && toAnim != 0 ) {
		spAnimationStateData_setMix ( mAnimationState->data, fromAnim, toAnim, duration );
	}
}

//----------------------------------------------------------------//
void MOAISpineSkeleton::UpdateBounds () {

	mSkeletonBounds.Init ( FLT_MAX, -FLT_MAX, -FLT_MAX, FLT_MAX, 0.f, 0.f );
	for ( u32 i = 0; i < mSkeleton->slotsCount; ++i ) {
		spSlot* slot = mSkeleton->drawOrder [ i ];
		if ( !slot->attachment ) {
			continue;
		}
		
		switch ( slot->attachment->type ) {
			
			case SP_ATTACHMENT_BOUNDING_BOX: {
				spBoundingBoxAttachment *attachment = ( spBoundingBoxAttachment* ) slot->attachment;
				mVertices.SetTop ( attachment->verticesCount );
				spBoundingBoxAttachment_computeWorldVertices ( attachment, slot->bone, mVertices );
				break;
			}
				
			case SP_ATTACHMENT_REGION: {
				spRegionAttachment *attachment = ( spRegionAttachment* ) slot->attachment;
				mVertices.SetTop ( 8 );
				spRegionAttachment_computeWorldVertices ( attachment, slot->bone, mVertices );
				break;
			}
				
			case SP_ATTACHMENT_MESH: {
				spMeshAttachment *attachment = ( spMeshAttachment* ) slot->attachment;
				mVertices.SetTop ( attachment->verticesCount );
				spMeshAttachment_computeWorldVertices ( attachment, slot, mVertices );
				break;
			}
				
			case SP_ATTACHMENT_SKINNED_MESH: {
				spSkinnedMeshAttachment *attachment = ( spSkinnedMeshAttachment* ) slot->attachment;
				mVertices.SetTop ( attachment->uvsCount );
				spSkinnedMeshAttachment_computeWorldVertices ( attachment, slot, mVertices );
				break;
			}
				
			default:
				break;
		}
		
		for ( u32 i = 0; i < mVertices.GetTop (); i += 2 ) {
			mSkeletonBounds.Grow ( ZLVec3D ( mVertices [ i ], mVertices [ i + 1 ], 0.0f ));
		}
	}
}

//----------------------------------------------------------------//
void MOAISpineSkeleton::UpdateSkeleton () {
	
	if ( !mSkeleton ) {
		return;
	}
	
	spSkeleton_updateWorldTransform ( mSkeleton );
}



