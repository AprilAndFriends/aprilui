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
#include "hltypes/hstring.h"
#include <stdio.h>
#include <stdlib.h>

AprilUI::Dataset* dataset;

bool render(float time_increase)
{
	rendersys->clear();
	rendersys->setOrthoProjection(800,600);

	dataset->getObject("root")->draw();

	int i=rand()%7+1;
	
	dataset->getObject("obj0"+hstr(i))->setZOrder(rand()%100);

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
