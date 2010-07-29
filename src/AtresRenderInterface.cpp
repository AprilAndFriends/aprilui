/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <map>

#include <april/RenderSystem.h>
#include <atres/Atres.h>
#include <hltypes/hstring.h>

#include "AtresRenderInterface.h"
#include "Exception.h"

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
		if (!t) throw FileNotFoundException(hstr(filename));
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
