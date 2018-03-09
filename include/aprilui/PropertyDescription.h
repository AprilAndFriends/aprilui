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
/// Defines a description for properties in objects.

#ifndef APRILUI_PROPERTY_DESCRIPTION_H
#define APRILUI_PROPERTY_DESCRIPTION_H

#include <april/Color.h>
#include <hltypes/henum.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>

#include "apriluiExport.h"

#define _DEFINE_SPECIAL_GET_CLASS(name, type, conversionCode) \
	template <typename O> \
	class apriluiExport Get ## name : public Accessor \
	{ \
	public: \
		type (O::*function)() const;  \
		inline Get ## name(type (O::*function)() const) : Accessor() { this->function = function; } \
		inline Get ## name(type (O::*function)()) : Accessor() { this->function = function; } \
		inline void execute(void* object, hstr& parameter) const { const type& result = (((O*)object)->*this->function)(); parameter = conversionCode; } \
	};

#define _DEFINE_SPECIAL_SET_CLASS(name, type, conversionCode) \
	template <typename O> \
	class apriluiExport Set ## name : public Accessor \
	{ \
	public: \
		void (O::*function)(const type&); \
		inline Set ## name(void (O::*function)(const type&)) : Accessor() { this->function = function; } \
		inline void execute(void* object, hstr& parameter) const { const type& converted = conversionCode; (((O*)object)->*this->function)(converted); } \
	};

namespace aprilui
{
	class apriluiExport PropertyDescription
	{
	public:
		HL_ENUM_CLASS_PREFIX_DECLARE(apriluiExport, Type,
		(
			HL_ENUM_DECLARE(Type, Int);
			HL_ENUM_DECLARE(Type, Float);
			HL_ENUM_DECLARE(Type, Char);
			HL_ENUM_DECLARE(Type, UChar);
			HL_ENUM_DECLARE(Type, Bool);
			HL_ENUM_DECLARE(Type, String);
			HL_ENUM_DECLARE(Type, Enum);
			HL_ENUM_DECLARE(Type, Grect);
			HL_ENUM_DECLARE(Type, Gvec2);
			HL_ENUM_DECLARE(Type, Gvec3);
			HL_ENUM_DECLARE(Type, Color);
		));

		class apriluiExport Accessor
		{
		public:
			inline Accessor() { }

			virtual void execute(void* object, hstr& parameter) const = 0;

		};

		template <typename O, typename T>
		class apriluiExport Get : public Accessor
		{
		public:
			T (O::*function)() const;

			inline Get(T (O::*function)() const) : Accessor() { this->function = function; }
			inline Get(T (O::*function)()) : Accessor() { this->function = function; }

			inline void execute(void* object, hstr& parameter) const { parameter = (((O*)object)->*this->function)(); }

		};

		template <typename O, typename T>
		class apriluiExport Set : public Accessor
		{
		public:
			void (O::*function)(const T&);

			inline Set(void (O::*function)(const T&)) : Accessor() { this->function = function; }

			inline void execute(void* object, hstr& parameter) const { (((O*)object)->*this->function)((T)parameter); }

		};

		_DEFINE_SPECIAL_GET_CLASS(UChar, unsigned char, hstr((int)result));
		_DEFINE_SPECIAL_SET_CLASS(UChar, unsigned char, (unsigned char)(int)parameter);
		_DEFINE_SPECIAL_GET_CLASS(Gvec2, gvec2, april::gvec2ToHstr(result));
		_DEFINE_SPECIAL_SET_CLASS(Gvec2, gvec2, april::hstrToGvec2(parameter));
		_DEFINE_SPECIAL_GET_CLASS(Gvec3, gvec3, april::gvec3ToHstr(result));
		_DEFINE_SPECIAL_SET_CLASS(Gvec3, gvec3, april::hstrToGvec3(parameter));
		_DEFINE_SPECIAL_GET_CLASS(Grect, grect, april::grectToHstr(result));
		_DEFINE_SPECIAL_SET_CLASS(Grect, grect, april::hstrToGrect(parameter));
		_DEFINE_SPECIAL_GET_CLASS(Color, april::Color, result.hex());
		_DEFINE_SPECIAL_SET_CLASS(Color, april::Color, aprilui::_makeColor(parameter));

		PropertyDescription(chstr name, Type type, bool arrayData = false);
		~PropertyDescription();

		HL_DEFINE_GET(hstr, name, Name);
		HL_DEFINE_GET(Type, type, Type);
		HL_DEFINE_IS(arrayData, ArrayData);

	protected:
		hstr name;
		Type type;
		bool arrayData;

	};

}

#undef _DEFINE_SPECIAL_GET_CLASS
#undef _DEFINE_SPECIAL_SET_CLASS

#endif
