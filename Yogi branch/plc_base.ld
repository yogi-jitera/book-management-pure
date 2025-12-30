--[ システム起動と初期化 ]--
--| |--( )--
  POWER_ON        ST_SYS_READY
--| |--[ RST ]--
  ST_SYS_READY    ERR_ALL_RESET
--[ モード管理 ]--
--| |--+--|/|--+--|/|--+--|/|--( )--
  IN_AUTO    IN_MANUAL    IN_MAINT    IN_DIAG    MODE_AUTO
--| |--+--|/|--+--|/|--+--|/|--( )--
  IN_MANUAL  IN_AUTO      IN_MAINT    IN_DIAG    MODE_MANUAL
--| |--+--|/|--+--|/|--+--|/|--( )--
  IN_MAINT   IN_AUTO      IN_MANUAL   IN_DIAG    MODE_MAINT
--| |--+--|/|--+--|/|--+--|/|--( )--
  IN_DIAG    IN_AUTO      IN_MANUAL   IN_MAINT   MODE_DIAG
--[ ライン1：原料供給＋整列 ]--
--| |--( )--
  MODE_AUTO    FEED_1_ON
--| |--( )--
  FEED_1_ON  ALIGN_1_READY
--| |--( )--
  ALIGN_1_READY  CONV_1_ON
--[ ライン2：原料供給＋整列 ]--
--| |--( )--
  MODE_AUTO    FEED_2_ON
--| |--( )--
  FEED_2_ON  ALIGN_2_READY
--| |--( )--
  ALIGN_2_READY  CONV_2_ON
--[ ライン3：原料供給＋整列 ]--
--| |--( )--
  MODE_AUTO    FEED_3_ON
--| |--( )--
  FEED_3_ON  ALIGN_3_READY
--| |--( )--
  ALIGN_3_READY  CONV_3_ON
--[ ライン4：原料供給＋整列 ]--
--| |--( )--
  MODE_AUTO    FEED_4_ON
--| |--( )--
  FEED_4_ON  ALIGN_4_READY
--| |--( )--
  ALIGN_4_READY  CONV_4_ON
--[ ライン5：原料供給＋整列 ]--
--| |--( )--
  MODE_AUTO    FEED_5_ON
--| |--( )--
  FEED_5_ON  ALIGN_5_READY
--| |--( )--
  ALIGN_5_READY  CONV_5_ON
--[ ライン1：加工ステップ1 ]--
--| |--( )--
  CONV_1_ON    STEP_1_1
--| |--[ TMR ]--( )--
  STEP_1_1    T_110    DONE_1_1
--[ ライン1：加工ステップ2 ]--
--| |--( )--
  STEP_1_1    STEP_1_2
--| |--[ TMR ]--( )--
  STEP_1_2    T_120    DONE_1_2
--[ ライン1：加工ステップ3 ]--
--| |--( )--
  STEP_1_2    STEP_1_3
--| |--[ TMR ]--( )--
  STEP_1_3    T_130    DONE_1_3
--[ ライン1：加工ステップ4 ]--
--| |--( )--
  STEP_1_3    STEP_1_4
--| |--[ TMR ]--( )--
  STEP_1_4    T_140    DONE_1_4
--[ ライン1：加工ステップ5 ]--
--| |--( )--
  STEP_1_4    STEP_1_5
--| |--[ TMR ]--( )--
  STEP_1_5    T_150    DONE_1_5
--[ ライン2：加工ステップ1 ]--
--| |--( )--
  CONV_2_ON    STEP_2_1
--| |--[ TMR ]--( )--
  STEP_2_1    T_210    DONE_2_1
--[ ライン2：加工ステップ2 ]--
--| |--( )--
  STEP_2_1    STEP_2_2
--| |--[ TMR ]--( )--
  STEP_2_2    T_220    DONE_2_2
--[ ライン2：加工ステップ3 ]--
--| |--( )--
  STEP_2_2    STEP_2_3
--| |--[ TMR ]--( )--
  STEP_2_3    T_230    DONE_2_3
--[ ライン2：加工ステップ4 ]--
--| |--( )--
  STEP_2_3    STEP_2_4
--| |--[ TMR ]--( )--
  STEP_2_4    T_240    DONE_2_4
--[ ライン2：加工ステップ5 ]--
--| |--( )--
  STEP_2_4    STEP_2_5
--| |--[ TMR ]--( )--
  STEP_2_5    T_250    DONE_2_5
--[ ライン3：加工ステップ1 ]--
--| |--( )--
  CONV_3_ON    STEP_3_1
--| |--[ TMR ]--( )--
  STEP_3_1    T_310    DONE_3_1
--[ ライン3：加工ステップ2 ]--
--| |--( )--
  STEP_3_1    STEP_3_2
--| |--[ TMR ]--( )--
  STEP_3_2    T_320    DONE_3_2
--[ ライン3：加工ステップ3 ]--
--| |--( )--
  STEP_3_2    STEP_3_3
