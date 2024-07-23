echo WScript.sleep 1000>sss.vbs

start MMORPGdbServer.bat
sss.vbs
start MMORPGCenterServer.bat
sss.vbs
start MMORPGGateServer.bat
sss.vbs
start MMORPGLoginServer.bat