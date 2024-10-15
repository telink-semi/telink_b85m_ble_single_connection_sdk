## V3.4.2.4_Patch_0002

### Features
* **ATT**
    - Add a new API "blc_att_modifyReadRspData" to set the respond data and its length when getting the ATT table information.

### Bug Fixes
* **OTA**
    - Fixed: When enabling the macro "APP_FLASH_PROTECTION_ENABLE", and an error firmware is used for OTA process, the MCU may crash with a very low probability.
    - Fixed: The local device acting as the Peripheral role OTA with the peer device acting as the Central role. The local device delays sending connection terminate after OTA success to avoid some peer device incorrectly identifying OTA failure.

* **Application**
    - Fixed: The local device acting as the Peripheral role connects with the peer device acting as the Central role. When the local device terminate the connection, the terminate reason is "HCI_ERR_CONN_TERM_BY_LOCAL_HOST" rather than "HCI_ERR_REMOTE_USER_TERM_CONN".
    - Fixed: Fix "feature_phy_test", "feature_privacy_slave", "feature_ota_hid" and "feature_soft_uart" compile error.
    - Fixed: Modify the bit[1] of the analog register "DEEP_ANA_REG0" in "b85m_ble_sample" and "b85m_ble_remote" to avoid conflicting with driver.

* **Others**
    - Fixed: (B85) Restore system clock source back to 24M RC when entering "cpu_wakeup_init" to prevent abnormal reboots which may occur due to multiple calls the "cpu_wakeup_init".
    - Fixed: When both the local device and the peer device use the resolvable private address(RPA), the identity address(IDA) type and the identity address in the interaction packets should be the same as their initial setting(public/random).

### Refactoring
* **Link**
    - Clean the link warning "warning: dot moved backwards before '.text'".

* **Others**
    - Update the early wakeup time of SUSPEND sleep mode.
    - Optimize the RX flow control.

### BREAKING CHANGES
* N/A.



### Features
* **ATT**
    - 新增API“blc_att_modifyReadRspData”来设置获取ATT表信息时的响应数据及其长度。

### Bug Fixes
* **OTA**
    - 修复：当使能宏“APP_FLASH_PROTECTION_ENABLE”，并且在OTA时使用了一个错误的firmware，极低概率下MCU可能卡死。
    - 修复：作为Peripheral角色的本地设备与Central角色的对端设备进行OTA，在OTA成功后本地设备延迟发送断连请求，以避免部分对端设备错误识别为OTA失败。

* **Application**
    - 修复：作为Peripheral角色的本地设备与Central角色的对端设备连接，如果本地设备主动断连，断连原因应为“HCI_ERR_CONN_TERM_BY_LOCAL_HOST”而不是“HCI_ERR_REMOTE_USER_TERM_CONN”。
    - 修复：修复“feature_phy_test”，“feature_privacy_slave”，“feature_ota_hid”和“feature_soft_uart”的编译错误。
    - 修复：修改“b85m_ble_sample”和“b85m_ble_remote”中的模拟寄存器“DEEP_ANA_REG0”的bit[1]，以避免与driver程序冲突。

* **Others**
    - 修复：(B85)在进入“cpu_wakeup_init”时先将系统时钟源切换为24M RC，以防多次调用“cpu_wakeup_init”可能导致的异常reboot。
    - 修复：当本地设备和对端设备都使用可解析私有地址（RPA）时，交互包中的身份地址（IDA）类型和身份地址应该与其初始设置（public/random）保持一致。

### Refactoring
* **Link**
    - 清理link警告“warning: dot moved backwards before '.text'”。

* **Others**
    - 更新SUSPEND模式下提前唤醒的时间。
    - 优化RX流控。

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
