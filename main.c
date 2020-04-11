#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

static int dev_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    printk(KERN_INFO "SIS1 Dev drive (%04X:%04X) plugged\n", id->idVendor, id->idProduct);
    return 0;
}

static void dev_disconnect(struct usb_interface *interface)
{
    printk(KERN_INFO "SIS1 Dev drive removed\n");
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
    .name = "SIS1_dev_driver",
    .id_table = dev_table,
    .probe = dev_probe,
    .disconnect = dev_disconnect,
};

static int __init dev_init(void)
{
    return usb_register(&dev_driver);
}
 
static void __exit dev_exit(void)
{
    usb_deregister(&dev_driver);
}


module_init(dev_init);
module_exit(dev_exit);

MODULE_LICENSE("GPL");
