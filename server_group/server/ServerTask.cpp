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

// ���ӶϿ�ʱ������
void ServerTaskAsync::OnDisconnect()
{
	TCPTask::OnDisconnect();

	Server* server = ServerManager::getInstance().getServerByUniqueId(mUniqueId);
	if (server) {
		ZoneManager::getInstance().removeZoneServer(server);

		std::string strIP = ::getStrIP(server->getIP());
		ServerManager::getInstance().removeServer(server);
		string strSvrType = serverTypeToName(server->getType());
		LOGI("������ע��[%s],ip=%s,port=%d", strSvrType.c_str(), strIP.c_str(), server->getPort());
		SAFE_DELETE(server);
	}
}

bool ServerTaskAsync::handleProtoMsg(const EventArgs& args)
{
	using namespace SuperCmd;

	stBaseCmd* pCmd = Event2BaseCmd(args);
	if (pCmd->cmd != SVR_MSG_SUPER_SERVICE) {

		// ���Բ���
		//sendCmd(cmd, cmdLen);
		//return true;

		LOGE("�Ƿ��ķ�������Ϣ�� cmd=%d", pCmd->cmd);
		return false;
	}

	// ��Ϣ���ܼ�¼
	char log[50];
	sprintf(log, "��Ϣִ��ʱ�����para=%d",pCmd->para);
	FUNC_PF_EXT(log,5);


	switch (pCmd->para)
	{
	case SUPER_OP_REGISTER_SERVER:
		{
			MsgRegisterServer rev;
			Event2Proto(args, rev);

			ServerInfo* info = rev.mutable_info();

			// ���û��IP���ɷ�������socket��ȡ��IP
			if (info->ip() == 0)
				info->set_ip( getDigitalIP() );

			mUniqueId = server_hash_id(info->ip(), info->port());
			info->set_uniqueid(mUniqueId);

			// ���������������
			Server* server = new Server;
			server->setServerInfo(*info);
			server->setTCPTask(this);
			server->setID(info->serverid());
			server->setLongID(mUniqueId);
			server->setState(SS_Normal);
			if (!ServerManager::getInstance().addServer(server)) {
				SAFE_DELETE(server);
				this->closeDelay();	// �ر�����
				break;
			}

			// ����ǵ�½���������͸��´�����Ϣ����
			if (info->type() == LoginServer) {
				ZoneManager::getInstance().refreshZoneInfoToLogin();
			} else {
				// ���������Ϣ������(�ض���ZoneServer)
				ZoneManager::getInstance().addZoneServer(server);
			}

			sendProtoMsg(rev, pCmd->getCmdID());

			string strSvrType = serverTypeToName((ServerType)info->type());
			LOGI("ע�������[%s],ip=%s,port=%d", strSvrType.c_str(), ::getStrIP(info->ip()).c_str(), info->port());
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
				// Zone�������쳣�Ͽ���δ����
				rev.set_result(1);
				sendProtoMsg(rev, pCmd->getCmdID());
				break;
			}

			// ����Zone������
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

			LOGI("[ͬ����Կ���]Super�յ�Zone������������Ϣ ip=%s, port=%d, result=%d", getStrIP(rev.gateway_ip()).c_str(), rev.gateway_port(), rev.result());

			SuperCmd::MsgLoginSelectZone cmdRet;
			cmdRet.set_result( rev.result() );
			cmdRet.set_account( rev.account() );
			cmdRet.set_login_key( rev.login_key() );
			cmdRet.set_client_taskid( rev.client_taskid() );
			cmdRet.set_gateway_ip( rev.gateway_ip() );
			cmdRet.set_gateway_port( rev.gateway_port() );

			Server* loginServer = ServerManager::getInstance().getServerByUniqueId(rev.login_server_uid());
			if (loginServer == NULL) {
				LOGE("Super�յ�Zone������������Ϣ ip=%s, port=%d�����ǵ�ǰLogin�ѶϿ�������ʧ��", getStrIP(rev.gateway_ip()).c_str(), rev.gateway_port());
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