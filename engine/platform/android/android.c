/*
android_nosdl.c - android backend
Copyright (C) 2016-2019 mittorn

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/
#include "platform/platform.h"
#if !defined(XASH_DEDICATED)
#include "input.h"
#include "client.h"
#include "sound.h"
#include "errno.h"
#include <pthread.h>
#include <sys/prctl.h>

#include <android/log.h>
#include <jni.h>

//convar_t *android_sleep;
struct jnimethods_s
{
	JavaVM *vm;
	JNIEnv *env;
    jobject activity;
    jclass actcls;
	jmethodID enableTextInput;
	jmethodID vibrate;
	jmethodID messageBox;
	jmethodID notify;
	jmethodID setTitle;
	jmethodID setIcon;
	jmethodID getAndroidId;
	jmethodID saveID;
	jmethodID loadID;
	jmethodID showMouse;
	jmethodID shellExecute;
	jmethodID swapBuffers;
	jmethodID toggleEGL;
	jmethodID createGLContext;
	jmethodID getGLAttribute;
	jmethodID deleteGLContext;
	jmethodID getSelectedPixelFormat;
	jmethodID getSurface;
	int width, height;
} jni;

//typedef void (*pfnChangeGame)( const char *progname );
//int EXPORT Host_Main( int argc, char **argv, const char *progname, int bChangeGame, pfnChangeGame func );

void Android_Init( void )
{
    jni.env = (JNIEnv *) SDL_AndroidGetJNIEnv();
    jni.activity = (jobject) SDL_AndroidGetActivity();
    jni.actcls = (*jni.env)->GetObjectClass( jni.env, jni.activity );
    jni.loadID = (*jni.env)->GetMethodID( jni.env, jni.actcls, "loadID", "()Ljava/lang/String;" );
    jni.getAndroidId = (*jni.env)->GetMethodID( jni.env, jni.actcls, "getAndroidID", "()Ljava/lang/String;" );
    jni.saveID = (*jni.env)->GetMethodID( jni.env, jni.actcls, "saveID", "(Ljava/lang/String;)V" );
//	jni.enableTextInput = (*env)->GetStaticMethodID(env, jni.actcls, "showKeyboard", "(I)V");
//	jni.vibrate = (*env)->GetStaticMethodID(env, jni.actcls, "vibrate", "(I)V" );
//	jni.messageBox = (*env)->GetStaticMethodID(env, jni.actcls, "messageBox", "(Ljava/lang/String;Ljava/lang/String;)V");
//	jni.notify = (*env)->GetStaticMethodID(env, jni.actcls, "engineThreadNotify", "()V");
//	jni.setTitle = (*env)->GetStaticMethodID(env, jni.actcls, "setTitle", "(Ljava/lang/String;)V");
//	jni.setIcon = (*env)->GetStaticMethodID(env, jni.actcls, "setIcon", "(Ljava/lang/String;)V");
//
//
//	jni.showMouse = (*env)->GetStaticMethodID(env, jni.actcls, "showMouse", "(I)V");
//	jni.shellExecute = (*env)->GetStaticMethodID(env, jni.actcls, "shellExecute", "(Ljava/lang/String;)V");
//
//	jni.swapBuffers = (*env)->GetStaticMethodID(env, jni.actcls, "swapBuffers", "()V");
//	jni.toggleEGL = (*env)->GetStaticMethodID(env, jni.actcls, "toggleEGL", "(I)V");
//	jni.createGLContext = (*env)->GetStaticMethodID(env, jni.actcls, "createGLContext", "([I[I)Z");
//	jni.getGLAttribute = (*env)->GetStaticMethodID(env, jni.actcls, "getGLAttribute", "(I)I");
//	jni.deleteGLContext = (*env)->GetStaticMethodID(env, jni.actcls, "deleteGLContext", "()Z");
//	jni.getSelectedPixelFormat = (*env)->GetStaticMethodID(env, jni.actcls, "getSelectedPixelFormat", "()I");
//	jni.getSurface = (*env)->GetStaticMethodID(env, jni.actcls, "getNativeSurface", "()Landroid/view/Surface;");

//    android_sleep = Cvar_Get( "android_sleep", "1", FCVAR_ARCHIVE, "Enable sleep in background" );
}

/*
========================
Android_MessageBox

Show messagebox and wait for OK button press
========================
*/
#if XASH_MESSAGEBOX == MSGBOX_ANDROID
void Platform_MessageBox( const char *title, const char *text, qboolean parentMainWindow )
{
	(*jni.env)->CallStaticVoidMethod( jni.env, jni.actcls, jni.messageBox, (*jni.env)->NewStringUTF( jni.env, title ), (*jni.env)->NewStringUTF( jni.env ,text ) );
}
#endif // XASH_MESSAGEBOX == MSGBOX_ANDROID

