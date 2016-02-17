@echo This batch file will link the main data dir to the android build dir
@echo --

cd GameEmptyReplaceMe/Game

cd SourceAndroid/AndroidApk
mkdir assets
cd assets
mklink /J "Data" "../../../Data"

pause