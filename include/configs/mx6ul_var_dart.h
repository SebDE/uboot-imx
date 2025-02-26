/*
 * Copyright (C) 2015 Freescale Semiconductor, Inc.
 *
 * Copyright (C) 2015-2019 Variscite Ltd.
 *
 * Configuration settings for Variscite i.MX6UL/L DART-6UL board family.
 *
 * SPDX-License-Identifier: GPL-2.0+
 */
#ifndef __MX6UL_VAR_DART_H
#define __MX6UL_VAR_DART_H

#include "mx6_common.h"

/* DCDC used on DART-6UL, no PMIC */
#undef CONFIG_LDO_BYPASS_CHECK

#ifdef CONFIG_SPL
#include "imx6_spl.h"
#endif

/* Support 128MB RAM */
#undef CONFIG_SPL_BSS_START_ADDR
#undef CONFIG_SPL_BSS_MAX_SIZE
#undef CONFIG_SYS_SPL_MALLOC_START
#undef CONFIG_SYS_SPL_MALLOC_SIZE

#define CONFIG_SPL_BSS_START_ADDR	0x87000000
#define CONFIG_SPL_BSS_MAX_SIZE		0x100000	/* 1 MB */
#define CONFIG_SYS_SPL_MALLOC_START	0x87100000
#define CONFIG_SYS_SPL_MALLOC_SIZE	0x100000	/* 1 MB */

#undef CONFIG_LOADADDR
#define CONFIG_LOADADDR			0x82000000

/* No need to instantiate the CAAM RNG in U-Boot */
#undef CONFIG_IMX_SEC_INIT

#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(16 * SZ_1M)

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE		UART1_BASE

/* MMC Configs */
#ifdef CONFIG_FSL_USDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR	USDHC2_BASE_ADDR

/* I2C configs */
#ifndef CONFIG_DM_I2C
#define CONFIG_SYS_I2C
#endif
#ifdef CONFIG_CMD_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_MXC_I2C1		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_MXC_I2C2		/* enable I2C bus 2 */
#define CONFIG_SYS_I2C_SPEED		100000
#endif

#define NAND_BOOT_ENV_SETTINGS \
	"nandargs=setenv bootargs console=${console},${baudrate} " \
		"ubi.mtd=4 root=ubi0:rootfs rootfstype=ubifs rw ${cma_size}\0" \
	"nandboot=echo Booting from nand ...; " \
		"run nandargs; " \
		"run optargs; " \
		"nand read ${loadaddr} 0x600000 0x7e0000; " \
		"nand read ${fdt_addr} 0xde0000 0x20000; " \
		"bootz ${loadaddr} - ${fdt_addr}\0" \
	"mtdids=" MTDIDS_DEFAULT "\0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0"

#define MMC_BOOT_ENV_SETTINGS \
	"mmcdev="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0" \
	"bootdir=/boot\0" \
	"mount=ro\0" \
	"bootpart=1\0" \
	"ustate=0\0 " \
	"mmcblk=0\0" \
	"mmcautodetect=yes\0" \
	"defaultEnv=1\0" \
	"wifiEnable=0\0" \
	"getBootDev= " \
		"if test ${boot_dev} = sd; then " \
			"setenv mmcdev 0; " \
			"setenv mount rw; " \
			"setenv ustate 0; " \
			"setenv wifiEnable 0; " \
			"setenv fdt_file recovery.dtb; " \
		"else " \
			"setenv mmcdev 1; " \
			"if itest ${wifiEnable} == 1; then " \
				"setenv fdt_file runWifi.dtb; " \
			"else " \
				"setenv fdt_file run.dtb; " \
			"fi; " \
		"fi;\0" \
	"checkDefaultEnv= " \
		"if itest ${defaultEnv} == 1; then " \
			"setenv defaultEnv 0; " \
			"saveenv; " \
		"fi;\0" \
	"updateTry=0\0" \
	"checkUpdate= " \
		"if itest ${updateTry} == 1; then " \
			"setenv updateTry 0; " \
			"setenv ustate 0; " \
			"saveenv; " \
		"fi;" \
		"if itest ${ustate} == 1; then " \
			"setenv updateTry 1; " \
			"saveenv; " \
			"if itest ${bootpart} == 1; then " \
				"setenv bootpart 2; " \
			"else if itest ${bootpart} == 2; then " \
				"setenv bootpart 1; " \
			"fi; fi;" \
		"fi;\0" \
	"mmcargs=setenv bootargs console=${console},${baudrate} " \
		"root=/dev/mmcblk${mmcdev}p${bootpart} rootwait ${mount} panic=5 logo.nologo " \
		"${cma_size}\0" \
	"loadbootenv=" \
		"load mmc ${mmcdev}:${bootpart} ${loadaddr} ${bootdir}/${bootenv}\0" \
	"importbootenv=echo Importing bootenv from mmc ...; " \
		"env import -t ${loadaddr} ${filesize}\0" \
	"loadbootscript=" \
		"load mmc ${mmcdev}:${bootpart} ${loadaddr} ${bootdir}/${script};\0" \
	"bootscript=echo Running bootscript from mmc ...; " \
		"source\0" \
	"loadimage=load mmc ${mmcdev}:${bootpart} ${loadaddr} ${bootdir}/${image}\0" \
	"loadfdt=run findfdt; " \
		"echo fdt_file=${fdt_file}; " \
		"load mmc ${mmcdev}:${bootpart} ${fdt_addr} ${bootdir}/${fdt_file}\0" \
	"mmcboot=echo Booting from mmc ...; " \
		"run mmcargs; " \
		"run optargs; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then " \
				"bootz ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi\0" \


