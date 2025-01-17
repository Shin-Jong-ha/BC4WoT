#include "index.h"
#include <fcntl.h>

/*
*******************************************************************
이 코드는 디지털 출력을 위해서 사용된 코드이다. 
데이터를 DB에서 읽고 해당 출력에 맞춰서 동작한다.

compare와 pair를 체크하여 동작한다.

*******************************************************************
*/

int pinMap[14] = {40, 51, 32, 15, 28, 17, 24, 27, 26, 19, 16, 25, 38, 39};

void output_DB(int fp ,int gpioNumber)
{ // DB에서 데이터를 읽고 해당 값을 출력하는 함수.
		int fp2, resultNumber;
		char data[255]; // read DB Data (ON / OFF)
		char fpData[100];
		char temp[100];
		char gpioChar[10];
		char gpio_pair[20];
		char result[10];
		int i = 0, readResult;

		levelInit(gpioNumber);
		gpioVariable(gpioNumber);

      if(gpioCompare(fp, gpioNumber) == 1)  //해당 핀의 compare를 확인.
      {
         printf("aawefpowjefpojwefopjwpfojpewfjpwofej");
         return;
      } 
      else
      {   
         printf("**********output DB********** \n");

         printf("Current GPIO # : %d\n", gpioNumber);
         
         resultNumber      = read(fp, data, 255); // DB에서 데이터를 읽어온다.
         data[resultNumber]   = '\0';
         printf("Current GPIO DATA :  %s\n", data);

         read(fp, data, 1); // read
         result[0]   = data[0];
         result[1]   = '\0';

         close(fp);

         if(result[0] == '1' ) // 해당 값이 출력인것을 체크
         {
               printf("SUCCESS!!@!@!#\n\n");
         }
         else if(result[0] == '0' )
         {
               printf("fail!!!!!!!!!!\n\n");
         }

         readResult = gpioPair(data_sprintf(gpioNumber), gpio_pair);// pair를 체크하기 위해서 해당 경로의 파일 내용을 읽어온다.

         if(readResult != 3)// 페어 상태를 확인.
         {
            gpio_pair[readResult] = '\0';
         }
         close(fp2);

         printf("GPIO_PAIR ::::::::::: %s", gpio_pair);

         if(gpio_pair[0] == '0')
         {
            printf("**************************NOPAIR\n");
         }
         else if(gpio_pair[0] > 0)
         {
            printf("**************************SUCCESS\n");
            DBWrite(gpio_pair, result);
         }
   // GPIO SEND      
         while(i < 26)
         {
            if(gpioNumber == pinMap[i])
            {
               break;
            }
            else
            {
               i++;
            }
         }
                     
         sprintf(gpioChar, "%d", pinMap[i]);
         gpioExport(gpioChar); //sysfs를 활용하여 gpio를 디지털 출력으로 셋.
         gpioDirection(gpioChar, "out");
         gpioValueWrite(gpioChar, result);
         i = 0;
      }

}