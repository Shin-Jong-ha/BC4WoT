#include "index.h"
#include <fcntl.h>

/*
*******************************************************************
이 코드는 PWM 출력을 위해서 구현된 것이다. period, duty 등 필요한 설정을 하고,
컴페어 동작을 체크하고 동작한다.

*******************************************************************
*/


// pwm 배열은 index.h 헤더파일에 들어있음.

int Number[]		= {3, 5, 6, 9, 10, 11};
int pwmNumber[]		= {3, 5, 6, 1, 7, 4};
int pwmMux[]		= {30, 18, 25, 19, 24, 16}; // 1 3 4 5 6 7 11번이 안되요

void analogOutput(int fp, int gpioNumber)
{ // PWM 출력을 하는 함수이다.
		int		fp2, findIndex = 0;
		int		readResult;
		char	data[50]; // read DB Data (ON / OFF)
		char	fpData[100];
		char	temp[100];
		char	gpioChar[10];
		char	test[10] = "1";
		char	result[1];
		//sysfs를 활용하기 위해서 Pin의 외적인 번호와 내부의 Mux번호가 다르기 때문에 배열로 생성 및 활용한다.
		for(findIndex = 0 ; findIndex < 6 ; findIndex++)
		{
				if(Number[findIndex] == gpioNumber)
				{
						printf("\n\n------------- gpioNumber = %d\n\n", gpioNumber);
						
						if(Number[findIndex] == 10)
						{
							gpioExport("42");
							gpioDirection("42", "out");
							gpioValueWrite("42", "1");
						}

						gpioExport(data_sprintf(pwmMux[findIndex]));
						gpioDirection(data_sprintf(pwmMux[findIndex]), "out");
						gpioValueWrite(data_sprintf(pwmMux[findIndex]), "1");

						// read DB
						readResult = read(fp, data, 50); // read
						data[readResult] = '\0';
						printf("\n\n---------------------------------datawefwef : %d : %s\n\n", gpioNumber, data);

						// GPIO SEND		
						pwmExportFunction(data_sprintf(pwmNumber[findIndex])); // PWM 심볼링 링크를 생성한다.
						pwmEnableFunction(data_sprintf(pwmNumber[findIndex]), "1");// PWM을 Enable 하여 활성화 한다.
						pwmPeriodFunction(data_sprintf(pwmNumber[findIndex]));// Period를 설정한다.
						pwmDutyFunction(data_sprintf(pwmNumber[findIndex]), data);//duty_를 설정한다.
						
						close(fp);
						break;
				}
		}

		if(gpioCompare(fp, gpioNumber) == 1) // 컴페어 확인 구문.
		{
			write(1, "aawefpowjefpojwefopjwpfojpewfjpwofej", 5);
			return;
		}


}