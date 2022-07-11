#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>

int main(int argc, char *argv[])
{
		struct tm *t;
		time_t tt;

		tt = time(NULL);	//time이라는 함수를 이용해 NULL을 넣으게 되면 리눅스 최초 개발일부터 (1971.?1.??)
							//현재까지를 초로 환산한 값이 리턴됨
		if(errno == EFAULT)
				perror("time\n");

		// GMT Time
		t = gmtime(&tt);	//그리니치시(표준시에 넣어준다)
		printf("gmt time   : %s", asctime(t));

		// Local Time
		t = localtime(&tt);
		printf("local time : %s", asctime(t));
		printf("local time : Hour:%d, Min:%d, Sec:%d\n",t->tm_hour,t->tm_min,t->tm_sec);

		return 0;
}
