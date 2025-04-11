all:
	if ! which i686-w64-mingw32-gcc > /dev/null 2>&1; then echo "Please install MingW32-gcc"; exit 1; fi
	mkdir -p bin
	cp -af auto-idle-action.ini bin/auto-idle-action.ini
	i686-w64-mingw32-gcc -o bin/auto-idle-action.exe auto-idle-action.c -mwindows
