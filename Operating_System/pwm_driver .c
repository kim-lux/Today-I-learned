#include <linux/module.h>
#include <linux/init.h> /* module_init, exit을 위해 필요함 */
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/pwm.h> /* pwm header 파일 */

#define DRIVER_NAME "k_pwm_driver"
#define DRIVER_CLASS "pwm_Class"

/* device 변수 */
static dev_t dev;
static struct class *dev_class;
static struct cdev motor_cdev;

static int __init motor_driver_init(void);
static void __exit motor_driver_exit(void); 

static int driver_open(struct inode *device_file, struct file *instance);
static int driver_close(struct inode *device_file, struct file *instance);

static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs);

/* pwm 변수 선언  */
struct pwm_device *pwm0 = NULL; /* pwm 초기화 */ 
u32 pwm_on_time = 50000;

/* Flie operation structure */
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.write = driver_write
};

/* device file이 열렸을 때 함수가 호출됨 */
static int driver_open(struct inode *device_file, struct file *instance) 
{
	pr_info(" Device File Opened \n");
	return 0;
}

/* device file이 닫혔 때 함수가 호출됨 */
static int driver_close(struct inode *device_file, struct file *instance) 
{
	pr_info("Device Driver Remove...Done!!\n");
	return 0;
}

/* device file을  때 함수가 호출됨 */
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	int cop, ncop, num;
	char value;

	/* data를 받아옴 */
	cop = min(count, sizeof(value));

	ncop = copy_from_user(&value, user_buffer, cop);

	/* PWM 시간 setting */
	if(value < 'a' || value > 'j')
		pr_info("Invalid Value\n");
	else
		pwm_config(pwm0, 10000 * (value - 'a'), 100000);
	
	num = cop - ncop;

	return num;
}

/* Module이 Kernel에 초기화될 때 호출되는 함수 */
static int __init motor_driver_init(void) {
	pr_info("Hello, Kernel!\n");

	/* Allocating major number */
	if( alloc_chrdev_region(&dev, 0, 1, DRIVER_NAME) < 0) {
		pr_err("Cannot allocate major number \n");
		return -1;
	}
	pr_info(" Major: %d, Minor: %d \n", dev >> 20, dev && 0xfffff);

	/* Creating struct class */
	if((dev_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
		pr_err("Cannot create the struct class\n");
		goto r_class;
	}

	/* Creating device */
	if(device_create(dev_class, NULL, dev, NULL, DRIVER_NAME) == NULL) {
		pr_err("Can not create device file!\n");
		goto r_device;
	}

	/* Creating cdev structure */
	cdev_init(&motor_cdev, &fops);

	/* Adding character device to the system */
	if(cdev_add(&motor_cdev, dev, 1) == -1) {
		pr_err("Cannot add the device to the sysytem\n");
		goto r_del;
	}

	pwm0 = pwm_request(0, "my-pwm");
	if(pwm0 == NULL) {
		pr_info("Could not get PWM0!\n");
		goto r_del;
	}

	pwm_config(pwm0, pwm_on_time, 100000); /* 100,000 = 0,01ms */
	pwm_enable(pwm0); /* pwm 활성화 */

	return 0;
r_del:
	device_destroy(dev_class, dev);
r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev, 1);
	return -1;
}

/* Module이 Kernel에서 제거될 때 호출되는 함수 */
static void __exit motor_driver_exit(void) {
	pwm_disable(pwm0);
	pwm_free(pwm0);
	cdev_del(&motor_cdev);
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	unregister_chrdev_region(dev, 1);
	pr_info("Device Driver Remove...Done!!\n");
}

module_init(motor_driver_init); /* insmod시에 module_init()에 등록한 초기화 함수가 자동으로 호출되도록 설정 */
module_exit(motor_driver_exit); /* rmmod시에 module_exit()에 등록한 초기화 함수가 자동으로 호출되도록 설정 */

/* 개발자 정보 : 있어야 실행 됨. */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kim Tae Kyoung");
MODULE_DESCRIPTION("My first Device Driver");

