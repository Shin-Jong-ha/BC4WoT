 #include "index.h"
 #include <stdio.h>
#include <fcntl.h>


/*
   7&#65533;&#65533; 1&#65533;&#65533; 박광&#65533;&#65533;
   갈릴&#65533;�오 기�&#65533;&#65533;�로 &#65533;�는 �&#65533;  
*/


char* data_sprintf(int number)
{
		static char data[100];
		
		sprintf(data, "%d", number);

		return data;
}

void analogRead(int fp, int gpioNumber)
{
		char	fpData[100];
		char	analogInt[][2] = {"0", "1", "2", "3", "4", "5"};
		int		fp2, readResult;
		char	result[10];
        int     temp;

		if(gpioCompare(fp, gpioNumber) == 1)
		{
			write(0, "aawefpowjefpojwefopjwpfojpewfjpwofej", 5);
			return;
		}
		else
		{
			printf("wofeijwoeifjwioefj");
		}

		strcpy(fpData, "/sys/bus/iio/devices/iio:device0/in_voltage");
		strcat(fpData, analogInt[gpioNumber - 14]);
		strcat(fpData, "_raw");

		fp2 = open(fpData, O_RDONLY);

		if(fp2 == -1)
		{
			perror("readResult (Main) : ");
			//exit(0);
		}

		readResult = read(fp2, result, 255);

		result[readResult] = '\0';
//
//		temp = atoi(result) / 16;
//
//		sprintf(result, "%d", temp);


		
		printf("\n\n --------------------------------------------------------------%d ::::::: %s\n\n", gpioNumber, result);

		DBWrite(data_sprintf(gpioNumber), result);
		
		close(fp2);
}

