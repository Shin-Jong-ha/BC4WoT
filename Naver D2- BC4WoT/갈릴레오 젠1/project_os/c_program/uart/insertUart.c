#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>


/*
*******************************************************************
�� �ڵ�� uart�� �ڽ� ���μ����� ���۽�Ű�� ���� ���� main �����̴�.
�� main���� �߽����� uart ���� ������ �����ǰ�, ���� ���α׷����� ȣ���Ҷ�, �ڽ����μ����μ� �����Ѵ�.

ȯ�漳�� ���Ͽ��� �����͸� �а� �ش� �����Ͱ� �۽����� ���������� üũ�Ͽ� �����ϵ��� �Ͽ���.
*******************************************************************
*/


extern int rxUart(int gpio_fd, int gpioNumber);
extern int txUart(int gpio_fd, int gpioNumber);

int main(void) // Uart �ڽ� ���μ����� ���� ����.
{
	int			status;
	int			fpConfig, fpReadIndex, i;
	char		data[2];
	char		location[50] = "/usr/local/apache/htdocs/project_os/DB/gpio_config_DB/";
	char*		gpioFile[2] = {"/usr/local/apache/htdocs/project_os/DB/gpio/0", "/usr/local/apache/htdocs/project_os/DB/gpio/1"};
	
	while(1)
	{
		status = 0; //rx,tx ����� üũ�Ѵ�.

		for(i = 0 ; i < 2 ; i++)
		{
			strcat(location, data_sprintf(i)); // gpio ���� ���� ���
			printf("location : %s\n", location);
			fpConfig			= open(location, O_RDONLY);

			if(fpConfig == -1)
			{
				perror("fpConfig");
			}
			fpReadIndex			= read(fpConfig, data, 1);// ������ ������ ������ �о�´�.
			
			data[fpReadIndex]	= '\0';

			if( data[0] == '5' || data[0] == '6')
			{
				status += (i + 1); // 0�϶��� ������, 1�� ���� RX, 2�� ���� TX, 3�� ���� �Ѵ� �����
			}

			close(fpConfig);
			strcpy(location, "/usr/local/apache/htdocs/project_os/DB/gpio_config_DB/");
		}
		printf("status = %d\n\n", status);

		if(status == 1)//rx�� ����ϵ��� �����Ǿ� �ִ°��
		{
			printf("rx");
			rxUart(gpioFile[0], 0);
		}
		else if(status == 2)// tx�� ����ϵ��� �����Ǿ� �ִ°��.
		{
			write(1,"tx", 3);
			txUart(gpioFile[1], 1);// tx�Լ�.
		}
		else if(status == 3)// rx,tx�� �Ѵ� ����ϴ� ���
		{
			printf("all");
			rxUart(gpioFile[0], 0);
			txUart(gpioFile[1], 1);
		}
		else
		{
			exit(0);
			//process����
		}
	}
}
