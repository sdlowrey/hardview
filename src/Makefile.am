AM_CPPFLAGS = -std=c++0x -g -O0

# Use the following for final build since it creates a shared object.
# For debugging, it's a PITA to deal with libtool 
# lib_LTLIBRARIES = libsmbios.la
# libsmbios_la_SOURCES = SmBios.cpp SmBiosBinary.cpp

# Use this to static link for dev
noinst_LIBRARIES = libsmbios.a
libsmbios_a_SOURCES = SmBios.cpp SmBiosBinary.cpp SmBiosFactory.cpp

include_HEADERS = SmBios.h SmBiosBinary.h SmBiosFactory.h

bin_PROGRAMS = smdump
smdump_SOURCES = smdump.cpp
smdump_LDADD = libsmbios.a
