TESTS = check_cli
check_PROGRAMS = check_cli
check_cli_SOURCES = check_cli.c $(top_builddir)/src/cliopt.h
check_cli_CFLAGS = @CHECK_CFLAGS@
check_cli_LDADD = $(top_builddir)/src/libhardview.a @CHECK_LIBS@

AM_CXXFLAGS = $(BOOST_CPPFLAGS) -I ../src
LDADD = $(top_builddir)/src/libnewview.a

bin_PROGRAMS = OsCollectorTest SessionTest
OsCollectorTest_SOURCES = OsCollectorTest.cpp
SessionTest_SOURCES = SessionTest.cpp
