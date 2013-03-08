AM_CXXFLAGS = $(BOOST_CPPFLAGS) -I ../src -std=c++11 -g -O0

# Use this if you want to link dynamically.  During dev, this creates
# ugly wrapper scripts around the executable.
# LDADD = $(top_builddir)/src/libsmbios.la
# Otherwise, use this:
LDADD = $(top_builddir)/src/libsmbios.a

bin_PROGRAMS = smbios
smbios_SOURCES = SmBiosTest.cpp
