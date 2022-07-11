#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "ioctl_test.h"

#define DEVICE_FILENAME  "/dev/ledkey"

int main()
{
    int dev;
    char buff = 15;
    int ret;
    int key_old = 0;
	ioctl_test_info info = {0, {0}};	// size 128을 0으로 초기화

    dev = open( DEVICE_FILENAME, O_RDWR|O_NDELAY );	// 장치파일 open
	if(dev<0)
	{
		perror("open()");
		return 1;
	}
	ret = ioctl(dev,IOCTLTEST_KEYLEDINIT);	// key, led init 두 개 다 호출
	// 디바이스 장치의 값을 R/W하기 위해 쓰는게 아니라 control 하기 위한 함수

	ret = ioctl(dev,IOCTLTEST_LEDON);
	sleep(1);
	ret = ioctl(dev,IOCTLTEST_LEDOFF);
	sleep(1);
	ret = ioctl(dev,IOCTLTEST_LEDON);
	sleep(1);	
	ret = ioctl(dev,IOCTLTEST_LEDOFF);
	sleep(1);	
	ret = ioctl(dev,IOCTLTEST_LEDWRITE);

	info.size = 1;
	info.buff[0] = 0x05;
	sleep(2);
	info.buff[0] = 0x00;
	ret = ioctl(dev,IOCTLTEST_KEYLEDFREE);

/*

    ret = write(dev,&buff,sizeof(buff));
	if(ret < 0)
		perror("write()");
	buff = 0;
	do {
    	ret = read(dev,&buff,sizeof(buff));              
  		if(ret < 0)
  			perror("read()");
		if(buff != key_old)
		{
			if(buff)
			{
				printf("key_no : %d\n",buff);
				write(dev,&buff,sizeof(buff));
			}
			if(buff == 8)
				break;
			key_old = buff;
		}
	} while(1);
*/
    close(dev);
    return 0;
}
