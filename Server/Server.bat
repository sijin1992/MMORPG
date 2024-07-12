echo WScript.sleep 500>sss.vbs

start MMORPGdbServer.bat
sss.vbs
start MMORPGGateServer.bat
sss.vbs
start MMORPGLoginServer.bat