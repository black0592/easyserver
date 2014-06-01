#include "TCPTask.h"
#include "platform/Platform.h"
#include "BaseMisc.h"
#include "NetService.h"
#include "NetDefense.h"
#include "logger/Logger.h"
#include "debugger/Profiler.h"
#include "BaseCmd.h"
#include "zlib.h"

#define unzip_size(zip_size)	((zip_size) * 120 / 100 + 12)

namespace easygame {

	// 密钥
	static byte EncryptKey[] = 
	{ 
		0x82, 0xf6, 0x79, 0x71, 0x3e, 0xa3, 0x21, 0xd8, 0xb7, 0x34, 0x9f, 0x39, 0xf3, 0x43, 0xb0, 0xec, 0xba, 0x35, 0xca, 0x48, 
		0xa1, 0x49, 0xcd, 0xdd, 0xb9, 0x6a, 0x03, 0x0a, 0xce, 0x92, 0x03, 0x58, 0xd0, 0x31, 0xb4, 0x05, 0xaf, 0x01, 0xbc, 0xa5, 
		0x2a, 0xa9, 0xf6, 0x6c, 0x14, 0x7f, 0xad, 0x1a, 0xce, 0xfa, 0xba, 0x76, 0x7c, 0x78, 0x05, 0x45, 0xad, 0x06, 0x31, 0xc6, 
		0x65, 0xd8, 0xaa, 0x5b, 0xf3, 0x69, 0xcf, 0x81, 0x94, 0xd4, 0x53, 0x31, 0xc2, 0x02, 0xcf, 0x2f, 0x7a, 0x7b, 0x72, 0x14, 
		0x6f, 0x92, 0xb5, 0xac, 0x26, 0xfa, 0x0d, 0xc9, 0xfa, 0x02, 0x4b, 0x08, 0xf9, 0x90, 0x4a, 0x38, 0x29, 0xe6, 0x5b, 0x31, 
		0x48, 0xee, 0x4a, 0xc2, 0x68, 0x7a, 0x78, 0x2e, 0x2e, 0x4b, 0x15, 0x1f, 0xda, 0xda, 0xdc, 0xbc, 0xa0, 0x3d, 0x73, 0x51, 
		0x1a, 0xbb, 0x61, 0x9a, 0xdd, 0xdf, 0xd1, 0x0d, 0xc3, 0x4e, 0x94, 0x91, 0xb1, 0x51, 0x71, 0xa2, 0x2d, 0xec, 0x2f, 0x6f, 
		0x7f, 0x54, 0x26, 0x4e, 0x87, 0x9b, 0xd4, 0x08, 0xd3, 0xd3, 0x51, 0x77, 0x89, 0x77, 0xce, 0xca, 0xbb, 0x5b, 0x73, 0xe7, 
		0x2a, 0x36, 0xda, 0xb9, 0x40, 0x35, 0x06, 0x12, 0xf4, 0xc6, 0x11, 0x87, 0x8e, 0x3d, 0x78, 0xb3, 0xcf, 0x03, 0x66, 0x4c, 
		0xe5, 0x97, 0xd4, 0x12, 0xdc, 0x22, 0x62, 0x7b, 0xf5, 0xb8, 0xb6, 0xfa, 0xcd, 0xca, 0x20, 0x4b, 0x30, 0xd2, 0x8a, 0x42, 
		0x17, 0x6d, 0xda, 0x98, 0xf1, 0xd4, 0x6c, 0x80, 0x5a, 0x4d, 0x0f, 0xdf, 0xd5, 0x7f, 0xd4, 0xb3, 0xd2, 0x61, 0x0a, 0xe0, 
		0x74, 0xf8, 0x72, 0x3c, 0x69, 0xf9, 0x6d, 0x36, 0x1d, 0x07, 0xd0, 0x5c, 0xad, 0xe7, 0x82, 0x1a, 0xea, 0x4a, 0x6a, 0x93, 
		0xc6, 0x2e, 0x09, 0x86, 0x36, 0xb3, 0x3f, 0x0a, 0x2a, 0x83, 0x5b, 0xb5, 0xb2, 0x36, 0x14, 0xbe, 0xa4, 0x82, 0x73, 0xe9, 
		0x64, 0x8d, 0x24, 0x79, 0x1c, 0xc9, 0x7b, 0xf4, 0x55, 0xbe, 0x77, 0x13, 0xc4, 0x50, 0x7b, 0x71, 0xf5, 0xd6, 0x98, 0xbb, 
		0xd9, 0xae, 0xf0, 0x3a, 0xd8, 0xcc, 0xf8, 0x7a, 0x3e, 0xc3, 0xc5, 0x80, 0xe3, 0x77, 0xeb, 0xc7, 0xd7, 0xac, 0x49, 0xa1, 
		0x84, 0xb8, 0xc7, 0xd0, 0x41, 0xbb, 0x95, 0x54, 0xe1, 0x6d, 0x69, 0x6c, 0xdb, 0x9d, 0x12, 0x26, 0xae, 0x70, 0x35, 0x58, 
		0x28, 0xc9, 0xed, 0x59, 0x23, 0x1d, 0x37, 0x47, 0x62, 0x9a, 0x33, 0x17, 0x9e, 0x0a, 0x10, 0xd9, 0x25, 0x90, 0x29, 0x5b, 
		0x35, 0x7e, 0xf2, 0x99, 0x3a, 0xcd, 0x09, 0x8d, 0x78, 0xed, 0x04, 0x4d, 0x3a, 0x67, 0x1d, 0x32, 0x26, 0xda, 0x18, 0x07, 
		0x64, 0x8c, 0x3a, 0x9c, 0x1f, 0x83, 0x24, 0x13, 0x54, 0x76, 0xc9, 0x0c, 0xea, 0x12, 0x79, 0xbe, 0x25, 0x9e, 0xbc, 0x15, 
		0x08, 0x3e, 0xfa, 0x50, 0xab, 0x32, 0x77, 0x9a, 0x9d, 0x92, 0x38, 0x09, 0x98, 0x0c, 0x76, 0x2d, 0x3e, 0x58, 0x4b, 0x5b, 
		0x19, 0xce, 0x21, 0x94, 0x5a, 0x74, 0xbe, 0x6f, 0xd6, 0xe6, 0x32, 0xbb, 0xee, 0xbb, 0x16, 0x29, 0x9c, 0x66, 0xe9, 0x4c, 
		0xdf, 0x35, 0x93, 0x56, 0xbe, 0x55, 0xa4, 0x08, 0xcb, 0xda, 0x43, 0x8f, 0x94, 0xb5, 0x9d, 0x88, 0x9a, 0x52, 0xf4, 0x28, 
		0xbc, 0x64, 0x29, 0x47, 0x53, 0xca, 0xbe, 0xac, 0x50, 0x0f, 0x75, 0x92, 0xa9, 0xcb, 0x5a, 0xd6, 0xbf, 0x96, 0x84, 0x01, 
		0x30, 0xba, 0x0b, 0x69, 0x0e, 0xf5, 0x69, 0xe8, 0x77, 0x57, 0x7c, 0x37, 0x30, 0x69, 0x10, 0x87, 0xcd, 0x40, 0x71, 0xaf, 
		0x12, 0xbc, 0xa1, 0x2e, 0x8e, 0xbd, 0x19, 0x68, 0xaf, 0x7f, 0x78, 0x3f, 0x5e, 0xb8, 0xc7, 0xc7, 0x0d, 0x40, 0x8f, 0x2c, 
	};

