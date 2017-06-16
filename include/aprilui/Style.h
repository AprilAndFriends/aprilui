/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a style class that contains custom all parameters.

#ifndef APRILUI_STYLE_H
#define APRILUI_STYLE_H

#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Cloneable.h"

namespace aprilui
{
	class Dataset;

	class apriluiExport Style : public Cloneable
	{
		APRILUI_CLONEABLE(Style);
	public:
		friend class Dataset;

		class apriluiExport Group
		{
		public:
			hmap<hstr, hstr> properties;

			Group(hmap<hstr, hstr> properties = hmap<hstr, hstr>());
			~Group();

		};

		Style(chstr name = "");
		~Style();

		HL_DEFINE_GET(hstr, name, Name);
		HL_DEFINE_GET(Dataset*, dataset, Dataset);
		hstr getFullName() const;

		hmap<hstr, hstr> findProperties(chstr className);

	protected:
		hstr name;
		Dataset* dataset;
		Group objectDefaults;
		Group animatorDefaults;
		hmap<hstr, Group> objects;
		hmap<hstr, Group> animators;

		void _inject(Style* other);
		Style* _injected(Style* other);

	};

}
#endif