#ifdef CONFIG_NAND_BOOT
#define BOOT_ENV_SETTINGS	NAND_BOOT_ENV_SETTINGS
#define CONFIG_BOOTCOMMAND \
	"run ramsize_check; " \
	"run nandboot || " \
	"run netboot"

#else
#define BOOT_ENV_SETTINGS	MMC_BOOT_ENV_SETTINGS
#define CONFIG_BOOTCOMMAND \
	"run ramsize_check; " \
	"run checkDefaultEnv;" \
	"run getBootDev; " \
	"mmc dev ${mmcdev};" \
	"mmc dev ${mmcdev}; if mmc rescan; then " \
		"if itest ${mmcdev} == 1; then " \
			"run checkUpdate; " \
		"fi; " \
		"if run loadbootenv; then " \
			"run importbootenv; " \
		"fi; " \
		"if run loadbootscript; then " \
			"run bootscript; " \
		"else " \
			"if run loadimage; then " \
				"run mmcboot; " \
			"else echo ERROR: No valid image; " \
			"fi; " \
		"fi; " \
	"else echo ERROR: No valid image; fi"

#endif

#define OPT_ENV_SETTINGS \
	"optargs=setenv bootargs ${bootargs} ${kernelargs};\0"

#define CONFIG_EXTRA_ENV_SETTINGS \
	BOOT_ENV_SETTINGS \
	OPT_ENV_SETTINGS \
	"bootenv=uEnv.txt\0" \
	"script=boot.scr\0" \
	"image=zImage\0" \
	"console=ttymxc0\0" \
	"fdt_file=undefined\0" \
	"fdt_addr=0x83000000\0" \
	"fdt_high=0xffffffff\0" \
	"initrd_high=0xffffffff\0" \
	"panel=VAR-WVGA-LCD\0" \
	"splashsourceauto=yes\0" \
	"splashenable=setenv splashfile /boot/splash.bmp; " \
		"setenv splashimage 0x83100000\0" \
	"splashdisable=setenv splashfile; setenv splashimage\0" \
	"boot_fdt=try\0" \
	"ip_dyn=yes\0" \
	"netargs=setenv bootargs console=${console},${baudrate} " \
		"root=/dev/nfs rw ${cma_size}" \
		"ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp\0" \
	"netboot=echo Booting from net ...; " \
		"run netargs; " \
		"run optargs; " \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"${get_cmd} ${image}; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"run findfdt; " \
			"echo fdt_file=${fdt_file}; " \
			"if ${get_cmd} ${fdt_addr} ${fdt_file}; then " \
				"bootz ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0" \
	"usbnet_devaddr=f8:dc:7a:00:00:02\0" \
	"usbnet_hostaddr=f8:dc:7a:00:00:01\0" \
	"ramsize_check="\
		"if test $sdram_size -lt 256; then " \
			"setenv cma_size cma=32MB; " \
			"setenv fdt_addr 0x84000000; " \
			"setenv loadaddr 0x84600000; " \
		"else " \
			"if test $sdram_size -lt 512; then " \
				"setenv cma_size cma=64MB; " \
			"fi; " \
		"fi;\0" \
	"findfdt="\
		"if test $fdt_file = undefined; then " \
			"if test $board_name = DART-6UL; then " \
				"setenv som var-dart; " \
				"setenv carrier 6ulcustomboard; " \
			"fi; " \
			"if test $board_name = VAR-SOM-6UL; then " \
				"setenv som var-som; " \
				"setenv carrier concerto-board; " \
			"fi; " \
			"if test $boot_dev = emmc || test $som_storage = emmc || " \
			   "test $som_storage = none; then " \
				"setenv storage emmc; " \
			"fi; " \
			"if test $boot_dev = nand || test $som_storage = nand; then " \
				"setenv storage nand; " \
			"fi; " \
			"if test $boot_dev = sd; then " \
				"setenv mmc0_dev sd-card; " \
			"else " \
				"if test $wifi = yes; then " \
					"setenv mmc0_dev wifi; " \
				"else " \
					"setenv mmc0_dev sd-card; " \
				"fi; " \
			"fi; " \
			"if test -n $soc_type && test -n $som && " \
			   "test -n $storage && test -n $mmc0_dev && test -n $carrier; then " \
				"setenv fdt_file ${soc_type}-${som}-${carrier}-${storage}-${mmc0_dev}.dtb; " \
			"fi; " \
			"setenv som; setenv carrier; setenv storage; setenv mmc0_dev; " \
			"if test $fdt_file = undefined; then " \
				"echo WARNING: Could not determine dtb to use; " \
			"fi; " \
		"fi;\0"


