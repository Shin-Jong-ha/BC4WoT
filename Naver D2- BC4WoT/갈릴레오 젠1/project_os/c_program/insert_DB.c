#include "index.h" //해당 프로젝트의 함수들을 활용하기 위한 헤더파일.
#include <fcntl.h>

/*
*******************************************************************
이 코드는 DB에 데이터가 저장되는 기능을 구현한 것이다. 
입력 되는 값을 읽고 해당 DB에 데이터를 저장한다. 입력 전 compare와 pair기능을 체크하고
마지막에 입력한다.
*******************************************************************
*/

extern int pinMap[14]; // gpio 핀의 값을 공유 extern을 활용.
extern int initNumber[];

void insert_DB(int fp, int gpio_number)// 데이터를 DB에 저장하는 함수. main함수에서 호출.
{	
	int fp2;
	char data[2]; // read DB Data (ON / OFF)
	char fpData[100];
	char temp[100];
	char gpioChar[10];
	char test[10] = "1";
	char result[3], gpio_pair[3];
	int i = 0, readResult;

	levelInit(gpio_number);// 초기화 하는 함수.

	for( i = 0 ; i < 2 ; i++)
	{
		if(i == 0)
		{
			sprintf(gpioChar, "%d", initNumber[gpio_number]);//입력 받은 숫자를 버퍼에 저장.
		}
		else
		{
			if(gpioCompare(fp, gpio_number) == 1) 
			{
				printf("aawefpowjefpojwefopjwpfojpewfjpwofej");
				break;
			}

			sprintf(gpioChar, "%d", pinMap[gpio_number]);
		}

		gpioExport(gpioChar);// 심볼릭 링크 생성.
// fpData에 경로를 복사한다.
		gpioDirection(gpioChar, "in"); // 심볼릭 링크의 파일중에 Direction 파일에서 in,out을 설정.

// GPIO 형식 설정하기
		gpioDrive(gpioChar, "strong");

// GPIO 해당하는 값 확인하기

// i가 0일 때, Level Shifter GPIO를 0으로 만들어준다.
		if(i == 0)
		{
			gpioValueWrite(gpioChar, "0");
		}
		else 
		{
// 먼저 gpio_pair의 값을 읽어온다. 0일때는 사용하지 않음, 다른 값일때는 사용 
			
			readResult = gpioPair(data_sprintf(gpio_number), gpio_pair); //해당 gpio와 함께 동작하는 Pair gpio를 체크.
																		//gpioPair함수로 이동하여 체크한다.
			gpio_pair[readResult] = '\0'; 
		
			close(fp2);

			if(gpio_pair[0] == '0')
			{
				printf("**************************NOPAIR\n");
			}
			else if(gpio_pair[0] > 0)
			{
				printf("**************************SUCCESS\n");
			}

// GPIO 값을 읽어옴. (ex. 버튼 누름 여부)

			strcpy(result, gpioValueRead(gpioChar));

			DBWrite(data_sprintf(gpio_number), result);
			

// GPIO PAIR (짝) 값에 보낸다.
		
		//	DBWrite(gpio_pair, result);

			if(result[0] == '0') // ex. 버튼 안눌렀을때
			{
				printf("----------------------------------------off \n\n");

				DBWrite(gpio_pair, "0");
			}
			else //ex.  눌렀을 때
			{
				printf("----------------------------------------ONsadsdasd \n\n");
				printf("%s aa\n" , result);

				DBWrite(gpio_pair, "1");
			}
		}
	}
}