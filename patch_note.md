## V3.4.2.4_Patch_0002

### Features
* Add a new API "blc_att_modifyReadRspData" to set the respond data and its length when getting the ATT table information.

### Bug Fixes
* Fixed: When enabling the macro "APP_FLASH_PROTECTION_ENABLE", and an error firmware is used for OTA process, the MCU may crash with a very low probability.
* Fixed: The local device acting as the Peripheral role connects with the peer device acting as the Central role. When the local device terminate the connection, the terminate reason is "HCI_ERR_CONN_TERM_BY_LOCAL_HOST" rather than "HCI_ERR_REMOTE_USER_TERM_CONN".

### BREAKING CHANGES
* N/A.



### Features
* 新增API“blc_att_modifyReadRspData”来设置获取ATT表信息时的响应数据及其长度。

### Bug Fixes
* 修复：当使能宏“APP_FLASH_PROTECTION_ENABLE”，并且在OTA时使用了一个错误的firmware，极低概率下MCU可能卡死。
* 修复：作为Peripheral角色的本端设备与Central角色的对端设备连接，如果本端设备主动断连，断连原因应为“HCI_ERR_CONN_TERM_BY_LOCAL_HOST”而不是“HCI_ERR_REMOTE_USER_TERM_CONN”。

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
