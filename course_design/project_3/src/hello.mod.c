#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0x50a74865, "module_layout" },
	{ 0x8d1c3e54, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x21f8f1fa, "cdev_add" },
	{ 0x310293be, "cdev_init" },
	{ 0x3fd78f3b, "register_chrdev_region" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0xfb578fc5, "memset" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "F4A091E645F4F0A568A8F32");