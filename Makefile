
-include config.mk

ifndef MSKHOME
$(error Configuration not properly defined. Please run the './setup.sh' script.)
else ifndef MSKVERSION
$(error Configuration not properly defined. Please run the './setup.sh' script.)
else ifndef PLATFORM
$(error Configuration not properly defined. Please run the './setup.sh' script.)
endif

MSKPATH=$(MSKHOME)/mosek/$(MSKVERSION)/tools/platform/$(PLATFORM)

BUILDPATH=./build/



# Compile all

.PHONY : all

all: CWC_MOSEK GCWF

# Include makefiles for different platforms 
# And execute based on the configuration file.
ifeq ($(PLATFORM),osx64x86)
include osx64x86.mk
else ifeq ($(PLATFORM),linux64x86)
include linux64x86.mk
else
	$(error No platform is assigned. Stop make.)
endif


# Clean

.PHONY : clean

clean:
	-rm -rf $(BUILDPATH)CWC $(BUILDPATH)CWC_Lin\
		$(BUILDPATH)CWC.dSYM $(BUILDPATH)CWC_Lin.dSYM\
		$(BUILDPATH)LCL $(BUILDPATH)EAV $(BUILDPATH)Grid
	
clean_MOSEK:
	make clean -C $(MSKPATH)/src/fusion_cxx
