#ifndef BASE_SHAPE_MODULE_HEADER_INCLUDED
#define BASE_SHAPE_MODULE_HEADER_INCLUDED

#include"ebs/BaseModule.h"
#include"graphics/VertexBufferEventModule.h"

class BaseShapeModule :
	public BaseModule<VertexBufferEvent>,
	public std::enable_shared_from_this<BaseShapeModule>
{
protected:
	GLuint mVAO;
	std::shared_ptr<VertexBufferEventModule> mVBuffModule;
	std::mutex mVAOMtx;
	std::condition_variable mVAOCond;
	void HandleVAOAck(std::shared_ptr<VertexBufferEvent>);
public:
	BaseShapeModule(std::shared_ptr<VertexBufferEventModule> = nullptr);
	virtual ~BaseShapeModule();
	GLuint GetVAO();
};

#endif