## V3.4.2.2


### Features
* SDK version:telink_b85m_ble_single_connection_sdk_V3.4.2.2
* Normalizes the naming of content related to private schemas.


### BREAKING CHANGES
* N/A.


### Features
* SDK 版本:telink_b85m_ble_single_connection_sdk_V3.4.2.2
* 规范化与私有模式相关内容的命名

### BREAKING CHANGES
* N/A.


## V3.4.2.1_Patch_0006

### Features
* SDK version：telink_b85m_ble_single_connection_sdk_v3.4.2.1_patch_0006.
* Support P25Q80U for B87.


### Bug Fixes
* Fix an issue that very few B85-chips may run abnormally when the clock is 48M.


### BREAKING CHANGES
* N/A.



### Features
* SDK 版本：telink_b85m_ble_single_connection_sdk_v3.4.2.1_patch_0005.
* B87支持P25Q80U.


### Bug Fixes
* 修复了少数b85芯片使用48M时钟时可能运行异常的问题.


### BREAKING CHANGES
* N/A.



## V3.4.2.1_Patch_0005

### Features
* SDK version：telink_b85m_ble_single_connection_sdk_v3.4.2.1_patch_0005.
* Add MCU_STALL function, use bls_adv_peak_current_optimize API to decrease ADV_power.
* Optimize ADV function, use bls_adv_decrease_time_optimize API to decrease ADV_power.


### Bug Fixes
* Fix EXT_ADV_Bug: it can not advertise in some cases.
* Fix an issue: secure_connect failed in some specific cases.
* Fix an issue: ADV_interval is not 150us correctly while SCANNING_IN_ADV_AND_CONN_SLAVE_ROLE.


### BREAKING CHANGES
* N/A.



### Features
* SDK 版本：telink_b85m_ble_single_connection_sdk_v3.4.2.1_patch_0005.
* 增加MCU_STALL功能，使用bls_adv_peak_current_optimize，开启此功能可降低广播功耗。
* 优化广播功能，使用bls_adv_decrease_time_optimize，开启此功能可降低广播功耗。


### Bug Fixes
* 修复扩展广播bug：一些情况下广播不出来的问题。
* 修复问题：secure connect在一些特定情况下会失败。
* 修复问题：在SCANNING_IN_ADV_AND_CONN_SLAVE_ROLE下ADV_interval 150us时间不准。


### BREAKING CHANGES
* N/A.




## V3.4.2.1_Patch_0004

### Features
* SDK version：telink_b85m_ble_single_connection_sdk_v3.4.2.1_patch_0004.
* Add write request reject API.
* The maximal SMP bond number is changed to 8.
* Support 8238.

### Bug Fixes
* Fix an issue: the IRK of the same device may change


### BREAKING CHANGES
* N/A.



### Features
* SDK 版本：telink_b85m_ble_single_connection_sdk_v3.4.2.1_patch_0004.
* 增加write request reject API.
* 最大SMP bond number修改为8。
* 支持8238


### Bug Fixes
* 修复一个问题：相同设备的IRK未保持一致。


### BREAKING CHANGES
* N/A.




## V3.4.2.1_Patch_0003

### Features
* SDK version：telink_b85m_ble_single_connection_sdk_v3.4.2.1_patch_0003.


### Bug Fixes
* Fix issue when update firmware from version before 3.4.2.0 to 3.4.2.0 and later by OTA, user may fail to reconnect and have to re-pair product.


### BREAKING CHANGES
* N/A.



### Features
* SDK 版本：telink_b85m_ble_single_connection_sdk_v3.4.2.1_patch_0003.


### Bug Fixes
* 修复一个问题：当客户程序从3.4.2.0之前的版本通过OTA升级到3.4.2.0及以后，会遇到无法重连，只能重新配对。


### BREAKING CHANGES
* N/A.



## V3.4.2.1_Patch_0002

### Features
* SDK version：telink_b85m_ble_single_connection_sdk_v3.4.2.1_patch_0002.


### Bug Fixes
* Fix boundary conditions issue of ATT.


### BREAKING CHANGES
* N/A.



### Features
* SDK 版本：telink_b85m_ble_single_connection_sdk_v3.4.2.1_patch_0002.


### Bug Fixes
* 增加处理ATT边界条件下的错误.


### BREAKING CHANGES
* N/A.



## V3.4.2.1_Patch_0001

### Features
* SDK version：telink_b85m_ble_single_connection_sdk_v3.4.2.1_patch_0001.


### Bug Fixes
* Add parameter cheack when doing EXCHANGE_MTU_SIZE .


### BREAKING CHANGES
* N/A.



### Features
* SDK 版本：telink_b85m_ble_single_connection_sdk_v3.4.2.1_patch_0001.


### Bug Fixes
* 增加处理EXCHANGE_MTU_SIZE前的参数监测.


### BREAKING CHANGES
* N/A.

## V3.4.2.1


### Features
* SDK version:telink_b85m_ble_single_connection_sdk_V3.4.2.1
* Compatible with Linux 


### BREAKING CHANGES
* N/A.


### Features
* SDK 版本:telink_b85m_ble_single_connection_sdk_V3.4.2.1
* 增加Linux编译支持 

### BREAKING CHANGES
* N/A.

