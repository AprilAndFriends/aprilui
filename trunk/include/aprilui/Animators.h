#ifndef ANIMATORS_H
#define ANIMATORS_H

#include "Objects.h"

/*******************************************************************************/
namespace AprilUI
{
	namespace Animators
	{ // dummy objects who perform operations on their parents
		class Mover : public Object
		{
			float mInitialX,mInitialY,mInitialSX,mInitialSY;
			float mAccelX,mAccelY;
			float mSpeedX,mSpeedY;
		public:
			Mover(std::string name);

			void notifyEvent(std::string event_name,void* params);
			void setProperty(std::string name,std::string value);
			void update(float k);
		};
		class Scaler : public Object
		{
			float mInitialW,mInitialH,mInitialSW,mInitialSH;
			float mAccelW,mAccelH;
			float mSpeedW,mSpeedH;
		public:
			Scaler(std::string name);

			void notifyEvent(std::string event_name,void* params);
			void setProperty(std::string name,std::string value);
			void update(float k);
		};


		class Rotator : public Object
		{
			float mInitialSpeed,mInitialAngle,mSpeed,mAccel;
		public:
			Rotator(std::string name);

			void notifyEvent(std::string event_name,void* params);
			void setProperty(std::string name,std::string value);
			void update(float k);
		};

		class AlphaFader : public Object
		{
			float mInitialSpeed,mInitialAlpha,mSpeed,mAccel;
		public:
			AlphaFader(std::string name);

			void notifyEvent(std::string event_name,void* params);
			void setProperty(std::string name,std::string value);
			void update(float k);
		};


		// note: only works on ColoredImages
		class ColorAlternator : public Object
		{
			float mLow[4],mHigh[4];
			float mTimer,mSpeed;
		public:
			ColorAlternator(std::string name);
			void setProperty(std::string name,std::string value);
			void update(float k);
		};

	}
}

#endif
