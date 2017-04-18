#include <HomeAutomation.h>

HomeAutomation::HomeAutomation(nodeType newNodeType,bool useSerial){
	_type = newNodeType;
	_useSerial = useSerial;

	client = PubSubClient(wificlient);

	//Connect to Wifi
	if(!_useSerial){wifimanager.setDebugOutput(false);}

	wifimanager.autoConnect("Home Automation Setup");
}

HomeAutomation::nodeType HomeAutomation::getNodeType(){ return _type;}

void HomeAutomation::printSerial(char* msg){(_useSerial ?Serial.println(msg) :NULL);}