/*
========================
Android_GetNativeObject
========================
*/

void *Android_GetNativeObject( const char *name )
{
	static const char *availObjects[] = { "JNIEnv", "ActivityClass", NULL };
	void *object = NULL;

	if( !name )
	{
		object = (void*)availObjects;
	}
	else if( !strcasecmp( name, "JNIEnv" ) )
	{
		object = (void*)jni.env;
	}
	else if( !strcasecmp( name, "ActivityClass" ) )
	{
		object = (void*)jni.actcls;
	}

	return object;
}

/*
========================
Android_GetAndroidID
========================
*/
const char *Android_GetAndroidID( void )
{
	static char id[32];
	jstring resultJNIStr;
    const char *resultCStr;

    if ( COM_CheckString( id ) ) return id;

	resultJNIStr = (*jni.env)->CallObjectMethod( jni.env, jni.activity, jni.getAndroidId );
	resultCStr = (*jni.env)->GetStringUTFChars( jni.env, resultJNIStr, NULL );
	Q_strncpy( id, resultCStr, sizeof(id) );
	(*jni.env)->ReleaseStringUTFChars( jni.env, resultJNIStr, resultCStr );

	return id;
}

/*
========================
Android_LoadID
========================
*/
const char *Android_LoadID( void )
{
    static char id[32];
    jstring resultJNIStr;
    const char *resultCStr;

    resultJNIStr = (*jni.env)->CallObjectMethod( jni.env, jni.activity, jni.loadID );
	resultCStr = (*jni.env)->GetStringUTFChars( jni.env, resultJNIStr, NULL );
    Q_strncpy( id, resultCStr, sizeof(id) );
	(*jni.env)->ReleaseStringUTFChars( jni.env, resultJNIStr, resultCStr );

	return id;
}

/*
========================
Android_SaveID
========================
*/
void Android_SaveID( const char *id )
{
	(*jni.env)->CallVoidMethod( jni.env, jni.activity, jni.saveID, (*jni.env)->NewStringUTF( jni.env, id ) );
}

/*
========================
Android_AddMove
========================
*/
void Android_AddMove( float x, float y )
{
//	jnimouse.x += x;
//	jnimouse.y += y;
}

/*
========================
Android_ShowMouse
========================
*/
void Android_ShowMouse( qboolean show )
{
	if( m_ignore->value )
		show = true;
	(*jni.env)->CallStaticVoidMethod( jni.env, jni.actcls, jni.showMouse, show );
}

/*
========================
Android_ShellExecute
========================
*/
void Platform_ShellExecute( const char *path, const char *parms )
{
	jstring jstr;

	if( !path )
		return; // useless

	// get java.lang.String
	jstr = (*jni.env)->NewStringUTF( jni.env, path );

	// open browser
	(*jni.env)->CallStaticVoidMethod(jni.env, jni.actcls, jni.shellExecute, jstr);

	// no need to free jstr
}

