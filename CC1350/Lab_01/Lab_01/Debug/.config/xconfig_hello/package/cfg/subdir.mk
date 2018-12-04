################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../.config/xconfig_hello/package/cfg/hello_pem3.cfg 


# Each subdirectory must supply rules for building sources it contributes
configPkg/linker.cmd: ../.config/xconfig_hello/package/cfg/hello_pem3.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"/xs" --xdcpath= xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M3 -p ti.platforms.simplelink:CC1350F128 -r release -c --compileOptions " -std=c99 " "$<"
	@echo 'Finished building: $<'
	@echo ' '


