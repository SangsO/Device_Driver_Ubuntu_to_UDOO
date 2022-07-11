#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>

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
    int result;
    printk("call call_intit \n");
    result = register_chrdev(CALL_DEV_MAJOR, CALL_DEV_NAME, &call_fops);
    //마지막 인자값으로 file operation의 address를 입력
    if(result < 0) return result;
    return 0;
}
static void call_exit(void)
{
    printk("call call_exit \n");
    unregister_chrdev(CALL_DEV_MAJOR, CALL_DEV_NAME);
    //여기 이 위의 줄을 막으면 재부팅 시 실행이 되지 않는다
}
module_init(call_init);
module_exit(call_exit);
MODULE_LICENSE("Dual BSD/GPL");
