AM_CFLAGS=-I../include/aprilui 
AM_CXXFLAGS=-I../include/aprilui 
SUBDIRS = animators images objects
noinst_LTLIBRARIES = libaprilui_src.la
#libaprilui_src_;a_configdir = $(libdir)/aprilui/include
#libaprilui_src_la_config_DATA = config.h
libaprilui_src_la_LIBADD =  $(LIBOBJS) \
	animators/libaprilui_src_animators.la \
	images/libaprilui_src_images.la \
	objects/libaprilui_src_objects.la
libaprilui_src_la_CPPFLAGS = $(XML_CFLAGS)
libaprilui_src_la_SOURCES =  \
aprilui.cpp        Dataset.cpp    Exception.cpp  xmlHelper.cpp \
CallbackEvent.cpp  EventArgs.cpp  Util.cpp



