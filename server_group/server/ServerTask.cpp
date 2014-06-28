#include "ServerHeader.h"
#include "ServerTask.h"

using namespace SuperCmd;

ServerTaskAsync::ServerTaskAsync()
{
	mUniqueId = 0;
}

ServerTaskAsync::~ServerTaskAsync()
{

}

// 连接断开时被调用
void ServerTaskAsync::OnDisconnect()
{
	TCPTask::OnDisconnect();

	Server* server = ServerManager::getInstance().getServerByUniqueId(mUniqueId);
	if (server) {
		ZoneManager::getInstance().removeZoneServer(server);

		std::string strIP = ::getStrIP(server->getIP());
		ServerManager::getInstance().removeServer(server);
		string strSvrType = serverTypeToName(server->getType());
		LOGI("服务器注销[%s],ip=%s,port=%d", strSvrType.c_str(), strIP.c_str(), server->getPort());
		SAFE_DELETE(server);
	}
}

bool ServerTaskAsync::handleProtoMsg(const EventArgs& args)
{
	using namespace SuperCmd;

	stBaseCmd* pCmd = Event2BaseCmd(args);
	if (pCmd->cmd != SVR_MSG_SUPER_SERVICE) {

		// 回显测试
		//sendCmd(cmd, cmdLen);
		//return true;

		LOGE("非法的服务器消息号 cmd=%d", pCmd->cmd);
		return false;
	}

	// 消息性能记录
	char log[50];
	sprintf(log, "消息执行时间过长para=%d",pCmd->para);
	FUNC_PF_EXT(log,5);


	switch (pCmd->para)
	{
	case SUPER_OP_REGISTER_SERVER:
		{
			MsgRegisterServer rev;
			Event2Proto(args, rev);

			ServerInfo* info = rev.mutable_info();

			// 如果没填IP，由服务器从socket中取出IP
			if (info->ip() == 0)
				info->set_ip( getDigitalIP() );

			mUniqueId = server_hash_id(info->ip(), info->port());
			info->set_uniqueid(mUniqueId);

			// 加入服务器管理器
			Server* server = new Server;
			server->setServerInfo(*info);
			server->setTCPTask(this);
			server->setID(info->serverid());
			server->setLongID(mUniqueId);
			server->setState(SS_Normal);
			if (!ServerManager::getInstance().addServer(server)) {
				SAFE_DELETE(server);
				this->closeDelay();	// 关闭链接
				break;
			}

			// 如果是登陆服务器，就更新大区信息给她
			if (info->type() == LoginServer) {
				ZoneManager::getInstance().refreshZoneInfoToLogin();
			} else {
				// 加入大区信息管理器(必定是ZoneServer)
				ZoneManager::getInstance().addZoneServer(server);
			}

			sendProtoMsg(rev, pCmd->getCmdID());

			string strSvrType = serverTypeToName((ServerType)info->type());
			LOGI("注册服务器[%s],ip=%s,port=%d", strSvrType.c_str(), ::getStrIP(info->ip()).c_str(), info->port());
		}
		break;

	case SUPER_OP_GET_SERVER_INFO:
		{
			MsgGetServerInfo rev;
			Event2Proto(args, rev);

			vector<Server*> serverList;
			if (rev.flag() == 1) {
				Server* server = ServerManager::getInstance().findFreeServerByType((ServerType)rev.type());
				if (server != NULL) serverList.push_back(server);
			} else {
				ServerManager::getInstance().getListByType((ServerType)rev.type(), serverList);
			}

			if (serverList.size() == 0) {
				rev.set_result(1);
			} else {
				for (size_t i=0; i<serverList.size(); i++) {
					Server* server = serverList[i];
					ServerInfo* pInfo = rev.add_server_list();
					pInfo->set_ip( server->getIP() );
					pInfo->set_port( server->getPort() );
				}
			}

			sendProtoMsg(rev, pCmd->getCmdID());
		}
		break;

	case SUPER_OP_LOGIN_SELECT_ZONE:
		{
			// login -> super ->zone
			MsgLoginSelectZone rev;
			Event2Proto(args, rev);

			Server* zoneServer = ServerManager::getInstance().getServerByUniqueId(rev.uniqueid());
			if (zoneServer == NULL) {
				// Zone服务器异常断开或未开启
				rev.set_result(1);
				sendProtoMsg(rev, pCmd->getCmdID());
				break;
			}

			// 发往Zone服务器
			SuperCmd::MsgSuperSelectZone cmd;
			cmd.set_account(rev.account());
			cmd.set_login_key(rev.login_key());
			cmd.set_client_taskid(rev.client_taskid());
			cmd.set_login_server_uid(mUniqueId);
			zoneServer->sendProtoMsg(cmd, CMD_SUPER_SERVICE(SUPER_OP_SUPER_SELECT_ZONE));
		}
		break;

	case SUPER_OP_SUPER_SELECT_ZONE:
		{
			// zone -> super -> login
			SuperCmd::MsgSuperSelectZone rev;
			Event2Proto(args, rev);

			LOGI("[同步秘钥结果]Super收到Zone发来的网关信息 ip=%s, port=%d, result=%d", getStrIP(rev.gateway_ip()).c_str(), rev.gateway_port(), rev.result());

			SuperCmd::MsgLoginSelectZone cmdRet;
			cmdRet.set_result( rev.result() );
			cmdRet.set_account( rev.account() );
			cmdRet.set_login_key( rev.login_key() );
			cmdRet.set_client_taskid( rev.client_taskid() );
			cmdRet.set_gateway_ip( rev.gateway_ip() );
			cmdRet.set_gateway_port( rev.gateway_port() );

			Server* loginServer = ServerManager::getInstance().getServerByUniqueId(rev.login_server_uid());
			if (loginServer == NULL) {
				LOGE("Super收到Zone发来的网关信息 ip=%s, port=%d，但是当前Login已断开，接入失败", getStrIP(rev.gateway_ip()).c_str(), rev.gateway_port());
				break;
			}

			loginServer->sendProtoMsg(cmdRet, CMD_SUPER_SERVICE(SUPER_OP_LOGIN_SELECT_ZONE));
		}
		break;

	case SUPER_OP_RECORD_CHAR_TO_LOGINDB:
		{
			// zone -> super -> login
			SuperCmd::MsgRecordCharToLoginDB rev;
			Event2Proto(args, rev);
			ServerManager::getInstance().broadcastProtoMsgByType(LoginServer, rev, pCmd->getCmdID());
		}
		break;

	default: break;
	}


	return true;
}