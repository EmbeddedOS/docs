# SWF siwx917

- `main()`:

    ```C
    int main(void)
    {
    int32_t status = RSI_SUCCESS;
    sl_cts_local_time_info_t local_time = { 0 };

    // Driver initialization
    status = rsi_driver_init(driver_buf, SL_DRIVER_BUF_LEN);
    if (( status < 0 ) || ( status > SL_DRIVER_BUF_LEN )) {
        return status;
    }

    // Silicon labs module intialisation
    status = rsi_device_init(LOAD_NWP_FW);
    if (status != RSI_SUCCESS) {
        SL_LOG_ERR("Device Initialization Failed, Error Code: 0x%08X",
                (uint32_t)(status));
        return status;
    }

    sl_log_init();
    sl_rtc_init();
    sl_sched_init();
    tmc2300_init();

    // Initialize default time zone: GTM +7
    local_time.time_zone = SL_TIME_ZONE;
    local_time.dst_offset = SL_DST_OFFSET;
    sl_sntp_set_offset_time(local_time);
    sl_softtimer_init();

    SL_LOG_INFO("Device Initialization Success");

    // Create application task
    rsi_task_create((rsi_task_function_t)(main_app_task),
                    (uint8_t *)("main_task"),
                    SL_MAIN_TASK_STACK_SIZE,
                    NULL,
                    SL_MAIN_TASK_PRIORITY,
                    &sl_main_task_handle);

    // Start the scheduler
    rsi_start_os_scheduler( );
    }

    ```

  - The main function first call `rsi_driver_init()` to init the wiSeConnect Driver. This is a **non-blocking** API. Designate memory to all driver components from the buffer `driver_buf`. And then it initialize Scheduler, Events, and Queues needed.

    ```C
    #define SL_DRIVER_BUF_LEN 15000 /**< Driver buffer length */

    /**
    * Memory buffer for driver initialization
    */
    static uint8_t driver_buf[SL_DRIVER_BUF_LEN] = { 0 };
    ```

    - The API receive first parameter as a pointer to buffer from application, Driver uses this buffer to hold driver control for its operation. And second parameter is  a length of the buffer.
    - This API returns the memory used, which is less than or equal to buffer length provided. If failure, it return maximum sockets is greater than 10.

  - After driver initialization, device is init with API `rsi_device_init()`. This API power cycle the module and set the firmware image type be loaded for WiSeConnect features. Initialize the module SPI. This is a blocking API.
    - `rsi_device_init()` must be called before this API.
    - The API only receives one parameter that provides two options to load Firmware image:
      - `LOAD_NWP_FW`: To load firmware image.
      - `LOAD_DEFAULT_NWP_FW_ACTIVE_LOW`: To load active low Firmware image.
      - Active low firmware will generate active low interrupts to indicate that packets are pending on the module, instead of the default active high.

  - Next, we initialize logging system `sl_log_init()` that function create the mutex for logging and create UART pipeline for logging:

    ```C
    void sl_log_init()
    {
    #if SL_ENABLE_LOG_DIRECT_VIA_UART
    rsi_mutex_create(&log_lock);
    #endif

    sl_uart_init(SL_UART_LOG, 115200);
    }
    ```

  - After init the logging system for debug purpose, we need to init RTC module `sl_rtc_init();` this function first select clock source for FSM low frequency clock `RSI_PS_FsmLfClkSel(KHZ_RC_CLK_SEL)`, we use RC clock for FSM (Clock source can be RO or RC or XTAL). and then, it initialize Realtime Clock RTC module:. What does `sl_rtc_init()` do ?
    - First it select clock source: `sl_config_fsm_low_frequency_clock();`
    - Init RTC using RSI API: `RSI_RTC_Init(RTC);`
    - Set datetime for RTC: `RSI_RTC_SetDateTime(RTC, &rtc_time);`
    - Enable RTC second interrupt: `sl_rtc_enable_sec_interrupt();`
    - Enable RTC millisecond interrupt: `sl_rtc_enable_msec_interrupt();`
    - Initialize RTC Calibration: `RSI_RTC_CalibInitilization()`'
      - To make very accurate RTCs, calibration and compensation mechanisms must be implemented.
    - Calibrate the RTC with RSI API: `RSI_RTC_ROCLK_Calib(TIME_PERIOD, 1, 1, 3, 1, 1, 0);`
      - with params:
        - `rtc`: TIME_PERIOD
        - `enable`: 1 to start rc calibration.
        - `periodic_en`: 1 to start periodically calibrate.
        - `trigger_time`: 3 - 30 sec.
        - `ro_enable`: 1 to enable ro calibration.
        - `periodic_en`: 1 to enable periodic ro calibration.
        - `trigger_time`: 0 - 1 timer in sec.
    - Set priority for RTC Alarm interrupt: `NVIC_SetPriority(SL_NVIC_RTC_ALARM, 8);`
    - Enable RTC second and RTC millisecond interrupts in NVIC: `NVIC_EnableIRQ(SL_NVIC_RTC);`
    - Enable RTC Alarm interrupt in NVIC: `NVIC_EnableIRQ(SL_NVIC_RTC_ALARM);`
    - Start RTC module: `RSI_RTC_Start(RTC);`

  - Next the schedule structure is initialized `sl_sched_init();` this function set memory of `sched` object to zero. And set state of all scheduler to invalid:
  
  ```C
  static sl_sched_t sched[SL_SCHED_MAX + 1] = { 0 };
  
  void sl_sched_init(void)
  {
    SL_CLEAR_DATA(sched, sizeof(sched));
    for (int i = 0; i < SL_SCHED_MAX + 1; i++) {
      sched[i].state = SL_SCHED_INVALID;
    }

    /* TODO: Reload sched data from nvm */
  }
  ```

  - Schedule structure:

  ```C
  /**
   * Schedule structure.
   */
  typedef struct {
    uint8_t state;                /**< State */
    uint8_t hour;                 /**< Hours */
    uint8_t minute;               /**< Minutes */
    uint8_t second;               /**< Seconds */
    uint8_t repeat;               /**< Repeat day mask */
    sl_sched_callback_t callback; /**< Callback function */
    void *arg;                    /**< Callback function arguments */
  } sl_sched_t;

  ```

  - This structure maintain a schedule for some task, callback it when it's time. For example, we can use structure for define some BLE task that going to setup and control the motor.

  - Next, the `main()` function initialize `tmc2300_init()`, TMC2300 chip control the motor. We need to initialize a UART pipeline to communicate with the TMC2300: `sl_uart_init(SL_UART_MOTOR, 19200);`

  - Next step, we need to init the SNTP server for time synchronization (timezone, time offset): `sl_sntp_set_offset_time()`

  - Finally, we initialize Soft-timer task and the main task and start scheduler for run our tasks: `rsi_start_os_scheduler();`

