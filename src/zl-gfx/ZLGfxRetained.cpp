// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/ZLGfxLogger.h>
#include <zl-gfx/ZLGfxRetained.h>

//================================================================//
// ZLGfxRetainedListenerRecord
//================================================================//

//----------------------------------------------------------------//
ZLGfxListenerRecord::ZLGfxListenerRecord () :
	mUserdata ( 0 ),
	mCallbackID ( UNKNOWN ),
	mEvent ( 0 ),
	mUniformAddr ( false ) {
}

//----------------------------------------------------------------//
ZLGfxListenerRecord::~ZLGfxListenerRecord () {
}


//================================================================//
// ZLGfxRetained
//================================================================//

//----------------------------------------------------------------//
void ZLGfxRetained::ActiveTexture ( u32 textureUnit ) {

	assert ( this->mStream );
	
	this->mStream->Write < u32 >( ZLGFX_ACTIVE_TEXTURE );
	this->mStream->Write < u32 >( textureUnit );
}

//----------------------------------------------------------------//
void ZLGfxRetained::AllocateResource ( ZLGfxResource& resource, ZLGfxEnum::_ param ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_ALLOCATE_RESOURCE );
	this->mStream->Write < ZLGfxResource* >( resource.RetainForWrite ());
	this->mStream->Write < u32 >( param );
}

//----------------------------------------------------------------//
void ZLGfxRetained::AttachShader ( ZLGfxResource& program, ZLGfxResource& shader ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_ATTACH_SHADER );
	this->mStream->Write < ZLGfxResource* >( program.RetainForWrite ());
	this->mStream->Write < ZLGfxResource* >( shader.RetainForWrite ());
}

//----------------------------------------------------------------//
void ZLGfxRetained::BindAttribLocation ( ZLGfxResource& program, u32 index, cc8* name ) {
	
	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_BIND_ATTRIB_LOCATION );
	this->mStream->Write < ZLGfxResource* >( program.RetainForWrite ());
	this->mStream->Write < u32 >( index );
	
	size_t size = strlen ( name );
	this->mStream->Write < size_t >( size );
	this->mStream->WriteBytes ( name, size );
}

//----------------------------------------------------------------//
void ZLGfxRetained::BindBuffer ( ZLGfxEnum::_ target, ZLGfxResource& handle ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_BIND_BUFFER );
	this->mStream->Write < u32 >( target );
	this->mStream->Write < ZLGfxResource* >( handle.RetainForWrite ());
}

//----------------------------------------------------------------//
void ZLGfxRetained::BindFramebuffer ( ZLGfxEnum::_ target, ZLGfxResource& handle ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_BIND_FRAMEBUFFER );
	this->mStream->Write < u32 >( target );
	this->mStream->Write < ZLGfxResource* >( handle.RetainForWrite ());
}

//----------------------------------------------------------------//
void ZLGfxRetained::BindRenderbuffer ( ZLGfxResource& handle ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_BIND_RENDERBUFFER );
	this->mStream->Write < ZLGfxResource* >( handle.RetainForWrite ());
}

//----------------------------------------------------------------//
void ZLGfxRetained::BindTexture ( ZLGfxResource& handle ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_BIND_TEXTURE );
	this->mStream->Write < ZLGfxResource* >( handle.RetainForWrite ());
}

//----------------------------------------------------------------//
void ZLGfxRetained::BindVertexArray ( ZLGfxResource& handle ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_BIND_VERTEX_ARRAY );
	this->mStream->Write < ZLGfxResource* >( handle.RetainForWrite ());
}

//----------------------------------------------------------------//
void ZLGfxRetained::BlendFunc ( ZLGfxEnum::_ sourceFactor, ZLGfxEnum::_ destFactor ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_BLEND_FUNC );
	this->mStream->Write < u32 >( sourceFactor );
	this->mStream->Write < u32 >( destFactor );
}

//----------------------------------------------------------------//
void ZLGfxRetained::BlendMode ( ZLGfxEnum::_ mode ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_BLEND_MODE );
	this->mStream->Write < u32 >( mode );
}

//----------------------------------------------------------------//
void ZLGfxRetained::BufferData ( ZLGfxEnum::_ target, size_t size, ZLSharedConstBuffer* buffer, size_t offset, ZLGfxEnum::_ usage ) {

	this->Retain ( buffer );

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_BUFFER_DATA );
	this->mStream->Write < u32 >( target );
	this->mStream->Write < size_t >( size );
	this->mStream->Write < ZLSharedConstBuffer* >( buffer );
	this->mStream->Write < size_t >( offset );
	this->mStream->Write < u32 >( usage );
}

//----------------------------------------------------------------//
void ZLGfxRetained::BufferSubData ( ZLGfxEnum::_ target, size_t offset, size_t size, ZLSharedConstBuffer* buffer, size_t srcOffset ) {

	this->Retain ( buffer );

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_BUFFER_SUB_DATA );
	this->mStream->Write < u32 >( target );
	this->mStream->Write < size_t >( offset );
	this->mStream->Write < size_t >( size );
	this->mStream->Write < ZLSharedConstBuffer* >( buffer );
	this->mStream->Write < size_t >( srcOffset );
}

