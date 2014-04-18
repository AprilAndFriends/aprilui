/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.05
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a dataset.

#ifndef APRILUI_DATASET_H
#define APRILUI_DATASET_H

#include <gtypes/Rectangle.h>
#include <hltypes/hmap.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "apriluiExport.h"
#include "EventReceiver.h"
#include "Exception.h"

namespace hlxml
{
	class Document;
	class Node;
}

namespace aprilui
{
	#define REGISTER_CALLBACK(dataDict, fn) dataDict->registerCallback(#fn, fn)
	
	class Object;
	class Image;
	class Texture;
	class NullImage;
	class Event;
	class EventArgs;
	
	class apriluiExport Dataset : public EventReceiver
	{
	public:
		Object* parseObject(hlxml::Node* node, Object* parent = NULL);
		
		Dataset(chstr filename, chstr name = "", bool useNameBasePath = false);
		~Dataset();
		
		HL_DEFINE_GET(hstr, name, Name);
		HL_DEFINE_GET(hstr, filePath, FilePath);
		HL_DEFINE_GETSET(hstr, textsPath, TextsPath);
		HL_DEFINE_IS(loaded, Loaded);
		HL_DEFINE_GET(Object*, focusedObject, FocusedObject);
		HL_DEFINE_GETSET(Object*, root, Root);
		inline hmap<hstr, Object*>& getObjects() { return this->objects; }
		inline hmap<hstr, Image*>& getImages() { return this->images; }
		inline hmap<hstr, Texture*>& getTextures() { return this->textures; }
		inline hmap<hstr, hstr>& getTexts() { return this->texts; }
		bool isAnimated();
		bool isWaitingAnimation();
		int getFocusedObjectIndex();

		void load();
		void unload();
		void registerManualObject(Object* o);
		void unregisterManualObject(Object* o);
		void registerManualImage(Image* img);
		void unregisterManualImage(Image* img);
		void registerManualTexture(Texture* tex);
		void unregisterManualTexture(Texture* tex);
		
		void registerCallback(chstr name, void (*callback)());
		void triggerCallback(chstr name);
		
		bool trySetFocusedObjectByIndex(int value, bool strict = false);
		harray<int> findPossibleFocusIndices(bool strict = false);
		harray<int> findAllFocusIndices();
		
		template <class T>
		inline hmap<hstr, T> getObjectsByType()
		{
			return this->objects.dyn_cast_value<hstr, T>();
		}
		
		void updateTextures(float k);
		void unloadUnusedTextures();
		void clearChildUnderCursor();
		
		void processEvents();
		void queueCallback(Event* event, EventArgs* args);
		void removeCallbackFromQueue(Event* event);
		
		virtual void update(float k);
		void draw();
		
		void _destroyTexture(chstr tex);
		void _destroyImage(chstr img);
		void _destroyTexture(Texture* tex);
		void _destroyImage(Image* img);
		
		void destroyObject(chstr name);
		void destroyObject(Object* object);
		
		bool onMouseDown(april::Key keyCode);
		bool onMouseUp(april::Key keyCode);
		bool onMouseCancel(april::Key keyCode);
		bool onMouseMove();
		bool onMouseScroll(float x, float y);
		bool onKeyDown(april::Key keyCode);
		bool onKeyUp(april::Key keyCode);
		bool onChar(unsigned int charCode);
		bool onTouch(const harray<gvec2>& touches);
		bool onButtonDown(april::Button buttonCode);
		bool onButtonUp(april::Button buttonCode);
		
		virtual Texture* getTexture(chstr name);
		virtual Image* getImage(chstr name);
		virtual hstr getTextEntry(chstr textKey);
		virtual bool hasTextEntry(chstr textKey);
		virtual hstr getText(chstr compositeTextKey);
		harray<hstr> getTexts(harray<hstr> keys);
		
		virtual Object* getObject(chstr name);
		bool hasImage(chstr name);
		bool hasTexture(chstr name);
		bool hasObject(chstr name);
		Object* tryGetObject(chstr name);
		
