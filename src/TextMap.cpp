/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "AprilUI.h"
#include "Exception.h"
#include "TextMap.h"
#include "Util.h"

namespace AprilUI
{
	#define BLOCK_SIZE 16384

	#ifndef _DEBUG
		char key_error_text[128];
	#endif

	TextMap::TextMap()
	{
		mBuffer=0;
		mBufferPos=1; // 1 in order to detect KeyErrors
	}
	
	TextMap::~TextMap()
	{
		unload();
	}

	bool TextMap::exists(chstr name)
	{
		return (mTexts.find(name) != mTexts.end());
	}

	void TextMap::load(chstr folder)
	{
		mName=folder;
		FILE* f;
		char buff[2][513];
		int len,mode,c; // mode: 0 - seeking for title, 1 - reading text
		harray<hstr> content;
		hstr key;
		char *trp,utfc; // performance optimization
		unsigned int str_start;
		
		logMessage("loading texts from '"+folder+"'");
		
		if (mBuffer == 0) mBuffer=(char*) malloc(BLOCK_SIZE);
		
		getdir(folder,content);
		
		foreach(hstr,it,content)
		{
			//logMessage("Reading text file: "+*it);
			f=fopen((*it).c_str(),"rb");
			if(!f)
				throw hl_exception(hstr("Failed to load file ") + *it);
			for (utfc=-1;utfc < 0;utfc=fgetc(f));
			fseek(f, -1, SEEK_CUR);

			for (mode=c=0;fgets(buff[c],512,f);c=!c)
			{
				len=strlen(buff[c]); trp=buff[c]+len-2;
				if (len > 1 && *trp == '\r') { *trp='\n'; trp[1]=0; len--; }
				if (mode == 0 && strncmp(buff[c],"{",1) == 0)
				{
					buff[!c][strlen(buff[!c])-1]=0;
					key=buff[!c]; mode=1;
					str_start=mBufferPos;
				}
				else if (mode == 1)
				{
					if (strncmp(buff[c],"}",1) == 0)
					{
						mode=0;
						mBuffer[mBufferPos-1]=0;
						mTexts[key]=str_start;
					}
					else
					{
						int bp=(mBufferPos+len+1)/BLOCK_SIZE;
						if  (bp > mBufferPos/BLOCK_SIZE) mBuffer=(char*) realloc(mBuffer,(bp+1)*BLOCK_SIZE);
						strcpy(mBuffer+mBufferPos,buff[c]);
						mBufferPos+=len;
					}
				}
			}
			
			fclose(f);
		}
	}

	void TextMap::unload()
	{
		if (mBuffer)
		{
			logMessage("destroying TextMap [ "+mName+" ]");
			{ free(mBuffer); mBuffer=0; mBufferPos=1; }
			mTexts.clear();
		}
	}

	hstr TextMap::operator [] (chstr name)
	{
		if (name == "") return "";
		if (!exists(name))
		{
			return hsprintf("[Text entry '%s' doesn't exist]",name.c_str());
		}
		return mBuffer+mTexts[name];
	}
}
