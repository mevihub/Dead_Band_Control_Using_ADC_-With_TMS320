################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
device/%.obj: ../device/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="E:/Workspace_test/Dead_Band_Test_2" --include_path="C:/ti/c2000/C2000Ware_4_01_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/c2000/C2000Ware_4_01_00_00/device_support/f2837xd/headers/include" --include_path="E:/Workspace_test/Dead_Band_Test_2/device" --include_path="C:/ti/c2000/C2000Ware_4_01_00_00/driverlib/f2837xd/driverlib" --include_path="C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/include" --define=_DUAL_HEADERS --define=_LAUNCHXL_F28379D --define=CPU1 --define=DEBUG --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="device/$(basename $(<F)).d_raw" --obj_directory="device" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