## 1. UART peripherals interface

- UART is one of digital peripherals of siwx917 chip.
- `siwx917` SoC have two UARTs and one USART controllers.
- 9-bit serial support.
- Multi-drop `RS485` interface support.
- 5, 6, 7, and 8-bit character encoding with even, odd and no parity.
- 1, 1.5 (only with 5 bit character encoding) ans 2 stop bits.
- Hardware Auto flow control (RTS, CTS).

- The UART controllers also support additional features listed below, which help in achieving `better performance` and reduce the burden on the processor:
  - Programable fractional baud rate support.
  - Programable baud rate supporting up to 7.3 M-bps.
  - Programable FIFO thresholds with maximum FIFO depth of 16 and support for DMA.
  - Prioritized interrupt identification.

- The UART controller in MCU ULP subsystem (`ULP_UART`) supports the following additional power-save features:
  - After the DMA is programmed in `PS2` state for UART transfer, the MCU can switch to `PS1` state (Processor shutdown) while the UART controller continues with the data transfer.
  - In `PS1` state (ULP Peripheral mode) the UART controller completes the data transfer and, triggered by the Peripheral Interrupt, shifts either to the sleep state (without processor intervention) or the active state.

### 1.1. How do we initialize UART controllers?

- UART configuration structure:

```C
typedef struct {
  uint8_t irq_num;    /**< Interrupt number */
  uint8_t irq_prio;   /**< Interrupt priority */
  USART0_Type *usart; /**< UART base address */
  USART_IO io;        /**< UART GPIO pinout */
  USART_CLOCK clock;  /**< UART clock */
} sl_uart_config_t;

```

