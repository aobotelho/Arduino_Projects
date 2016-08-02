#define BUFFER_SIZE  32
#define PIPE         0
#define AUTO_ACK     false
#define CHANNEL      120

#define  CE_PIN   9
#define  CSN_PIN  10

uint8_t  buf[BUFFER_SIZE];

static const uint64_t addr[6] = {
                              0xDEADBEEF00LL,
                              0xABAB555500LL,
                              0xABAB555533LL,
                              0xABAB555566LL,
                              0xABAB555599LL,
                              0xABAB5555CCLL};
                              
void psat_InitAntenna(RF24 rfVar, rf24_datarate_e  Speed, rf24_pa_dbm_e Power, uint8_t Channel){
  rfVar.begin();
  rfVar.setCRCLength(RF24_CRC_16);
  rfVar.setDataRate(Speed);
  rfVar.setPALevel(Power);
  rfVar.setChannel(Channel); 
}

void psat_SetPipe(RF24 rfVar, uint8_t pipe){
        rfVar.setPayloadSize(BUFFER_SIZE);
        rfVar.setAutoAck(pipe,AUTO_ACK);
}
