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