- This structure contains UART properties such as: IO pin structure, IRQ number, IRQ priority, Clock and type. We need to define three interface for `UART0`, `UART1` and `ULP_UART`. For example, initialization of UART0 look like:

```C
static const sl_uart_config_t uart0_config = {
  .irq_num = USART0_IRQn,
  .irq_prio = 8,
  .usart = UART0,
  .io.clock = NULL,
  .io.tx = &usart0_tx,
  .io.rx = &usart0_rx,
  .clock.uart_clock_src = RTE_USART0_CLK_SRC,
  .clock.ulp_uart_source = ULP_UART_REF_CLK,
  .clock.divfact = RTE_USART0_CLK_DIV_FACT,
  .clock.frac_div_en = RTE_USART0_FRAC_DIV_EN,
};
```

- With `USART0_IRQn`, `UART0`, `RTE_USART0_CLK_SRC`, etc. are defined in Siw SDK.

- UART context structure:

```C
typedef struct {
  uint8_t uart;                   /**< UART index */
  uint8_t initialize;             /**< UART initialize */
  rsi_mutex_handle_t tx_lock;     /**< UART tx mutex lock */
  rsi_mutex_handle_t rx_lock;     /**< UART rx mutex lock */
  QueueHandle_t tx_queue;         /**< UART TX queue */
  QueueHandle_t rx_queue;         /**< UART RX queue */
  const sl_uart_config_t *config; /**< UART configuration */
} sl_uart_context_t;
```

- For each uart interface, we need one UART context to manage them.
- This structure contains:
  - `initialize`: context is initialize or not.

- We use array of `sl_uart_context_t` structure to manage `UART0`, `UART1` and `ULP_UART` controller:

```C
static sl_uart_context_t uart_context[SL_UART_NUM] = {
  { .uart = SL_UART0, .initialize = 0, .config = &uart0_config, },
  { .uart = SL_UART1, .initialize = 0, .config = &uart1_config, },
  { .uart = SL_UART_ULP, .initialize = 0, .config = &uart_ulp_config, },
};
```

- For logging and motor driving purpose, we will use UART1 and UART_ULP for them:

```C
#define SL_UART0    0 /**< UART0 index */
#define SL_UART1    1 /**< UART1 index */
#define SL_UART_ULP 2 /**< ULP UART index */

#define SL_UART_NUM 3 /**< Number of UARTs */

#define SL_UART_LOG                  SL_UART1
#define SL_UART_MOTOR                SL_UART_ULP
```

### 1.2. Initialize UART controller

- To initialize a UART interface we use:

```C
int sl_uart_init(uint8_t uart, uint32_t baud);
```

- With first param `uart` is UART index and second param `baud` is baud rate.

- This function do:
  - 1. Check context is init or not: `if (ctx->initialize) {}` if is already initialized, we don't need to do it again.
  - 2. Create Rx and Tx Queue:
    - `ctx->tx_queue = xQueueCreate(SL_UART_TX_BUFFER_SIZE, sizeof(uint8_t));`
    - `ctx->rx_queue = xQueueCreate(SL_UART_RX_BUFFER_SIZE, sizeof(uint8_t));`
  - 3. Create mutexes for Rx and Tx:
    - `rsi_mutex_create(&ctx->tx_lock);`
    - `rsi_mutex_create(&ctx->rx_lock);`

  - 4. Config uart clock: `uart_clockconfig(uart, true);`
  - 5. Config uart IO pin: `uart_ioconfigure(uart);`
  - 6. Config baud-rate, stop bit, parity , nbits: `uart_config(uart, baud, 0, 8, 1);`

  - 7. Set interrupt priority and enable the interrupt:

    ```C
    NVIC_SetPriority(config->irq_num, config->irq_prio);
    // Enable interrupt
    NVIC_ClearPendingIRQ(config->irq_num);
    NVIC_EnableIRQ(config->irq_num);
    ```

  - 8. Mark the context as initialized: `ctx->initialize = 1;`

### 1.3. UART function interfaces

- `int sl_uart_init(uint8_t uart, uint32_t baud);`: This function initializes an UART context with params:
  - uart: UART index.
  - baud: Baud rate.

