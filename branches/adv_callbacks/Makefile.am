ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src include/aprilui
EXTRA_DIST = autogen.sh

lib_LTLIBRARIES = libaprilui.la
libaprilui_la_SOURCES = 
libaprilui_la_LIBADD =  $(LIBOBJS) \
	src/libaprilui_src.la
libaprilui_la_CPPFLAGS = $(XML_CFLAGS)
libaprilui_la_LDFLAGS = -version-info @LT_VERSION@ $(XML_LDFLAGS)

