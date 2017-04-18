#ifndef HAConstants_h
#define	HAConstants_h

#include <string>
using namespace std;
class HAConst{
public:
	bool setVals(char* mac, char* nodeType);

	HAConst();

	const int light1 = 0;
	const int light2 = 1;
	const int light3 = 2;
	const int light4 = 3;
	const int singleSwitch = 4;
	const int doubleSwitch = 5;

	const int nodeON = 0;
	const int nodeOFF = 1;

	const int nodeIN = 0;
	const int nodeOUT = 1;
	const int nodeNA = 2;
	
	char* formatTopic(char* topic);
	char* formatTopic(char* topic,char* out);

	char* getStatusTopic();
	char* getStatusCallback();
	char* getStatusCallback(char* out);
	char* getDiscoverTopic();
	char* getDiscoverCallback();
	char* getUpdateTopic();	
	char* getResetTopic();
	char* getRegisterPayload();
	char* getMQTTIP();
	int getMQTTPort();
	char* getCloudMQTTIP();
	int getCloudMQTTPort();
	char* getCloudMQTTUser();
	char* getCloudMQTTPass();
	char* getCloudMQTTTopic();
private:
	string STATUS_TOPIC = "/update/status";
	string STATUS_CALLBACK = "/update/callback/";
	string DISCOVER_TOPIC = "/discover/newDevices";
	string DISCOVER_CALLBACK = "/discover/registerDevice/";
	string UPDATE_TOPIC;
	string RESET_TOPIC = "/update/reset/";
	string REGISTER_PAYLOAD;
	
	string MQTTIP = "192.168.1.33";
	const int MQTTPort = 1883;

	string cloudMQTTIP = "mqtt.dioty.co";
	const int cloudMQTTPort = 1883;
	string cloudMQTTUser = "andrefrejat@gmail.com";
	string cloudMQTTPass = "45f66b00";
	string cloudMQTTTopic = "/andrefrejat@gmail.com";
};

#endif
