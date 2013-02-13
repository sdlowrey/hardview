AM_CPPFLAGS = -std=c++0x
noinst_LIBRARIES = libsmbios.a
libsmbios_a_SOURCES = SmBios.cpp SmElement.cpp SmReader.cpp

bin_PROGRAMS = hardview
hardview_SOURCES = hardview.cpp
hardview_LDADD = libsmbios.a
