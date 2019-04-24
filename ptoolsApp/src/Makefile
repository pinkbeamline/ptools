TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# build a support library

LIBRARY_IOC += ptools

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxxRecord
# install ptools.dbd into <top>/dbd
DBD += ptools.dbd

#ptools_DBD += ptsup.dbd

# specify all source files to be compiled and added to the library
ptools_SRCS += ptsup.c

ptools_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

