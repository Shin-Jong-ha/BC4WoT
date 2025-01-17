#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

/*
*******************************************************************
이 코드는 uart의 rx를 활용하기 위해서 사용되는 코드이다. 
외부 라이브러리 wiringPI와 리눅스의 dev/ttyS0를 활용하였다. 
환경설정 파일에 있는 설정 명령과 Baudrate를 읽어오고, '-' 시작 문자와 '=' 종료 문자를 통해서 
구현되었다.
데이터가 수신되는 dev/ttyS0에서 시작문자와 종료문자를 찾아서 배열에 저장하고 저장된 배열을 DB에 저장하는
형태로 데이터가 수신되는 형태이다.
*******************************************************************
*/

char* data_sprintf(int number) //숫자를 문자로 변환하기 위해 만든 함수.
{
		static char data[100];
		sprintf(data, "%d", number);
		return data;
}

int rxUart(int gpio_fd, int gpioNumber)
{// uart의 rx기능을 사용 하기 위한 함수, 해당 기능은 wiringPI라는 외부 라이브러리를 활용하여 구현하였다.

   static int loopCounter = 0;
   int serialFd, fd ,fd1, fd2,num,j=0,Setfd,SetNum,Baudrate;
   int i = 0;
   char data[255],con_data[255],Set_data[255];
   char gpio_config_str[255];
   char gpioLocation[255]; // DB에서 읽어온 값을 저장한다.
   char uart[100];
   static char result[2];
   static char config[100],Set_Baudrate[100];
   static char config_count[100];
   char dataTemp;
   int dataCondition = 0, m = 0;

    char *pwd_config_temp   = "/usr/local/apache/htdocs/project_os/DB/gpio_config_DB/";// 설정 환경을 읽어온다.
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

   if ((fd = serialOpen ("/dev/ttyS0", Baudrate)) < 0)// serialOpen 함수를 활용하여 Baudrate와 디바이스 ttyS0를 활용.
   {
      fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno));
      return 1;
   }
   
   char *Gpio = "/usr/local/apache/htdocs/project_os/DB/gpio/";// 데이터가 저장되는 부분.
   strcpy(gpioLocation,Gpio);
   strcat(gpioLocation, data_sprintf(gpioNumber));
   
   i = 0;

   while(1)
   {
//    printf("m : %d\n", m++);
      dataTemp = (char)serialGetchar(fd);
      
      printf("dataTemp %c", dataTemp);
      if((int)dataTemp == -1)
      {
         break;
      }

      if(dataCondition == 1)
      {
         if(dataTemp == '=')// 마지막 문자일때
         {
            if((fd2 = open(gpioLocation, O_WRONLY | O_TRUNC)) == -1)
			 {
				perror("open");
				exit(0);
			 }
            data[i] = '\0';
            
            printf("data : %s gpioNumber : %d", data, gpioNumber);
            write(fd2,data,strlen(data)); // 해당 ttyS0에 쌓인 버퍼를 데이터 파일에 저장한다.
            close(fd2);   

            break;
         }
         else
         {
            data[i] = dataTemp;
         }

         sprintf(uart,"\nPC > RPi = %c \n",(char)data[i]);
         write(1,uart,strlen(uart));//데이터를 입력한다.
         fflush(stdout);//버퍼를 지워준다.
         i++;
      }

      if(dataTemp == '-')//시작 문자일 때,
      {
         dataCondition = 1;
      }      
   }
   serialClose(fd);

   if(gpioCompare(gpio_fd, gpioNumber) == 1)//컴페어 구문을 체크한다.
   {
      printf("aawefpowjefpojwefopjwpfojpewfjpwofej");
   }

   return 0 ;
}