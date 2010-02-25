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
#include <iostream>

AprilUI::Dataset* dataset;

bool render(float time_increase)
{
	rendersys->clear();
	rendersys->setViewport(800,600);

	dataset->getObject("obj01")->draw();
	dataset->getObject("obj02")->draw();

	return true;
}

int main()
{
	try
	{
		April::init("OpenGL",800,600,0,"demo_simple");
		rendersys->registerUpdateCallback(render);
		AprilUI::init();
		dataset=new AprilUI::Dataset("../media/demo_simple.datadef");
		dataset->load();
		Atres::loadFont("../media/arial.font");

		rendersys->enterMainLoop();
		delete dataset;
		AprilUI::destroy();
		April::destroy();
	}
	catch (AprilUI::_GenericException e)
	{
		std::cout << e.repr() << "\n";
	}
	return 0;
}
