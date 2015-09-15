#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");

int init_module(void) {
  printk(KERN_INFO ">>> Test module starting");
  return 0;
}

void cleanup_module(void) {
  printk(KERN_INFO "<<< Test module stopping");
}
