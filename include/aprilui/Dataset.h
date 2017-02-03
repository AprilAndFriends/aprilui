/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
	class RamTexture;

	class apriluiExport Dataset : public EventReceiver
	{
	public:
		virtual Object* parseObject(hlxml::Node* node, Object* parent = NULL);
		
		Dataset(chstr filename, chstr name = "");
		~Dataset();
		
		virtual hstr getFilePath() { return mFilePath; }

		virtual void load(chstr path = "");
		virtual void unload();
		virtual bool isLoaded() { return mLoaded; }
		void registerManualObject(Object* o);
		void unregisterManualObject(Object* o);
		void registerObjects(Object* object, bool unused = true); // aprilui trunk compatibility
		void unregisterObjects(Object* object); // aprilui trunk compatibility
		void registerManualImage(Image* img);
		void unregisterManualImage(Image* img);
		void registerImage(Image* img); // aprilui trunk compatibility
		void unregisterImage(Image* img); // aprilui trunk compatibility
		void registerManualTexture(Texture* tex);
		void registerTexture(Texture* texture); // aprilui trunk compatibility
		void unregisterTexture(Texture* texture); // aprilui trunk compatibility

		virtual bool hasImage(chstr name) { return mImages.hasKey(name); }
		virtual bool hasRamTexture(chstr name) { return mRamTextures.hasKey(name); }
		virtual void registerCallback(chstr name, void (*callback)());
		virtual void triggerCallback(chstr name);
		
		virtual bool isAnimated();
		void setFocusedObject(Object* object) { mFocusedObject = object; }
		Object* getFocusedObject() { return mFocusedObject; }
		
		// use these functions only in debug purposes
		virtual void _setFilename(chstr filename) { mFilename = filename; }
		virtual void _setFilenamePrefix(chstr prefix) { mFilenamePrefix = prefix; }
		virtual hstr _getFilename() { return mFilename; }
		virtual hstr _getFilenamePrefix() { return mFilenamePrefix; }
		hmap<hstr, Object*>& getObjects() { return mObjects; }

		virtual void updateTextures(float k);
		virtual void update(float k);
		virtual void draw();
		
		void _destroyTexture(chstr tex);
		void _destroyImage(chstr img);
		void _destroyTexture(Texture* tex);
		void _destroyImage(Image* img);
		void destroyObject(Object* object);

		void OnMouseDown(float x, float y, int button);
		void OnMouseUp(float x, float y, int button);
		void OnMouseMove(float x, float y);
		void OnKeyDown(unsigned int keyCode);
		void OnKeyUp(unsigned int keyCode);
		void OnChar(unsigned int charCode);
		
		virtual Object* getObject(chstr name);
		virtual Texture* getTexture(chstr name);
		virtual RamTexture* getRamTexture(chstr name);
		virtual Image* getImage(chstr name);
		virtual hstr getTextEntry(chstr textKey);
		virtual hstr getText(chstr compositeTextKey);
		virtual bool textExists(chstr name);
		virtual hmap<hstr, hstr>& getTexts() { return mTexts; }
		virtual harray<hstr> getTextEntries(harray<hstr> keys);

		virtual hstr getName() { return mName; }
		
		void setTextureExtensionOverride(hstr override) { mTexExtOverride = override; }

		virtual void notifyEvent(chstr name, void* params);
		virtual void reloadTexts();
		virtual void reloadTextures();

	protected:
		hstr mName;
		hstr mFilename, mFilePath;
		hstr mFilenamePrefix;
		bool mLoaded;
		hstr mTexExtOverride;
		Object* mFocusedObject;
		hmap<hstr, Object*> mObjects;
		hmap<hstr, Texture*> mTextures;
		hmap<hstr, Image*> mImages;
		hmap<hstr, hstr> mTexts;
		hmap<hstr, RamTexture*> mRamTextures;

		hmap<hstr, void (*)()> mCallbacks;

		virtual Texture* parseTexture(hlxml::Node* node);
		virtual void parseRamTexture(hlxml::Node* node);
		virtual void parseCompositeImage(hlxml::Node* node);
		virtual void parseExternalXMLNode(hlxml::Node* node) { }
		virtual Object* parseExternalObjectClass(hlxml::Node* node, chstr obj_name, grect rect) { return 0; }
		
		virtual Object* recursiveObjectParse(hlxml::Node* node, Object* parent);
		
		virtual void readFile(chstr filename);
		virtual void _loadTexts(hstr path);
		hstr _makeTextsPath();
		
		virtual hstr _makeFilePath(chstr filename, chstr name, bool useNameBasePath);
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