/* Miscellaneous configurable options */
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + 0x8000000)

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM			MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

#ifdef CONFIG_NAND_BOOT
#define CONFIG_CMD_NAND
#define CONFIG_SYS_NAND_U_BOOT_OFFS	0x200000
#endif

/* NAND pin conflicts with usdhc2 */
#ifdef CONFIG_CMD_NAND
#define CONFIG_SYS_FSL_USDHC_NUM	1
#else
#define CONFIG_SYS_FSL_USDHC_NUM	2
#endif
#endif

#define CONFIG_SYS_MMC_ENV_DEV		0   	/* USDHC1 */
#define CONFIG_SYS_MMC_ENV_PART		0	/* user area */

#if defined(CONFIG_ENV_IS_IN_MMC)
#define CONFIG_ENV_OFFSET		(14 * SZ_64K)
#define CONFIG_ENV_SIZE			SZ_8K
#elif defined(CONFIG_ENV_IS_IN_NAND)
#define CONFIG_ENV_OFFSET		0x400000
#define CONFIG_ENV_SIZE			SZ_128K
#endif

#define CONFIG_FAT_WRITE

#ifdef CONFIG_CMD_NAND
/* NAND flash command */
#define CONFIG_CMD_NAND_TRIMFFS

/* UBI/UBIFS support */
#define CONFIG_UBI_SILENCE_MSG
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS

#define MTDIDS_DEFAULT		"nand0=nandflash-0"

/*
 * Partitions layout for NAND is:
 *     mtd0: 2M       (spl) First boot loader
 *     mtd1: 2M       (u-boot)
 *     mtd2: 2M       (u-boot env.)
 *     mtd3: 8M       (kernel, dtb)
 *     mtd4: left     (rootfs)
 */
/* Default mtd partition table */
#define MTDPARTS_DEFAULT	"mtdparts=nandflash-0:"\
					"2m(spl),"\
					"2m(u-boot),"\
					"2m(u-boot-env),"\
					"8m(kernel),"\
					"-(rootfs)"	/* ubifs */

/* NAND stuff */
#define CONFIG_NAND_MXS
#define CONFIG_NAND_MXS_BCH_LEGACY_GEO
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		0x40000000
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_ONFI_DETECTION

/* DMA stuff, needed for GPMI/MXS NAND support */
#define CONFIG_APBH_DMA
#define CONFIG_APBH_DMA_BURST
#define CONFIG_APBH_DMA_BURST8
#endif

#define CONFIG_NETCONSOLE

/* Framebuffer */
#ifndef CONFIG_SPL_BUILD
#ifdef CONFIG_VIDEO
#define CONFIG_VIDEO_MXS
#define CONFIG_VIDEO_LOGO
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_CMD_BMP
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_VIDEO_BMP_LOGO
#define CONFIG_IMX_VIDEO_SKIP
#endif
#endif

/* SPLASH SCREEN Configs  */
#ifdef CONFIG_SPLASH_SCREEN
#define CONFIG_CMD_BMP
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_SPLASH_SOURCE
#endif

/* USB Configs */
#ifdef CONFIG_CMD_USB
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_USB_ETHER_MCS7830
#define CONFIG_USB_ETHER_SMSC95XX
#define CONFIG_MXC_USB_PORTSC	(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS	0
#define CONFIG_USB_MAX_CONTROLLER_COUNT	2

#define CONFIG_USBD_HS

#define CONFIG_USB_ETHER
#define CONFIG_USB_ETH_CDC

#endif /* CONFIG_CMD_USB */

#ifdef CONFIG_CMD_NET
#define CONFIG_CMD_MII
#define CONFIG_FEC_MXC
#define CONFIG_MII
#define CONFIG_FEC_ENET_DEV		0

#if (CONFIG_FEC_ENET_DEV == 0)
#define IMX_FEC_BASE			ENET_BASE_ADDR
#define CONFIG_FEC_MXC_PHYADDR		0x1
#define CONFIG_FEC_XCV_TYPE		RMII
#ifdef CONFIG_DM_ETH
#define CONFIG_ETHPRIME			"eth0"
#else
#define CONFIG_ETHPRIME			"FEC0"
#endif
#elif (CONFIG_FEC_ENET_DEV == 1)
#define IMX_FEC_BASE			ENET2_BASE_ADDR
#define CONFIG_FEC_MXC_PHYADDR		0x3
#define CONFIG_FEC_XCV_TYPE		RMII
#ifdef CONFIG_DM_ETH
#define CONFIG_ETHPRIME			"eth1"
#else
#define CONFIG_ETHPRIME			"FEC1"
#endif
#endif

#define CONFIG_PHYLIB
#define CONFIG_PHY_MICREL
#endif

#define CONFIG_IMX_THERMAL

#endif