--| |--[ TMR ]--( )--
  STEP_3_3    T_330    DONE_3_3
--[ ライン3：加工ステップ4 ]--
--| |--( )--
  STEP_3_3    STEP_3_4
--| |--[ TMR ]--( )--
  STEP_3_4    T_340    DONE_3_4
--[ ライン3：加工ステップ5 ]--
--| |--( )--
  STEP_3_4    STEP_3_5
--| |--[ TMR ]--( )--
  STEP_3_5    T_350    DONE_3_5
--[ ライン4：加工ステップ1 ]--
--| |--( )--
  CONV_4_ON    STEP_4_1
--| |--[ TMR ]--( )--
  STEP_4_1    T_410    DONE_4_1
--[ ライン4：加工ステップ2 ]--
--| |--( )--
  STEP_4_1    STEP_4_2
--| |--[ TMR ]--( )--
  STEP_4_2    T_420    DONE_4_2
--[ ライン4：加工ステップ3 ]--
--| |--( )--
  STEP_4_2    STEP_4_3
--| |--[ TMR ]--( )--
  STEP_4_3    T_430    DONE_4_3
--[ ライン4：加工ステップ4 ]--
--| |--( )--
  STEP_4_3    STEP_4_4
--| |--[ TMR ]--( )--
  STEP_4_4    T_440    DONE_4_4
--[ ライン4：加工ステップ5 ]--
--| |--( )--
  STEP_4_4    STEP_4_5
--| |--[ TMR ]--( )--
  STEP_4_5    T_450    DONE_4_5
--[ ライン5：加工ステップ1 ]--
--| |--( )--
  CONV_5_ON    STEP_5_1
--| |--[ TMR ]--( )--
  STEP_5_1    T_510    DONE_5_1
--[ ライン5：加工ステップ2 ]--
--| |--( )--
  STEP_5_1    STEP_5_2
--| |--[ TMR ]--( )--
  STEP_5_2    T_520    DONE_5_2
--[ ライン5：加工ステップ3 ]--
--| |--( )--
  STEP_5_2    STEP_5_3
--| |--[ TMR ]--( )--
  STEP_5_3    T_530    DONE_5_3
--[ ライン5：加工ステップ4 ]--
--| |--( )--
  STEP_5_3    STEP_5_4
--| |--[ TMR ]--( )--
  STEP_5_4    T_540    DONE_5_4
--[ ライン5：加工ステップ5 ]--
--| |--( )--
  STEP_5_4    STEP_5_5
--| |--[ TMR ]--( )--
  STEP_5_5    T_550    DONE_5_5
--[ 検査工程 ]--
--| |--( )--
  DONE_1_5 OR DONE_2_5 OR DONE_3_5 OR DONE_4_5 OR DONE_5_5    INSPECT_START
--| |--( )--
  INSPECT_PASS    EJECT_GOOD
--| |--( )--
  INSPECT_FAIL    EJECT_REJECT
--| |--[ +1 ]--( )--
  EJECT_GOOD    COUNT_GOOD
--| |--[ +1 ]--( )--
  EJECT_REJECT  COUNT_REJECT
--[ 排出とパレタイズ ]--
--|>=|--( )--
  COUNT_GOOD >= 500    PALLET_FULL
--| |--( )--
  PALLET_FULL    PALLET_LOAD_ON
--| |--[ RST ]--
  RESET_PALLET   COUNT_GOOD
--| |--( )--
  PALLET_LOAD_ON    PALLET_PLACING
--[ 異常検知と自動復旧 ]--
--| |--+--|/|--[ TMR ]--( )--
  CONV_1_ON  SENSOR_1_OK    T_ERR1    ERR_TIMEOUT1
--| |--+--| |--( )--
  ERR_TIMEOUT1  RETRY_MODE1    RETRY1_ON
--| |--[ RST ]--
  RESET_ERR1  ERR_TIMEOUT1
--[ アナログ処理：温度制御 ]--
--|>|--( )--
  TEMP_RAW > 750   TEMP_HIGH
--|<|--( )--
  TEMP_RAW < 250   TEMP_LOW
--| |--( )--
  TEMP_HIGH    COOLER_ON
--| |--( )--
  TEMP_LOW     HEATER_ON
--[ 積算管理とメンテ通知 ]--
--| |--[ +1 ]--( )--
  DONE_5_5    RUN_HOURS
--|>=|--( )--
  RUN_HOURS >= 10000    MAINT_ALERT
--| |--( )--
  MAINT_ALERT    INDICATOR_MAINT
--| |--[ RST ]--
  RESET_HOURS    RUN_HOURS
--[ 手動バイパス操作 ]--
--| |--( )--
  MODE_MANUAL    BYPASS_FEED
--| |--( )--
  MODE_MANUAL    BYPASS_PROC
--| |--( )--
  MODE_MANUAL    BYPASS_EJECT