## V3.4.2.4_Patch_0002

### Features
* N/A.

### Bug Fixes
* **OTA**
    - Fixed: When the macro "APP_FLASH_PROTECTION_ENABLE" is enabled and an error firmware is used for the OTA process, the MCU may crash with a very low probability.
    - Fixed: In the OTA process, the local device acting as the peripheral role needs to delay sending connection terminate after OTA success to avoid some peer devices incorrectly identifying OTA failure.

* **Application**
    - Fixed: The local device acting as the Peripheral role connects with the peer device acting as the Central role. When the local device terminates the connection, the terminate reason is "HCI_ERR_CONN_TERM_BY_LOCAL_HOST" instead of "HCI_ERR_REMOTE_USER_TERM_CONN".
    - Fixed: Fix the "feature_phy_test", "feature_privacy_slave", "feature_ota_hid" and "feature_soft_uart" compile errors.
    - Fixed: The bit[1] of the analog register "DEEP_ANA_REG0" has been used in driver code, so modify the bit[1] to bit[4] in "b85m_ble_sample" and "b85m_ble_remote" to avoid conflicts.

* **Others**
    - Fixed: (B85) Restore the system clock source to 24M RC when entering "cpu_wakeup_init" to prevent abnormal reboots that may occur due to multiple calls the "cpu_wakeup_init".
    - Fixed: When the local device uses the resolvable Private Address(RPA), the identity Address(IDA) in the Identity Address Information packet is incorrectly distributed as the RPA. IDA can only be a public device address or a static random address.

### Refactoring
* **Link**
    - Clean the link warning "warning: dot moved backwards before '.text'".

* **Others**
    - Optimize the early wake-up time for the SUSPEND sleep mode.
    - Optimize the RX flow control to improve the program's robustness.
    - Add the "FLASH_P25Q80U_SUPPORT_EN" macro control in ble_flash.c and custom_pair.c.

### BREAKING CHANGES
* N/A.



### Features
* N/A.

### Bug Fixes
* **OTA**
    - 修复：当使能宏“APP_FLASH_PROTECTION_ENABLE”，并且在OTA时使用了一个错误的firmware，极低概率下MCU可能卡死。
    - 修复：在OTA过程中，作为Peripheral角色的本地设备需要在OTA成功后延迟发送断连请求，以避免部分对端设备错误识别为OTA失败。

* **Application**
    - 修复：作为Peripheral角色的本地设备与Central角色的对端设备连接，如果本地设备主动断连，断连原因应为“HCI_ERR_CONN_TERM_BY_LOCAL_HOST”而不是“HCI_ERR_REMOTE_USER_TERM_CONN”。
    - 修复：修复“feature_phy_test”，“feature_privacy_slave”，“feature_ota_hid”和“feature_soft_uart”的编译错误。
    - 修复：模拟寄存器“DEEP_ANA_REG0”的bit[1]已在driver程序中被使用，所以将“b85m_ble_sample”和“b85m_ble_remote”中的bit[1]修改为bit[4]，以避免和driver程序冲突。

* **Others**
    - 修复：(B85)在进入“cpu_wakeup_init”时先将系统时钟源切换为24M RC，以防多次调用“cpu_wakeup_init”可能导致的异常reboot。
    - 修复：当本地设备使用可解析地址（RPA）时，Identity Address Information报文中的身份地址（IDA）会被错误地分发为RPA。IDA只能是公共设备地址或静态随机地址。

### Refactoring
* **Link**
    - 清理link警告“warning: dot moved backwards before '.text'”。

* **Others**
    - 优化SUSPEND模式下提前唤醒的时间。
    - 优化RX流控以提高程序的鲁棒性。
    - 在ble_flash.c和custom_pair.c中添加“FLASH_P25Q80U_SUPPORT_EN”宏控制。

### BREAKING CHANGES
* N/A.




## V3.4.2.4_Patch_0001

### Features
* N/A.


### Bug Fixes
* Fix an issue: When the read or write permission for attribute values is insufficient, the ATT Server does not respond with the ATT_ERROR_RSP PDU, causing the failure of the Service Discovery.

### BREAKING CHANGES
* N/A.



### Features
* N/A.

### Bug Fixes
* 修复：修复属性值读写权限不足时，ATT Server未回复ATT_ERROR_RSP PDU，导致服务发现失败。


### BREAKING CHANGES
* N/A.
