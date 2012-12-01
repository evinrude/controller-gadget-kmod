/**
 *
 * USB Controller Gadget
 * Copyright (C) 2012 Mickey Malone	<mickey.malone at gmail.com>
 *
 * This is a kernel driver intended for a usb device that has 2 interrupt
 * endpoints, in and out. In my case it is a usb device that controls my
 * 6 sprinkler zones. It creates a character device that udev should
 * create for you. If it does not, then you can create it manually. The
 * major number is whatever is listed at usb_device under /proc/devices
 * and the minor is set in the module (31).
 *
 * Influenced by the Dream Cheeky USB missile launcher
 * Copyright (C) 2007 Matthias Vallentin <vallentin@icsi.berkeley.edu>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/ioctl.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>


/**
 * Static constants
 **/
#define VENDOR_ID                                       0x04d8
#define PRODUCT_ID                                      0x0042
#define TIMEOUT_MS                                      5000
#define STATUS                                          0x09
#define ASCII_STATUS                                    0x53

#define STATUS_OFF                                      0x00
#define OFF                                             0x10
#define ASCII_OFF                                       0x30
#define MESSAGE_OFF                                     "0"

#define STATUS_SPRINKLER_ZONE1                          0x01
#define SPRINKLER_ZONE1                                 0x11
#define ASCII_SPRINKLER_ZONE1                           0x31
#define MESSAGE_ZONE1                                   "1"

#define STATUS_SPRINKLER_ZONE2                          0x02
#define SPRINKLER_ZONE2                                 0x12
#define ASCII_SPRINKLER_ZONE2                           0x32
#define MESSAGE_ZONE2                                   "2"

#define STATUS_SPRINKLER_ZONE3                          0x04
#define SPRINKLER_ZONE3                                 0x13
#define ASCII_SPRINKLER_ZONE3                           0x33
#define MESSAGE_ZONE3                                   "3"

#define STATUS_SPRINKLER_ZONE4                          0x08
#define SPRINKLER_ZONE4                                 0x14
#define ASCII_SPRINKLER_ZONE4                           0x34
#define MESSAGE_ZONE4                                   "4"

#define STATUS_SPRINKLER_ZONE5                          0x10
#define SPRINKLER_ZONE5                                 0x15
#define ASCII_SPRINKLER_ZONE5                           0x35
#define MESSAGE_ZONE5                                   "5"

#define STATUS_SPRINKLER_ZONE6                          0x20
#define SPRINKLER_ZONE6                                 0x16
#define ASCII_SPRINKLER_ZONE6                           0x36
#define MESSAGE_ZONE6                                   "6"


#define SC_MINOR_BASE                                   31

/**
 * Helper macros
 **/
#define LOGGER_INFO(fmt, args ...) printk( KERN_INFO "[info]  %s(%d): " fmt, __FUNCTION__, __LINE__, ## args)
#define LOGGER_ERR(fmt, args ...) printk( KERN_ERR "[err]  %s(%d): " fmt, __FUNCTION__, __LINE__, ## args)
#define LOGGER_WARN(fmt, args ...) printk( KERN_ERR "[warn]  %s(%d): " fmt, __FUNCTION__, __LINE__, ## args)
#define LOGGER_DEBUG(fmt, args ...) if (debug == 1) { printk( KERN_DEBUG "[debug]  %s(%d): " fmt, __FUNCTION__, __LINE__, ## args); }

static int debug = 0;
module_param(debug, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(debug, "Set the log level to debug");

static int GLOBAL_havedata;
static int GLOBAL_open;

/**
 * Prototype Functions
 **/
static int sc_probe(struct usb_interface *interface, const struct usb_device_id *id);
static void sc_disconnect(struct usb_interface *interface);
static int __init usb_sc_init(void);
static void __exit usb_sc_exit(void);
static void sc_int_in_callback(struct urb *urb);
static int sc_open(struct inode *inode, struct file *file);
static int sc_release(struct inode *inode, struct file *file);
static ssize_t sc_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos);
static ssize_t sc_read(struct file *file, char __user *user, size_t count, loff_t *ppos);

/**
 * The main data structure used throuough this module
 **/
struct usb_sc {
	struct usb_device                      *udev;
	struct usb_interface                   *interface;
	int                                    timeout;
	char                                   status;
	struct semaphore                       sem;

