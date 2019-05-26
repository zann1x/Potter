if not exist "%JENKINS_ARCHIVE%" (
    mkdir "%JENKINS_ARCHIVE%"
)
cd "%JENKINS_ARCHIVE%"

:: This amazing solution for creating a timestamp is taken from https://stackoverflow.com/a/23476347/8313359.
:: All credits to user https://stackoverflow.com/users/2299431/foxidrive.
for /f "tokens=2 delims==" %%a in ('wmic OS Get localdatetime /value') do set "dt=%%a"
set "YY=%dt:~2,2%" & set "YYYY=%dt:~0,4%" & set "MM=%dt:~4,2%" & set "DD=%dt:~6,2%"
set "HH=%dt:~8,2%" & set "Min=%dt:~10,2%" & set "Sec=%dt:~12,2%"

set "datestamp=%YYYY%%MM%%DD%" & set "timestamp=%HH%%Min%%Sec%"
set "fullstamp=%datestamp%%timestamp%"

"C:\Program Files\7-Zip\7z.exe" a Potter_%fullstamp%.zip "%JENKINS_BUILD%\WindowsNoEditor\*"
