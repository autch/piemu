
FlashRipper.exe: FlashRipper.cpp
	cl /Fe$@ $<

mkpfi.exe: mkpfi.cpp
	cl /Fe$@ $<

pffs.exe: pffs.cpp CAuPFFS.h CAuPFIFile.h psi.h ../pfi.h
	cl /GX /Fe$@ $< -I../include/
