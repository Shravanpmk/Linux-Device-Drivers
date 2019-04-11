#include <linux/init.h>
//#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/switch_to.h> /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_from/to_user */
#include <linux/usb.h>

MODULE_LICENSE("Dual BSD/GPL");


/* Declaration of memory.c functions */
int memory_open(struct inode *inode, struct file *filp);
int memory_release(struct inode *inode, struct file *filp);
ssize_t memory_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t memory_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
void memory_exit(void);
int memory_init(void);



/* Global variables of the driver */
/* Major number */
int memory_major = 60;
/* Buffer to store data */
char *memory_buffer;


/*------------------------------------------------------------------------------------*/
/*To open the file and do associated operations for a particular Registed Char device*/
/*inode - sends info to kernal regarding Major number and Minor Number */
/*file structure - To determine the type of file operation*/
int memory_open(struct inode *inode, struct file *filp) {
        printk("in opeeeeeeeeeeeeeen");
        /* Success */
        return 0;
}

/*To release memory after the device is being closed */
/*inode -  refer before */
/*same file structure but to unallocate the space */
int memory_release(struct inode *inode, struct file *filp) {
        printk("in clooooooosseeeeeee");
         /* Success */
return 0;
}


/*To read a registered device */
/*a type file structure*/
/*buf - from which the user space function(fread) will read */
/*count - counter to count number of bytes*/
/*f_pos - the position to start reading*/
ssize_t memory_read(struct file *filp, char *buf,size_t count, loff_t *f_pos) {
        /* Transfering data to user space */
        raw_copy_to_user(buf,memory_buffer,1);
        printk("reaaaaaaadddddddd");
        /* Structure that declares the usual file */
        /* access functions */
        /* Changing reading position as best suits */
         if (*f_pos == 0) {
                 *f_pos+=1;
                return 1;
         }else {
                return 0;
         }
}

/*filp - file operations structure*/
/*buf - a a buffer in which the user space function will write*/
/*count - a counter to with the number of bytes to transfer*/
/*f_pos - postion on where to start reading the file*/
ssize_t memory_write(struct file *filp, const char *buf,size_t count, loff_t *f_pos) {
        const char *tmp=buf+count-1;
        raw_copy_from_user(memory_buffer,tmp,1);
        printk("in writeeeee");
        return 1;
}


/*-------------------------------------------------------------------------------------*/

struct file_operations memory_fops = {
        read: memory_read,
        write: memory_write,
        open: memory_open,
        release: memory_release
};


/***************************************************************************************/

static struct usb_driver skel_driver = {
        .name        = "skeleton",
        .probe       = skel_driver,
        .disconnect  = seq_list_next,
        .fops        = &skel_fops,
        .minor       = USB_SKEL_MINOR_BASE,
        .id_table    = skel_table,
};

/*------------------------------------------------------------------------------------*/

static int __init usb_skel_init(void)
{
        int result;

        /* register this driver with the USB subsystem */
        result = usb_register(&skel_driver);
        if (result < 0) {
                err("usb_register failed for the "__FILE__ "driver."
                    "Error number %d", result);
                return -1;
        }

        return 0;
}
module_init(usb_skel_init);


static void __exit usb_skel_exit(void)
{
        /* deregister this driver with the USB subsystem */
        usb_deregister(&skel_driver);
}
module_exit(usb_skel_exit);


/* table of devices that work with this driver */
static struct usb_device_id skel_table [] = {
        { USB_DEVICE(USB_SKEL_VENDOR_ID, USB_SKEL_PRODUCT_ID) },
        { }                      /* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, skel_table);


/*-------------------------------------------------------------------------------------*/

int memory_init(void) {
	//int result;
         /* Registering device */
 	//printk(KERN_INFO "Registerrrrrrrrrriinnnnnnnnggggggg");
        //result = register_chrdev(memory_major, "memory", &memory_fops);
        //printk(KERN_INFO "Registerrrrrreeeeeeeeddddddddd");
        /*If registering if failed , returns and proces  is terminated*/
        //if (result < 0) {
        //       printk("<1>memory: cannot obtain major number %d\n", memory_major);
        //        return result;
        //}


        /* Allocating memory for the buffer */
        memory_buffer = kmalloc(1, GFP_KERNEL);
        if (!memory_buffer) {
                result = -ENOMEM;
                goto fail;
        }
        memset(memory_buffer, 0, 1);
        printk("<1>Inserting memory module\n");
        return 0;

        fail:
                memory_exit();
                return result;
}


void memory_exit(void) {
        /* Freeing the major number */
        unregister_chrdev(memory_major, "memory");

        /* Freeing buffer memory */
        if (memory_buffer) {
                kfree(memory_buffer);
        }
        printk("<1>Removing memory module\n");
}

/* Declaration of the init and exit functions */
module_init(memory_init);
module_exit(memory_exit);


