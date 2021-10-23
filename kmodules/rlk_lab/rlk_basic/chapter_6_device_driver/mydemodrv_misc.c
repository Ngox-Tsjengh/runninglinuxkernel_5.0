#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/device.h>
#include <linux/slab.h>

#define DEMO_NAME "my_demo_dev"
static struct device *mydemodrv_device;

static char * device_buffer;
#define MAX_DEVICE_BUFFER_SIZE 64

static int demodrv_open(struct inode *inode, struct file *file)
{
	int major = MAJOR(inode->i_rdev);
	int minor = MINOR(inode->i_rdev);

	printk("%s: major=%d, minor=%d\n", __func__, major, minor);

	return 0;
}

static int demodrv_release(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t
demodrv_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	int max_free = MAX_DEVICE_BUFFER_SIZE - *ppos;
	int need_read = max_free > count ? count : max_free;
	int ret;
	int actual_readen;
	if (need_read == 0) {
		dev_warn(mydemodrv_device, "No space for read"); }

	ret	= copy_to_user(buf, device_buffer + *ppos, need_read);
	if (ret == need_read) {
		return -EFAULT; }

	actual_readen	= need_read - ret;
	*ppos += actual_readen;

	printk("%s, need_read=%d, actual_readen=%d, pos=%lld\n", __func__,need_read, actual_readen, *ppos);
	return actual_readen;
}

static ssize_t
demodrv_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	int free = MAX_DEVICE_BUFFER_SIZE - *ppos;
	int need_written = count > free ? free : count;
	int ret;
	int actual_written;
	if (need_written == 0) {
		dev_warn(mydemodrv_device, "No space to write"); }


	ret	= copy_from_user(device_buffer + *ppos, buf, need_written);
	if (ret == need_written) {
		return -EFAULT; }

	actual_written	= need_written - ret;
	*ppos += actual_written;
	printk("%s, need_written=%d, actual_written=%d, pos=%lld\n", __func__,need_written, actual_written, *ppos);

	return actual_written;
}

static const struct file_operations demodrv_fops = {
	.owner = THIS_MODULE,
	.open = demodrv_open,
	.release = demodrv_release,
	.read = demodrv_read,
	.write = demodrv_write
};

static struct miscdevice mydemodrv_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEMO_NAME,
	.fops = &demodrv_fops,
};

static int __init simple_char_init(void)
{
	int ret;

	device_buffer = kmalloc(MAX_DEVICE_BUFFER_SIZE, GFP_KERNEL);
	if (!device_buffer)
		return -ENOMEM;

	ret = misc_register(&mydemodrv_misc_device);
	if (ret) {
		printk("failed register misc device\n");
		kfree(device_buffer);
		return ret;
	}

	mydemodrv_device = mydemodrv_misc_device.this_device;

	printk("succeeded register char device: %s\n", DEMO_NAME);

	return 0;
}

static void __exit simple_char_exit(void)
{
	printk("removing device\n");

	kfree(device_buffer);
	misc_deregister(&mydemodrv_misc_device);
}

module_init(simple_char_init);
module_exit(simple_char_exit);

MODULE_AUTHOR("Benshushu");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("simpe character device");
