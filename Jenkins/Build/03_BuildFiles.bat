if not exist "%JENKINS_BUILD%" (
	mkdir "%JENKINS_BUILD%"
)
"%UE_LOC%\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun -project="%JENKINS_WS%\GetaGameJam7.uproject" -noP4 -platform=Win64 -clientconfig=Development -cook -allmaps -build -stage -pak -archive -archivedirectory="%JENKINS_BUILD%"
