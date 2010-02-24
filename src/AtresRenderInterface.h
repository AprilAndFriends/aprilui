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
#ifndef APRIL_ATRES_RENDER_IFACE_H
#define APRIL_ATRES_RENDER_IFACE_H

#include "atres/RenderingInterface.h"

namespace AprilUI
{
class AtresAprilInterface : public Atres::RenderInterface
{
public:
	AtresAprilInterface();
	~AtresAprilInterface();
	
	unsigned int loadResource(std::string filename);
	void render(Atres::CharacterRenderOp* rops,int n);
};
}

#endif
