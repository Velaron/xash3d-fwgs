/*
aasset.c - android assets support for filesystem
Copyright (C) 2022 Velaron

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "port.h"

#if XASH_ANDROID

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stddef.h>
#include STDINT_H
#include "filesystem_internal.h"
#include "crtlib.h"
#include "common/com_strings.h"
#include <SDL.h>

struct aasset_s
{
	string path;
//	int		infotableofs;
//	int		numlumps;
//	poolhandle_t mempool;			// W_ReadLump temp buffers
//	file_t		*handle;
//	dlumpinfo_t	*lumps;
//	time_t		filetime;
};

int FS_FileTimeAAsset( aasset_t *aasset )
{
    return 0;
}

int FS_FindFileAAsset( aasset_t *aasset, const char *name )
{
    int result = 0;
    SDL_RWops *file = SDL_RWFromFile( name, "r" );

    if ( file )
    {
        result = file->type == SDL_RWOPS_JNIFILE;
        SDL_RWclose(file);
    }

    return result;
}

void FS_CloseAAsset( aasset_t *aasset )
{
    return;
}

void FS_SearchAAsset( stringlist_t *list, aasset_t *aasset, const char *pattern )
{
    return;
}

file_t *FS_OpenAAssetFile( aasset_t *aasset, const char *filename )
{
    file_t *file = (file_t *)Mem_Calloc( fs_mempool, sizeof( file_t ) );
    SDL_RWops *asset = SDL_RWFromFile( filename, "r" );

    file->handle = -1;
    file->real_length = SDL_RWsize( asset );
    file->offset = 0;
    file->position = 0;
    file->ungetc = EOF;

    return file;
}

qboolean FS_AddAAsset_Fullpath( const char *path, qboolean *already_loaded, int flags )
{
    searchpath_t *search;
	aasset_t *aasset = (aasset_t *)Mem_Calloc( fs_mempool, sizeof( *aasset ) );
	//const char	*ext = COM_FileExtension( zipfile );
	//int		errorcode = ZIP_LOAD_COULDNT_OPEN;

	for( search = fs_searchpaths; search; search = search->next )
	{
		if( search->type == SEARCHPATH_AASSET && !Q_stricmp( search->aasset->path, path ) )
		{
			if( already_loaded ) *already_loaded = true;
			return true; // already loaded
		}
	}

	if( already_loaded ) *already_loaded = false;

    Q_strncpy( aasset->path, path, sizeof( aasset->path ) );

	if( aasset )
	{
		//string	fullpath;
		//int i;

		search = (searchpath_t *)Mem_Calloc( fs_mempool, sizeof( searchpath_t ) );
        Q_strncpy( search->filename, path, sizeof ( search->filename ) );
		search->aasset = aasset;
		search->type = SEARCHPATH_AASSET;
		search->next = fs_searchpaths;
		search->flags |= flags;
		fs_searchpaths = search;

//		Con_Reportf( "Adding zipfile: %s (%i files)\n", zipfile, zip->numfiles );
//
//		// time to add in search list all the wads that contains in current pakfile (if do)
//		for( i = 0; i < zip->numfiles; i++ )
//		{
//			if( !Q_stricmp( COM_FileExtension( zip->files[i].name ), "wad" ))
//			{
//				Q_snprintf( fullpath, MAX_STRING, "%s/%s", zipfile, zip->files[i].name );
//				FS_AddWad_Fullpath( fullpath, NULL, flags );
//			}
//		}
		return true;
	}
	else
	{
//		if( errorcode != ZIP_LOAD_NO_FILES )
//			Con_Reportf( S_ERROR "FS_AddZip_Fullpath: unable to load zip \"%s\"\n", zipfile );
		return false;
	}
}

#endif // XASH_ANDROID
