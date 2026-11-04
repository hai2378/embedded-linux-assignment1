#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/atomic.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/string.h>

#define DEVICE_NAME "counter"
#define PROC_NAME "counter_info"

static atomic_t counter = ATOMIC_INIT(0);
static atomic_t total_reads = ATOMIC_INIT(0);
static atomic_t total_writes = ATOMIC_INIT(0);

static struct proc_dir_entry *proc_entry;

/* Read counter and increment automatically */
static ssize_t counter_read(struct file *file,
                            char __user *buf,
                            size_t count,
                            loff_t *ppos)
{
    char temp[32];
    int value;
    int len;
    ssize_t ret;

    if (*ppos != 0)
        return 0;

    value = atomic_fetch_inc(&counter);

    len = scnprintf(temp, sizeof(temp),
                    "%d\n", value);

    ret = simple_read_from_buffer(buf, count, ppos,
                                  temp, len);

    if (ret > 0)
        atomic_inc(&total_reads);

    return ret;
}

/* Write a new counter value */
static ssize_t counter_write(struct file *file,
                             const char __user *buf,
                             size_t count,
                             loff_t *ppos)
{
    char temp[32];
    int value;

    if (count == 0 || count >= sizeof(temp))
        return -EINVAL;

    if (copy_from_user(temp, buf, count))
        return -EFAULT;

    temp[count] = '\0';

    if (kstrtoint(temp, 10, &value) != 0)
        return -EINVAL;

    if (value < 0)
        return -EINVAL;

    atomic_set(&counter, value);
    atomic_inc(&total_writes);

    pr_info("counter: set value = %d\n", value);

    return count;
}

/* Display counter statistics via procfs */
static int counter_proc_show(struct seq_file *m,
                             void *v)
{
    seq_printf(m, "Current counter: %d\n",
               atomic_read(&counter));

    seq_printf(m, "Total reads: %d\n",
               atomic_read(&total_reads));

    seq_printf(m, "Total writes: %d\n",
               atomic_read(&total_writes));

    return 0;
}

static int counter_proc_open(struct inode *inode,
                             struct file *file)
{
    return single_open(file, counter_proc_show, NULL);
}

static const struct proc_ops counter_proc_ops = {
    .proc_open = counter_proc_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static const struct file_operations counter_fops = {
    .owner = THIS_MODULE,
    .read = counter_read,
    .write = counter_write,
    .llseek = no_llseek,
};

static struct miscdevice counter_misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &counter_fops,
    .mode = 0666,
};

/* Initialize counter driver */
static int __init counter_init(void)
{
    int ret;

    ret = misc_register(&counter_misc);

    if (ret) {
        pr_err("counter: misc_register failed\n");
        return ret;
    }

    proc_entry = proc_create(PROC_NAME, 0444,
                             NULL, &counter_proc_ops);

    if (!proc_entry) {
        misc_deregister(&counter_misc);
        return -ENOMEM;
    }

    pr_info("counter: driver loaded\n");

    return 0;
}

/* Cleanup driver */
static void __exit counter_exit(void)
{
    proc_remove(proc_entry);
    misc_deregister(&counter_misc);

    pr_info("counter: driver unloaded\n");
}

module_init(counter_init);
module_exit(counter_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("Assignment 01 Counter Misc Device Driver");
MODULE_VERSION("1.0");
