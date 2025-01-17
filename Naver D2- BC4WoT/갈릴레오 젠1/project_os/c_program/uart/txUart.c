#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

/*
*******************************************************************
이 코드는 uart의 tx를 활용하기 위해서 사용되는 코드이다. 
외부 라이브러리 wiringPI와 리눅스의 dev/ttyS0를 활용하였다. 
환경설정 파일에 있는 설정 명령과 Baudrate를 읽어오고, '-' 시작 문자와 '=' 종료 문자를 통해서 
구현되었다.
데이터가 저장되있는 파일에서 시작문자와 종료문자를 찾아서 배열에 저장하고 저장된 배열을 dev/ttyS0에 저장하여
데이터가 전송되는 형태이다.
*******************************************************************
*/


char* data_sprintf(int number) //숫자를 문자로 변환하기 위해 만든 함수.
{
		static char data[100];
		sprintf(data, "%d", number);
		return data;
}


int loopCounter = 0;

int txUart(int gpio_fd, int gpioNumber)
{ // uart의 tx기능을 활용하기 위한 함수 , 외부라이브러리를 활용하였다.(wiringPI)
   
   int serialFd, fd ,fd1, fd2,num,Setfd,SetNum,j=0,Baudrate;
   int i = 0;
   char con_data[255];
   char data[255];
   char gpio_config_str[255],Set_data[255];
   char uart[100];
   static char result[2];
   static char config[100];
   static char config_count[100],Set_Baudrate[255];
   char dataTemp;
   char dataResult[100];
   int dataCondition = 0;   
   char testloop[6];
   int fd2Read;

   char test[10];

   char *gpioLocation = "/usr/local/apache/htdocs/project_os/DB/gpio/1";//보내는 데이터가 저장되어있는 경로.
   char *pwd_config_temp   = "/usr/local/apache/htdocs/project_os/DB/gpio_config_DB/"; // 환경설정 경로
   
   gpioExport("4");
   gpioExport("40");
   gpioExport("41");

   gpioDirection("4", "out");
   gpioDirection("40", "out");
   gpioDirection("41", "out");

   gpioValueWrite("4", "1");
   gpioValueWrite("40", "0");
   gpioValueWrite("41", "0"); // 
	
	strcat(pwd_config_temp,data_sprintf(gpioNumber));

	Setfd = open(pwd_config_temp,O_RDONLY)
	SetNum = read(Setfd,Set_data,255);
	Set_data[SetNum] = '\0';//종료 문자 입력.
    close(Setfd);

	i = 0;
	j = 2;

	while(1)
	{
		Set_Baudrate[i++] = Set_data[j]; // 공백을 체크하고 Baudrate를 분리하는 부분.

		if(Set_data[j++] == ' ')
		{
			Set_Baudrate[i] = '\0';
			printf("\n Set_Baudrate %s \n",Set_Baudrate);
			break;   
		}
	}
	
	Baudrate = atoi(Set_Baudrate); //Baudrate를 int형으로 형변환.

   if ((serialFd = serialOpen ("/dev/ttyS0", Baudrate)) < 0) //serialOpen 함수를 활용하여 Baudrate와 디바이스 ttyS0를 활용.
   {
      printf("aabb");
      fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno));
      return 1;
   }
   else
	{
	   printf("success\n");
	}


   i = 0;

   fd2            = open(gpioLocation, O_RDONLY);
   fd2Read         = read(fd2, data, 255);// 데이터를 불러오는 부분.
   data[fd2Read]   = '\0';

   close(fd2);

   i = 0;

   while(1)
   {
      if(dataCondition == 1)
      {
         if(data[i] == '=')// 마지막 문자일때
         {      
            printf("data : %s", dataResult);

            dataResult[i - 1] = '\0';
            write(serialFd,dataResult,strlen(dataResult)); //저장된 배열을 wirte를 통해서 ttyS0에 쓰면 uart tx가 전송된다. 
            fflush(stdout);

            break;
         }

         dataResult[i - 1] = data[i];//종료문자를 찾을때까지 배열에 저장.

         i++;


      }
      else
      {

         if(data[i] == '-')// 전송의 첫문자
         {
            printf("%c" , data[i]);
            i++;
            dataCondition = 1;
         }

      }
   }

   serialClose(serialFd);

   return 0;
}