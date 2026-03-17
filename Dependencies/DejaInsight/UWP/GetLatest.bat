set DEJAINSIGHT=\DejaTools\main\Products\Insight\DejaLibUWP
p4 edit .\lib\...
COPY %DEJAINSIGHT%\ARM\Debug\DejaLibUWP\DejaLibUWP.lib .\lib\ARM\Debug
COPY %DEJAINSIGHT%\ARM\Release\DejaLibUWP\DejaLibUWP.lib .\lib\ARM\Release
COPY %DEJAINSIGHT%\x64\Debug\DejaLibUWP\DejaLibUWP.lib .\lib\x64\Debug
COPY %DEJAINSIGHT%\x64\Release\DejaLibUWP\DejaLibUWP.lib .\lib\x64\Release
COPY %DEJAINSIGHT%\Debug\DejaLibUWP\DejaLibUWP.lib .\lib\Win32\Debug
COPY %DEJAINSIGHT%\Release\DejaLibUWP\DejaLibUWP.lib .\libWin32\Release
