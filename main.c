#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "SPI_SD.h"  
#include "malloc.h"  
#include "Serial.h"  
#include "MySPI.h"  
//#include "ff.h" 

#define RECBUFFER_SIZE  16
#define RECORDERDIR	"0:/music"


uint8_t MID;
uint16_t DID;

uint8_t ArrayWrite[] = {0x05, 0x06, 0x07, 0x08};
uint8_t ArrayRead[4];

uint8_t buffer0[RECBUFFER_SIZE];  /* ���ݻ�����1 ��ʵ��ռ���ֽ�����RECBUFFER_SIZE*2 */
//uint16_t buffer1[RECBUFFER_SIZE];  /* ���ݻ�����2 ��ʵ��ռ���ֽ�����RECBUFFER_SIZE*2 */


/* WAV�ļ�ͷ��ʽ */
typedef __packed struct
{ 
	uint32_t	riff;							/* = "RIFF"	0x46464952*/
	uint32_t	size_8;						/* ���¸���ַ��ʼ���ļ�β�����ֽ���	*/
	uint32_t	wave;							/* = "WAVE" 0x45564157*/
	
	uint32_t	fmt;							/* = "fmt " 0x20746d66*/
	uint32_t	fmtSize;					/* ��һ���ṹ��Ĵ�С(һ��Ϊ16) */
	uint16_t	wFormatTag;				/* ���뷽ʽ,һ��Ϊ1	*/
	uint16_t	wChannels;				/* ͨ������������Ϊ1��������Ϊ2 */
	uint32_t	dwSamplesPerSec;	/* ������ */
	uint32_t	dwAvgBytesPerSec;	/* ÿ���ֽ���(= ������ �� ÿ���������ֽ���) */
	uint16_t	wBlockAlign;			/* ÿ���������ֽ���(=����������/8*ͨ����) */
	uint16_t	wBitsPerSample;		/* ����������(ÿ��������Ҫ��bit��) */
																			  
	uint32_t	data;							/* = "data" 0x61746164*/
	uint32_t	datasize;					/* �����ݳ��� */
} WavHead;	


//FIL file;											/* file objects */
//FRESULT result; 
//UINT bw;            					/* File R/W count */
//DIR dir;
 //FATFS *fs[3];											/* Work area (file system object) for logical drives */	


static WavHead rec_wav;            /* WAV�豸  */
static char recfilename[25]={"E:/music/001.txt"};  


/**
  * @brief  ����WM8978��STM32��I2S��ʼ������
  * @param  ��
  * @retval ��
  */
/*static void DataTransport(const char *filename)
{
//	printf("��ǰ�����ļ� -> %s\n",filename);
	
	result=f_open(&file,filename,FA_READ);
	if(result!=FR_OK)
	{
//		printf("����Ƶ�ļ�ʧ��!!!->%d\r\n",result);
		result = f_close (&file);
		return;
	}
	//��ȡWAV�ļ�ͷ
	result = f_read(&file,&rec_wav,sizeof(rec_wav),&bw);
	//�ȶ�ȡ��Ƶ���ݵ�������
	result = f_read(&file,(uint16_t *)buffer0,RECBUFFER_SIZE*2,&bw);
//	result = f_read(&file,(uint16_t *)buffer1,RECBUFFER_SIZE*2,&bw);
Serial_SendString(buffer0);

	
}

*/
void SD_Read_Sectorx(u32 sec)
{
	u8 *buf;
	u16 i;
	buf=mymalloc(512);				//�����ڴ�
	if(SD_ReadDisk(buf,sec,1)==0)	//��ȡ0����������
	{	
	//	LCD_ShowString(60,190,200,16,16,"USART1 Sending Data...");
//		printf("SECTOR 0 DATA:\r\n");
		for(i=0;i<512;i++)Serial_Printf("%x ",buf[i]);//��ӡsec��������    	   
	//	printf("\r\nDATA ENDED\r\n");
	//	LCD_ShowString(60,190,200,16,16,"USART1 Send Data Over!");
	}
	myfree(buf);//�ͷ��ڴ�	
}


int main(void)
{
//	uint8_t cid_data_space[16]={0}; 
	uint8_t cid_data[16];
//	uint8_t csd_data[16]; 
//	uint8_t cid_inf;
//	uint8_t csd_inf;
	uint32_t sd_size;
	uint8_t *buf;
	int i;
uint8_t res; 
//FRESULT result; 


	OLED_Init();
	Serial_Init();
  res=SD_Initialize();
	
		if(res)
	{
    OLED_ShowString(1, 1, "SD  ERROR");       
		while(1);
	}

/* ���SD���ɹ� */
OLED_Clear();		
OLED_ShowString(1, 1, "SD OK");
	
while(1)	
{	
		buf=mymalloc(512);				//�����ڴ�
//	res=SD_ReadDisk(buf,0,1);
//sd_size  =SD_GetSectorCount();
//	buf=mymalloc(512);				//�����ڴ�
	if(SD_ReadDisk(buf,0,1)==0)	//��ȡ0����������
	{	
	//	LCD_ShowString(60,190,200,16,16,"USART1 Sending Data...");
//		printf("SECTOR 0 DATA:\r\n");
		for(i=0;i<512;i++)Serial_SendByte(buf[i]);//��ӡsec��������    	   
	//	printf("\r\nDATA ENDED\r\n");
	//	LCD_ShowString(60,190,200,16,16,"USART1 Send Data Over!");
	}
	myfree(buf);//�ͷ��ڴ�	
//	if(res)
//	{
//    OLED_ShowString(2, 1, "SD  READ ERROR");       
//		while(1);
//	}
//	OLED_ShowString(2, 1, "SD  READ OK");       
//	Serial_SendString(buf);
//	Serial_SendNumber(sd_size,32);
//	Serial_SendArray(buf,512);
	
	
}
	
	
//	*buf=12;
//	sd_write_disk(buf,0,1);
	//	sd_get_cid(cid_data);
//Serial_SendArray(cid_data,16);

/*	for(i=0;i<3;i++)
	{
			fs[i]=(FATFS*)mymalloc(sizeof(FATFS));	//Ϊ����i�����������ڴ�	
		if(!fs[i])break;
	}
	result = f_mount(fs[0],"0:",1);  //�����ļ�ϵͳ
	if(result!=FR_OK)
	{
		OLED_ShowString(2, 1, "SD FATFS ERROR");
		while(1);
	}
	OLED_ShowString(2, 1, "SD FATFS OK");
	result = f_opendir(&dir,"0:/music");//�������ļ���
	
	 	while(result)
 	{	 
		OLED_ShowString(3, 1, "OPEN DIR ERROR");		  
    while(1);	  
	} 
	OLED_ShowString(3, 1, "OPEN DIR OK");
*/	
//	DataTransport(recfilename);


//	while(result != FR_OK)
//	{
//		f_mkdir(RECORDERDIR);
//		result = f_opendir(&dir,RECORDERDIR);
//	}		
	

		
//	Serial_SendArray(cid_data_space,16);


//	while (1)
//	{
		
//	}
}
