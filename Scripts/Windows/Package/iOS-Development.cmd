set Platform=IOS
set ClientConfig=Development

call .\DoPackageGame.cmd -deploy -device=IOS@All_iOS_On_%computername%

pause
