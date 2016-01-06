#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/*
	*******************************************************************
	���ڵ�� �������� �ð��� ��Ÿ���� time�� �̿��Ͽ� Ÿ�̸Ӹ� ������ �ڵ��̴�.
	Time_DB�� ������ Pin#, Timer#, ������°� ex)3 10 0 �� �Է��� �Ǹ� �� ���� �о�
	�ð��� �帧�� ���� Timer#�� �ٿ��� 0�� �Ǹ� �ش� Pin#�� ���¸� ������ �� �ֵ��� �����Ͽ���.
	*******************************************************************
*/

time_t   current_time;

struct tm *t;

int timer(void) //time_linux
{
	static int sec = 0;
	int secNext = 0,buffer = 0;
	char buf_data[255];
	char *Time_DB_PATH  = "/usr/local/apache/htdocs/project_os/DB/Time_DB/timer";	// web���� �Է��� ��ex) 3 10 0--> 3�� �ɿ� 10�� Ÿ�̸� �ʱⰪ�� 0(on)
	int time_fd;
	char chr_string[13][3] = {0};			// ������ �迭
	int i, j, k, l = 0;						
	char *ptr;								// ���� ������ Time_DB_PATH�� �����͸� �о ������ ������
	char result[255];						// ������� ������ �迭
	int cnt = 0;							// �迭 i�� ���� �˱� ���� ī����
	char insertDatabase[255] = {0};
	char insertDatabaseTemp[10] = {0};
	/*
	   ***************************
	   1. secNext		- ���� �ð��� �ʸ� ��Ÿ����.
	   2. sec			- ���� �����ν� ���� �� �ð��� �ʸ� ��Ÿ����.
	   ***************************
	*/
	time( &current_time);

	t = gmtime(&current_time);
	secNext = t->tm_sec;
	printf("tm_sec => %d\n", t->tm_sec);

	if( secNext - sec != 0)					// �ð��� �귶�� ��쿡
	{
		//��� ���� : time_DB�� �����͸� �о�ͼ� 
		//������ ������ \n�� ������ ���� ó�� EOF�� ������ ������ ���� ����
		time_fd				= open(Time_DB_PATH, O_RDWR);
		buffer				= read(time_fd, buf_data, 255);
		close(time_fd);
		buf_data[buffer]	= '\0';		// buffer ���� 0 �ƴϸ� -1?

	//1. ������ �迭�� �� ���� ����		
		ptr	= strtok( buf_data, " " "\n");	// buf_data�� ���� ���� ������ ��� ptr�� ����

		for( i = 0 ; ptr != NULL ; i++)
		{
			for( j = 0 ; j < 3 ; j++ )
			{
				chr_string[i][j] = atoi(ptr);		/// �迭�� ptr�� ���� ������ ���޵� ���� ����
				printf("chr_string[%d][%d] => %d\n", i, j, chr_string[i][j]);
				ptr = strtok(NULL, " " "\n");
			}
			cnt++;
		}
		
	//2. Ÿ�̸� ���� 0 �̻��� ��츦 ã�� 1�� ����, Ÿ�̸� ���� 0�� ��� ���� ������Ű��
		for( i = 0 ; i < cnt ; i++)
		{
			chr_string[i][1] -= 1;	
			for( l = i ; l < cnt ; l++)			// ���� �ϰ� �� �迭�� ���� ���� �ݺ���
			{
				if( chr_string[i][1] == 0)		// Ÿ�̸Ӱ� 0�� �Ǹ� ������ ����
				{
					sprintf(insertDatabaseTemp, "%d", chr_string[i][2]); // insertDatabaseTemp�� �ӽ÷� �����Ѵ�.
					DBWrite(data_sprintf(i), insertDatabaseTemp);
					
					printf("Delete result = %d %d %d \n", chr_string[i][0], chr_string[i][1], chr_string[i][2]);
					// Ÿ�̸Ӱ� �� �Ǿ��� ������ �ش� �׼��� ����

					// 0�� �Ǹ� �� �迭�� �״��� �迭 �ֱ� (����)
					k = i;
					for( ; k < cnt ; k++)
					{
						for( j = 0 ; j < 3 ; j++ )
						{
							chr_string[k][j] = chr_string[k+1][j];
						}
					}
					chr_string[i][1] -= 1;
					cnt--;
				}
			}
		}

		k = 0;

		time_fd				= open(Time_DB_PATH, O_WRONLY | O_TRUNC); // timer ������ ���� �ʱ�ȭ ��ä�� ����.

		for( i = 0 ; i < cnt ; i++)		// result 1���� �迭�� chr_string 2���� �迭 �� �ֱ�
		{
			for( j = 0 ; j < 3 ; j++ )
			{
				sprintf(insertDatabaseTemp, "%d", chr_string[i][j]); // insertDatabaseTemp�� �ӽ÷� �����Ѵ�.

				if(i == 0 && j == 0){
					strcpy(insertDatabase, insertDatabaseTemp); // ó�� ������ ��쿡 insertDatabase�� �����Ѵ�.
				}else{
	
					if(j != 0){ // ù ��° ������ ó���� �ƴ� �� ������⸦ �������� �ʴ´�.
						strcat(insertDatabase, " ");
					}
					strcat(insertDatabase, insertDatabaseTemp); // ���ڸ� ��� �̾ ������.
				}
			}
			strcat(insertDatabase, "\n");
		}

		write( time_fd, insertDatabase, strlen(insertDatabase)); // �����ͺ��̽��� �����͸� �Է��Ѵ�.

		close(time_fd);
	}

	sec = secNext;
 
   return 0 ;
}

