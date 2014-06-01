#ifndef __PROTOBUF_DEFINE_H__
#define __PROTOBUF_DEFINE_H__

#if 1
#include <google/protobuf/message_lite.h>
using namespace google::protobuf;
typedef MessageLite		ProtoMessage;
#else
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
using namespace google::protobuf;
typedef Message			ProtoMessage;
#endif

#endif