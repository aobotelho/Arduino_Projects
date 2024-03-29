#include <HAConstants.h>

HAConst::HAConst(){}

bool HAConst::setVals(char* mac, char* nodeType){
	STATUS_CALLBACK.append(mac);
	DISCOVER_CALLBACK.append(mac);
	
	REGISTER_PAYLOAD = "\"id\":\"";
	REGISTER_PAYLOAD.append(mac);
	REGISTER_PAYLOAD += "\",\"type\":";
	REGISTER_PAYLOAD.append(nodeType);
	UPDATE_TOPIC = "/";
	UPDATE_TOPIC.append(mac);
	UPDATE_TOPIC += "/status";

	return true;
}

char* HAConst::formatTopic(char* topic){
	string tempStr = cloudMQTTTopic;
	tempStr.append(topic);
	return &tempStr[0u];
}
char* HAConst::formatTopic(char* topic,char* out){
	string tempStr = cloudMQTTTopic;
	tempStr.append(topic);
	tempStr += "/";
	tempStr.append(out);
	return &tempStr[0u];
}

char* HAConst::getStatusTopic(){return &STATUS_TOPIC[0u];}
char* HAConst::getStatusCallback(){return &STATUS_CALLBACK[0u];}
char* HAConst::getStatusCallback(char* out){
	string aux = STATUS_CALLBACK + "/";
	aux.append(out);
	return &aux[0u];
}
char* HAConst::getDiscoverTopic(){return &DISCOVER_TOPIC[0u];}
char* HAConst::getDiscoverCallback(){return &DISCOVER_CALLBACK[0u];}
char* HAConst::getUpdateTopic(){return &UPDATE_TOPIC[0u];}
char* HAConst::getResetTopic(){return &RESET_TOPIC[0u];}
char* HAConst::getRegisterPayload(){return &REGISTER_PAYLOAD[0u];}
char* HAConst::getMQTTIP(){return &MQTTIP[0u];}
int HAConst::getMQTTPort(){return MQTTPort;}
char* HAConst::getCloudMQTTIP(){return &cloudMQTTIP[0u];}
int HAConst::getCloudMQTTPort(){return cloudMQTTPort;}
char* HAConst::getCloudMQTTUser(){return &cloudMQTTUser[0u];}
char* HAConst::getCloudMQTTPass(){return &cloudMQTTPass[0u];}
char* HAConst::getCloudMQTTTopic(){return &cloudMQTTTopic[0u];}
