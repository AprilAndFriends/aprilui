/************************************************************************************
This source file is part of the APRIL User Interface Library
For latest info, see http://libatres.sourceforge.net/
*************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License (LGPL) as published by the
Free Software Foundation; either version 2 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*************************************************************************************/
#include "AprilUI.h"
#include "atres/Atres.h"
#include "AtresRenderInterface.h"
#include "april/RenderSystem.h"
#include <map>

namespace AprilUI
{
	std::map<int,April::Texture*> g_font_textures;
	AtresAprilInterface* atres_render_iface=0;
	void init()
	{
		atres_render_iface=new AtresAprilInterface();
		Atres::setRenderInterface(atres_render_iface);
	}
	
	void destroy()
	{
		if (atres_render_iface) delete atres_render_iface;

		for (std::map<int,April::Texture*>::iterator it=g_font_textures.begin();it!=g_font_textures.end();it++)
		{
			delete it->second;
		}
	}
}
