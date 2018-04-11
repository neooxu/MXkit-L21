/**
 ******************************************************************************
 * @file    io_button.h
 * @author  William Xu
 * @version V1.0.0
 * @date    9-Apr-2018
 * @brief   GPIO button driver header file
 ******************************************************************************
 *
 * Copyright (c) 2009-2018 MXCHIP Co.,Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

#ifndef _IO_BUTTON_H_
#define _IO_BUTTON_H_

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup drivers */
/** @{*/

/** \addtogroup IO_Button */
/** @{*/

//--------------------------------  pin defines --------------------------------

enum _button_idle_state_e{
    IOBUTTON_IDLE_STATE_LOW = 0,
    IOBUTTON_IDLE_STATE_HIGH,
};

typedef uint8_t btn_idle_state;

typedef void (*button_pressed_cb)(void) ;
typedef void (*button_long_pressed_cb)(void) ;

typedef struct {
  const uint8_t port;
  const uint8_t pin;
  void (*io_irq)(void);
  btn_idle_state idle;
  uint32_t long_pressed_timeout;
  button_pressed_cb pressed_func;
  button_long_pressed_cb long_pressed_func;
  /* Use by driver, do not initialize */
  volatile bool clicked;
  volatile bool timer_enabled;
  volatile uint32_t start_time;
} btn_instance_t;

//------------------------------ user interfaces -------------------------------


/**
 * @brief Initialize button device.
 *
 * @param[in] btn: button driver context data, should be persist at button's life time
 *
 * @return    none
 */
void button_init(btn_instance_t * const btn);

/**
 * @brief Button driver service, should be called in main loop.
 *
 * @param[in] btn: button driver context data, should be persist at button's life time
 *
 * @return none
 */
void button_srv(btn_instance_t * const btn);

/**
 * @brief This function should be called by IRQ routine.
 *
 * @param[in] btn: button driver context data, should be persist at button's life time
 *
 * @return none
 */
void button_irq_handler(btn_instance_t * const btn);

/** @}*/
/** @}*/

#ifdef __cplusplus
}
#endif

#endif  // _IO_BUTTON_H_