//----------------------------------------------------------------//
void ZLGfxRetained::CheckFramebufferStatus ( ZLGfxEnum::_ target ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_CHECK_FRAMEBUFFER_STATUS );
	this->mStream->Write < u32 >( target );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Clear ( u32 mask ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_CLEAR );
	this->mStream->Write < u32 >( mask );
}

//----------------------------------------------------------------//
void ZLGfxRetained::ClearColor ( float r, float g, float b, float a ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_CLEAR_COLOR );
	this->mStream->Write < float >( r );
	this->mStream->Write < float >( g );
	this->mStream->Write < float >( b );
	this->mStream->Write < float >( a );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Color ( float r, float g, float b, float a ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_COLOR );
	this->mStream->Write < float >( r );
	this->mStream->Write < float >( g );
	this->mStream->Write < float >( b );
	this->mStream->Write < float >( a );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Comment ( cc8* comment ) {
	
	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_COMMENT );
	
	size_t size = comment ? strlen ( comment ) : 0;
	this->mStream->Write < size_t >( size );
	this->mStream->WriteBytes ( comment, size );
}

//----------------------------------------------------------------//
void ZLGfxRetained::CompileShader ( ZLGfxResource& shader, bool log ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_COMPILE_SHADER );
	this->mStream->Write < ZLGfxResource* >( shader.RetainForWrite ());
	this->mStream->Write < bool >( log );
}

//----------------------------------------------------------------//
void ZLGfxRetained::CompressedTexImage2D ( u32 level, ZLGfxEnum::_ internalFormat, u32 width, u32 height, u32 imageSize, ZLSharedConstBuffer* buffer ) {

	this->Retain ( buffer );

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_COMPRESSED_TEX_IMAGE_2D );
	this->mStream->Write < u32 >( level );
	this->mStream->Write < u32 >( internalFormat );
	this->mStream->Write < u32 >( width );
	this->mStream->Write < u32 >( height );
	this->mStream->Write < u32 >( imageSize );
	this->mStream->Write < ZLSharedConstBuffer* >( buffer );
}

//----------------------------------------------------------------//
void ZLGfxRetained::CullFace ( ZLGfxEnum::_ mode ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_CULL_FACE );
	this->mStream->Write < u32 >( mode );
}

//----------------------------------------------------------------//
void ZLGfxRetained::DeleteResource ( ZLGfxResource& resource ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_DELETE_RESOURCE );
	this->mStream->Write < ZLGfxResource* >( resource.RetainForWrite ());
}

//----------------------------------------------------------------//
void ZLGfxRetained::DepthFunc ( ZLGfxEnum::_ depthFunc ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_DEPTH_FUNC );
	this->mStream->Write < u32 >( depthFunc );
}

//----------------------------------------------------------------//
void ZLGfxRetained::DepthMask ( bool flag ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_DEPTH_MASK );
	this->mStream->Write < u32 >( flag ? 1 : 0 );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Disable ( ZLGfxEnum::_ cap ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_DISABLE );
	this->mStream->Write < u32 >( cap );
}

//----------------------------------------------------------------//
void ZLGfxRetained::DisableClientState ( ZLGfxEnum::_ cap ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_DISABLE_CLIENT_STATE );
	this->mStream->Write < u32 >( cap );
}

