#include <hltypes/exception.h>
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "EventArgs.h"
#include "Object.h"
#include "QueuedCallbackEvent.h"

namespace aprilui
{
	QueuedCallbackEvent::QueuedCallbackEvent(aprilui::Event* e)
	{
		this->event = e;
		this->dataset = NULL;
	}

	QueuedCallbackEvent::~QueuedCallbackEvent()
	{
		if (this->dataset)
		{
			this->dataset->removeCallbackFromQueue(this->event);
		}
		delete this->event;
	}

	void QueuedCallbackEvent::execute(void* params)
	{
		aprilui::EventArgs* args = (aprilui::EventArgs*)params;
		this->dataset = dynamic_cast<Dataset*>(args->object->getDataset());
		if (this->dataset == NULL)
		{
			throw hl_exception("QueuedCallbackEvent: unable to process queued event, object's dataset is not a Chapter class.");
		}
		this->dataset->queueCallback(this->event, new aprilui::EventArgs(args->object, args->x, args->y, args->keyCode, args->extra));
	}

}