	//Interrupt endpoint used for status
	char                                   *int_in_buffer;
	struct usb_endpoint_descriptor         *int_in_endpoint;
	struct urb                             *int_in_urb;

	//Interrupt endpoint used for sending commands
	char                                   *int_out_buffer;
	struct usb_endpoint_descriptor         *int_out_endpoint;
	struct urb                             *int_out_urb;
};

/**
 * Data structure of the USB vid:pid device that we will support
 **/
static struct usb_device_id sc_table [] = {
	{ USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
	{ }
};

/**
 * USB driver data structure
 **/
static struct usb_driver sc_driver = {
	.name           = "gadget",
	.id_table       = sc_table,
	.probe          = sc_probe,
	.disconnect     = sc_disconnect,
};

/**
 * File operations data structure for the character device
 * that will be implemented in this module
 **/
static struct file_operations sc_fops = {
	.owner          = THIS_MODULE,
	.open           = sc_open,
	.write          = sc_write,
	.read           = sc_read,
	.release        = sc_release,
};

/**
 * USB class data structure
 **/
 struct usb_class_driver sc_class = {
	.name           = "gadget",
	.fops           = &sc_fops,
	.minor_base     = SC_MINOR_BASE,
};

/**
 * The interrupt input callback
 * This method is called if the usb device sends any data through the in endpoint
 **/
static void sc_int_in_callback(struct urb *urb)
{
	struct usb_sc *dev = urb->context;
	LOGGER_DEBUG("Interrupt In [0x%X]\n", dev->int_in_buffer[0]);
	GLOBAL_havedata = 1;
}

/**
 * File ops:read
 * This method returns the gadget status contained in the dev->int_in_buffer[0]
 **/
static ssize_t sc_read(struct file *file, char __user *user, size_t count, loff_t *ppos)
{
	struct usb_sc        *dev;
	char                 *message;
	int                  retval = 0;

	LOGGER_DEBUG("Inside of sc_read");
        dev = file->private_data;
	if (!dev)
        {
		LOGGER_ERR("Missing usb_sc structure, cannot continue");
		retval = -ENODEV;
		goto nounlock;
	}

	if(!GLOBAL_open)
	{
		LOGGER_ERR("Illegal operation. An `open` operation is required to process a `read` operation.");
		retval = -EIO;
                goto nounlock;
	}

	if(!GLOBAL_havedata)
	{
		LOGGER_DEBUG("No data to be read from the interrupt in endpoint");
		retval = 0;
		goto nounlock;
	}

	down_interruptible(&dev->sem);
	LOGGER_DEBUG("Interrupt in endpoint to copy [0x%X]", dev->int_in_buffer[0]);
	switch(dev->int_in_buffer[0])
	{
	case STATUS_OFF:
		LOGGER_DEBUG("Setting status message to OFF");
		message = MESSAGE_OFF;
		break;
	case STATUS_SPRINKLER_ZONE1:
		LOGGER_DEBUG("Setting status message to ZONE1");
		message = MESSAGE_ZONE1;
		break;
	case STATUS_SPRINKLER_ZONE2:
		LOGGER_DEBUG("Setting status message to ZONE2");
		message = MESSAGE_ZONE2;
		break;
	case STATUS_SPRINKLER_ZONE3:
		LOGGER_DEBUG("Setting status message to ZONE3");
		message = MESSAGE_ZONE3;
		break;
	case STATUS_SPRINKLER_ZONE4:
		LOGGER_DEBUG("Setting status message to ZONE4");
		message = MESSAGE_ZONE4;
		break;
	case STATUS_SPRINKLER_ZONE5:
		LOGGER_DEBUG("Setting status message to ZONE5");
		message = MESSAGE_ZONE5;
		break;
	case STATUS_SPRINKLER_ZONE6:
		LOGGER_DEBUG("Setting status message to ZONE6");
		message = MESSAGE_ZONE6;
		break;
	default:
		LOGGER_ERR("An unknown value was returned from the end point [%c]", dev->int_out_buffer[0]);
		message = (char*) 0xFF;
		break;
	}

	//copy 1 byte to user land
	retval = copy_to_user(user, message, 1);
	if(retval)
	{
		LOGGER_ERR("copy_to_user had an error [%d]",retval);
		goto exit;
	}

exit:
	up(&dev->sem);
nounlock:
	if(GLOBAL_havedata && retval == 0)
	{
		//return 1 byte
		GLOBAL_havedata = 0;
		return 1;
	}
	else
	{
		//return nothing
		return retval;
	}
}

/**
 * File ops:write
 * This method carries out writing to the usb interrupt out endpoint
 *
 * The method returns the number of bytes written to the device. To keep userland
 * tools sane, return the count.
 **/
static ssize_t sc_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
	struct usb_sc        *dev;
	int                  retval = 0;
	int                  *actual = 0, i = 0;
	char                 cmd = '\0';

