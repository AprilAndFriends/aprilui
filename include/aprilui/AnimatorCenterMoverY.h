/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines an animator that can move object centers vertically.

#ifndef APRILUI_CENTER_MOVER_Y_H
#define APRILUI_CENTER_MOVER_Y_H

#pragma message("'aprilui/AnimatorCenterMoverY.h' is deprecated. Include 'aprilui/AnimatorPivotMoverY.h' instead.")
#include "AnimatorCenterMoverY.h"
#include "apriluiExport.h"

namespace aprilui
{
	namespace Animators
	{
		DEPRECATED_ATTRIBUTE typedef PivotMoverY CenterMoverY;
	}
}

#endif
