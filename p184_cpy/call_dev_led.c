#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>

#include <linux/gpio.h>
#define DEBUG 1
#define IMX_GPIO_NR(bank, nr)       (((bank) - 1) * 32 + (nr))

int led[] = {
    IMX_GPIO_NR(1, 16),   //16
    IMX_GPIO_NR(1, 17),   //17
    IMX_GPIO_NR(1, 18),   //18
    IMX_GPIO_NR(1, 19),   //19
};
static int led_init(void)                       //초기화하는 함수
{
    int ret = 0;
    int i;

    for (i = 0; i < ARRAY_SIZE(led); i++) {     //배열의 크기가 리턴됨(매크로함수)
        ret = gpio_request(led[i], "gpio led"); //GPIO리퀘스트 함수 사용 -> GPIO번호를 넘김 (16번을 쓰겠다)
        if(ret<0){                              //GPIO리퀘스트:누가 해당 GPIO사용중이면 FAIL 리턴함
            printk("#### FAILED Request gpio %d. error : %d \n", led[i], ret);
        }                                       //항상 gpio request를 이용하여 초기화 해야함
    }                                           //for문 4회 반복하여 GPIO등록
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
        gpio_direction_output(led[i], (data >> i ) & 0x01); //led[i] : output으로 설정
                                                            //&와 시프트 연산 활용
    }
}
//void led_read(unsigned long led_data)
void led_read(char * led_data)
{
    int i;
    unsigned long data=0;
    unsigned long temp;
    for(i=0;i<4;i++)
    {
        gpio_direction_input(led[i]); //error led all turn off
        temp = gpio_get_value(led[i]) << i; //direction이 input이 아니면 읽어올수가 없다                 
        data |= temp;
    }   //4번 읽어서 만들어야해서 위와같이 read 함수를 작성
	
   /*for(i=3;i>=0;i--)
    {
        gpio_direction_input(led[i]); //error led all turn off
        temp = gpio_get_value(led[i]);
        data |= temp;
        if(i==0)
            break;
        data <<= 1;  //data <<= 1;
   }*/
}
//***************** p184code ************************//

#define CALL_DEV_NAME "calldev"
#define CALL_DEV_MAJOR 240
static int call_open(struct inode *inode, struct file *filp)
{
    int num = MINOR(inode->i_rdev);
    //매크로함수
    printk("Call opne-> minor : %d\n", num);
    num = MAJOR(inode->i_rdev);
    //#define MAJOR(dev)	((unsigned int) ((dev) >> MINORBITS))
    //이러한 원형을 가지고 있음
    printk("Call open-> major : %d\n", num);
	led_init(); //장치박히면 led init
    return 0;
}
static loff_t call_llseek(struct file *flip, loff_t off, int whence)
{
    printk("call llseek -> off : %08X, whence : %08X\n", (unsigned int)off, whence);
    return 0x23;
}
static ssize_t call_read(struct file *flip, char *buf, size_t count, loff_t *f_pos)
{
    printk("call read -> buf : %08X, count : %08X\n", (unsigned int)buf, count);
    return 0x33;
}
static ssize_t call_write(struct file *flip, const char *buf, size_t count, loff_t *f_pos)
{	
	led_write(*buf);
    printk("call write -> buf : %08X, count : %08X\n", (unsigned int)buf, count);
    return 0x43;
}
static long call_ioctl(struct file *flip, unsigned int cmd, unsigned long arg)
{
    printk("call ioctl -> cmd : %08X, arg : %08X\n", cmd, (unsigned int)arg);
    return 0x53;
}
static int call_release(struct inode *inode, struct file *flip)
{
    printk("call release \n");
	led_exit();
    return 0;
}
struct file_operations call_fops =
//file operation 이라는 구조체 선언
{
    .owner = THIS_MODULE,
    .llseek = call_llseek,
    .read = call_read,
    .write = call_write,
    .unlocked_ioctl = call_ioctl,
    .open = call_open,
    .release = call_release,
};
static int call_init(void)
{
    led_init();
    led_write(15);
    int result;
    printk("call call_intit \n");
    result = register_chrdev(CALL_DEV_MAJOR, CALL_DEV_NAME, &call_fops);
    //마지막 인자값으로 file operation의 address를 입력
    if(result < 0) return result;
    return 0;
}
static void call_exit(void)
{
    led_write(0);
    led_exit();
    printk("call call_exit \n");
    unregister_chrdev(CALL_DEV_MAJOR, CALL_DEV_NAME);
    //여기 이 위의 줄을 막으면 재부팅 시 실행이 되지 않는다
}
module_init(call_init);
module_exit(call_exit);
MODULE_LICENSE("Dual BSD/GPL");
