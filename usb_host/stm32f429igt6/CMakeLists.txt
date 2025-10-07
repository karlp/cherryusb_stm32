cmake_minimum_required(VERSION 3.25)
# https://cliutils.gitlab.io/modern-cmake/chapters/intro/dodonot.html#selecting-a-minimum-in-2022
project(cherry-usbh-stm32f429 ASM C CXX)

# this is "meant" to be done with features....
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc")

set(CHRYROOT "../../CherryUSB")
set(FREERTOS_ROOT "${CHRYROOT}/third_party/FreeRTOS-10.4")
set(FATFS_ROOT "${CHRYROOT}/third_party/fatfs-0.14")
#set(LWIP_ROOT "${CHRYROOT}/third_party/lwip-2.1.2/src")
set(LWIP_ROOT "../..//lwip-2.1.2/src")
set(CHRYMP_ROOT "${CHRYROOT}/third_party/cherrymp")
set(CHRYRB_ROOT "${CHRYROOT}/third_party/cherryrb")

# Step 1, just slam it all together. modular wot?
# cherry has freertos 10.4 in their repo...
set(MCU_COMPILER_TYPE "cortex-m4")
set(MCU_COMPILER_FPU_DEV "fpv4-sp-d16")
set(MCU_COMPILER_INST_SET "-mthumb")
# fixme - open the cubemx file, regenerate the project and save the linker file it generates...
set(LD_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/STM32F429XX_FLASH.ld")

add_library(proj_comp_flags INTERFACE)
target_compile_options(proj_comp_flags INTERFACE
    $<$<COMPILE_LANGUAGE:CXX>:-mcpu=${MCU_COMPILER_TYPE}>
    $<$<COMPILE_LANGUAGE:C>:-mcpu=${MCU_COMPILER_TYPE}>
    $<$<COMPILE_LANGUAGE:ASM>:-mcpu=${MCU_COMPILER_TYPE}>

    $<$<COMPILE_LANGUAGE:CXX>:${MCU_COMPILER_INST_SET}>
    $<$<COMPILE_LANGUAGE:C>:${MCU_COMPILER_INST_SET}>
    $<$<COMPILE_LANGUAGE:ASM>:${MCU_COMPILER_INST_SET}>

    $<$<COMPILE_LANGUAGE:CXX>:-mfloat-abi=hard>
    $<$<COMPILE_LANGUAGE:CXX>:-mfpu=${MCU_COMPILER_FPU_DEV}>
    $<$<COMPILE_LANGUAGE:C>:-mfloat-abi=hard>
    $<$<COMPILE_LANGUAGE:C>:-mfpu=${MCU_COMPILER_FPU_DEV}>
    $<$<COMPILE_LANGUAGE:ASM>:-mfloat-abi=hard>
    $<$<COMPILE_LANGUAGE:ASM>:-mfpu=${MCU_COMPILER_FPU_DEV}>

    -Os
    -g3
    $<$<COMPILE_LANGUAGE:CXX>:-Wall>
    $<$<COMPILE_LANGUAGE:CXX>:-Wextra>
    $<$<COMPILE_LANGUAGE:C>:-Wall>
    $<$<COMPILE_LANGUAGE:C>:-Wextra>
    -fdata-sections
    -ffunction-sections
)

target_compile_definitions(proj_comp_flags INTERFACE
    STM32F429xx
)

add_library(proj_linker_flags INTERFACE)
target_link_options(proj_linker_flags INTERFACE
    -mcpu=${MCU_COMPILER_TYPE}
    -mfloat-abi=hard -mfpu=${MCU_COMPILER_FPU_DEV}
    ${MCU_COMPILER_INST_SET}
    -Os
    -g3
    -Wl,-T ${LD_SCRIPT}
    LINKER:--print-memory-usage,--gc-sections,-Map=output.map
)



add_executable(cherry-usbh-stm32f429
        Core/Src/main.c
        Core/Src/stm32f4xx_hal_msp.c
        Core/Src/stm32f4xx_it.c
        Core/Src/system_stm32f4xx.c
        Core/Src/startup_stm32f429xx.s
        Core/Src/syscalls.c
        #Core/Src/syscalls_swo.c
        Core/Src/syscalls_rtt.c
        Core/Src/SEGGER_RTT.c
        Core/Src/sysmem.c

        ${CHRYROOT}/demo/usb_host.c
        ${CHRYROOT}/core/usbh_core.c
        ${CHRYROOT}/class/hub/usbh_hub.c
        ${CHRYROOT}/class/cdc/usbh_cdc_acm.c
        ${CHRYROOT}/class/cdc/usbh_cdc_ecm.c
        ${CHRYROOT}/class/cdc/usbh_cdc_ncm.c
        ${CHRYROOT}/class/msc/usbh_msc.c
        ${CHRYROOT}/class/msc/usbh_msc.c
        ${CHRYROOT}/class/wireless/usbh_rndis.c
        ${CHRYROOT}/class/audio/usbh_audio.c
        ${CHRYROOT}/class/video/usbh_video.c
        ${CHRYROOT}/class/vendor/net/usbh_asix.c
        ${CHRYROOT}/class/vendor/net/usbh_rtl8152.c
        ${CHRYROOT}/osal/usb_osal_freertos.c
        ${CHRYROOT}/port/dwc2/usb_hc_dwc2.c
        ${CHRYROOT}/port/dwc2/usb_glue_st.c
        ${CHRYROOT}/port/dwc2/usb_hc_dwc2.c
        ${CHRYROOT}/port/dwc2/usb_hc_dwc2.c

        ${CHRYROOT}/platform/fatfs/usbh_fatfs.c
        ${FATFS_ROOT}/source/diskio.c
        ${FATFS_ROOT}/source/ff.c
        ${FATFS_ROOT}/source/ffsystem.c
        ${FATFS_ROOT}/source/ffunicode.c

        ${CHRYROOT}/platform/lwip/usbh_lwip.c
        ${LWIP_ROOT}/core/altcp.c
        ${LWIP_ROOT}/core/altcp_alloc.c
        ${LWIP_ROOT}/core/altcp_tcp.c
        ${LWIP_ROOT}/core/def.c
        ${LWIP_ROOT}/core/dns.c
        ${LWIP_ROOT}/core/inet_chksum.c
        ${LWIP_ROOT}/core/init.c
        ${LWIP_ROOT}/core/ip.c
        ${LWIP_ROOT}/core/mem.c
        ${LWIP_ROOT}/core/memp.c
        ${LWIP_ROOT}/core/netif.c
        ${LWIP_ROOT}/core/pbuf.c
        ${LWIP_ROOT}/core/raw.c
        ${LWIP_ROOT}/core/stats.c
        ${LWIP_ROOT}/core/sys.c
        ${LWIP_ROOT}/core/tcp.c
        ${LWIP_ROOT}/core/tcp_in.c
        ${LWIP_ROOT}/core/tcp_out.c
        ${LWIP_ROOT}/core/timeouts.c
        ${LWIP_ROOT}/core/udp.c
        ${LWIP_ROOT}/core/ipv4/autoip.c
        ${LWIP_ROOT}/core/ipv4/dhcp.c
        ${LWIP_ROOT}/core/ipv4/etharp.c
        ${LWIP_ROOT}/core/ipv4/icmp.c
        ${LWIP_ROOT}/core/ipv4/igmp.c
        ${LWIP_ROOT}/core/ipv4/ip4.c
        ${LWIP_ROOT}/core/ipv4/ip4_addr.c
        ${LWIP_ROOT}/core/ipv4/ip4_frag.c
        ${LWIP_ROOT}/core/ipv6/dhcp6.c
        ${LWIP_ROOT}/core/ipv6/ethip6.c
        ${LWIP_ROOT}/core/ipv6/icmp6.c
        ${LWIP_ROOT}/core/ipv6/inet6.c
        ${LWIP_ROOT}/core/ipv6/ip6.c
        ${LWIP_ROOT}/core/ipv6/ip6_addr.c
        ${LWIP_ROOT}/core/ipv6/ip6_frag.c
        ${LWIP_ROOT}/core/ipv6/mld6.c
        ${LWIP_ROOT}/core/ipv6/nd6.c
        ${LWIP_ROOT}/api/api_lib.c
        ${LWIP_ROOT}/api/api_msg.c
        ${LWIP_ROOT}/api/err.c
        ${LWIP_ROOT}/api/if_api.c
        ${LWIP_ROOT}/api/netbuf.c
        ${LWIP_ROOT}/api/netdb.c
        ${LWIP_ROOT}/api/netifapi.c
        ${LWIP_ROOT}/api/sockets.c
        ${LWIP_ROOT}/api/tcpip.c
        ${LWIP_ROOT}/netif/ethernet.c
        ${LWIP_ROOT}/include/arch/sys_arch.c

        ${CHRYRB_ROOT}/chry_ringbuffer.c
        ${CHRYMP_ROOT}/chry_mempool.c
        ${CHRYMP_ROOT}/chry_mempool_osal_freertos.c

        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.c
        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c
        ${FREERTOS_ROOT}/croutine.c
        ${FREERTOS_ROOT}/event_groups.c
        ${FREERTOS_ROOT}/list.c
        ${FREERTOS_ROOT}/queue.c
        ${FREERTOS_ROOT}/tasks.c
        ${FREERTOS_ROOT}/timers.c
        ${FREERTOS_ROOT}/portable/MemMang/heap_4.c
        ${FREERTOS_ROOT}/portable/GCC/ARM_CM4F/port.c

    )

target_include_directories(cherry-usbh-stm32f429 PRIVATE
    Core/Inc
    Drivers/STM32F4xx_HAL_Driver/Inc
    ${CHRYROOT}/core
    ${CHRYROOT}/common
    ${CHRYROOT}/class/hub
    ${CHRYROOT}/class/cdc
    ${CHRYROOT}/class/hid
    ${CHRYROOT}/class/msc
    ${CHRYROOT}/class/audio
    ${CHRYROOT}/class/video
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/CMSIS/Include
    ${FREERTOS_ROOT}/include
    ${FREERTOS_ROOT}/portable/GCC/ARM_CM4F
    ${LWIP_ROOT}/include
    ${FATFS_ROOT}/source
)

#target_compile_options(cherry-usb-stm32f429 asdfadsf)
#target_link_options(cherry-usb-stm32f429 asdfadsf)
target_link_libraries(cherry-usbh-stm32f429 PRIVATE
    proj_comp_flags
    proj_linker_flags)
target_compile_features(cherry-usbh-stm32f429 PUBLIC c_std_11)