#ifndef __PROTOBUF_DEFINE_H__
#define __PROTOBUF_DEFINE_H__
#include "BaseConfig.h"

#if ENABLE_PROTO_REFLECT
// 支持自动反射
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
using namespace google::protobuf;
typedef Message			ProtoMessage;
#else
// lite版，不支持自动反射（适合移动设备）
#include <google/protobuf/message_lite.h>
using namespace google::protobuf;
typedef MessageLite		ProtoMessage;
#endif

// protobuf反射机制
inline ProtoMessage* createMessage(const std::string& type_name)
{
	ProtoMessage* message = NULL;
	const Descriptor* descriptor = DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
	if (descriptor) {
		const ProtoMessage* prototype = MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype) {
			message = prototype->New();
		}
	}
	return message;
}

#endif