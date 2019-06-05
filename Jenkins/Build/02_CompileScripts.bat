call "%UE_LOC%\Engine\Build\BatchFiles\GetMSBuildPath.bat"
%MSBUILD_EXE% "%JENKINS_WS%\GetaGameJam7.sln" /t:build /p:Platform=Win64;verbosity=diagnostic
