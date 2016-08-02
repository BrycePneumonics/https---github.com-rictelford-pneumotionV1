################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../I2C_Com.c \
../VU_Filter_Alg.c \
../VU_main.c \
../matrix_math.c 

SRC_OBJS += \
./I2C_Com.doj \
./VU_Filter_Alg.doj \
./VU_main.doj \
./matrix_math.doj 

C_DEPS += \
./I2C_Com.d \
./VU_Filter_Alg.d \
./VU_main.d \
./matrix_math.d 


# Each subdirectory must supply rules for building sources it contributes
%.doj: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="VU_Meter_3" -proc ADSP-BF706 -flags-compiler --no_wrap_diagnostics -si-revision any -g -D_DEBUG -DADI_DEBUG -DNO_UTILITY_ROM -DCORE0 -I"C:\Users\Pneumonics\Documents\ADSP\VU_Meter_3\system" -I"C:/Analog Devices/ADSP-BF706_EZ-KIT_Mini-Rel1.1.0/BF706_EZ-Kit_MINI/Blackfin/include" -I"C:/Analog Devices/ADSP-BF706_EZ-KIT_Mini-Rel1.1.0/BF706_EZ-Kit_MINI/Blackfin/include/drivers/codec/adau1761" -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -no-utility-rom -gnu-style-dependencies -MD -Mo "I2C_Com.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


