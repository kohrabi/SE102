@echo off
set "ext=tmx"
for %%f in (*.%ext%) do (
    echo CollisionGenerator.exe Wall %%f %%~nf.txt
	CollisionGenerator.exe Wall %%f %%~nf.txt
	
)