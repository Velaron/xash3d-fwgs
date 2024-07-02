/*
gl_vgui.c - OpenGL vbo stuff
Copyright (C) 2024 Velaron

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "gl_local.h"

gl_fbo_t gl_fbo;

void GL_InitFBO( void )
{
	// int buf_width, buf_height;

	if( !r_fbo.value )
		return;
	
	if( !GL_Support( GL_FRAMEBUFFER_OBJECT ))
	{
		gEngfuncs.Con_Printf( S_WARN "FBO initialization failed: not supported\n" );
		gEngfuncs.Cvar_FullSet( "gl_fbo", "0", FCVAR_READ_ONLY );
		return;
	}
	
	// gl_fbo.rotation = gEngfuncs.pfnGetCvarFloat( "vid_rotate" );
	// gl_fbo.offset_x = gl_fbo.offset_y = gEngfuncs.pfnGetCvarFloat( "vid_offset" );
	// gl_fbo.scale_x = gl_fbo.scale_y = gEngfuncs.pfnGetCvarFloat( "vid_scale" );
	// gl_fbo.scale_x = gl_fbo.scale_y = 1.0f;

	// gl_fbo.win_width = gpGlobals->width / gl_fbo.scale_x;
	// gl_fbo.win_height = gpGlobals->height / gl_fbo.scale_y;
	
	// 3d buffer
	pglGenFramebuffers( 1, &gl_fbo.fbo[0] );
	pglBindFramebuffer( GL_FRAMEBUFFER, gl_fbo.fbo[0] );
	
	pglGenRenderbuffers( 1, &gl_fbo.depthbuf );
	pglBindRenderbuffer( GL_RENDERBUFFER, gl_fbo.depthbuf );
	pglRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, gpGlobals->render_width, gpGlobals->render_height );
	pglFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gl_fbo.depthbuf );

	pglGenTextures( 1, &gl_fbo.texture[0] );
	pglBindTexture( GL_TEXTURE_2D, gl_fbo.texture[0] );
	pglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	pglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	pglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	pglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	pglTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	pglTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, gpGlobals->render_width, gpGlobals->render_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
	pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gl_fbo.texture[0], 0 );

	// 2d buffer
	pglGenFramebuffers( 1, &gl_fbo.fbo[1] );
	pglBindFramebuffer( GL_FRAMEBUFFER, gl_fbo.fbo[1] );

	pglGenTextures( 1, &gl_fbo.texture[1] );
	pglBindTexture( GL_TEXTURE_2D, gl_fbo.texture[1] );
	pglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	pglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	pglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	pglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	pglTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, gpGlobals->width, gpGlobals->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
	pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gl_fbo.texture[1], 0 );

	if ( pglCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
	{
		gEngfuncs.Con_Printf( S_WARN "FBO initialization failed: creation failure\n" );
		gEngfuncs.Cvar_FullSet( "gl_fbo", "0", FCVAR_READ_ONLY );
		GL_ShutdownFBO();
	}

	pglBindTexture( GL_TEXTURE_2D, 0 );
	pglBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void GL_ShutdownFBO( void )
{
	if( gl_fbo.fbo )
	{
		pglDeleteFramebuffers( 1, &gl_fbo.fbo[0] );
		pglDeleteFramebuffers( 1, &gl_fbo.fbo[1] );

		memset( gl_fbo.fbo, 0, sizeof( gl_fbo.fbo ));
	}

	if( gl_fbo.depthbuf )
	{
		pglDeleteRenderbuffers( 1, &gl_fbo.depthbuf );
		gl_fbo.depthbuf = 0;
	}

	if ( gl_fbo.texture )
	{
		pglDeleteTextures( 1, &gl_fbo.texture[0] );
		pglDeleteTextures( 1, &gl_fbo.texture[1] );

		memset( gl_fbo.texture, 0, sizeof( gl_fbo.texture ));
	}
}

void GL_FBOSet2DMode( qboolean mode )
{
	if( !gl_fbo.fbo )
		return;
	
	if( gl_fbo.mode == mode )
		return;
	
	gl_fbo.mode = mode;
	
	pglBindFramebuffer( GL_DRAW_FRAMEBUFFER, mode ? 0 : gl_fbo.fbo[0] );
}

void GL_FBOBeginFrame( void )
{
	if( !gl_fbo.fbo )
		return;

	pglBindFramebuffer( GL_DRAW_FRAMEBUFFER, gl_fbo.fbo[!gl_fbo.mode] );
	pglClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	if( gl_clear->value )
		pglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	else
		pglClear( GL_DEPTH_BUFFER_BIT );
	
	pglBindFramebuffer( GL_DRAW_FRAMEBUFFER, gl_fbo.fbo[gl_fbo.mode] );
	pglClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	if( gl_clear->value )
		pglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	else
		pglClear( GL_DEPTH_BUFFER_BIT );
}

void GL_FBOEndFrame( void )
{
	if( !gl_fbo.fbo )
		return;

	pglBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
	// pglClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	// pglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	pglBindFramebuffer( GL_READ_FRAMEBUFFER, gl_fbo.fbo[0] );
	pglBlitFramebuffer( 0, 0, gpGlobals->render_width, gpGlobals->render_height, 0, 0, gpGlobals->width, gpGlobals->height, GL_COLOR_BUFFER_BIT, GL_NEAREST );

	// pglColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

	// pglBindTexture( GL_TEXTURE_2D, gl_fbo.texture[0] );
	// pglBegin( GL_QUADS );
	// pglTexCoord2f( 0.0f, 0.0f );
	// pglVertex2f( 0.0f, 0.0f );
	// pglTexCoord2f( 1.0f, 0.0f );
	// pglVertex2f( gpGlobals->width, 0.0f );
	// pglTexCoord2f( 1.0f, 1.0f );
	// pglVertex2f( gpGlobals->width, gpGlobals->height );
	// pglTexCoord2f( 0.0f, 1.0f );
	// pglVertex2f( 0.0f, gpGlobals->height );
	// pglEnd();

	// pglBindTexture( GL_TEXTURE_2D, gl_fbo.texture[1] );
	// pglBegin( GL_QUADS );
	// pglTexCoord2f( 0.0f, 0.0f );
	// pglVertex2f( 0.0f, 0.0f );
	// pglTexCoord2f( 1.0f, 0.0f );
	// pglVertex2f( gpGlobals->width, 0.0f );
	// pglTexCoord2f( 1.0f, 1.0f );
	// pglVertex2f( gpGlobals->width, gpGlobals->height );
	// pglTexCoord2f( 0.0f, 1.0f );
	// pglVertex2f( 0.0f, gpGlobals->height );
	// pglEnd();

	pglBindTexture( GL_TEXTURE_2D, 0 );

	// pglBindFramebuffer( GL_READ_FRAMEBUFFER, gl_fbo.fbo[0] );
	// pglBlitFramebuffer( 0, 0, gpGlobals->render_width, gpGlobals->render_height, 0, 0, gpGlobals->width, gpGlobals->height, GL_COLOR_BUFFER_BIT, GL_LINEAR );

	// pglBindFramebuffer( GL_READ_FRAMEBUFFER, gl_fbo.fbo[1] );
	// pglBlitFramebuffer( 0, 0, gpGlobals->width, gpGlobals->height, 0, 0, gpGlobals->width, gpGlobals->height, GL_COLOR_BUFFER_BIT, GL_LINEAR );

	pglBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );
}