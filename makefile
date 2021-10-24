all:
	g++ -finstrument-functions -O0 -o rpmain main.cpp silver.cpp
	objdump -S rpmain | grep ">:" > dump
	g++ reader.cpp -o reader
