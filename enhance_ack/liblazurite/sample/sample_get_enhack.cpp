#include <stdio.h>
/*!
  @file test_tx.cpp
  @brief about test_tx <br>
  sample code to send payload.

  @subsection how to use <br>

  test_tx ch panid txaddr rate pwr payload <br>
  parameters can be ommited. <br>
  follows are parameters in default. <br>
  ch = 36
  panid = 0xffff
  txaddr = 0xffff
  rate = 100
  pwr =20
  payload ="hello world"

  
  (ex)
  @code
  test_tx 36 0xabcd 0x1007 100 20 "hello world"
  @endcode

  then send 
  when push Ctrl+C, process is quited.
  */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "../lib/liblazurite.h"
#include <time.h>
#include <errno.h>


using namespace std;
using namespace lazurite;
bool bStop;
void sigHandle(int sigName)
{
	bStop = true;
	printf("sigHandle = %d\n",sigName);
	return;
}
int setSignal(int sigName)
{
	if(signal(sigName,sigHandle)==SIG_ERR) return -1;
	return 0;
}
int main(int argc, char **argv)
{
	int result;
	char* en;
	uint8_t ch=36;
	uint16_t panid=0xabcd;
	uint16_t txaddr=0x3FC0;
	uint8_t rate = 100;
	uint8_t pwr  = 20;
	char payload[250] = {"hello world\n"};
	uint8_t eack_data[16];
	uint16_t eack_size;

	// set Signal Trap
	setSignal(SIGINT);

	if((result=lazurite_init())!=0) {
		printf("lazurite_init fail = %d\n",result);
		return EXIT_FAILURE;
	}

	bStop = false;
	if(argc>1) {
		ch = strtol(argv[1],&en,0);
	}
	if(argc>2) {
		panid = strtol(argv[2],&en,0);
	}
	if(argc>3) {
		txaddr = strtol(argv[3],&en,0);
	}
	if(argc>4) {
		rate = strtol(argv[4],&en,0);
	}
	if(argc>5) {
		pwr = strtol(argv[5],&en,0);
	}
	if(argc>6) {
		strncpy(payload,argv[6],sizeof(payload));
	}

	result = lazurite_begin(ch,panid,rate,pwr);
	if(result < 0) 
	{
		lazurite_remove();
		printf("lazurite_begin fail = %d\n",result);
		return EXIT_FAILURE;
	}

	while (bStop == false)
	{
		if((result=lazurite_send(panid,txaddr,payload,strlen(payload))) <0 ) 
		{
			printf("tx error = %s\n",strerror(result*-1));
		} else {
			result = lazurite_getEnhanceAck((char *)eack_data,&eack_size);
			if (result < 0) {
				printf("lazurite_getEnhanceAck failed %d\n",result);
			} else {
				printf("getEnhanceAck data = ");
				for (int i=0;i<eack_size;i++) {
					printf("0x%02hhx ",*(eack_data+i));
				}
				printf("\n");
			}
		}
		usleep(1000*1000);
	}
	if((result = lazurite_close()) !=0) {
		printf("lazurite close failure %d",result);
	}
	if((result = lazurite_remove()) !=0) {
		printf("lazurite remove failure %d",result);
	}
	return 0;
}
