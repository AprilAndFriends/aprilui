/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)                                  *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
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
	rendersys->setOrthoProjection(800,600);

	dataset->getObject("root")->draw();
	dataset->getObject("root")->update(time_increase);
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