	static const uint ENCRYPT_KEY_LEN = count_of(EncryptKey);


	TCPTask::TCPTask()
		: mRecvBuff(MAX_USER_RECV_SIZE)
	{
		mEnableZip = false;
		mEnableEncrypt = false;
		mEncIndex = 0;
		mDecIndex = 0;
		delayCloseTime = 0;
		mId = 0;
		mType = AsyncType;
		//mState = verify;
		//mRecycleTick = 0;

		mLastRecvTime = Platform::tick();
		mDDSCheckCount = 0;
		mLastDestroyTime = 0;
	}

	TCPTask::~TCPTask(void)
	{
		
	}

	//////////////////////////////////////////////////////////////////////////

	bool TCPTask::isInBlacklist()
	{
#ifdef DDS_CHECK
		if (NetDefense::getInstance().isEnable()) {
			if ( NetDefense::getInstance().isInBlacklist(getIP()) ) {
				BLOGW("OnConnect: IP(%s)在黑名单中，断开链接\n", getIP());
				return true;
			}
		}
#endif

		return false;
	}

	void TCPTask::addConnectWaitList()
	{
		// 加入等待池，还未真正的加入逻辑队列
		delayCloseTime = 0;
		TCPTaskManager::getInstance().addWaitList(this);
	}

