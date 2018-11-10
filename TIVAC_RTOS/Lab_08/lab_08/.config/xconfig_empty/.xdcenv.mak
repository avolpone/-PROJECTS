#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/ccsv8/ccs_base;C:/ti/tirtos_tivac_2_16_00_08/packages;C:/ti/tirtos_tivac_2_16_00_08/products/tidrivers_tivac_2_16_00_08/packages;C:/ti/tirtos_tivac_2_16_00_08/products/bios_6_45_01_29/packages;C:/ti/tirtos_tivac_2_16_00_08/products/ndk_2_25_00_09/packages;C:/ti/tirtos_tivac_2_16_00_08/products/uia_2_00_05_50/packages;C:/ti/tirtos_tivac_2_16_00_08/products/ns_1_11_00_10/packages;C:/ti/tirtos_tivac_2_16_00_08/products/bios_6_41_00_26/packages;C:/ti/tirtos_tivac_2_16_00_08/products/ndk_2_24_01_18/packages;C:/ti/tirtos_tivac_2_16_00_08/products/uia_2_00_02_39/packages;C:/Users/Volpone/Dropbox/CPE403~2/TIVAC_RTOS/Lab_08/.config
override XDCROOT = C:/ti/xdctools_3_32_00_06_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/ccsv8/ccs_base;C:/ti/tirtos_tivac_2_16_00_08/packages;C:/ti/tirtos_tivac_2_16_00_08/products/tidrivers_tivac_2_16_00_08/packages;C:/ti/tirtos_tivac_2_16_00_08/products/bios_6_45_01_29/packages;C:/ti/tirtos_tivac_2_16_00_08/products/ndk_2_25_00_09/packages;C:/ti/tirtos_tivac_2_16_00_08/products/uia_2_00_05_50/packages;C:/ti/tirtos_tivac_2_16_00_08/products/ns_1_11_00_10/packages;C:/ti/tirtos_tivac_2_16_00_08/products/bios_6_41_00_26/packages;C:/ti/tirtos_tivac_2_16_00_08/products/ndk_2_24_01_18/packages;C:/ti/tirtos_tivac_2_16_00_08/products/uia_2_00_02_39/packages;C:/Users/Volpone/Dropbox/CPE403~2/TIVAC_RTOS/Lab_08/.config;C:/ti/xdctools_3_32_00_06_core/packages;..
HOSTOS = Windows
endif
