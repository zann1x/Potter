if not exist "%JENKINS_WS%" (
    mkdir "%JENKINS_WS%"
)
"%UE_LOC%\Engine\Binaries\DotNET\UnrealBuildTool.exe" -projectfiles -project="%JENKINS_WS%\GetaGameJam7.uproject" -game -rocket -progress