/*
========================
Android_RunEvents

Execute all events from queue
========================
*/
/*
void Platform_RunEvents( void )
{
	int i;

	// enter events read
	Android_Lock();
	pthread_mutex_unlock( &events.framemutex );

	for( i = 0; i < events.count; i++ )
	{
		switch( events.queue[i].type )
		{
		case event_touch_down:
		case event_touch_up:
		case event_touch_move:
			IN_TouchEvent( (touchEventType)events.queue[i].type, events.queue[i].arg,
						   events.queue[i].touch.x, events.queue[i].touch.y,
						   events.queue[i].touch.dx, events.queue[i].touch.dy );
			break;

		case event_key_down:
			Key_Event( events.queue[i].arg, true );

			if( events.queue[i].arg == K_AUX31 || events.queue[i].arg == K_AUX29 )
			{
				host.force_draw_version = true;
				host.force_draw_version_time = host.realtime + FORCE_DRAW_VERSION_TIME;
			}
			break;
		case event_key_up:
			Key_Event( events.queue[i].arg, false );

			if( events.queue[i].arg == K_AUX31 || events.queue[i].arg == K_AUX29 )
			{
				host.force_draw_version = true;
				host.force_draw_version_time = host.realtime + FORCE_DRAW_VERSION_TIME;
			}
			break;

		case event_set_pause:
			// destroy EGL surface when hiding application
			if( !events.queue[i].arg )
			{
				SNDDMA_Activate( true );
//				(*jni.env)->CallStaticVoidMethod( jni.env, jni.actcls, jni.toggleEGL, 1 );
				Android_UpdateSurface( true );
				host.status = HOST_FRAME;
				SetBits( gl_vsync->flags, FCVAR_CHANGED ); // set swap interval
				host.force_draw_version = true;
				host.force_draw_version_time = host.realtime + FORCE_DRAW_VERSION_TIME;
			}

			if( events.queue[i].arg )
			{
				SNDDMA_Activate( false );
				Android_UpdateSurface( false );
				host.status = HOST_NOFOCUS;
//				(*jni.env)->CallStaticVoidMethod( jni.env, jni.actcls, jni.toggleEGL, 0 );
			}
			break;

		case event_resize:
			// reinitialize EGL and change engine screen size
			if( host.status == HOST_FRAME &&( refState.width != jni.width || refState.height != jni.height ) )
			{
//				(*jni.env)->CallStaticVoidMethod( jni.env, jni.actcls, jni.toggleEGL, 0 );
//				(*jni.env)->CallStaticVoidMethod( jni.env, jni.actcls, jni.toggleEGL, 1 );
				Android_UpdateSurface( true );
				SetBits( gl_vsync->flags, FCVAR_CHANGED ); // set swap interval
				VID_SetMode();
			}
			break;
		case event_joyadd:
			Joy_AddEvent();
			break;
		case event_joyremove:
			Joy_RemoveEvent();
			break;
		case event_joyball:
			if( !Joy_IsActive() )
				Joy_AddEvent();
			Joy_BallMotionEvent( events.queue[i].ball.ball,
				events.queue[i].ball.xrel, events.queue[i].ball.yrel );
			break;
		case event_joyhat:
			if( !Joy_IsActive() )
				Joy_AddEvent();
			Joy_HatMotionEvent( events.queue[i].hat.hat, events.queue[i].hat.key );
			break;
		case event_joyaxis:
			if( !Joy_IsActive() )
				Joy_AddEvent();
			Joy_AxisMotionEvent( events.queue[i].axis.axis, events.queue[i].axis.val );
			break;
		case event_joybutton:
			if( !Joy_IsActive() )
				Joy_AddEvent();
			Joy_ButtonEvent( events.queue[i].button.button, (byte)events.queue[i].button.down );
			break;
		case event_ondestroy:
			//host.skip_configs = true; // skip config save, because engine may be killed during config save
			Sys_Quit();
			(*jni.env)->CallStaticVoidMethod( jni.env, jni.actcls, jni.notify );
			break;
		case event_onpause:
#ifdef PARANOID_CONFIG_SAVE
			switch( host.status )
			{
			case HOST_INIT:
			case HOST_CRASHED:
			case HOST_ERR_FATAL:
				Con_Reportf( S_WARN "Abnormal host state during onPause (%d), skipping config save!\n", host.status );
				break;
			default:
				// restore all latched cheat cvars
				Cvar_SetCheatState( true );
				Host_WriteConfig();
			}
#endif
			// disable sound during call/screen-off
			SNDDMA_Activate( false );
//			host.status = HOST_NOFOCUS;
			// stop blocking UI thread
			(*jni.env)->CallStaticVoidMethod( jni.env, jni.actcls, jni.notify );

			break;
		case event_onresume:
			// re-enable sound after onPause
//			host.status = HOST_FRAME;
			SNDDMA_Activate( true );
			host.force_draw_version = true;
			host.force_draw_version_time = host.realtime + FORCE_DRAW_VERSION_TIME;
			break;
		case event_onfocuschange:
			host.force_draw_version = true;
			host.force_draw_version_time = host.realtime + FORCE_DRAW_VERSION_TIME;
			break;
		}
	}

	events.count = 0; // no more events

	// text input handled separately to allow unicode symbols
	for( i = 0; events.inputtext[i]; i++ )
	{
		int ch;

		// if engine does not use utf-8, we need to convert it to preferred encoding
		if( !Q_stricmp( cl_charset->string, "utf-8" ) )
			ch = (unsigned char)events.inputtext[i];
		else
			ch = Con_UtfProcessCharForce( (unsigned char)events.inputtext[i] );

		if( !ch ) // utf-8
			continue;

		// some keyboards may send enter as text
		if( ch == '\n' )
		{
			Key_Event( K_ENTER, true );
			Key_Event( K_ENTER, false );
			continue;
		}

		// otherwise just push it by char, text render will decode unicode strings
		CL_CharEvent( ch );
	}
	events.inputtext[0] = 0; // no more text

	jnimouse.x += events.mousex;
	events.mousex = 0;
	jnimouse.y += events.mousey;
	events.mousey = 0;

	//end events read
	Android_Unlock();
	pthread_mutex_lock( &events.framemutex );
}
*/
#endif // XASH_DEDICATED
