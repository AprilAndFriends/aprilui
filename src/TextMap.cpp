/************************************************************************************
This source file is part of the APRIL User Interface Library
For latest info, see http://libatres.sourceforge.net/
*************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License (LGPL) as published by the
Free Software Foundation; either version 2 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*************************************************************************************/
#include "TextMap.h"
#include "Exception.h"
#include "Util.h"
#include <list>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

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
		destroy();
	}

	bool TextMap::exists(std::string name)
	{
		unsigned int s=mTexts[name];
		return (s != 0);
	}

	void TextMap::load(std::string folder)
	{
		FILE* f;
		char buff[2][513];
		int len,mode,c; // mode: 0 - seeking for title, 1 - reading text
		std::vector<std::string> content;
		std::string key;
		char *trp,utfc; // performance optimisation
		unsigned int str_start;
		
		writelog("loading texts from '"+folder+"'");
		
		if (mBuffer == 0) mBuffer=(char*) malloc(BLOCK_SIZE);
		
		getdir(folder,content);
		
		foreach_v(std::string,content)
		{
			f=fopen(it->c_str(),"rb");
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
						mBuffer[mBufferPos-1]=0; // get rid of last newline
						//printf("%s:-----\n%s\n",key.c_str(),str);
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

	void TextMap::destroy()
	{
		writelog("destroying texts");
		if (mBuffer) { free(mBuffer); mBuffer=0; mBufferPos=1; }
		mTexts.clear();
	}

	char* TextMap::operator [] (std::string name)
	{
		if (name == "") return "";
		unsigned int s=mTexts[name];
		if (!s)
		{
	#ifdef _DEBUG
			throw KeyException("Unable to find text entry '"+name+"'");
	#else
			sprintf(key_error_text,"<Text entry '%s' doesn't exist>",name.c_str());
			return key_error_text;
	#endif
		}
		return mBuffer+s;
	}
}
