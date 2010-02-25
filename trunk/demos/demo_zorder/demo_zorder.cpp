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
#include "april/RenderSystem.h"
#include "aprilui/AprilUI.h"
#include "aprilui/Dataset.h"
#include "aprilui/Objects.h"
#include "atres/Atres.h"
#include <stdio.h>
#include <stdlib.h>

AprilUI::Dataset* dataset;

std::string str(int i)
{
	char s[32];
	sprintf(s,"%d",i);
	return std::string(s);
}

bool render(float time_increase)
{
	rendersys->clear();
	rendersys->setViewport(800,600);

	dataset->getObject("root")->draw();

	int i=rand()%7+1;
	
	dataset->getObject("obj0"+str(i))->setZOrder(rand()%100);

	return true;
}

int main()
{
	April::init("OpenGL",800,600,0,"demo_zorder");
	rendersys->registerUpdateCallback(render);
	AprilUI::init();
	dataset=new AprilUI::Dataset("../media/demo_zorder.datadef");
	dataset->load();

	rendersys->enterMainLoop();
	delete dataset;
	AprilUI::destroy();
	April::destroy();

	return 0;
}
