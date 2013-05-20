#ifndef CAGE_QUEUED_CALLBACK_EVENT
#define CAGE_QUEUED_CALLBACK_EVENT

#include <hltypes/hstring.h>
#include "apriluiExport.h"
#include "Event.h"

namespace aprilui
{
	class Dataset;

	class apriluiExport QueuedCallbackEvent : public aprilui::Event
	{
		aprilui::Event* event;
		Dataset* dataset;
	public:
		QueuedCallbackEvent(aprilui::Event* e);
		~QueuedCallbackEvent();
		
		aprilui::Event* getEvent() { return event; }
		void execute(void* params);
	};
}

#endif
