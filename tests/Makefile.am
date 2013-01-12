TESTS = check_cli
check_PROGRAMS = check_cli
check_cli_SOURCES = check_cli.c $(top_builddir)/src/cliopt.h
check_cli_CFLAGS = @CHECK_CFLAGS@
check_cli_LDADD = $(top_builddir)/src/libhardview.a @CHECK_LIBS@

bin_PROGRAMS = OsCollectorTest
OsCollectorTest_SOURCES = OsCollectorTest.cpp
OsCollectorTest_CPPFLAGS = $(BOOST_CPPFLAGS) -I ../src
# This just adds -R/usr/local/lib -L/usr/local/lib to the linker; not sure why
OsCollectorTest_LDADD = $(top_builddir)/src/libnewview.a
