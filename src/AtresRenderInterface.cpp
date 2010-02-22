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

namespace AprilUI
{
	AtresAprilInterface::AtresAprilInterface()
	{
		
	}

	AtresAprilInterface::~AtresAprilInterface()
	{
		
	}

	unsigned int AtresAprilInterface::loadResource(std::string filename)
	{
		return 0;//loadTexture(filename.c_str());
	}

	void AtresAprilInterface::render(Atres::CharacterRenderOp* rops,int n)
	{
		/*
		glBindTexture(GL_TEXTURE_2D,rops[0].resource);
		glColor4f(rops[0].r/255.0f,
				  rops[0].g/255.0f,
				  rops[0].b/255.0f,
				  rops[0].a/255.0f);
		
		glBegin(GL_QUADS);
		float w=512,h=1024;
		for (Atres::CharacterRenderOp* op=rops;op < rops+n;op++)
		{
			glTexCoord2d(op->sx/w,op->sy/h);                   glVertex2f(op->dx,op->dy);
			glTexCoord2d((op->sx+op->sw)/w,op->sy/h);          glVertex2f(op->dx+op->dw,op->dy);
			glTexCoord2d((op->sx+op->sw)/w,(op->sy+op->sh)/h); glVertex2f(op->dx+op->dw,op->dy+op->dh);
			glTexCoord2d(op->sx/w,(op->sy+op->sh)/h);          glVertex2f(op->dx,op->dy+op->dh);
		}
		glEnd();
		 */
	}

}