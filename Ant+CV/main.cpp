#include <Windows.h>
#include <iostream>

typedef BOOL(*P_ANT_INIT)(UCHAR, USHORT);
typedef BOOL(*P_ANT_SETKEY)(UCHAR, UCHAR*);
typedef BOOL(*P_ANT_ASSIGN)(UCHAR, UCHAR, UCHAR);
typedef BOOL(*P_ANT_SETID)(UCHAR, USHORT, UCHAR, UCHAR);
typedef BOOL(*P_ANT_SETFREQ)(UCHAR, UCHAR);
typedef BOOL(*P_ANT_SETPER)(UCHAR, USHORT);
typedef BOOL(*P_ANT_OPENCHNL)(UCHAR);
typedef BOOL(*P_ANT_TX)(UCHAR, UCHAR*);
typedef BOOL(*P_ANT_CLOSE)();

P_ANT_INIT				ANT_Init = NULL;
P_ANT_SETKEY			ANT_SetNetworkKey = NULL;
P_ANT_ASSIGN			ANT_AssignChannel = NULL;
P_ANT_SETID				ANT_SetChannelId = NULL;
P_ANT_SETFREQ			ANT_SetChannelRFFreq = NULL;
P_ANT_SETPER			ANT_SetChannelPeriod = NULL;
P_ANT_OPENCHNL			ANT_OpenChannel = NULL;
P_ANT_TX				ANT_SendBroadcastData = NULL;
P_ANT_CLOSE				ANT_Close = NULL;

#define ANTPLUS_USB_DEVICE				0
#define ANTPLUS_BAUDRATE				57600
#define ANTPLUS_NETWORK_NUMBER			0
#define ANTPLUS_NETWORK_KEY				{ 0xB9,0xA5,0x21,0xFB,0xBD,0x72,0xC3,0x45 }
#define ANTPLUS_Channel_Number			0
#define ANTPLUS_Channe_Type				0x10
#define ANTPLUS_Device_Number			100
#define ANTPLUS_RF_FREQ					((UCHAR) 0x39)
#define ANTPLUS_Channel_Period			8182
#define ANTPLUS_Transmission_Type		5
#define ANTPLUS_Device_Type				11
int main()
{
	HMODULE hDLL = LoadLibrary("ANT_DLL.dll");
	if (!hDLL)
		return -1;

	ANT_Init = (P_ANT_INIT)GetProcAddress(hDLL, "ANT_Init");
	ANT_SetNetworkKey = (P_ANT_SETKEY)GetProcAddress(hDLL, "ANT_SetNetworkKey");
	ANT_AssignChannel = (P_ANT_ASSIGN)GetProcAddress(hDLL, "ANT_AssignChannel");
	ANT_SetChannelId = (P_ANT_SETID)GetProcAddress(hDLL, "ANT_SetChannelId");
	ANT_SetChannelRFFreq = (P_ANT_SETFREQ)GetProcAddress(hDLL, "ANT_SetChannelRFFreq");
	ANT_SetChannelPeriod = (P_ANT_SETPER)GetProcAddress(hDLL, "ANT_SetChannelPeriod");
	ANT_OpenChannel = (P_ANT_OPENCHNL)GetProcAddress(hDLL, "ANT_OpenChannel");
	ANT_SendBroadcastData = (P_ANT_TX)GetProcAddress(hDLL, "ANT_SendBroadcastData");
	ANT_Close = (P_ANT_CLOSE)GetProcAddress(hDLL, "ANT_Close");
	std::cout << (ANT_Init != NULL) << (ANT_SetNetworkKey != NULL) << (ANT_AssignChannel != NULL) << (ANT_SetChannelId != NULL) << (ANT_SetChannelRFFreq != NULL) << (ANT_SetChannelPeriod != NULL) << (ANT_OpenChannel != NULL) << (ANT_SendBroadcastData != NULL) << (ANT_Close != NULL) << std::endl;
	
	
	std::cout << ANT_Init(ANTPLUS_USB_DEVICE, ANTPLUS_BAUDRATE);
	UCHAR aucANTSportNetworkKey[] = ANTPLUS_NETWORK_KEY;
	std::cout << ANT_SetNetworkKey(ANTPLUS_NETWORK_NUMBER, aucANTSportNetworkKey);
	std::cout << ANT_AssignChannel(ANTPLUS_Channel_Number, ANTPLUS_Channe_Type, ANTPLUS_NETWORK_NUMBER);
	std::cout << ANT_SetChannelId(ANTPLUS_Channel_Number, ANTPLUS_Device_Number, ANTPLUS_Device_Type, ANTPLUS_Transmission_Type);
	std::cout << ANT_SetChannelRFFreq(ANTPLUS_Channel_Number, ANTPLUS_RF_FREQ);
	std::cout << ANT_SetChannelPeriod(ANTPLUS_Channel_Number, ANTPLUS_Channel_Period);
	std::cout << ANT_OpenChannel(ANTPLUS_Channel_Number) << std::endl;

	UCHAR pucTxBuffer[8] = { 0x10, 0, 0xff, 0xff, 0, 0, 0, 0 };
	UCHAR powerEventCount = 0;
	USHORT usPower = 100, usAcumPower = 0;
	for (unsigned int xo = 0; xo < 10; xo++)
	{
		Sleep(250);
		usAcumPower += usPower;
		pucTxBuffer[1] = ++powerEventCount;		// Power event count
		pucTxBuffer[4] = usAcumPower & 0xFF;	// Cumulative power (W)
		pucTxBuffer[5] = (usAcumPower >> 8) & 0xFF;
		pucTxBuffer[6] = usPower & 0xFF;		// Instantaneous power (W)
		pucTxBuffer[7] = (usPower >> 8) & 0xFF;
		std::cout << ANT_SendBroadcastData(ANTPLUS_Channel_Number, pucTxBuffer);
	}

	ANT_Close();
	FreeLibrary(hDLL);
	return 0;
}