- `int sl_uart_deinit(uint8_t uart);`: This function de-initialize an UART context.
- `int sl_uart_putc(uint8_t uart, uint8_t ch);`: put one character to an UART context.
- `int sl_uart_send_bytes(uint8_t uart, uint8_t *bufs, uint16_t length);`: send a buffer data stream to an UART context.
- `int sl_uart_read_bytes(uint8_t uart, uint8_t *bufs, uint16_t length);`: receive a buffer data stream from an UART context.

### 1.4. UART for logging

- Logging system use UART interface to send log info. This system provide three functions:
  - `void sl_log_init();`: This function initializes log module.
  - `void sl_log_printf(const char *format, ...);`
  - `void sl_log_println(const char *str);`

### 1.5. UART for TMC2300 Motor driver

- TMC2300 controller use the UART interface to set value to registers and read value from them. Via this function we can control the Motor system:
  - `int tmc2300_write_reg(uint8_t address, uint32_t value);`
  - `int tmc2300_read_reg(uint8_t address, uint32_t *value);`

## 2. Soft-timer system

- To make a task that could be run after specify time. We need to make a `sl_softtimer_data_t` object and add it to the soft-timer list:

```C
typedef struct {
  uint32_t start;                   /**< Time start in milliseconds */
  uint32_t time;                    /**< Timeout in milliseconds */
  sl_softtimer_callback_t callback; /**< Callback function */
  void *params;                     /**< Arguments for callback */
} sl_softtimer_data_t;
```

- The soft-timer list is maintains by the object:

```C
#define SL_SOFTTIMER_MAX 12 /**< Maximum number of Software Timers */

static sl_softtimer_data_t softtimer_list[SL_SOFTTIMER_MAX] = { 0 };
```

- After make an soft-timer object and add it to the list. The task that is made by calling `sl_softtimer_init()` ( This function create a real-time task using `rsi_task_create()` API that based on FreeRTOS library)will check all callback functions in the list every 10 milliseconds. And call them `callback(param);` if the time of them is expired:

```C
#define SL_TIMER_EXPIRED(timer) \
  ((uint32_t)(sl_get_ms() - timer.start) >= timer.time)

static void softtimer_task(void *arg)
{
  SL_UNUSED(arg);
  uint32_t index;
  void *param = NULL;
  sl_softtimer_callback_t callback = NULL;

  while (1) {
    for (index = 0ul; index < SL_SOFTTIMER_MAX; index++) {
      if (( softtimer_list[index].callback != NULL )
          && SL_TIMER_EXPIRED(softtimer_list[index])) {
        callback = softtimer_list[index].callback;
        param = softtimer_list[index].params;
        sl_softtimer_cancel(callback);
        callback(param);
        // softtimer_list[index].callback = NULL;
      }
    }
    sl_delay_ms(10);
  }
}
```

## 3. The main app task main_app_task()

- The main task is responsible for:
  - 1. Initialize event queue: `main_app_init_event_queue();`
    - That function create a queue that contains the events which is received from FreeRTOS CLI.

      ```C
      typedef void* sl_queue_t;
      static sl_queue_t main_app_event_queue = NULL;

      static void main_app_init_event_queue(void)
      {
        main_app_event_queue = (void *)xQueueCreate(SL_EVENT_QUEUE_LEN, sizeof(sl_queue_message_t));
      }
      ```

  - Put init main event to the `main_app_event_queue`.

### 3.1. Infinite loop that handle events

- We use the `while(1){}` to catch all event from user. This while loop get new event from the queue `main_app_event_queue` every time it finish handling a event before.
  - `main_app_get_event(&event, &data, &data_len);` will block task until the queue is pushed a new item or the `portMAX_DELAY` timeout. This function based on `xQueueReceive()` FreeRTOS API.

- After get new event, we use switch case to handle it. The main app event includes:

```C
typedef enum {
  SL_MAIN_EVENT_INIT                 = 0, /**< Initiation event */
  SL_MAIN_EVENT_WAKEUP_ALARM         = 1, /**< Wake-up by RTC Alarm event */
  SL_MAIN_EVENT_DIRECT_SHADE_CONTROL = 2, /**< Direct shade control event */
  SL_MAIN_EVENT_LOCAL_SHADE_CONTROL  = 3  /**< Local shade control event */
} sl_main_app_state_e;
```

