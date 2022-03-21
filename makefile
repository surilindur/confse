!include <win32.mak>

objson.dll:
  $(cc) $(cdebug) $(cflags) $(cvars) main.cpp
