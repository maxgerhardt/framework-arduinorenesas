include_guard()

if (NOT BOARD)
  message(FATAL_ERROR "BOARD not specified")
endif ()

set(ST_FAMILY g0)
set(ST_PREFIX stm32${ST_FAMILY}xx)

set(ST_HAL_DRIVER ${TOP}/hw/mcu/st/stm32${ST_FAMILY}xx_hal_driver)
set(ST_CMSIS ${TOP}/hw/mcu/st/cmsis_device_${ST_FAMILY})
set(CMSIS_5 ${TOP}/lib/CMSIS_5)

# include board specific
include(${CMAKE_CURRENT_LIST_DIR}/boards/${BOARD}/board.cmake)

# toolchain set up
set(CMAKE_SYSTEM_PROCESSOR cortex-m0plus CACHE INTERNAL "System Processor")
set(CMAKE_TOOLCHAIN_FILE ${TOP}/tools/cmake/toolchain/arm_${TOOLCHAIN}.cmake)

set(FAMILY_MCUS STM32G0 CACHE INTERNAL "")

# enable LTO if supported
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED)
if (IPO_SUPPORTED)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()


#------------------------------------
# BOARD_TARGET
#------------------------------------
# only need to be built ONCE for all examples
function(add_board_target BOARD_TARGET)
  if (NOT TARGET ${BOARD_TARGET})
    add_library(${BOARD_TARGET} STATIC
      ${ST_CMSIS}/Source/Templates/system_${ST_PREFIX}.c
      ${ST_HAL_DRIVER}/Src/${ST_PREFIX}_hal.c
      ${ST_HAL_DRIVER}/Src/${ST_PREFIX}_hal_cortex.c
      ${ST_HAL_DRIVER}/Src/${ST_PREFIX}_hal_pwr_ex.c
      ${ST_HAL_DRIVER}/Src/${ST_PREFIX}_hal_rcc.c
      ${ST_HAL_DRIVER}/Src/${ST_PREFIX}_hal_rcc_ex.c
      ${ST_HAL_DRIVER}/Src/${ST_PREFIX}_hal_gpio.c
      ${ST_HAL_DRIVER}/Src/${ST_PREFIX}_hal_uart.c
      ${ST_HAL_DRIVER}/Src/${ST_PREFIX}_hal_uart_ex.c
      ${STARTUP_FILE_${CMAKE_C_COMPILER_ID}}
      )
    target_include_directories(${BOARD_TARGET} PUBLIC
      ${CMAKE_CURRENT_FUNCTION_LIST_DIR}
      ${CMSIS_5}/CMSIS/Core/Include
      ${ST_CMSIS}/Include
      ${ST_HAL_DRIVER}/Inc
      )
    target_compile_options(${BOARD_TARGET} PUBLIC
      )
    target_compile_definitions(${BOARD_TARGET} PUBLIC
      )

    update_board(${BOARD_TARGET})

    cmake_print_variables(CMAKE_C_COMPILER_ID)

    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
      target_link_options(${BOARD_TARGET} PUBLIC
        "LINKER:--script=${LD_FILE_GNU}"
        -nostartfiles
        # nanolib
        --specs=nosys.specs
        --specs=nano.specs
        )
    elseif (CMAKE_C_COMPILER_ID STREQUAL "IAR")
      target_link_options(${BOARD_TARGET} PUBLIC
        "LINKER:--config=${LD_FILE_IAR}"
        )
    endif ()
  endif ()
endfunction()


#------------------------------------
# Functions
#------------------------------------
function(family_configure_example TARGET)
  family_configure_common(${TARGET})

  # Board target
  add_board_target(board_${BOARD})

  #---------- Port Specific ----------
  # These files are built for each example since it depends on example's tusb_config.h
  target_sources(${TARGET} PUBLIC
    # TinyUSB Port
    ${TOP}/src/portable/st/stm32_fsdev/dcd_stm32_fsdev.c
    # BSP
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/family.c
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../board.c
    )
  target_include_directories(${TARGET} PUBLIC
    # family, hw, board
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../../
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/boards/${BOARD}
    )

  # Add TinyUSB
  family_add_tinyusb(${TARGET} OPT_MCU_STM32G0)

  # Link dependencies
  target_link_libraries(${TARGET} PUBLIC board_${BOARD} ${TARGET}-tinyusb)

  # Flashing
  family_flash_stlink(${TARGET})
  #family_flash_jlink(${TARGET})
endfunction()


function(family_configure_device_example TARGET)
  family_configure_example(${TARGET})
endfunction()

function(family_configure_host_example TARGET)
  family_configure_example(${TARGET})
endfunction()

function(family_configure_dual_usb_example TARGET)
  family_configure_example(${TARGET})
endfunction()