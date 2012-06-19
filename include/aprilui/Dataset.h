/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_DATASET_H
#define APRILUI_DATASET_H

#include <gtypes/Rectangle.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "EventReceiver.h"
#include "Exception.h"

namespace hlxml
{
	class Node;
}

namespace aprilui
{
	#define REGISTER_CALLBACK(dataDict, fn) dataDict->registerCallback(#fn, fn)

	class Object;
	class Image;
	class Texture;

	class apriluiExport Dataset : public EventReceiver
	{
	public:
		Object* parseObject(hlxml::Node* node, Object* parent = NULL);
		
		Dataset(chstr filename, chstr name = "");
		~Dataset();

		void load(chstr path = "");
		void unload();
		bool isLoaded() { return mLoaded; }
		void registerManualObject(Object* o);
		void unregisterManualObject(Object* o);
		void registerManualImage(Image* img);
		void registerManualTexture(Texture* tex);
		void unregisterManualImage(Image* img);

		void registerCallback(chstr name, void (*callback)());
		void triggerCallback(chstr name);
		
		bool isAnimated();
		void setFocusedObject(Object* object) { mFocusedObject = object; }
		Object* getFocusedObject() { return mFocusedObject; }
		
		// use these functions only in debug purposes
		void _setFilename(chstr filename) { mFilename = filename; }
		void _setFilenamePrefix(chstr prefix) { mFilenamePrefix = prefix; }
		hstr _getFilename() { return mFilename; }
		hstr _getFilenamePrefix() { return mFilenamePrefix; }
		hmap<hstr, Object*>& getObjects() { return mObjects; }

		void updateTextures(float k);
		virtual void update(float k);
		void draw();
		
		void _destroyTexture(chstr tex);
		void _destroyImage(chstr img);
		void _destroyTexture(Texture* tex);
		void _destroyImage(Image* img);

		void OnMouseDown(float x, float y, int button);
		void OnMouseUp(float x, float y, int button);
		void OnMouseMove(float x, float y);
		void OnKeyDown(unsigned int keycode);
		void OnKeyUp(unsigned int keycode);
		void OnChar(unsigned int charcode);
		
		virtual Object* getObject(chstr name);
		virtual Texture* getTexture(chstr name);
		virtual Image* getImage(chstr name);
		virtual hstr getTextEntry(chstr textKey);
		virtual hstr getText(chstr compositeTextKey);
		virtual bool textExists(chstr name);
		hmap<hstr, hstr>& getTexts() { return mTexts; }
		harray<hstr> getTextEntries(harray<hstr> keys);

		hstr getName() { return mName; }
		
		void setTextureExtensionOverride(hstr override) { mTexExtOverride = override; }

		void notifyEvent(chstr name, void* params);
		void reloadTexts();
		void reloadTextures();

	protected:
		hstr mName;
		hstr mFilename;
		hstr mFilenamePrefix;
		bool mLoaded;
		hstr mTexExtOverride;
		Object* mFocusedObject;
		hmap<hstr, Object*> mObjects;
		hmap<hstr, Texture*> mTextures;
		hmap<hstr, Image*> mImages;
		hmap<hstr, hstr> mTexts;

		hmap<hstr, void (*)()> mCallbacks;

		Texture* parseTexture(hlxml::Node* node);
		void parseRamTexture(hlxml::Node* node);
		void parseCompositeImage(hlxml::Node* node);
		virtual void parseExternalXMLNode(hlxml::Node* node) { }
		virtual Object* parseExternalObjectClass(hlxml::Node* node, chstr obj_name, grect rect) { return 0; }
		
		Object* recursiveObjectParse(hlxml::Node* node, Object* parent);
		
		void readFile(chstr filename);
		void _loadTexts(hstr path);
		
		hstr _makeFilePath(chstr filename, chstr name, bool useNameBasePath);
		hstr _makeLocalizedTextureName(chstr filename);

		hstr _parseCompositeTextKey(chstr key);
		bool _processCompositeTextKeyArgs(chstr argString, harray<hstr>& args);
		bool _preprocessCompositeTextKeyFormat(chstr format, harray<hstr> args, hstr& preprocessedFormat, harray<hstr>& preprocessedArgs);
		bool _processCompositeTextKeyFormat(chstr format, harray<hstr> args, hstr& result);
		/// @note The returned indexes count the positions relative to the last format tag (minus the 2 characters of the format tag itself), not from the beginning of the string
		bool _getCompositeTextKeyFormatIndexes(chstr format, harray<int>& indexes);

	};

}
#endif