- Events maybe have commands to control something. The `SL_MAIN_EVENT_DIRECT_SHADE_CONTROL` `SL_MAIN_EVENT_LOCAL_SHADE_CONTROL` events also carry on some messages that indicates some commands that the user want to perform. for example, these events have many commands to control shade directly or locally.

```C
typedef enum {
  SL_SHADE_COMMAND_STOP              = 0x10,
  SL_SHADE_COMMAND_JOG               = 0x11,
  SL_SHADE_COMMAND_MOVE_UP           = 0x12,
  SL_SHADE_COMMAND_MOVE_DOWN         = 0x13,
  SL_SHADE_COMMAND_MOVE_PERCENT      = 0x14,
  SL_SHADE_COMMAND_MOVE_PRESET       = 0x15,
  SL_SHADE_COMMAND_MOVE_HOME         = 0x16,
  SL_SHADE_COMMAND_STORE_PRESET      = 0x17,
  SL_SHADE_COMMAND_BATTERY_PERCENT   = 0x22,
  SL_SHADE_COMMAND_ENTER_LIMIT_MODE  = 0x30,
  SL_SHADE_COMMAND_EXIT_LIMIT_MODE   = 0x31,
  SL_SHADE_COMMAND_CANCEL_LIMIT_MODE = 0x32,
  SL_SHADE_COMMAND_SET_HOME          = 0x33,
  SL_SHADE_COMMAND_START_INCLUSION   = 0x34,
  SL_SHADE_COMMAND_STOP_INCLUSION    = 0x35
} sl_shade_command_e;
```

### 3.2. Main Initialization Event

- The event will be pushed one time before our program join to the infinity loop. What does it do?
  - 1. First of all, initialize the Bluetooth stack with Low Energy Mode using `intialize_bt_stack(STACK_BTLE_MODE);` API of the sdk.
  - 2. Make BLE task: `sl_ble_app_task`.
  - 3. Make Wireless Driver task: `rsi_wireless_driver_task`.
    - This API is provided by the SDK. It handle driver events. Called in application main loop for non-OS platform. With OS, this API is blocking and with bare-metal this API is non-blocking.
  - 4. Make WLAN task: `sl_wlan_app_task`.
  - 5. Initialize WiSeConnect:
    - `rsi_wireless_init(uint16_t opermode, uint16_t coex_mode);` With WLAN Operating mode is `Client mode` and Coexistence mode is `WLAN + BLE`
      - Initialize WiSeConnect or Module features. This is a blocking API.
      - `rsi_driver_init()` followed by `rsi_device_init()` API needs to be called before this API.

  - 6. Initialize BLE app: `sl_ble_app_init();`
  - 5. Init FreeRTOS CLI to handle user command: `sl_cli_init();`

## 4. BLE app

### 4.1. Initialize BLE app

## 5. WLAN app

## 6. Command line interface

### 6.1. Initialize CLI

- To initialize CLI, we need to register command with the OS:

```C
  cli_register_cmd(&reset_command);
  cli_register_cmd(&heap_command);
  cli_register_cmd(&time_command);
  cli_register_cmd(&sntp_command);
  cli_register_cmd(&wlan_scan_command);
  cli_register_cmd(&wlan_connect_command);
  cli_register_cmd(&wlan_disconnect_command);
  cli_register_cmd(&sched_list_command);
  cli_register_cmd(&sched_add_command);
  cli_register_cmd(&sched_remove_command);
  cli_register_cmd(&sched_enable_command);
  cli_register_cmd(&sched_disable_command);
```

- Every command have the name, help, param and callback function:

```C
typedef struct {
  char *name;                         /**< Command name string */
  char *help;                         /**< Command help string */
  sl_cli_command_callback_t callback; /**< Command callback function */
  int nb_param;                       /**< Number of parameters */
} sl_cli_command_t;
``

- Every command is managed by an object:

```C
static sl_cli_int_cmd_t sl_cli_cmd_registered[SL_CLI_NB_MAX_CMD] = { 0 };
```