int main(void)
{
		int status;
		// &#65533;�일 &#65533;�스&#65533;�립&#65533;&#65533;
		int gpio_file, gpio_config_file, gpio_unexport, gpio_pair;
		
		// GPIO &#65533;�어&#65533;�기 &#65533;�한 &#65533;�시 �&#65533;&#65533;&#65533;
		int i = 0;
		int temp = 0;
		
// �ڽ� ���μ���
		int pid, childPid, childPidTemp;


		// GPIO 번호 
		int gpio_number = -1, gpio_config_number = -1;

		// DB�&#65533; �&#65533;&#65533;&#65533; &#65533;�어&#65533;&#65533; �&#65533; &#65533;&#65533;&#65533;&#65533;
		
		char gpio_config_str[255] = {0};
		char gpio_result_str[2]; // DB&#65533;�서 &#65533;�어&#65533;&#65533; 값을 &#65533;&#65533;&#65533;�한&#65533;&#65533;.
		char gpio_value_str[40];

		// DB 경로
		char *pwd_temp			= "/var/www/project_os/DB/gpio";
		char *pwd_config_temp	= "/var/www/project_os/DB/gpio_config_DB/";
		char *pwd_value_temp	= "/sys/class/gpio/gpio";

		char* test[3] = {"nohup",  "/var/www/project_os/c_program/uart/uart", "&"};

		char pwd[50] = {0};

		/*
		   &#65533;&#65533;&#65533;��&#65533; &#65533;�일 &#65533;�출&#65533;&#65533; 구현

		   &#65533;�에&#65533;&#65533; GPIO 먼�&#65533; &#65533;�정&#65533;�면

		   **************************************************
		   1. gpio_DB.txt : GPIO&#65533;�서 &#65533;�어&#65533;�는 값을 &#65533;�이&#65533;�베&#65533;�스&#65533;&#65533; &#65533;&#65533;&#65533;�함.
		   2. gpio_config_DB.txt : GPIO�&#65533; &#65533;�력&#65533;�로 &#65533;��&#65533; 출력&#65533;�로 &#65533;��&#65533; &#65533;�한&#65533;&#65533;. (&#65533;�에&#65533;&#65533; &#65533;�정&#65533;�고 C&#65533;�로그램&#65533;�서 &#65533;�는&#65533;&#65533;.)
		   **************************************************
	    */

		init(); // Galileo Level Shifter 초기&#65533;&#65533;
		pid = fork(); // ����Ʈ �ڽ� ���μ��� ����
		while(gpio_config_number++ < 19)
		{
				// &#65533;�정 DB &#65533;�일 &#65533;�픈
				strcpy(gpio_config_str, pwd_config_temp);
				strcat(gpio_config_str, data_sprintf(gpio_config_number));

				gpio_config_file = open(gpio_config_str, O_RDONLY, S_IRUSR); // O_RDONLY, S_IRUSR

				if( gpio_config_file == -1 ) // DB &#65533;�는&#65533;&#65533; &#65533;�류�&#65533; &#65533;�을 경우 &#65533;�로그램 처음&#65533;�로 &#65533;�아�&#65533;
				{
						perror("config_file error (MAIN)");
						close(gpio_config_file);

						continue; // 처음&#65533;�로
				}

				read(gpio_config_file, gpio_result_str, 2); // GPIO &#65533;�정 값을 &#65533;�어&#65533;�다.

				gpio_number++; // 몇번 GPIO&#65533;��&#65533;

				/*
				   ***************************
				   1. HTML &#65533;�력 (HTML&#65533;�으�&#65533; 버튼) - 값을 &#65533;�어&#65533;&#65533; GPIO�&#65533; 보내주어&#65533;�함
				   2. &#65533;��&#65533;&#65533;&#65533; &#65533;�력 - &#65533;��&#65533; 버튼 &#65533;�력 : DB&#65533;&#65533; &#65533;&#65533;&#65533;�함
				   3. &#65533;��&#65533;&#65533;&#65533; 출력 - DB값을 &#65533;�어&#65533;&#65533; 출력&#65533;�다. (HIGH / LOW)
				   4. &#65533;�날로그 &#65533;�력 - &#65533;�날로그 값을 &#65533;�어&#65533;&#65533; DB&#65533;&#65533; &#65533;&#65533;&#65533;�함
				   5. &#65533;�날로그 출력 - DB값을 &#65533;�어&#65533;&#65533; 출력&#65533;&#65533;
				   ***************************
				*/

				// GPIO &#65533;�이&#65533;��&#65533; &#65533;&#65533;&#65533;�하거나 &#65533;�어&#65533;�기 &#65533;�한 �&#65533;�&#65533;
				strcpy(pwd, pwd_temp); // 경로 (/project_os/DB/gpio/) �&#65533; 복사&#65533;&#65533;
				strcat(pwd, "/");
				strcat(pwd, data_sprintf(gpio_config_number)); // GPIO번호�&#65533; 붙인&#65533;&#65533;.
		
				if((gpio_file = open(pwd, O_RDWR, S_IRUSR)) == -1) // GPIO DB &#65533;�일&#65533;&#65533; &#65533;�다.
				{
						perror("open_gpio_file (MAIN)");
						continue;
				}

				/* 
					output_DB(int data) : DB&#65533;&#65533; &#65533;&#65533;&#65533;�된 값을 &#65533;�어&#65533;�다.
					insert_DB(int data) : DB&#65533;&#65533; 값을 &#65533;�입&#65533;�다.
				*/

				printf("\n\n --------------------------------------------------------------%d %c\n\n", gpio_config_number, gpio_result_str[0]);

				switch( gpio_result_str[0] )
				{
						case '0' : // &#65533;�용&#65533;��&#65533; &#65533;�는 경우
								printf("&#65533;�용&#65533;��&#65533; &#65533;�음\n");
								break;
						case '1' : // &#65533;��&#65533;&#65533;&#65533; 출력
								output_DB(gpio_file, gpio_config_number);
								break;
						case '2' : // &#65533;��&#65533;&#65533;&#65533; &#65533;�력
								insert_DB(gpio_file, gpio_config_number); // 10000 &#65533;&#65533;&#65533;�에 GPIO&#65533;�서 &#65533;�어&#65533;�는 값으�&#65533; �&#65533;경하�&#65533;
								break;
						case '3' : // &#65533;�날로그 &#65533;�력
								analogRead(gpio_file, gpio_config_number);
								break;
						case '4' : // PWM 출력
								analogOutput(gpio_file, gpio_config_number);
								break;
						case '5' : // Rx_insert. 

									if( pid == -1 )
									{
										perror("fork error");
									}
									else if( pid == 0 )
									{
										
	//									//Rx_rasb(gpio_file, gpio_config_number);
										childPid = getpid();
										status = 1;

	//										insertUart();
										if(execlp(test[1], test[1], (char *)NULL) == 0)
										{
											perror("execlp");
											exit(0);
										}
									}
									else
									{
										printf("wefoijweojfeoiw pid = %d, %d", childPid,pid);
									}

								
								break;
						case '6' :

									if( pid == -1 )
									{
										perror("fork error");
									}
									else if( pid == 0 )
									{
	//									//Rx_rasb(gpio_file, gpio_config_number);
	//										printf("pid = %d", pid);
											status = 1;
	//										insertUart();
										if(execlp(test[1], test[1], (char *)NULL) == 0)
										{
											perror("execlp");
											exit(0);
										}
	//									

									}
									else
									{
										printf("wefoijweojfeoiw pid = %d", pid);
	//										break;
									}

						default : // &#65533;�외 처리
								printf("&#65533;�류\n");
								break;
				}

				close(gpio_file);
				close(gpio_config_file);
		
				i = 0; // 초기&#65533;&#65533;
				gpio_number = -1;

				if(gpio_config_number == 19)
				{
					gpio_config_number = -1;
				}
		}
}
