mkdir Intermediate\
mkdir Output\
mkdir ProjectFiles\
call "Premake/premake5" --file="Premake/premake5.lua" vs2022
start BlipBlop.sln