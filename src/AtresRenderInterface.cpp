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
#include "AtresRenderInterface.h"
#include "atres/Atres.h"
#include "april/RenderSystem.h"
#include "Exception.h"
#include <map>

namespace AprilUI
{
	extern std::map<int,April::Texture*> g_font_textures;
	
	AtresAprilInterface::AtresAprilInterface()
	{
		
	}

	AtresAprilInterface::~AtresAprilInterface()
	{
		
	}

	unsigned int AtresAprilInterface::loadResource(const char* filename)
	{
		static int index=1;
		April::Texture* t=rendersys->loadTexture(filename);
		if (!t) throw FileNotFoundException(std::string(filename));
		g_font_textures[index]=t;
		return index++;
	}

	void AtresAprilInterface::render(Atres::CharacterRenderOp* rops,int n)
	{
		April::TexturedVertex v[4096];
		April::Texture* t=g_font_textures[rops[0].resource];
		int i=0;
		float w=(float)t->getWidth(),h=(float)t->getHeight();
		for (Atres::CharacterRenderOp* op=rops;op < rops+n;op++)
		{
			v[i].x=op->dx;        v[i].y=op->dy;        v[i].z=0; v[i].u=op->sx/w;          v[i].v=op->sy/h;          i++;
			v[i].x=op->dx+op->dw; v[i].y=op->dy;        v[i].z=0; v[i].u=(op->sx+op->sw)/w; v[i].v=op->sy/h;          i++;
			v[i].x=op->dx;        v[i].y=op->dy+op->dh; v[i].z=0; v[i].u=op->sx/w;          v[i].v=(op->sy+op->sh)/h; i++;
			v[i].x=op->dx+op->dw; v[i].y=op->dy;        v[i].z=0; v[i].u=(op->sx+op->sw)/w; v[i].v=op->sy/h;          i++;
			v[i].x=op->dx+op->dw; v[i].y=op->dy+op->dh; v[i].z=0; v[i].u=(op->sx+op->sw)/w; v[i].v=(op->sy+op->sh)/h; i++;
			v[i].x=op->dx;        v[i].y=op->dy+op->dh; v[i].z=0; v[i].u=op->sx/w;          v[i].v=(op->sy+op->sh)/h; i++;

//			v[i].x=(op->sx+op->sw)/w; v[i].y=op->sy/h;          v[i].z=0; v[i].u=op->dx+op->dw; v[i].v=op->dy; i++;
//			v[i].x=(op->sx+op->sw)/w; v[i].y=(op->sy+op->sh)/h; v[i].z=0; v[i].u=op->dx+op->dw; v[i].v=op->dy+op->dh; i++;
//			v[i].x=op->sx/w;          v[i].y=(op->sy+op->sh)/h; v[i].z=0; v[i].u=op->dx;        v[i].v=op->dy+op->dh; i++;
		}
		rendersys->setTexture(t);
		rendersys->render(April::TriangleList,v,i,rops[0].r/255.0f,rops[0].g/255.0f,rops[0].b/255.0f,rops[0].a/255.0f);
	}
}
