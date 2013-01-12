TESTS = check_cli
check_PROGRAMS = check_cli
check_cli_SOURCES = check_cli.c $(top_builddir)/src/cliopt.h
check_cli_CFLAGS = @CHECK_CFLAGS@
check_cli_LDADD = $(top_builddir)/src/libhardview.a @CHECK_LIBS@

bin_PROGRAMS = OsCollectorTest SessionTest
OsCollectorTest_SOURCES = OsCollectorTest.cpp
OsCollectorTest_CXXFLAGS = $(BOOST_CPPFLAGS) -I ../src
OsCollectorTest_LDADD = $(top_builddir)/src/libnewview.a
SessionTest_SOURCES = SessionTest.cpp
SessionTest_CXXFLAGS = $(BOOST_CPPFLAGS) -I ../src
SessionTest_LDADD = $(top_builddir)/src/libnewview.a
