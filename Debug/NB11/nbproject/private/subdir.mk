################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../NB11/nbproject/private/c_standard_headers_indexer.c 

OBJS += \
./NB11/nbproject/private/c_standard_headers_indexer.o 

C_DEPS += \
./NB11/nbproject/private/c_standard_headers_indexer.d 


# Each subdirectory must supply rules for building sources it contributes
NB11/nbproject/private/%.o: ../NB11/nbproject/private/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


