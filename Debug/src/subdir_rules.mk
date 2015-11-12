################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/buttons.obj: ../src/buttons.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/TI_CodeComposer/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="C:/TI_CodeComposer/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/TI_CodeComposer/StellarisWare-LM3S1968" --include_path="H:/EECS_388/TI_CCS_Archive/LM3S1968_Drivers" --include_path="H:/EECS_388/TI_CCS_Archive/FreeRTOS/include" --include_path="H:/EECS_388/TI_CCS_Projects/EECS_388_P6/targetConfigs" -g --gcc --define=CCS6_ARMCM3 --define="ccs" --define=PART_LM3S1968 --display_error_number --emit_warnings_as_errors --diag_suppress=179 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="src/buttons.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/game.obj: ../src/game.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/TI_CodeComposer/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="C:/TI_CodeComposer/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/TI_CodeComposer/StellarisWare-LM3S1968" --include_path="H:/EECS_388/TI_CCS_Archive/LM3S1968_Drivers" --include_path="H:/EECS_388/TI_CCS_Archive/FreeRTOS/include" --include_path="H:/EECS_388/TI_CCS_Projects/EECS_388_P6/targetConfigs" -g --gcc --define=CCS6_ARMCM3 --define="ccs" --define=PART_LM3S1968 --display_error_number --emit_warnings_as_errors --diag_suppress=179 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="src/game.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/led.obj: ../src/led.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/TI_CodeComposer/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="C:/TI_CodeComposer/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/TI_CodeComposer/StellarisWare-LM3S1968" --include_path="H:/EECS_388/TI_CCS_Archive/LM3S1968_Drivers" --include_path="H:/EECS_388/TI_CCS_Archive/FreeRTOS/include" --include_path="H:/EECS_388/TI_CCS_Projects/EECS_388_P6/targetConfigs" -g --gcc --define=CCS6_ARMCM3 --define="ccs" --define=PART_LM3S1968 --display_error_number --emit_warnings_as_errors --diag_suppress=179 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="src/led.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/oled.obj: ../src/oled.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/TI_CodeComposer/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="C:/TI_CodeComposer/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/TI_CodeComposer/StellarisWare-LM3S1968" --include_path="H:/EECS_388/TI_CCS_Archive/LM3S1968_Drivers" --include_path="H:/EECS_388/TI_CCS_Archive/FreeRTOS/include" --include_path="H:/EECS_388/TI_CCS_Projects/EECS_388_P6/targetConfigs" -g --gcc --define=CCS6_ARMCM3 --define="ccs" --define=PART_LM3S1968 --display_error_number --emit_warnings_as_errors --diag_suppress=179 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="src/oled.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/system.obj: ../src/system.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/TI_CodeComposer/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="C:/TI_CodeComposer/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/TI_CodeComposer/StellarisWare-LM3S1968" --include_path="H:/EECS_388/TI_CCS_Archive/LM3S1968_Drivers" --include_path="H:/EECS_388/TI_CCS_Archive/FreeRTOS/include" --include_path="H:/EECS_388/TI_CCS_Projects/EECS_388_P6/targetConfigs" -g --gcc --define=CCS6_ARMCM3 --define="ccs" --define=PART_LM3S1968 --display_error_number --emit_warnings_as_errors --diag_suppress=179 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="src/system.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


