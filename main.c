#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/ktime.h>
#include <linux/timer.h>


#define TIMER_TIMEOUT 1
static struct timer_list mytimer;
struct timespec tv;

void print_current_time(void) {
   getnstimeofday(&tv);
   long long hours = (tv.tv_sec/3600 + 6)%24;
   long long minutes = (tv.tv_sec/60)%60;
   long long seconds = tv.tv_sec%60; 
   pr_info("Current time: %lld:%lld:%lld", hours, minutes, seconds);
}

static void timer_handler(struct timer_list *t1) {
   static size_t nseconds;
   nseconds += TIMER_TIMEOUT;
   pr_info("timer is on: nseconds = %d\n", nseconds);
   print_current_time();
   mod_timer(t1, jiffies + TIMER_TIMEOUT * HZ);
}


static int dev_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    printk(KERN_INFO "SIS2 Dev drive (%04X:%04X) plugged\n", id->idVendor, id->idProduct);
    return 0;
}

static void dev_disconnect(struct usb_interface *interface)
{
    printk(KERN_INFO "SIS2 Dev drive removed\n");
}


//For example: Vendor ID - 0XABCD, Product ID - 0x1234;
//To check some devices - https://www.the-sz.com/products/usbid/index.php?v=ABCD&p=1234&n=

static struct usb_device_id dev_table[] =
{
    { USB_DEVICE(0xABCD, 0x1234) },
    {}  
};

MODULE_DEVICE_TABLE (usb, dev_table);

static struct usb_driver dev_driver =
{
    .name = "SIS2_dev_driver",
    .id_table = dev_table,
    .probe = dev_probe,
    .disconnect = dev_disconnect,
};

static int __init dev_init(void)
{
    timer_setup(&mytimer, timer_handler, 0);
    mod_timer(&mytimer, jiffies + TIMER_TIMEOUT * HZ);
    return usb_register(&dev_driver);
}
 
static void __exit dev_exit(void)
{
    usb_deregister(&dev_driver);
    del_timer(&mytimer);
    pr_info("timer is off");
}


module_init(dev_init);
module_exit(dev_exit);

MODULE_LICENSE("GPL");
