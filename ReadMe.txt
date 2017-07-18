auto_monitor.conf:
dbProcessName:dmserver    不变 
dbLogDir:/home/d5000/dmdbms/log #unuse now 暂时不用到
myDbLog:/home/d5000/xinjiang/var/log/monitor_db 配置日志,如果~var/log/不可写或不存在可配置到其他文件夹中 monitor_db是日志名称
dbType:DM602  //不变
dbIp:127.0.0.1  //配置到本机上
dbPort:12345  //端口 
dbUser:SYSDBA  // 数据库用户名
dbPwd:SYSDBA   //数据库密码