	LOGGER_DEBUG("Counted [%d] bytes from user land", (int)count);

        dev = file->private_data;
        if (!dev)
        {
                LOGGER_ERR("Missing usb_sc structure, cannot continue");
		retval = -ENODEV;
                goto nounlock;
        }

        if(!GLOBAL_open)
        {
                LOGGER_ERR("Illegal operation. An `open` operation is required to process a `write` operation.");
		retval = -EIO;
                goto nounlock;
        }

	down_interruptible(&dev->sem);

	//copy 1 byte from userland
	if (copy_from_user(&cmd, user_buf, 1))
	{
		LOGGER_ERR("copy_from_user failed\n");
		retval = -EFAULT;
		goto exit;
	}
	else
	{
		LOGGER_DEBUG("copied from user [0x%X]\n", cmd);
	}

	//set the command
	dev->int_out_buffer[0] = cmd;

	//Be user space friendly, see if we are using ascii [ S, 0, 1, 2, 3, 4, 5, 6 ]
	switch(dev->int_out_buffer[0])
	{
	case ASCII_STATUS:
		dev->int_out_buffer[0] = STATUS;
		break;
	case ASCII_OFF:
		dev->int_out_buffer[0] = OFF;
		break;
	case ASCII_SPRINKLER_ZONE1:
		dev->int_out_buffer[0] = SPRINKLER_ZONE1;
		break;
	case ASCII_SPRINKLER_ZONE2:
		dev->int_out_buffer[0] = SPRINKLER_ZONE2;
		break;
	case ASCII_SPRINKLER_ZONE3:
		dev->int_out_buffer[0] = SPRINKLER_ZONE3;
		break;
	case ASCII_SPRINKLER_ZONE4:
		dev->int_out_buffer[0] = SPRINKLER_ZONE4;
		break;
	case ASCII_SPRINKLER_ZONE5:
		dev->int_out_buffer[0] = SPRINKLER_ZONE5;
		break;
	case ASCII_SPRINKLER_ZONE6:
		dev->int_out_buffer[0] = SPRINKLER_ZONE6;
		break;
	}

	//fill the rest of the buffer with 0x0's
	for ( i = 1; i <= sizeof(dev->int_out_buffer -1); i++ ) {
		dev->int_out_buffer[i] = 0x00;
	}

	//only set the callback for status checks
	if(dev->int_out_buffer[0] == STATUS)
	{
		LOGGER_DEBUG("Filling the usb int urb");
		usb_fill_int_urb(dev->int_in_urb,
		                 dev->udev,
		                 usb_rcvintpipe(dev->udev, dev->int_in_endpoint->bEndpointAddress),
		                 dev->int_in_buffer,
		                 le16_to_cpu(dev->int_in_endpoint->wMaxPacketSize),
		                 sc_int_in_callback,
		                 dev,
		                 dev->int_in_endpoint->bInterval);

		retval = usb_submit_urb(dev->int_in_urb, GFP_KERNEL);
		if (retval)
		{
			LOGGER_ERR("submitting int urb failed [%d]", retval);
			goto exit;
		}
		else
		{
			LOGGER_DEBUG("Submitted the usb int urb successfully");
		}
	}

