/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2011 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "CmD3D9VideoModeList.h"
#include "CmException.h"
#include "CmD3D9RenderSystem.h"

namespace BansheeEngine 
{
	D3D9VideoModeList::D3D9VideoModeList( D3D9Driver* pDriver )
	{
		if( NULL == pDriver )
			CM_EXCEPT(InvalidParametersException, "pDriver parameter is NULL");

		mpDriver = pDriver;
		enumerate();
	}

	D3D9VideoModeList::~D3D9VideoModeList()
	{
		mpDriver = NULL;
		mModeList.clear();
	}

	BOOL D3D9VideoModeList::enumerate()
	{
		UINT iMode;
		IDirect3D9* pD3D = D3D9RenderSystem::getDirect3D9();
		UINT adapter = mpDriver->getAdapterNumber();

		for( iMode=0; iMode < pD3D->GetAdapterModeCount( adapter, D3DFMT_R5G6B5 ); iMode++ )
		{
			D3DDISPLAYMODE displayMode;
			pD3D->EnumAdapterModes( adapter, D3DFMT_R5G6B5, iMode, &displayMode );

			// Filter out low-resolutions
			if( displayMode.Width < 640 || displayMode.Height < 400 )
				continue;

			// Check to see if it is already in the list (to filter out refresh rates)
			BOOL found = FALSE;
			Vector<D3D9VideoMode>::iterator it;
			for( it = mModeList.begin(); it != mModeList.end(); it++ )
			{
				D3DDISPLAYMODE oldDisp = it->getDisplayMode();
				if( oldDisp.Width == displayMode.Width &&
					oldDisp.Height == displayMode.Height &&
					oldDisp.Format == displayMode.Format )
				{
					// Check refresh rate and favour higher if poss
					if (oldDisp.RefreshRate < displayMode.RefreshRate)
						it->increaseRefreshRate(displayMode.RefreshRate);
					found = TRUE;
					break;
				}
			}

			if( !found )
				mModeList.push_back( D3D9VideoMode( displayMode ) );
		}

		for( iMode=0; iMode < pD3D->GetAdapterModeCount( adapter, D3DFMT_X8R8G8B8 ); iMode++ )
		{
			D3DDISPLAYMODE displayMode;
			pD3D->EnumAdapterModes( adapter, D3DFMT_X8R8G8B8, iMode, &displayMode );

			// Filter out low-resolutions
			if( displayMode.Width < 640 || displayMode.Height < 400 )
				continue;

			// Check to see if it is already in the list (to filter out refresh rates)
			BOOL found = FALSE;
			Vector<D3D9VideoMode>::iterator it;
			for( it = mModeList.begin(); it != mModeList.end(); it++ )
			{
				D3DDISPLAYMODE oldDisp = it->getDisplayMode();
				if( oldDisp.Width == displayMode.Width &&
					oldDisp.Height == displayMode.Height &&
					oldDisp.Format == displayMode.Format )
				{
					// Check refresh rate and favour higher if poss
					if (oldDisp.RefreshRate < displayMode.RefreshRate)
						it->increaseRefreshRate(displayMode.RefreshRate);
					found = TRUE;
					break;
				}
			}

			if( !found )
				mModeList.push_back( D3D9VideoMode( displayMode ) );
		}

		return TRUE;
	}

	size_t D3D9VideoModeList::count()
	{
		return mModeList.size();
	}

	D3D9VideoMode* D3D9VideoModeList::item( size_t index )
	{
		Vector<D3D9VideoMode>::iterator p = mModeList.begin();

		return &p[index];
	}

	D3D9VideoMode* D3D9VideoModeList::item( const String &name )
	{
		Vector<D3D9VideoMode>::iterator it = mModeList.begin();
		if (it == mModeList.end())
			return NULL;

		for (;it != mModeList.end(); ++it)
		{
			if (it->getDescription() == name)
				return &(*it);
		}

		return NULL;
	}
}
