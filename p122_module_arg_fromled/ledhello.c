#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/gpio.h>

#define DEBUG 1
#define IMX_GPIO_NR(bank, nr)       (((bank) - 1) * 32 + (nr))

int led[] = {
	IMX_GPIO_NR(1, 16),   //16
	IMX_GPIO_NR(1, 17),	  //17
	IMX_GPIO_NR(1, 18),   //18
	IMX_GPIO_NR(1, 19),   //19
};
static int led_init(void)						//초기화하는 함수
{
	int ret = 0;
	int i;

	for (i = 0; i < ARRAY_SIZE(led); i++) {		//배열의 크기가 리턴됨(매크로함수)
		ret = gpio_request(led[i], "gpio led"); //GPIO리퀘스트 함수 사용 -> GPIO번호를 넘김 (16번을 쓰겠다)
		if(ret<0){								//GPIO리퀘스트:누가 해당 GPIO사용중이면 FAIL 리턴함
			printk("#### FAILED Request gpio %d. error : %d \n", led[i], ret);
		} 										//항상 gpio request를 이용하여 초기화 해야함
	}											//for문 4회 반복하여 GPIO등록
	return ret;
}
static void led_exit(void)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(led); i++){
		gpio_free(led[i]);
	}
}

void led_write(unsigned long data)
{
	int i;
	for(i = 0; i < ARRAY_SIZE(led); i++){
		gpio_direction_output(led[i], (data >> i ) & 0x01);	//led[i] : output으로 설정
															//data >> i & 0x01 : write할 초기값을 정해줌
															//&와 시프트 연산 활용
//		gpio_set_value(led[i], (data >> i ) & 0x01);
	}
#if DEBUG
	printk("#### %s, data = %ld\n", __FUNCTION__, data);
#endif
}
void led_read(unsigned long * led_data)	//사실상 필요 없는 코드...
{
	int i;
	unsigned long data=0;
	unsigned long temp;
	for(i=0;i<4;i++)
	{
  		gpio_direction_input(led[i]); //error led all turn off
		temp = gpio_get_value(led[i]) << i; //direction이 input이 아니면 읽어올수가 없다											
		data |= temp;
	}	//4번 읽어서 만들어야해서 위와같이 read 함수를 작성
/*	
	for(i=3;i>=0;i--)
	{
  		gpio_direction_input(led[i]); //error led all turn off
		temp = gpio_get_value(led[i]);
		data |= temp;
		if(i==0)
			break;
		data <<= 1;  //data <<= 1;
	}
*/
#if DEBUG
	printk("#### %s, data = %ld\n", __FUNCTION__, data);
#endif
	*led_data = data;
	led_write(data);///
	return;
}
asmlinkage long sys_mysyscall(unsigned long led_data)
{
	led_init();
	led_write(led_data);
  	led_read(&led_data); ///막아도 됨
	led_exit();		//힘수가 빠져나갈때 gpio free를 해주어야 함(그렇지 않으면 또 request 과정에서 오류가 생김)
					//처음 한 번 ./syscall_app이 정상실행이지만 그 이후부터는 gpio할당 문제로 실행이 안됨(불이 안들어옴)
					//dmesg -> FAILED Request, gpio 에러코드 발현
	return (long)led_data;
}

static int hello_init(void) {

	printk("Hello module!\n");
	led_init();
	led_write(15);

	return 0;
}

static void hello_exit(void) {

	printk("goodbye world\n");
	led_write(0);
	led_exit();
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("ogsHi");
MODULE_DESCRIPTION("test module");
MODULE_LICENSE("Dual BSD/GPL");