//----------------------------------------------------------------//
void ZLGfxRetained::DisableVertexAttribArray ( u32 index ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_DISABLE_VERTEX_ATTRIB_ARRAY );
	this->mStream->Write < u32 >( index );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Draw ( ZLGfx& draw ) {

	//zglBegin ();

	assert ( this->mStream );

	size_t top = this->mStream->GetCursor ();
	if ( top == 0 ) return;
	
	this->mStream->Seek ( 0, SEEK_SET );
	
	while ( this->mStream->GetCursor () < top ) {
	
		u32 command = this->mStream->Read < u32 >( ZLGFX_UNKNOWN );
		
		if ( command == ZLGFX_UNKNOWN ) {
			printf ( "UNKOWN\n" );
			break;
		}
	
		switch ( command ) {
	
			case ZLGFX_ACTIVE_TEXTURE: {
			
				draw.ActiveTexture (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ZLGFX_ALLOCATE_RESOURCE: {
			
				ZLGfxResource* resource		= this->mStream->Read < ZLGfxResource* >( 0 );
				ZLGfxEnum::_ param				= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				
				draw.AllocateResource ( *resource, param );
				
				resource->Release ();
				break;
			}
			case ZLGFX_ATTACH_SHADER: {
			
				ZLGfxResource* program = this->mStream->Read < ZLGfxResource* >( 0 );
				ZLGfxResource* shader = this->mStream->Read < ZLGfxResource* >( 0 );
				
				draw.AttachShader ( *program, *shader );
				
				program->Release ();
				shader->Release ();
				break;
			}
			case ZLGFX_BIND_ATTRIB_LOCATION: {
			
				ZLGfxResource* program	= this->mStream->Read < ZLGfxResource* >( 0 );
				u32 index				= this->mStream->Read < u32 >( 0 );
	
				size_t size				= this->mStream->Read < size_t >( 0 );
				
				char* name = ( char* )alloca ( size + 1 );
				this->mStream->ReadBytes ( name, size );
				name [ size ] = 0;
				
				draw.BindAttribLocation ( *program, index, name );
				
				program->Release ();
				break;
			}
			case ZLGFX_BIND_BUFFER: {
			
				ZLGfxEnum::_ target			= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				ZLGfxResource* buffer	= this->mStream->Read < ZLGfxResource* >( 0 );
			
				draw.BindBuffer ( target, *buffer );
				
				buffer->Release ();
				break;
			}
			case ZLGFX_BIND_FRAMEBUFFER: {
			
				ZLGfxEnum::_ target			= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				ZLGfxResource* buffer	= this->mStream->Read < ZLGfxResource* >( 0 );
				
				draw.BindFramebuffer ( target, *buffer );
				
				buffer->Release ();
				break;
			}
			case ZLGFX_BIND_RENDERBUFFER: {
			
				ZLGfxResource* buffer = this->mStream->Read < ZLGfxResource* >( 0 );
				draw.BindRenderbuffer ( *buffer );
				buffer->Release ();
				break;
			}
			case ZLGFX_BIND_TEXTURE: {
			
				ZLGfxResource* texture = this->mStream->Read < ZLGfxResource* >( 0 );
				draw.BindTexture ( *texture );
				texture->Release ();
				break;
			}
			case ZLGFX_BIND_VERTEX_ARRAY: {
			
				ZLGfxResource* array = this->mStream->Read < ZLGfxResource* >( 0 );
				draw.BindVertexArray ( *array );
				array->Release ();
				break;
			}
			case ZLGFX_BLEND_FUNC: {
			
				draw.BlendFunc (
					( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 ),
					( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ZLGFX_BLEND_MODE: {
			
				draw.BlendMode (
					( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ZLGFX_BUFFER_DATA: {
			
				ZLGfxEnum::_ target					= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				size_t size						= this->mStream->Read < size_t >( 0 );
				ZLSharedConstBuffer* buffer		= this->mStream->Read < ZLSharedConstBuffer* >( 0 );
				size_t offset					= this->mStream->Read < size_t >( 0 );
				ZLGfxEnum::_ usage					= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
			
				draw.BufferData ( target, size, buffer, offset, usage );
				
				break;
			}
			case ZLGFX_BUFFER_SUB_DATA: {
			
				ZLGfxEnum::_ target					= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				size_t offset					= this->mStream->Read < size_t >( 0 );
				size_t size						= this->mStream->Read < size_t >( 0 );
				ZLSharedConstBuffer* buffer		= this->mStream->Read < ZLSharedConstBuffer* >( 0 );
				size_t srcOffset				= this->mStream->Read < size_t >( 0 );
				
				draw.BufferSubData ( target, offset, size, buffer, srcOffset );
				
				break;
			}
			case ZLGFX_CHECK_FRAMEBUFFER_STATUS: {
			
				// TODO: GFX
				//u32 target						= this->mStream->Read < u32 >( 0 );
				this->mStream->Read < u32 >( 0 );

				break;
			}
			case ZLGFX_CLEAR: {
			
				draw.Clear (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ZLGFX_CLEAR_COLOR: {
			
				draw.ClearColor (
					this->mStream->Read < float >( 0 ),
					this->mStream->Read < float >( 0 ),
					this->mStream->Read < float >( 0 ),
					this->mStream->Read < float >( 0 )
				);
				break;
			}
			case ZLGFX_COLOR: {
			
				draw.Color (
					this->mStream->Read < float >( 0 ),
					this->mStream->Read < float >( 0 ),
					this->mStream->Read < float >( 0 ),
					this->mStream->Read < float >( 0 )
				);
				break;
			}
			case ZLGFX_COMMENT: {
	
				size_t size				= this->mStream->Read < size_t >( 0 );
				
				if ( size ) {
					char* comment = ( char* )alloca ( size + 1 );
					this->mStream->ReadBytes ( comment, size );
					comment [ size ] = 0;
					draw.Comment ( comment );
				}
				else {
					draw.Comment ();
				}
				break;
			}
			case ZLGFX_COMPILE_SHADER: {
			
				ZLGfxResource* shader	= this->mStream->Read < ZLGfxResource* >( 0 );
				bool log				= this->mStream->Read < bool >( true );
			
				draw.CompileShader ( *shader, log );
				
				shader->Release ();
				break;
			}
			case ZLGFX_COMPRESSED_TEX_IMAGE_2D: {
			
				u32 level						= this->mStream->Read < u32 >( 0 );
				ZLGfxEnum::_ internalFormat			= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				u32 width						= this->mStream->Read < u32 >( 0 );
				u32 height						= this->mStream->Read < u32 >( 0 );
				u32 imageSize					= this->mStream->Read < u32 >( 0 );
				ZLSharedConstBuffer* buffer		= this->mStream->Read < ZLSharedConstBuffer* >( 0 );
				
				draw.CompressedTexImage2D ( level, internalFormat, width, height, imageSize, buffer );
			
				break;
			}
			case ZLGFX_CULL_FACE: {
			
				draw.CullFace (
					( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ZLGFX_DELETE_RESOURCE: {
				
				ZLGfxResource* resource = this->mStream->Read < ZLGfxResource* >( 0 );
				draw.DeleteResource ( *resource );
				resource->Release ();
				break;
			}
			case ZLGFX_DEPTH_FUNC: {
			
				draw.DepthFunc (
					( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ZLGFX_DEPTH_MASK: {

				draw.DepthMask (
					this->mStream->Read < u32 >( 0 ) == 1
				);
				break;
			}
			case ZLGFX_DISABLE: {
			
				draw.Disable (
					( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ZLGFX_DISABLE_CLIENT_STATE: {
			
				draw.DisableClientState (
					( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ZLGFX_DISABLE_VERTEX_ATTRIB_ARRAY: {
			
				draw.DisableVertexAttribArray (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ZLGFX_DRAW_ARRAYS: {
			
				draw.DrawArrays (
					( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ZLGFX_DRAW_ELEMENTS: {
			
				ZLGfxEnum::_ primType				= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				u32 count						= this->mStream->Read < u32 >( 0 );
				ZLGfxEnum::_ indexType				= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				ZLSharedConstBuffer* buffer		= this->mStream->Read < ZLSharedConstBuffer* >( 0 );
				size_t offset					= this->mStream->Read < size_t >( 0 );
			
				draw.DrawElements ( primType, count, indexType, buffer, offset );
				
				break;
			}
			case ZLGFX_ENABLE: {
			
				draw.Enable (
					( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ZLGFX_ENABLE_CLIENT_STATE: {
			
				draw.EnableClientState (
					( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ZLGFX_ENABLE_VERTEX_ATTRIB_ARRAY: {
			
				draw.EnableVertexAttribArray (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ZLGFX_EVENT: {

				u32 event				= this->mStream->Read < u32 >( 0 );
				u32 listenerRecordIdx	= this->mStream->Read < u32 >( 0 );
				
				draw.Event ( this, event, ( void* )(( size_t )listenerRecordIdx ));
				
				break;
			}
			case ZLGFX_FLUSH: {
			
				bool finish				= this->mStream->Read < bool >( 0 );
			
				draw.Flush ( finish );
				break;
			}
			case ZLGFX_FRAMEBUFFER_RENDERBUFFER: {
			
				ZLGfxEnum::_ target					= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				ZLGfxEnum::_ attachment				= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				ZLGfxResource* renderbuffer		= this->mStream->Read < ZLGfxResource* >( 0 );
				
				draw.FramebufferRenderbuffer ( target, attachment, *renderbuffer );
				
				renderbuffer->Release ();
				break;
			}
			case ZLGFX_FRAMEBUFFER_TEXTURE_2D: {
			
				ZLGfxEnum::_ target					= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				ZLGfxEnum::_ attachment				= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				ZLGfxResource* texture			= this->mStream->Read < ZLGfxResource* >( 0 );
				s32 level						= this->mStream->Read < s32 >( 0 );
				
				draw.FramebufferTexture2D ( target, attachment, *texture, level );
				
				texture->Release ();
				break;
			}
			case ZLGFX_GET_CURRENT_FRAMEBUFFER: {
			
				ZLGfxResource* framebuffer = this->mStream->Read < ZLGfxResource* >( 0 );
				draw.GetCurrentFramebuffer ( *framebuffer );
				framebuffer->Release ();
				break;
			}
			case ZLGFX_GET_UNIFORM_LOCATION: {
			
				ZLGfxResource* program = this->mStream->Read < ZLGfxResource* >( 0 );
				size_t size = this->mStream->Read < size_t >( 0 );
				
				char* name = ( char* )alloca ( size + 1 );
				this->mStream->ReadBytes ( name, size );
				name [ size ] = 0;
				
				u32 listenerRecordIdx = this->mStream->Read < u32 >( 0 );
				
				draw.GetUniformLocation ( *program, name, this, ( void* )(( size_t )listenerRecordIdx ));
				
				program->Release ();
				break;
			}
			case ZLGFX_LINE_WIDTH: {
			
				draw.LineWidth (
					this->mStream->Read < float >( 0 )
				);
				break;
			}
			case ZLGFX_LINK_PROGRAM: {
			
				ZLGfxResource* program	= this->mStream->Read < ZLGfxResource* >( 0 );
				bool log				= this->mStream->Read < bool >( true );
			
				draw.LinkProgram ( *program, log );
				
				program->Release ();
				break;
			}
			case ZLGFX_READ_PIXELS: {
		
				s32 x				= this->mStream->Read < s32 >( 0 );
				s32 y				= this->mStream->Read < s32 >( 0 );
				
				u32 width			= this->mStream->Read < u32 >( 0 );
				u32 height			= this->mStream->Read < u32 >( 0 );
				
				ZLGfxEnum::_ format		= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				ZLGfxEnum::_ type		= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				u32 pixelSize		= this->mStream->Read < u32 >( 0 );
				
				u32 listenerRecordIdx = this->mStream->Read < u32 >( 0 );
				
				draw.ReadPixels ( x, y, width, height, format, type, pixelSize, this, ( void* )(( size_t )listenerRecordIdx ));
				
				break;
			}
			case ZLGFX_RENDER_BUFFER_STORAGE: {
				
				ZLGfxEnum::_ internalFormat		= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				u32 width					= this->mStream->Read < u32 >( 0 );
				u32 height					= this->mStream->Read < u32 >( 0 );
				
				draw.RenderbufferStorage ( internalFormat, width, height );
				
				break;
			}
			case ZLGFX_SCISSOR: {
			
				draw.Scissor (
					this->mStream->Read < s32 >( 0 ),
					this->mStream->Read < s32 >( 0 ),
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ZLGFX_SHADER_SOURCE: {
			
				ZLGfxResource* shader	= this->mStream->Read < ZLGfxResource* >( 0 );
				size_t length			= this->mStream->Read < size_t >( 0 );
				
				char* source = ( char* )alloca ( length + 1 );
				this->mStream->ReadBytes ( source, length );
				source [ length ] = 0;
			
				draw.ShaderSource ( *shader, source, length );
				
				shader->Release ();
				break;
			}
			case ZLGFX_TEX_ENVI: {
			
				draw.TexEnvi (
					( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 ),
					( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ZLGFX_TEX_IMAGE_2D: {
			
				u32 level						= this->mStream->Read < u32 >( 0 );
				ZLGfxEnum::_ internalFormat			= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				u32 width						= this->mStream->Read < u32 >( 0 );
				u32 height						= this->mStream->Read < u32 >( 0 );
				ZLGfxEnum::_ format					= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				ZLGfxEnum::_ type					= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				ZLSharedConstBuffer* buffer		= this->mStream->Read < ZLSharedConstBuffer* >( 0 );
				
				draw.TexImage2D ( level, internalFormat, width, height, format, type, buffer );
				
				break;
			}
			case ZLGFX_TEX_PARAMETERI: {
			
				draw.TexParameteri (
					( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 ),
					( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ZLGFX_TEX_SUB_IMAGE_2D: {
				
				u32 level						= this->mStream->Read < u32 >( 0 );
				u32 xOffset						= this->mStream->Read < u32 >( 0 );
				u32 yOffset						= this->mStream->Read < u32 >( 0 );
				u32 width						= this->mStream->Read < u32 >( 0 );
				u32 height						= this->mStream->Read < u32 >( 0 );
				ZLGfxEnum::_ format					= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				ZLGfxEnum::_ type					= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				ZLSharedConstBuffer* buffer		= this->mStream->Read < ZLSharedConstBuffer* >( 0 );
				
				draw.TexSubImage2D ( level, xOffset, yOffset, width, height, format, type, buffer );
				
				break;
			}
			case ZLGFX_UNIFORM_FLOAT: {
			
				u32 location = this->mStream->Read < u32 >( 0 );
				u32 index = this->mStream->Read < u32 >( 0 );
				u32 width = this->mStream->Read < u32 >( 0 );
				u32 count = this->mStream->Read < u32 >( 0 );
				
				assert ( width < 16 );
				
				float vec [ 16 ];
				this->mStream->ReadBytes ( vec, sizeof ( float ) * width );
			
				draw.UniformFloat ( location, index, width, count, vec );
				break;
			}
			case ZLGFX_UNIFORM_INT: {
			
				u32 location = this->mStream->Read < u32 >( 0 );
				u32 index = this->mStream->Read < u32 >( 0 );
				u32 width = this->mStream->Read < u32 >( 0 );
				u32 count = this->mStream->Read < u32 >( 0 );
				
				assert ( width < 16 );
				
				s32 vec [ 16 ];
				this->mStream->ReadBytes ( vec, sizeof ( s32 ) * width );
			
				draw.UniformInt ( location, index, width, count, vec );
				break;
			}
			case ZLGFX_USE_PROGRAM: {
			
				ZLGfxResource* program = this->mStream->Read < ZLGfxResource* >( 0 );
				draw.UseProgram ( *program );
				program->Release ();
				break;
			}
			case ZLGFX_VERTEX_ATTRIB_POINTER: {
			
				u32 index						= this->mStream->Read < u32 >( 0 );
				u32 size						= this->mStream->Read < u32 >( 0 );
				ZLGfxEnum::_ type					= ( ZLGfxEnum::_ )*this->mStream->Read < u32 >( 0 );
				bool normalized					= this->mStream->Read < bool >( false );
				u32 stride						= this->mStream->Read < u32 >( 0 );
				ZLSharedConstBuffer* buffer		= this->mStream->Read < ZLSharedConstBuffer* >( 0 );
				size_t offset					= this->mStream->Read < size_t >( 0 );
				
				draw.VertexAttribPointer (
					index,
					size,
					type,
					normalized,
					stride,
					buffer,
					offset
				);
				
				break;
			}
			case ZLGFX_VIEWPORT: {
			
				draw.Viewport (
					this->mStream->Read < s32 >( 0 ),
					this->mStream->Read < s32 >( 0 ),
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
		}
	}
	
	this->mStream->Seek (( long )top, SEEK_SET );
	
	//zglEnd ();
}

//----------------------------------------------------------------//
void ZLGfxRetained::DrawArrays ( ZLGfxEnum::_ primType, u32 first, u32 count ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_DRAW_ARRAYS );
	this->mStream->Write < u32 >( primType );
	this->mStream->Write < u32 >( first );
	this->mStream->Write < u32 >( count );
}

//----------------------------------------------------------------//
void ZLGfxRetained::DrawElements ( ZLGfxEnum::_ primType, u32 count, ZLGfxEnum::_ indexType, ZLSharedConstBuffer* buffer, size_t offset ) {

	this->Retain ( buffer );

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_DRAW_ELEMENTS );
	this->mStream->Write < u32 >( primType );
	this->mStream->Write < u32 >( count );
	this->mStream->Write < u32 >( indexType );
	this->mStream->Write < ZLSharedConstBuffer* >( buffer );
	this->mStream->Write < size_t >( offset );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Enable ( ZLGfxEnum::_ cap ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_ENABLE );
	this->mStream->Write < u32 >( cap );
}

//----------------------------------------------------------------//
void ZLGfxRetained::EnableClientState ( ZLGfxEnum::_ cap ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_ENABLE_CLIENT_STATE );
	this->mStream->Write < u32 >( cap );
}

//----------------------------------------------------------------//
void ZLGfxRetained::EnableVertexAttribArray ( u32 index ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_ENABLE_VERTEX_ATTRIB_ARRAY );
	this->mStream->Write < u32 >( index );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Event ( ZLGfxListener* listener, u32 event, void* userdata ) {

	assert ( this->mStream );

	if ( listener ) {
	
		this->mStream->Write < u32 >( ZLGFX_EVENT );
		this->mStream->Write < u32 >( event );
		this->WriteListenerRecord ( listener, userdata );
	}
}

//----------------------------------------------------------------//
void ZLGfxRetained::Flush ( bool finish ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_FLUSH );
	this->mStream->Write < bool >( finish );
}

//----------------------------------------------------------------//
void ZLGfxRetained::FramebufferRenderbuffer ( ZLGfxEnum::_ target, ZLGfxEnum::_ attachment, ZLGfxResource& renderbuffer ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_FRAMEBUFFER_RENDERBUFFER );
	this->mStream->Write < u32 >( target );
	this->mStream->Write < u32 >( attachment );
	this->mStream->Write < ZLGfxResource* >( renderbuffer.RetainForWrite ());
}

//----------------------------------------------------------------//
void ZLGfxRetained::FramebufferTexture2D ( ZLGfxEnum::_ target, ZLGfxEnum::_ attachment, ZLGfxResource& texture, s32 level ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_FRAMEBUFFER_TEXTURE_2D );
	this->mStream->Write < u32 >( target );
	this->mStream->Write < u32 >( attachment );
	this->mStream->Write < ZLGfxResource* >( texture.RetainForWrite ());
	this->mStream->Write < s32 >( level );
}

//----------------------------------------------------------------//
void ZLGfxRetained::GetCurrentFramebuffer ( ZLGfxResource& framebuffer ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_GET_CURRENT_FRAMEBUFFER );
	this->mStream->Write < ZLGfxResource* >( framebuffer.RetainForWrite ());
}

//----------------------------------------------------------------//
void ZLGfxRetained::GetUniformLocation ( ZLGfxResource& program, cc8* uniformName, ZLGfxListener* listener, void* userdata ) {

	assert ( this->mStream );

	if ( listener ) {

		this->mStream->Write < u32 >( ZLGFX_GET_UNIFORM_LOCATION );
		this->mStream->Write < ZLGfxResource* >( program.RetainForWrite ());
		
		size_t size = strlen ( uniformName );
		this->mStream->Write < size_t >( size );
		this->mStream->WriteBytes ( uniformName, size );
		
		this->WriteListenerRecord ( listener, userdata );
	}
}

//----------------------------------------------------------------//
bool ZLGfxRetained::HasContent () {

	return ( this->mStream->GetCursor () > 0 );
}

//----------------------------------------------------------------//
bool ZLGfxRetained::IsImmediate () {

	return false;
}

//----------------------------------------------------------------//
void ZLGfxRetained::LineWidth ( float width ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_LINE_WIDTH );
	this->mStream->Write < float >( width );
}

//----------------------------------------------------------------//
void ZLGfxRetained::LinkProgram ( ZLGfxResource& program, bool log ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_LINK_PROGRAM );
	this->mStream->Write < ZLGfxResource* >( program.RetainForWrite ());
	this->mStream->Write < bool >( log );
}

//----------------------------------------------------------------//
void ZLGfxRetained::PopSection () {
}

//----------------------------------------------------------------//
void ZLGfxRetained::PublishEvents () {

	ZLSize top = this->mListenerRecords.GetTop ();
	for ( ZLIndex i = 0; i < top; ++i ) {
	
		ZLGfxListenerRecord& record = this->mListenerRecords [ i ];
		
		ZLGfxListener* listener = record.mListener;
		if ( listener ) {
		
			switch ( record.mCallbackID ) {
			
				case ZLGfxListenerRecord::ON_EVENT:
					listener->ZLGfxListener_OnGfxEvent ( record.mEvent, record.mUserdata );
					break;
				
				case ZLGfxListenerRecord::ON_READ_PIXELS:
					listener->ZLGfxListener_OnReadPixels ( record.mCopyOnWrite, record.mUserdata );
					break;
			
				case ZLGfxListenerRecord::ON_UNIFORM_LOCATION:
					listener->ZLGfxListener_OnUniformLocation ( record.mUniformAddr, record.mUserdata );
					break;
			}
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxRetained::PublishEventsAndReset () {

	this->PublishEvents ();
	this->Reset ();
}

//----------------------------------------------------------------//
bool ZLGfxRetained::PushErrorHandler () {

	return false;
}

//----------------------------------------------------------------//
void ZLGfxRetained::PushSection () {
}

//----------------------------------------------------------------//
bool ZLGfxRetained::PushSuccessHandler () {

	// TODO: gfx

	return true;
}

//----------------------------------------------------------------//
void ZLGfxRetained::ReadPixels ( s32 x, s32 y, u32 width, u32 height, ZLGfxEnum::_ format, ZLGfxEnum::_ type, u32 pixelSize, ZLGfxListener* listener, void* userdata ) {
	
	assert ( this->mStream );

	if ( listener ) {

		this->mStream->Write < u32 >( ZLGFX_READ_PIXELS );
		
		this->mStream->Write < s32 >( x );
		this->mStream->Write < s32 >( y );
		
		this->mStream->Write < u32 >( width );
		this->mStream->Write < u32 >( height );
		
		this->mStream->Write < u32 >( format );
		this->mStream->Write < u32 >( type );
		this->mStream->Write < u32 >( pixelSize );
		
		this->WriteListenerRecord ( listener, userdata );
	}
}

//----------------------------------------------------------------//
void ZLGfxRetained::RenderbufferStorage ( ZLGfxEnum::_ internalFormat, u32 width, u32 height ) {
	
	assert ( this->mStream );
	
	this->mStream->Write < u32 >( ZLGFX_RENDER_BUFFER_STORAGE );
	this->mStream->Write < u32 >( internalFormat );
	this->mStream->Write < u32 >( width );
	this->mStream->Write < u32 >( height );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Reset () {

	assert ( this->mStream );

	size_t totalListeners = this->mListenerRecords.GetTop ();
	if ( totalListeners > 0 ) {
	
		this->Abandon ();
		
		for ( ZLIndex i = 0; i < totalListeners; ++i ) {
			this->mListenerRecords [ i ].mListener = 0;
		}
	}
	this->mListenerRecords.Reset ();

	this->mStream->Seek ( 0, SEEK_SET );
	
	while ( this->mReleaseStack.GetTop ()) {
		ZLRefCountedObject* object = this->mReleaseStack.Pop ();
		object->Release ();
	}
}

//----------------------------------------------------------------//
void ZLGfxRetained::Retain ( ZLRefCountedObject* object ) {

	if ( object ) {
		object->Retain ();
		this->mReleaseStack.Push ( object );
	}
}

//----------------------------------------------------------------//
void ZLGfxRetained::Scissor ( s32 x, s32 y, u32 w, u32 h ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_SCISSOR );
	this->mStream->Write < s32 >( x );
	this->mStream->Write < s32 >( y );
	this->mStream->Write < u32 >( w );
	this->mStream->Write < u32 >( h );
}

//----------------------------------------------------------------//
void ZLGfxRetained::ShaderSource ( ZLGfxResource& shader, cc8* source, size_t length ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_SHADER_SOURCE );
	this->mStream->Write < ZLGfxResource* >( shader.RetainForWrite ());
	this->mStream->Write < size_t >( length );
	this->mStream->WriteBytes ( source, length );
}

//----------------------------------------------------------------//
void ZLGfxRetained::TexEnvi ( ZLGfxEnum::_ pname, ZLGfxEnum::_ param ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_TEX_ENVI );
	this->mStream->Write < u32 >( pname );
	this->mStream->Write < u32 >( param );
}

//----------------------------------------------------------------//
void ZLGfxRetained::TexImage2D ( u32 level, ZLGfxEnum::_ internalFormat, u32 width, u32 height, ZLGfxEnum::_ format, ZLGfxEnum::_ type, ZLSharedConstBuffer* buffer ) {
	
	this->Retain ( buffer );
	
	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_TEX_IMAGE_2D );
	this->mStream->Write < u32 >( level );
	this->mStream->Write < u32 >( internalFormat );
	this->mStream->Write < u32 >( width );
	this->mStream->Write < u32 >( height );
	this->mStream->Write < u32 >( format );
	this->mStream->Write < u32 >( type );
	this->mStream->Write < ZLSharedConstBuffer* >( buffer );
}

//----------------------------------------------------------------//
void ZLGfxRetained::TexParameteri ( ZLGfxEnum::_ pname, ZLGfxEnum::_ param ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_TEX_PARAMETERI );
	this->mStream->Write < u32 >( pname );
	this->mStream->Write < u32 >( param );
}

//----------------------------------------------------------------//
void ZLGfxRetained::TexSubImage2D ( u32 level, s32 xOffset, s32 yOffset, u32 width, u32 height, ZLGfxEnum::_ format, ZLGfxEnum::_ type, ZLSharedConstBuffer* buffer ) {

	this->Retain ( buffer );

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_TEX_SUB_IMAGE_2D );
	this->mStream->Write < u32 >( level );
	this->mStream->Write < s32 >( xOffset );
	this->mStream->Write < s32 >( yOffset );
	this->mStream->Write < u32 >( width );
	this->mStream->Write < u32 >( height );
	this->mStream->Write < u32 >( format );
	this->mStream->Write < u32 >( type );
	this->mStream->Write < ZLSharedConstBuffer* >( buffer );
}

//----------------------------------------------------------------//
void ZLGfxRetained::UniformFloat ( u32 location, u32 index, u32 width, u32 count, const float* value ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_UNIFORM_FLOAT );
	this->mStream->Write < u32 >( location );
	this->mStream->Write < u32 >( index );
	this->mStream->Write < u32 >( width );
	this->mStream->Write < u32 >( count );
	this->mStream->WriteBytes ( value, sizeof ( float ) * width );
}

//----------------------------------------------------------------//
void ZLGfxRetained::UniformInt ( u32 location, u32 index, u32 width, u32 count, const s32* value ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_UNIFORM_INT );
	this->mStream->Write < u32 >( location );
	this->mStream->Write < u32 >( index );
	this->mStream->Write < u32 >( width );
	this->mStream->Write < u32 >( count );
	this->mStream->WriteBytes ( value, sizeof ( s32 ) * width );
}

//----------------------------------------------------------------//
void ZLGfxRetained::UseProgram ( ZLGfxResource& program ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_USE_PROGRAM );
	this->mStream->Write < ZLGfxResource* >( program.RetainForWrite ());
}

//----------------------------------------------------------------//
void ZLGfxRetained::VertexAttribPointer ( u32 index, u32 size, ZLGfxEnum::_ type, bool normalized, u32 stride, ZLSharedConstBuffer* buffer, size_t offset ) {

	this->Retain ( buffer );

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_VERTEX_ATTRIB_POINTER );
	this->mStream->Write < u32 >( index );
	this->mStream->Write < u32 >( size );
	this->mStream->Write < u32 >( type );
	this->mStream->Write < bool >( normalized );
	this->mStream->Write < u32 >( stride );
	this->mStream->Write < ZLSharedConstBuffer* >( buffer );
	this->mStream->Write < size_t >( offset );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Viewport ( s32 x, s32 y, u32 w, u32 h ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ZLGFX_VIEWPORT );
	this->mStream->Write < s32 >( x );
	this->mStream->Write < s32 >( y );
	this->mStream->Write < u32 >( w );
	this->mStream->Write < u32 >( h );
}

//----------------------------------------------------------------//
ZLGfxListenerRecord& ZLGfxRetained::WriteListenerRecord ( ZLGfxListener* listener, void* userdata ) {
	
	u32 idx = ( u32 )this->mListenerRecords.GetTop ();
	ZLGfxListenerRecord& record = this->mListenerRecords.Push ();

	record.mListener			= listener;
	record.mUserdata			= userdata;
	record.mCallbackID			= ZLGfxListenerRecord::UNKNOWN;
	record.mEvent				= 0;
	record.mUniformAddr			= 0;

	this->mStream->Write < u32 >( idx );
	
	return record;
}

//----------------------------------------------------------------//
ZLGfxRetained::ZLGfxRetained () :
	mStream ( &this->mMemStream ) {
}

//----------------------------------------------------------------//
ZLGfxRetained::~ZLGfxRetained () {
}

//================================================================//
// ZLGfxRetained
//================================================================//

//----------------------------------------------------------------//
void ZLGfxRetained::ZLGfxListener_OnGfxEvent ( u32 event, void* userdata ) {

	ZLIndex idx = ( ZLIndex )userdata;
	if ( idx < this->mListenerRecords.GetTop ()) {
		ZLGfxListenerRecord& listenerRecord = this->mListenerRecords [ idx ];
		
		listenerRecord.mCallbackID = ZLGfxListenerRecord::ON_EVENT;
		listenerRecord.mEvent = event;
	}
}

//----------------------------------------------------------------//
void ZLGfxRetained::ZLGfxListener_OnReadPixels ( const ZLCopyOnWrite& copyOnWrite, void* userdata ) {

	ZLIndex idx = ( ZLIndex )userdata;
	if ( idx < this->mListenerRecords.GetTop ()) {
		ZLGfxListenerRecord& listenerRecord = this->mListenerRecords [ idx ];
		
		listenerRecord.mCallbackID = ZLGfxListenerRecord::ON_READ_PIXELS;
		listenerRecord.mCopyOnWrite = copyOnWrite;
	}
}

//----------------------------------------------------------------//
void ZLGfxRetained::ZLGfxListener_OnUniformLocation ( u32 addr, void* userdata ) {
	
	ZLIndex idx = ( ZLIndex )userdata;
	if ( idx < this->mListenerRecords.GetTop ()) {
		ZLGfxListenerRecord& listenerRecord = this->mListenerRecords [ idx ];
		
		listenerRecord.mCallbackID = ZLGfxListenerRecord::ON_UNIFORM_LOCATION;
		listenerRecord.mUniformAddr = addr;
	}
}
