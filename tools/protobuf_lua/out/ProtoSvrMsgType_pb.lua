-- Generated By protoc-gen-lua Do not Edit
local protobuf = require "protobuf"
module('ProtoSvrMsgType_pb')


local SVRMESSAGETYPE = protobuf.EnumDescriptor();
local SVRMESSAGETYPE_SVR_MSG_SUPER_SERVICE_ENUM = protobuf.EnumValueDescriptor();
local SVRMESSAGETYPE_SVR_MSG_ZONE_SERVICE_ENUM = protobuf.EnumValueDescriptor();
local SVRMESSAGETYPE_SVR_MSG_LOGIN_SERVICE_ENUM = protobuf.EnumValueDescriptor();
local SVRMESSAGETYPE_SVR_MSG_GATEWAY_SERVICE_ENUM = protobuf.EnumValueDescriptor();
local SVRMESSAGETYPE_SVR_MSG_GAME_SERVICE_ENUM = protobuf.EnumValueDescriptor();
local SVRMESSAGETYPE_SVR_MSG_SESSION_SERVICE_ENUM = protobuf.EnumValueDescriptor();
local SVRMESSAGETYPE_SVR_MSG_LOGINDB_SERVICE_ENUM = protobuf.EnumValueDescriptor();
local SVRMESSAGETYPE_SVR_MSG_GAMEDB_SERVICE_ENUM = protobuf.EnumValueDescriptor();

SVRMESSAGETYPE_SVR_MSG_SUPER_SERVICE_ENUM.name = "SVR_MSG_SUPER_SERVICE"
SVRMESSAGETYPE_SVR_MSG_SUPER_SERVICE_ENUM.index = 0
SVRMESSAGETYPE_SVR_MSG_SUPER_SERVICE_ENUM.number = 1
SVRMESSAGETYPE_SVR_MSG_ZONE_SERVICE_ENUM.name = "SVR_MSG_ZONE_SERVICE"
SVRMESSAGETYPE_SVR_MSG_ZONE_SERVICE_ENUM.index = 1
SVRMESSAGETYPE_SVR_MSG_ZONE_SERVICE_ENUM.number = 2
SVRMESSAGETYPE_SVR_MSG_LOGIN_SERVICE_ENUM.name = "SVR_MSG_LOGIN_SERVICE"
SVRMESSAGETYPE_SVR_MSG_LOGIN_SERVICE_ENUM.index = 2
SVRMESSAGETYPE_SVR_MSG_LOGIN_SERVICE_ENUM.number = 3
SVRMESSAGETYPE_SVR_MSG_GATEWAY_SERVICE_ENUM.name = "SVR_MSG_GATEWAY_SERVICE"
SVRMESSAGETYPE_SVR_MSG_GATEWAY_SERVICE_ENUM.index = 3
SVRMESSAGETYPE_SVR_MSG_GATEWAY_SERVICE_ENUM.number = 4
SVRMESSAGETYPE_SVR_MSG_GAME_SERVICE_ENUM.name = "SVR_MSG_GAME_SERVICE"
SVRMESSAGETYPE_SVR_MSG_GAME_SERVICE_ENUM.index = 4
SVRMESSAGETYPE_SVR_MSG_GAME_SERVICE_ENUM.number = 5
SVRMESSAGETYPE_SVR_MSG_SESSION_SERVICE_ENUM.name = "SVR_MSG_SESSION_SERVICE"
SVRMESSAGETYPE_SVR_MSG_SESSION_SERVICE_ENUM.index = 5
SVRMESSAGETYPE_SVR_MSG_SESSION_SERVICE_ENUM.number = 6
SVRMESSAGETYPE_SVR_MSG_LOGINDB_SERVICE_ENUM.name = "SVR_MSG_LOGINDB_SERVICE"
SVRMESSAGETYPE_SVR_MSG_LOGINDB_SERVICE_ENUM.index = 6
SVRMESSAGETYPE_SVR_MSG_LOGINDB_SERVICE_ENUM.number = 7
SVRMESSAGETYPE_SVR_MSG_GAMEDB_SERVICE_ENUM.name = "SVR_MSG_GAMEDB_SERVICE"
SVRMESSAGETYPE_SVR_MSG_GAMEDB_SERVICE_ENUM.index = 7
SVRMESSAGETYPE_SVR_MSG_GAMEDB_SERVICE_ENUM.number = 8
SVRMESSAGETYPE.name = "SvrMessageType"
SVRMESSAGETYPE.full_name = ".ServerProtocol.SvrMessageType"
SVRMESSAGETYPE.values = {SVRMESSAGETYPE_SVR_MSG_SUPER_SERVICE_ENUM,SVRMESSAGETYPE_SVR_MSG_ZONE_SERVICE_ENUM,SVRMESSAGETYPE_SVR_MSG_LOGIN_SERVICE_ENUM,SVRMESSAGETYPE_SVR_MSG_GATEWAY_SERVICE_ENUM,SVRMESSAGETYPE_SVR_MSG_GAME_SERVICE_ENUM,SVRMESSAGETYPE_SVR_MSG_SESSION_SERVICE_ENUM,SVRMESSAGETYPE_SVR_MSG_LOGINDB_SERVICE_ENUM,SVRMESSAGETYPE_SVR_MSG_GAMEDB_SERVICE_ENUM}

SVR_MSG_GAMEDB_SERVICE = 8
SVR_MSG_GAME_SERVICE = 5
SVR_MSG_GATEWAY_SERVICE = 4
SVR_MSG_LOGINDB_SERVICE = 7
SVR_MSG_LOGIN_SERVICE = 3
SVR_MSG_SESSION_SERVICE = 6
SVR_MSG_SUPER_SERVICE = 1
SVR_MSG_ZONE_SERVICE = 2