	//check for supported commands
	if ( dev->int_out_buffer[0] == STATUS || dev->int_out_buffer[0] == OFF || dev->int_out_buffer[0] == SPRINKLER_ZONE1 || dev->int_out_buffer[0] == SPRINKLER_ZONE2 || dev->int_out_buffer[0] == SPRINKLER_ZONE3 || dev->int_out_buffer[0] == SPRINKLER_ZONE4 || dev->int_out_buffer[0] == SPRINKLER_ZONE5 || dev->int_out_buffer[0] == SPRINKLER_ZONE6 )
	{
		//send the message
		retval = usb_interrupt_msg(dev->udev, usb_sndintpipe(dev->udev, dev->int_out_endpoint->bEndpointAddress), dev->int_out_buffer, sizeof(dev->int_out_buffer), actual, 5000);
		if (retval) {
			LOGGER_ERR("submitting usb_interrupt_msg failed [%d]", retval);
			goto exit;
		}

		LOGGER_DEBUG("Sent the usb interrupt message");
	}
	else
	{
		LOGGER_ERR("Unknown message in buffer [0x%2X]", dev->int_out_buffer[0]);
	}

exit:
	up(&dev->sem);
nounlock:
	if(retval == 0)
        {
		//return the number of bytes submitted from user space - passifier
		return count;
	}
	else
	{
		return retval;
	}
}

/**
 * File Ops:open
 * This method will locate and save the usb_sc saved earlier during the probing of the usb device in file->private_data
 **/
static int sc_open(struct inode *inode, struct file *file)
{
	struct usb_sc        *dev = NULL;
	struct usb_interface *interface;
	int                  subminor;
	int                  retval = 0;

	LOGGER_DEBUG("Inside sc_open\n");

	subminor = iminor(inode);
	interface = usb_find_interface(&sc_driver, subminor);
	if (!interface)
	{
		LOGGER_ERR("can't find device for minor %d", subminor);
		retval = -ENODEV;
		goto exit;
	}

	dev = usb_get_intfdata(interface);
	if (!dev)
	{
		retval = -ENODEV;
		LOGGER_ERR("dev is NULL in sc_open\n");
		goto exit;
	}

	GLOBAL_open = 1;
	file->private_data = dev;

exit:
	return retval;

}

/**
 * File Ops:release
 * Any cleanup that we have will be contained in this method
 **/
static int sc_release(struct inode *inode, struct file *file)
{
	struct usb_sc        *dev;

	LOGGER_DEBUG("Inside sc_release\n");

	/* not sure if there is another way around this. If the device becomes disconnected while a user has
         * the character device /dev/gadget open, then the next write/read op will fail. This flag is here so
         * so that the ops fail smoothly.
         */
	dev = file->private_data;

	LOGGER_DEBUG("Set the open flag to 0\n");
	GLOBAL_open = 0;
	return 0;
}

/**
 * USB: Probe
 * This method will be called if the device we plug in matches the vid:pid we are listening for
 **/
