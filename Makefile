TOP=../..
include $(TOP)/configure/CONFIG

#
# Tested on the following host architectures
#
ifeq ($(findstring Host,$(VALID_BUILDS)),Host)
LIB_INSTALLS = castuff.oct
endif

MYINSTALLS = caput.oct caget.oct cagetnelem.oct \
             casetmonitor.oct caclrmonitor.oct canewmonval.oct \
             cagetTimeout.oct cagetRetryCount.oct \
             casetTimeout.oct casetRetryCount.oct

include $(TOP)/configure/RULES

ifeq ($(findstring Host,$(VALID_BUILDS)),Host)
buildInstall: $(addprefix $(INSTALL_LIB)/,$(MYINSTALLS))
endif

castuff.oct:../castuff.cc
	$(CP) ../castuff.cc castuff.cc
	mkoctfile -o $@ \
        $(INCLUDES) \
        $(addprefix -L,$(EPICS_BASE_LIB)) \
        $(addprefix -L,$(EPICS_EXTENSIONS_LIB)) \
        -lezca $(addprefix -l,$(EPICS_BASE_HOST_LIBS)) castuff.cc

$(INSTALL_LIB)/%.oct: $(INSTALL_LIB)/castuff.oct
	$(RM) $@
ifdef WIN32
	$(CP) castuff.oct $@
else
	ln -s castuff.oct $@
endif
