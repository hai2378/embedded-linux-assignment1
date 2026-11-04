#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/string.h>

#include "asgn1_ioctl.h"

#define DEVICE_NAME "asgn1"
#define DEVICE_MAJOR 241
#define BUFFER_SIZE 256

static char device_buffer[BUFFER_SIZE];
static int buffer_len;
static int open_count;
static int write_count;
static int read_count;
static int last_write_size;
static int device_mode = 1;

static DEFINE_MUTEX(device_mutex);

static int asgn1_open(struct inode *inode, struct file *file)
{
    mutex_lock(&device_mutex);
    open_count++;
    mutex_unlock(&device_mutex);

    pr_info("asgn1: device opened\n");
    return 0;
}

static int asgn1_release(struct inode *inode, struct file *file)
{
    pr_info("asgn1: device closed\n");
    return 0;
}
static ssize_t asgn1_read(struct file *file,
                          char __user *buf,
                          size_t count,
                          loff_t *ppos)
{
    ssize_t ret;

    if (mutex_lock_interruptible(&device_mutex))
        return -ERESTARTSYS;

    ret = simple_read_from_buffer(buf, count, ppos,
                                  device_buffer, buffer_len);

    if (ret > 0)
        read_count++;

    mutex_unlock(&device_mutex);

    return ret;
}

static ssize_t asgn1_write(struct file *file,
                           const char __user *buf,
                           size_t count,
                           loff_t *ppos)
{
    size_t to_copy;
    size_t prefix_len = 0;

    if (count == 0)
        return 0;

    if (mutex_lock_interruptible(&device_mutex))
        return -ERESTARTSYS;

    if (device_mode == 0) {
        prefix_len = strlen("[KERNEL]");
        memcpy(device_buffer, "[KERNEL]", prefix_len);
    }

    to_copy = min_t(size_t, count,
                    BUFFER_SIZE - prefix_len - 1);

    if (copy_from_user(device_buffer + prefix_len,
                       buf, to_copy)) {
        mutex_unlock(&device_mutex);
        return -EFAULT;
    }

    buffer_len = prefix_len + to_copy;
    device_buffer[buffer_len] = '\0';

    write_count++;
    last_write_size = to_copy;

    *ppos = 0;

    mutex_unlock(&device_mutex);

    return to_copy;
}
static long asgn1_ioctl(struct file *file,
                        unsigned int cmd,
                        unsigned long arg)
{
    struct asgn1_stats stats;
    int new_mode;
    int ret = 0;

    static const char version[ASGN1_VERSION_LEN] =
        "asgn1_driver v1.0";

    if (mutex_lock_interruptible(&device_mutex))
        return -ERESTARTSYS;

    switch (cmd) {

    case ASGN1_RESET_BUFFER:
        memset(device_buffer, 0, BUFFER_SIZE);
        buffer_len = 0;
        write_count = 0;
        read_count = 0;

        pr_info("asgn1: RESET_BUFFER\n");
        break;

    case ASGN1_GET_STATS:
        stats.open_count = open_count;
        stats.write_count = write_count;
        stats.read_count = read_count;
        stats.buffer_len = buffer_len;
        stats.last_write_size = last_write_size;

        if (copy_to_user((void __user *)arg,
                         &stats, sizeof(stats)))
            ret = -EFAULT;

        pr_info("asgn1: GET_STATS\n");
        break;

    case ASGN1_SET_MODE:
        if (copy_from_user(&new_mode,
                           (void __user *)arg,
                           sizeof(new_mode))) {
            ret = -EFAULT;
            break;
        }

        if (new_mode != 0 && new_mode != 1) {
            ret = -EINVAL;
            break;
        }

        device_mode = new_mode;

        pr_info("asgn1: SET_MODE = %d\n", device_mode);
        break;

    case ASGN1_GET_VERSION:
        if (copy_to_user((void __user *)arg,
                         version, sizeof(version)))
            ret = -EFAULT;

        pr_info("asgn1: GET_VERSION\n");
        break;

    default:
        pr_info("asgn1: UNKNOWN_IOCTL\n");
        ret = -ENOTTY;
        break;
    }

    mutex_unlock(&device_mutex);

    return ret;
}
static const struct file_operations asgn1_fops = {
    .owner = THIS_MODULE,
    .open = asgn1_open,
    .release = asgn1_release,
    .read = asgn1_read,
    .write = asgn1_write,
    .unlocked_ioctl = asgn1_ioctl,
};

static int __init asgn1_init(void)
{
    int ret;

    ret = register_chrdev(DEVICE_MAJOR,
                          DEVICE_NAME,
                          &asgn1_fops);

    if (ret < 0) {
        pr_err("asgn1: registration failed\n");
        return ret;
    }

    pr_info("asgn1: driver loaded, major=%d\n",
            DEVICE_MAJOR);

    return 0;
}

static void __exit asgn1_exit(void)
{
    unregister_chrdev(DEVICE_MAJOR, DEVICE_NAME);

    pr_info("asgn1: driver unloaded\n");
}

module_init(asgn1_init);
module_exit(asgn1_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("Assignment 01 ioctl character driver");
MODULE_VERSION("1.0");
