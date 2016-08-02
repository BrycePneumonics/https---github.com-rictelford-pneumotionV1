################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.1.0/Blackfin/lib/src/drivers/source/sport/adi_sport.c 

SRC_OBJS += \
./system/drivers/sport/adi_sport.doj 

C_DEPS += \
./system/drivers/sport/adi_sport.d 


# Each subdirectory must supply rules for building sources it contributes
system/drivers/sport/adi_sport.doj: C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.1.0/Blackfin/lib/src/drivers/source/sport/adi_sport.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="VU_Meter_3" -proc ADSP-BF706 -flags-compiler --no_wrap_diagnostics -si-revision any -g -D_DEBUG -DADI_DEBUG -DNO_UTILITY_ROM -DCORE0 -I"C:\Users\Pneumonics\Documents\ADSP\VU_Meter_3\system" -I"C:/Analog Devices/ADSP-BF706_EZ-KIT_Mini-Rel1.1.0/BF706_EZ-Kit_MINI/Blackfin/include" -I"C:/Analog Devices/ADSP-BF706_EZ-KIT_Mini-Rel1.1.0/BF706_EZ-Kit_MINI/Blackfin/include/drivers/codec/adau1761" -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -no-utility-rom -gnu-style-dependencies -MD -Mo "system/drivers/sport/adi_sport.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


