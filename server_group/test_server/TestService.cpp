#include "TestHeader.h"
#include "TestService.h"
#include "TestLogger.h"

TestService::TestService()
	: NetService("testserver")
{
	mLocalPort = 0;
	mMaxClient = 0;
}

TestService::~TestService()
{

}

bool TestService::onInitialise()
{ 
	TestLogger::getInstance().start("./log/testserver/testserver", "test", true);
	loadConfig();

	return true;
}

void TestService::onShutdown()		
{
	TestService::getInstance().stop();
}

bool TestService::loadConfig()
{
	//pugi::xml_document doc;
	//if (!doc.load_file("./config/config.xml")) {
	//	LOGE("¼ÓÔØÎÄ¼þÊ§°Ü");
	//	return false;
	//}

	//pugi::xml_node node = doc.child("Config").child("Global").child("SuperServer");
	//mLocalPort = atoi(node.attribute("port").value());
	//mMaxClient = atoi(node.attribute("maxclient").value());

	mLocalPort = 6666;
	mMaxClient = 4000;

	return true;
}

void TestService::updateWindowTitle()
{
	string strTitle = strformat("SuperServer %d",getLocalPort());
	strTitle = Platform::utf8ToGbk(strTitle);
	Platform::setWindowTitle(strTitle.c_str());
}