#ifndef __IOCTL_H__			//중복 정의를 막기 위해서 ifndef를 활용
#define __IOCTL_H__

#define IOCTLTEST_MAGIC 't' //명령을 만들기 위해 가장 먼저 매직 번호 선언 //매직 번호는 임의로 설정할 수 있으므로 t문자를 사용하여 정의
							//아스키코드t가 선언된것임
typedef struct
{
	unsigned long size;
	unsigned char buff[128];
} __attribute__((packed)) ioctl_test_info;
//data 를 밀집하여 저장

#define IOCTLTEST_KEYLEDINIT _IO(IOCTLTEST_MAGIC, 0) //부가적 데이터 정보가 필요없는 IOCTLTEST_LEDOFF,LEDON,GETSATE
#define IOCTLTEST_KEYLEDFREE _IO(IOCTLTEST_MAGIC, 1) //명령을 만들기 위해 IO매크로를 사용
#define IOCTLTEST_LEDOFF	_IO(IOCTLTEST_MAGIC, 2)  //IO매크로는 매직번호와 명령을 구분하기 위해 인덱스 번호를 이용하여 명령 생성
#define IOCTLTEST_LEDON		_IO(IOCTLTEST_MAGIC, 3)
#define IOCTLTEST_LEDWRITE	_IOW(IOCTLTEST_MAGIC, 4, ioctl_test_info)
#define IOCTLTEST_MAXNR		5
/*#define IOCTLTEST_MAXNR		4*/
//#define IOCTLTEST_MAXNR 	_IO(IOCTLTEST_MAGIC, 4)
#endif
