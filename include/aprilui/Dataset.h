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
	class BaseImage;
	class BaseObject;
	class Event;
	class EventArgs;
	class Image;
	class Object;
	class Style;
	class Texture;
	
	class apriluiExport Dataset : public EventReceiver
	{
		// TODO - enable cloning of dataset
	protected:
		virtual Dataset* clone() const { return NULL; }
		//APRILUI_CLONEABLE(Dataset);
	public:
		BaseObject* parseObject(hlxml::Node* node, Object* parent = NULL);
		
		Dataset(chstr filename, chstr name = "", bool useNameBasePath = false);
		~Dataset();
		
		HL_DEFINE_GET(hstr, name, Name);
		HL_DEFINE_GET(hstr, filePath, FilePath);
		HL_DEFINE_GETSET(hstr, textsPath, TextsPath);
		HL_DEFINE_GET(Object*, focusedObject, FocusedObject);
		HL_DEFINE_GETSET(Object*, root, Root);
		inline hmap<hstr, Object*>& getObjects() { return this->objects; }
		inline hmap<hstr, Animator*>& getAnimators() { return this->animators; }
		inline hmap<hstr, BaseImage*>& getImages() { return this->images; }
		inline hmap<hstr, Texture*>& getTextures() { return this->textures; }
		inline hmap<hstr, Style*>& getStyles() { return this->styles; }
		inline hmap<hstr, hstr>& getTexts() { return this->texts; }
		bool isLoaded() const;
		hmap<hstr, BaseObject*> getAllObjects() const;
		int getFocusedObjectIndex() const;
		bool isAnimated() const;
		bool isWaitingAnimation() const;

		void load();
		void unload();
		void registerObjects(BaseObject* root, bool setRootIfNull = true);
		void unregisterObjects(BaseObject* root);
		void registerTexture(Texture* texture);
		void unregisterTexture(Texture* texture);
		void registerImage(BaseImage* image);
		void unregisterImage(BaseImage* image);
		void registerStyle(Style* style);
		void unregisterStyle(Style* style);

		void registerCallback(chstr name, void (*callback)());
		void triggerCallback(chstr name);
		
		bool trySetFocusedObjectByIndex(int value, bool strict = false);
		harray<int> findPossibleFocusIndices(bool strict = false);
		harray<int> findAllFocusIndices();
		
		template <typename T>
		inline hmap<hstr, T> getObjectsByType() const
		{
			return this->objects.dynamicCastValues<hstr, T>();
		}
		template <typename T>
		inline hmap<hstr, T> getAnimatorsByType() const
		{
			return this->animators.dynamicCastValues<hstr, T>();
		}

		void updateTextures(float timeDelta);
		void unloadUnusedResources();
		void clearChildUnderCursor();
		
		void processEvents();
		void queueCallback(Event* event, EventArgs* args);
		void removeCallbackFromQueue(Event* event);
		
		virtual void update(float timeDelta);
		void draw();
		
		void _destroyTexture(chstr name);
		void _destroyImage(chstr name);
		void _destroyStyle(chstr name);
		void _destroyTexture(Texture* texture);
		void _destroyImage(BaseImage* image);
		void _destroyStyle(Style* style);
		
		void destroyObjects(chstr rootName);
		void destroyObjects(BaseObject* root);
		
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
		
		virtual Object* getObject(chstr name);
		virtual Animator* getAnimator(chstr name);
		Object* tryGetObject(chstr name) const;
		Animator* tryGetAnimator(chstr name) const;
		virtual Texture* getTexture(chstr name);
		virtual BaseImage* getImage(chstr name);
		virtual Style* getStyle(chstr name);
		bool hasTexture(chstr name) const;
		bool hasImage(chstr name) const;
		bool hasStyle(chstr name) const;
		bool hasObject(chstr name) const;
		bool hasAnimator(chstr name) const;

		virtual hstr getTextEntry(chstr textKey);
		virtual bool hasTextEntry(chstr textKey);
		virtual hstr getText(chstr compositeTextKey);
		harray<hstr> getTexts(harray<hstr> keys);
		
		template <typename T>
		inline T getObject(chstr name)
		{
			T object = dynamic_cast<T>(this->getObject(name));
			if (object == NULL)
			{
				_throwInvalidObjectTypeCast("Object", name, this->getName());
			}
			return object;
		}
		template <typename T>
		inline void getObject(chstr name, T& out)
		{
			out = this->getObject<T>(name);
		}
		template <typename T>
		inline T tryGetObject(chstr name)
		{
			T object = dynamic_cast<T>(this->tryGetObject(name));
			if (object == NULL)
			{
				hlog::warn(logTag, "Dynamic cast in getObject<T> failed, object: " + name);
			}
			return object;
		}
		template <typename T>
		inline void tryGetObject(chstr name, T& out)
		{
			out = this->tryGetObject<T>(name);
		}

		template <typename T>
		inline T getAnimator(chstr name)
		{
			T animator = dynamic_cast<T>(this->getAnimator(name));
			if (animator == NULL)
			{
				_throwInvalidObjectTypeCast("Animator", name, this->getName());
			}
			return animator;
		}
		template <typename T>
		inline void getAnimator(chstr name, T& out)
		{
			out = this->getAnimator<T>(name);
		}
		template <typename T>
		inline T tryGetAnimator(chstr name)
		{
			T animator = dynamic_cast<T>(this->tryGetAnimator(name));
			if (animator == NULL)
			{
				hlog::warn(logTag, "Dynamic cast in getAnimator<T> failed, animator: " + name);
			}
			return animator;
		}
		template <typename T>
		inline void tryGetAnimator(chstr name, T& out)
		{
			out = this->tryGetAnimator<T>(name);
		}

		void notifyEvent(chstr type, EventArgs* args);
		void reloadTexts();
		void reloadTextures();
		void focus(Object* object);
		void removeFocus();

		void parseGlobalInclude(chstr path);
		void parseGlobalIncludeFile(chstr filename);
		BaseObject* parseObjectInclude(chstr path, Object* parent, Style* style, chstr namePrefix, chstr nameSuffix, gvec2 offset);
		BaseObject* parseObjectIncludeFile(chstr filename, Object* parent, Style* style, chstr namePrefix, chstr nameSuffix, gvec2 offset);

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
		hmap<hstr, Animator*> animators;
		hmap<hstr, Texture*> textures;
		hmap<hstr, BaseImage*> images;
		hmap<hstr, Style*> styles;
		hmap<hstr, hstr> texts;
		harray<QueuedCallback> callbackQueue;
		hmap<hstr, void (*)()> callbacks;
		hmap<hstr, hlxml::Document*> includeDocuments;
		
		hlxml::Document* _openDocument(chstr filename);
		void _closeDocuments();
		
		void parseTexture(hlxml::Node* node);
		void parseTextureGroup(hlxml::Node* node);
		void parseCompositeImage(hlxml::Node* node);
		void parseStyle(hlxml::Node* node);
		virtual inline void parseExternalXMLNode(hlxml::Node* node) { }
		virtual inline BaseObject* parseExternalObjectClass(hlxml::Node* node, chstr objName, grect rect) { return NULL; }
		
		BaseObject* recursiveObjectParse(hlxml::Node* node, Object* parent);
		BaseObject* recursiveObjectParse(hlxml::Node* node, Object* parent, Style* style, chstr namePrefix, chstr nameSuffix, gvec2 offset);
		BaseObject* recursiveObjectIncludeParse(hlxml::Node* node, Object* parent, Style* style, chstr namePrefix, chstr nameSuffix, gvec2 offset);

		void readFile(chstr filename);
		virtual void _loadTexts(chstr path);
		void _loadTextResource(hstream& data, hmap<hstr, hstr>& textsMap);
		hstr _makeFilePath(chstr filename, chstr name = "", bool useNameBasePath = false);
		hstr _makeLocalizedTextureName(chstr filename);
		hstr _makeTextsPath();
		bool _findTextEntry(chstr textKey, hstr* text);
		
		typedef std::basic_string<unsigned int> ustr;
		
		hstr _parseCompositeTextKey(chstr key);
		bool _processCompositeTextKeyArgs(ustr uArgString, harray<ustr>& uArgs);
		bool _preprocessCompositeTextKeyFormat(ustr uFormat, harray<ustr> uArgs, ustr& uPreprocessedFormat, harray<ustr>& uPreprocessedArgs);
		bool _processCompositeTextKeyFormat(ustr uFormat, harray<ustr> uArgs, hstr& result);
		/// @note The returned indices count the positions relative to the last format tag (minus the 2 characters of the format tag itself), not from the beginning of the string
		bool _getCompositeTextKeyFormatIndices(ustr uFormat, harray<int>& indices);
		harray<ustr> _getArgEntries(ustr uString);
		/// @note This method is here to silence linker warnings on LLVM compiler.
		void _throwInvalidObjectTypeCast(chstr typeName, chstr objName, chstr datasetName);

	};

}
#endif
