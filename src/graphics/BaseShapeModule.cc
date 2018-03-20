#include"graphics/BaseShapeModule.h"

void BaseShapeModule::HandleVAOAck(std::shared_ptr<VertexBufferEvent> event_item)
{
	{
		std::unique_lock<std::mutex> locker;
		//mVAO = event_item->Get<unsigned int>(EventName::VBUFF_VAO_NAME);
		mVAOCond.notify_all();
	}
	

}

BaseShapeModule::BaseShapeModule(std::shared_ptr<VertexBufferEventModule> vbuff_ptr) :
	BaseModule<VertexBufferEvent>(),
	mVAO(0),
	mVBuffModule(vbuff_ptr)
{
	RegisterEventHandler(EventName::VBUFF_ACK_VAO_REQUEST, &BaseShapeModule::HandleVAOAck);
}

BaseShapeModule::~BaseShapeModule()
{

}

GLuint BaseShapeModule::GetVAO()
{
	std::unique_lock<std::mutex> locker(mVAOMtx);
	{
		if (!mVAO)
		{
			mVBuffModule->AddEvent(MakeVBuffEventPtr(EventName::VBUFF_REQUEST_VAO, mId, &mVAO));
			while (!mVAO)
			{
				mVAOCond.wait(locker);
			}
		}
	}
	return mVAO;
}

