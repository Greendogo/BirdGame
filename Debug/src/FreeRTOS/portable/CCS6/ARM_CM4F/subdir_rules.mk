################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/FreeRTOS/portable/CCS6/ARM_CM4F/port.obj: H:/EECS_388/TI_CCS_Archive/FreeRTOS/portable/CCS6/ARM_CM4F/port.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/TI_CodeComposer/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="C:/TI_CodeComposer/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/TI_CodeComposer/StellarisWare-LM3S1968" --include_path="H:/EECS_388/TI_CCS_Archive/LM3S1968_Drivers" --include_path="H:/EECS_388/TI_CCS_Archive/FreeRTOS/include" --include_path="H:/EECS_388/TI_CCS_Projects/EECS_388_P9/targetConfigs" -g --gcc --define=CCS6_ARMCM3 --define="ccs" --define=PART_LM3S1968 --display_error_number --emit_warnings_as_errors --diag_suppress=179 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="src/FreeRTOS/portable/CCS6/ARM_CM4F/port.pp" --obj_directory="src/FreeRTOS/portable/CCS6/ARM_CM4F" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/FreeRTOS/portable/CCS6/ARM_CM4F/portasm.obj: H:/EECS_388/TI_CCS_Archive/FreeRTOS/portable/CCS6/ARM_CM4F/portasm.s $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/TI_CodeComposer/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="C:/TI_CodeComposer/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/TI_CodeComposer/StellarisWare-LM3S1968" --include_path="H:/EECS_388/TI_CCS_Archive/LM3S1968_Drivers" --include_path="H:/EECS_388/TI_CCS_Archive/FreeRTOS/include" --include_path="H:/EECS_388/TI_CCS_Projects/EECS_388_P9/targetConfigs" -g --gcc --define=CCS6_ARMCM3 --define="ccs" --define=PART_LM3S1968 --display_error_number --emit_warnings_as_errors --diag_suppress=179 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="src/FreeRTOS/portable/CCS6/ARM_CM4F/portasm.pp" --obj_directory="src/FreeRTOS/portable/CCS6/ARM_CM4F" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