		template <class T>
		inline T getObject(chstr name)
		{
			T object = dynamic_cast<T>(this->getObject(name));
			if (object == NULL)
			{
				throw InvalidObjectTypeCast(hsprintf("Object '%s' found in dataset '%s' but dynamic cast failed.", name.c_str(), this->getName().c_str()));
			}
			return object;
		}
		template <class T>
		inline void getObject(chstr name, T& out)
		{
			out = this->getObject<T>(name);
		}
		template <class T>
		inline T tryGetObject(chstr name)
		{
			T object = dynamic_cast<T>(this->tryGetObject(name));
			if (object == NULL)
			{
				hlog::warn(aprilui::logTag, "Dynamic cast in getObject<T> failed, object: " + name);
			}
			return object;
		}
		template <class T>
		inline void tryGetObject(chstr name, T& out)
		{
			out = this->tryGetObject<T>(name);
		}
		
		void notifyEvent(chstr name, void* params);
		void reloadTexts();
		void reloadTextures();
		void focus(Object* object);
		void removeFocus();

		void parseObjectIncludeFile(chstr filename, Object* parent, chstr namePrefix, chstr nameSuffix, gvec2 offset);

	protected:
		struct QueuedCallback
		{
			aprilui::Event* event;
			aprilui::EventArgs* args;
		};

		hstr name;
		hstr filename;
		hstr filePath;
		hstr textsPath;
		bool loaded;
		Object* focusedObject;
		Object* root;
		hmap<hstr, Object*> objects;
		hmap<hstr, Texture*> textures;
		hmap<hstr, Image*> images;
		hmap<hstr, hstr> texts;
		NullImage* nullImage;
		harray<QueuedCallback> callbackQueue;
		hmap<hstr, void (*)()> callbacks;
		hmap<hstr, hlxml::Document*> includeDocuments;
		
		hlxml::Document* _openDocument(chstr filename);
		void _closeDocuments();
		
		void parseTexture(hlxml::Node* node);
		void parseRamTexture(hlxml::Node* node);
		void parseTextureGroup(hlxml::Node* node);
		void parseCompositeImage(hlxml::Node* node);
		void parseGlobalInclude(chstr path);
		void parseObjectInclude(chstr path, Object* parent, chstr namePrefix, chstr nameSuffix, gvec2 offset);
		virtual inline void parseExternalXMLNode(hlxml::Node* node) { }
		virtual inline Object* parseExternalObjectClass(hlxml::Node* node, chstr objName, grect rect) { return 0; }
		
		Object* recursiveObjectParse(hlxml::Node* node, Object* parent);
		Object* recursiveObjectParse(hlxml::Node* node, Object* parent, chstr namePrefix, chstr nameSuffix, gvec2 offset);
		
		void readFile(chstr filename);
		void _loadTexts(chstr path);
		hstr _makeFilePath(chstr filename, chstr name = "", bool useNameBasePath = false);
		hstr _makeLocalizedTextureName(chstr filename);
		hstr _makeTextsPath();
		bool _findTextEntry(chstr textKey, hstr* text);
		
		typedef std::basic_string<unsigned int> ustr;
		
		hstr _parseCompositeTextKey(chstr key);
		bool _processCompositeTextKeyArgs(ustr argString, harray<ustr>& args);
		bool _preprocessCompositeTextKeyFormat(ustr format, harray<ustr> args, ustr& preprocessedFormat, harray<ustr>& preprocessedArgs);
		bool _processCompositeTextKeyFormat(ustr format, harray<ustr> args, hstr& result);
		/// @note The returned indexes count the positions relative to the last format tag (minus the 2 characters of the format tag itself), not from the beginning of the string
		bool _getCompositeTextKeyFormatIndexes(ustr format, harray<int>& indexes);
		harray<ustr> _getArgEntries(ustr string);

	};

}
#endif
