#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/notifier.h>
#include <linux/err.h>

#include <plat/mailbox.h>

MODULE_AUTHOR("Kevin Hilman");
MODULE_LICENSE("GPL");

/* load-time options */
int debug = 0;
int count = 16;
char *name = "dsp";

module_param(debug, int, 0);
module_param(count, int, 0);
module_param(name, charp, 0);

static int callback(struct notifier_block *nb, unsigned long len, void *v)
{
	int ret = 0;
	mbox_msg_t msg = (mbox_msg_t)v;

	if (len != sizeof(mbox_msg_t)) {
		printk("%s: invalid message size: %lu\n", __func__, len);
		ret = -EINVAL;
	}

	printk("%s: msg=%u\n", __func__, msg);

	return ret;
}

struct notifier_block nb = {
	.notifier_call = callback,
};

static struct omap_mbox *mbox;

static void __exit mytest_cleanup(void) 
{
	if (mbox)
		omap_mbox_put(mbox, &nb);
}

static int __init mytest_init(void) 
{
	int i, r, ret = 0;

	mbox = omap_mbox_get(name, &nb);
	if (IS_ERR(mbox)) {
		printk("%s: omap_mbox_get() failed: 0x%p\n", __func__, mbox);
		mbox = NULL;
		ret = -EINVAL;
		goto out;
	}

	for (i = 0; i < count; i++) {
		mbox_msg_t msg = i;

		r = omap_mbox_msg_send(mbox, msg);
		if (r) {
			printk("%s: omap_mbox_msg_send() failed: %d\n",
			       __func__, r);
			ret = r;
			break;
		}
	}

out:
	return ret;
}

module_init(mytest_init);
module_exit(mytest_cleanup);