	bool TCPTask::OnConnect()
	{
		// 真正的进入逻辑处理
		//BLOGD("OnConnect: id=%d, ip=%s, port=%d, taskcount=%d\n", getID(), getIP(), port, TCPTaskManager::getInstance().size());

		// 通知调度系统接收数据
		// 数据这时并没有真正接收，当客户端有数据发送来时
		// 调度器自动接收数据，然后通过OnRecv通知数据接收完成
		recv();
		return true;
	}

	void TCPTask::OnDisconnect()
	{
//		mRecvBuff.releaseBuffer();
		//BLOGD("OnDisconnect: id=%d, ip=%s, port=%d, taskcount=%d\n", getID(), getIP(), port, TCPTaskManager::getInstance().size());
	}

	int TCPTask::OnRecv(const void* data, int len)
	{
		int64 curTick = Platform::tick();

#ifdef DDS_CHECK
		if (NetDefense::getInstance().isEnable()) {
			if (curTick - mLastRecvTime < DDS_CHECK_TIME) {
				mDDSCheckCount++;
				if (mDDSCheckCount > DDS_CHECK_COUNT) {
					BLOGW("DDS攻击检测成功，断开链接 ip=%s\n", getIP());
					// 断开链接
					NetDefense::getInstance().addBlacklist(getIP());
					closeDelay();
					return RECV_ERROR;
				}
			} else {
				mDDSCheckCount = 0;
			}
		}
#endif

		mLastRecvTime = curTick;

		if ( !mRecvBuff.write(data, len) ) {
			// 写入失败，可能超过安全容量，断开和客户端的链接
			BLOGW("接收超过安全容量，可能被攻击了 ip=%s\n", getIP());
			closeDelay();
			return RECV_ERROR;
		}

		parsePacket();

		return 0;
	}

	void TCPTask::parsePacket()
	{
		while(true) {
			ByteBuffer* revBuff = &mRecvBuff;

			// 如果当前缓存小于2个字节，说明包数据还不够
			if (revBuff->size() < HEADER_SIZE)
				break;

			// 取包长度信息
			ushort cmdLen = 0;
			revBuff->get(&cmdLen, sizeof(cmdLen));
			if (cmdLen < HEADER_SIZE) {
				printf("[系统级错误]TCPTask无效的消息格式\n");
				revBuff->clearBuffer();
				break;
			}

			if (cmdLen > MAX_USER_CMD_SIZE) {
				printf("[系统级错误]用户封包超过最大限制，可能遭到攻击了\n");
				revBuff->clearBuffer();
				break;
			}

			// 如果包数据不够，继续接收数据
			if ((ushort)revBuff->size() < (cmdLen + HEADER_SIZE)) {
				break;
			}

			stBaseCmd* pCmd = (stBaseCmd*)(revBuff->getBuffer() + HEADER_SIZE);
			if (pCmd->flag & FLAG_ENCRYPT) {
				// 解密消息
				byte* pData = (byte*)pCmd + CMD_FLAG_SIZE;
				uint dataLen = cmdLen - CMD_FLAG_SIZE;
				Decrypt(pData, dataLen);
			}

			// 把解析出来的消息，放入消息队列
			OnRecvCommand(revBuff->getBuffer() + HEADER_SIZE, cmdLen);

			// 删除处理过的数据
			revBuff->remove(cmdLen + HEADER_SIZE);
		}
	}

	// 加密
	void TCPTask::Encrypt(byte* data, uint dataLen)
	{
		uint lastIndex = mEncIndex;
		for(uint i=0; i<dataLen; i++) {
			if(lastIndex >= ENCRYPT_KEY_LEN) {
				lastIndex = 0;
			}

			data[i] ^= EncryptKey[lastIndex];
			lastIndex++;
		}

		nextEncryptKey(mEncIndex);
	}

	// 解密
	void TCPTask::Decrypt(byte* data, uint dataLen)
	{
		uint lastIndex = mDecIndex;
		for(uint i=0; i<dataLen; i++) {
			if(lastIndex >= ENCRYPT_KEY_LEN) {
				lastIndex = 0;
			}

			data[i] ^= EncryptKey[lastIndex];
			lastIndex++;
		}

		nextEncryptKey(mDecIndex);
	}

	void TCPTask::nextEncryptKey(uint& index)
	{
		index++;
		if (index >= ENCRYPT_KEY_LEN)
			index = 0;
	}

