/// @file
/// @version 4.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines an animator that can move object centers horizontally.

#ifndef APRILUI_CENTER_MOVER_X_H
#define APRILUI_CENTER_MOVER_X_H

#pragma message("'aprilui/AnimatorCenterMoverX.h' is deprecated. Include 'aprilui/AnimatorPivotMoverX.h' instead.")
#include "AnimatorCenterMoverX.h"
#include "apriluiExport.h"

namespace aprilui
{
	namespace Animators
	{
		HL_DEPRECATED("Deprecated API. Use PivotMoverX instead.") typedef PivotMoverX CenterMoverX;
	}
}

#endif