static int sc_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	struct usb_device              *udev = interface_to_usbdev(interface);  //grab the usb_device from the interface
	struct usb_sc                  *dev = NULL;                             //our data structure
	struct usb_host_interface      *iface_desc;                             // interfaces of the usb device
	struct usb_endpoint_descriptor *endpoint;                               // endpoint decriptors
	int                            i;
	int                            int_end_size;
	int                            retval = -ENODEV;

	//initialize the data flag
	GLOBAL_havedata = 0;

	// Make sure we have something to work with
	if (!udev)
	{
		LOGGER_ERR("udev is NULL\n");
		goto exit;
	}

	// malloc the usb_sc structure
	dev = kzalloc(sizeof(struct usb_sc), GFP_KERNEL);
	if (!dev)
	{
		LOGGER_ERR("Cannot allocate memory for struct usb_sc\n");
		retval = -ENOMEM;
		goto exit;
	}

	GLOBAL_open = 0;
	dev->udev = udev;
	dev->interface = interface;
	iface_desc = interface->cur_altsetting;

	/// Set up interrupt end points
	for (i = 0; i < iface_desc->desc.bNumEndpoints; ++i)
	{
		endpoint = &iface_desc->endpoint[i].desc;

		//printk("Looping on bEndpointAddress: [%X]\n", endpoint->bEndpointAddress);

		if (((endpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN) && ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_INT))
		{
			dev->int_in_endpoint = endpoint;
			//printk("set the int_in_endpoint\n");
		}

		if (((endpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_OUT) && ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_INT))
		{
			dev->int_out_endpoint = endpoint;
			//printk("set the int_out_endpoint\n");
		}
	}

	if (!dev->int_out_endpoint)
	{
		LOGGER_ERR("could not find interrupt out endpoint\n");
		goto error;
	}

	if (!dev->int_in_endpoint)
	{
		LOGGER_ERR("could not find interrupt in endpoint\n");
		goto error;
	}

	int_end_size = le16_to_cpu(dev->int_out_endpoint->wMaxPacketSize);
	dev->int_out_buffer = kmalloc(int_end_size, GFP_KERNEL);

	int_end_size = le16_to_cpu(dev->int_in_endpoint->wMaxPacketSize);
	dev->int_in_buffer = kmalloc(int_end_size, GFP_KERNEL);

	dev->int_out_urb = usb_alloc_urb(0, GFP_KERNEL);
	if (!dev->int_out_urb)
	{
		LOGGER_ERR("could not allocate int_out_urb\n");
		retval = -ENOMEM;
		goto error;
	}

	dev->int_in_urb = usb_alloc_urb(0, GFP_KERNEL);
	if (!dev->int_in_urb)
	{
		LOGGER_ERR("could not allocate int_in_urb\n");
		retval = -ENOMEM;
		goto error;
	}

	dev->timeout = TIMEOUT_MS;

	retval = usb_register_dev(interface, &sc_class);
	if (retval)
	{
		LOGGER_ERR("not able to get a minor for this device\n");
		usb_set_intfdata(interface, NULL);
		goto error;
	}

	usb_set_intfdata(interface, dev);

	sema_init(&dev->sem,1);

	/**
	 * A little test for the impatient
	 *
	 * int *actual = 0;
	 * dev->int_out_buffer[0] = OFF;
	 * for ( i = 1;i <= sizeof(dev->int_out_buffer -1);i++ ) {
	 *		dev->int_out_buffer[i] = 0x00;
	 * }
	 * retval = usb_interrupt_msg(dev->udev, usb_sndintpipe(dev->udev, dev->int_out_endpoint->bEndpointAddress), dev->int_out_buffer, sizeof(dev->int_out_buffer), actual, 5000);
	 * printk("retval [%d]\n", retval);
	 **/

exit:
	LOGGER_INFO("Sprinkler controller was initialized successfully\n");
	return retval;

error:
	LOGGER_ERR("An error occured while initializing the device\n");
	if (dev->int_in_urb)
	{
		usb_free_urb(dev->int_in_urb);
	}
	if (dev->int_out_urb)
	{
		usb_free_urb(dev->int_out_urb);
	}

	kfree(dev->int_in_buffer);
	kfree(dev->int_out_buffer);
	kfree(dev);
	return retval;
}


/**
 * USB:disconnect
 * This method is used to cleanup everything after the device is disconnected
 **/
static void sc_disconnect(struct usb_interface *interface)
{
	struct usb_sc *dev;
	dev = usb_get_intfdata(interface);
	GLOBAL_open = 0;
	
        usb_set_intfdata(interface, NULL);
	usb_deregister_dev(interface, &sc_class);

	if (dev->int_in_urb)
	{
		usb_free_urb(dev->int_in_urb);
	}
	if (dev->int_out_urb)
	{
		usb_free_urb(dev->int_out_urb);
	}

	kfree(dev->int_in_buffer);
	kfree(dev->int_out_buffer);
	kfree(dev);

	LOGGER_INFO("USB gadget controller now disconnected\n");
}

/**
 * Kernel module init
 **/
static int __init usb_sc_init(void)
{
	int result;

	LOGGER_DEBUG("Logging level is set to debug");

	result = usb_register(&sc_driver);
	if (result)
	{
		LOGGER_ERR("registering driver failed\n");
	}
	else
	{
		LOGGER_INFO("driver registered successfully\n");
	}

	return result;
}

/**
 * Kernel module exit
 **/
static void __exit usb_sc_exit(void)
{
	usb_deregister(&sc_driver);
	LOGGER_INFO("module deregistered\n");
}


MODULE_DEVICE_TABLE(usb, sc_table);
module_init(usb_sc_init);
module_exit(usb_sc_exit);

MODULE_AUTHOR("Mickey Malone");
MODULE_LICENSE("GPL");