	//////////////////////////////////////////////////////////////////////////

	int TCPTask::getDigitalIP()
	{
		return ip;
	}

	const char* TCPTask::getIP()
	{
		if (mIP.empty())
			mIP = ::getStrIP(ip);
		return mIP.c_str();
	}

	int TCPTask::getID()
	{
		return mId;
	}

	void TCPTask::setID(int id)
	{
		mId = id;
	}

	// 先加密，后压缩
	bool TCPTask::sendCmd(const void* cmd, ushort cmdLen)
	{
		if (delayCloseTime > 0) {
			return false;
		}

		FUNC_PF_EXT("TCPTask::sendCmd", 100);

		if (cmdLen > MAX_USER_CMD_SIZE) {
			BLOGE("发送的消息太长了，发送失败!!!");
			return false;
		}

		if (cmd == NULL || cmdLen == 0)
			return false;

		int error = 0;

		stBaseCmd* pFinalCmd = (stBaseCmd*)cmd;
		const char* pFinalBuff = (const char*)cmd;
		uint finalSize = cmdLen;

		if (mEnableZip || mEnableEncrypt) {
			// 处理压缩后的数据，会改变数据大小
			byte tempBuffer[MAX_USER_CMD_SIZE*2];
			if (mEnableZip && cmdLen > 50) {
				stBaseCmd* pCmd = (stBaseCmd*)cmd;
				stBaseCmd* pZipCmd = (stBaseCmd*)tempBuffer;
				pZipCmd->flag = pCmd->flag;
				// 需要跳过flag的长度
				ulong destLen = sizeof(tempBuffer) - 1;
				int ret = compress((byte*)tempBuffer+CMD_FLAG_SIZE, &destLen, (const byte*)cmd+CMD_FLAG_SIZE, cmdLen-CMD_FLAG_SIZE);
				if(ret != Z_OK) {
					BLOGE("压缩数据出错了error=%d，发送失败!!!", ret);
					return false;
				}

#if 0
				if (destLen < cmdLen) {
					BLOGI("压缩效果还行 %d->%d(zip)", cmdLen, destLen);
				} else {
					BLOGW("压缩效果太差了 %d->%d(zip)", cmdLen, destLen);
				}
#endif


				if (destLen < cmdLen) {
					// 压缩有效果，就发送压缩后的数据
					pFinalBuff = (const char*)tempBuffer;
					finalSize = destLen + CMD_FLAG_SIZE;
					pFinalCmd = (stBaseCmd*)pFinalBuff;
					pFinalCmd->flag |= FLAG_ZIP;
				}
			} 

			if (mEnableEncrypt) {
				// 加密消息，不改变数据大小
				if ( !(pFinalCmd->flag & FLAG_ZIP) ) {
					// 如果没有压缩过，就需要复制消息到临时缓存，准备加密
					memcpy(tempBuffer, cmd, cmdLen);
				}

				// 加密数据，就必须使用临时缓存
				pFinalBuff = (const char*)tempBuffer;
				pFinalCmd = (stBaseCmd*)pFinalBuff;
				pFinalCmd->flag |= FLAG_ENCRYPT;

				// 加密数据
				byte* pData = (byte*)pFinalBuff + CMD_FLAG_SIZE;
				uint dataLen = finalSize - CMD_FLAG_SIZE;
				Encrypt(pData, dataLen);
			}
		}

		// 不压缩直接发送
		error = this->send((const char*)pFinalBuff, finalSize, true);
		if (error != SEND_ERROR && error != finalSize + HEADER_SIZE) {
			BLOGE("【错误】TCPTask::sendCmd数据未发送全 %d\n", error);
		}

		return true;
	}

	bool TCPTask::sendRawData(const void* data, ushort dataLen)
	{
		if (delayCloseTime > 0) {
			return false;
		}

		FUNC_PF_EXT("TCPTask::sendRawData", 100);

		if (dataLen > MAX_USER_CMD_SIZE) {
			BLOGE("发送的消息太长了，发送失败!!!");
			return false;
		}

		if (data == NULL || dataLen == 0)
			return false;

		int error = this->send((const char*)data, dataLen, false);
		if (error != SEND_ERROR && error != dataLen) {
			BLOGE("【错误】TCPTask::sendRawData数据未发送全 %d\n", error);
		}

		return true;
	}

	void TCPTask::onUpdate()
	{
		
	}

}	// namespace