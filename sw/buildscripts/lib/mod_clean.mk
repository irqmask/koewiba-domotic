include targets.mk

CLEANDIRS_DEBUG = $(SUBDIRS:%=cleandebug-%)
CLEANDIRS_RELEASE = $(SUBDIRS:%=cleanrelease-%)

clean: clean_debug clean_release

clean_debug: $(CLEANDIRS_DEBUG)
$(CLEANDIRS_DEBUG):
	@$(MAKE) -f Makefile -k clean_debug -s -C $(@:cleandebug-%=%)

clean_release: $(CLEANDIRS_RELEASE)
$(CLEANDIRS_RELEASE):
	@$(MAKE) -f Makefile -k clean_release -s -C $(@:cleanrelease-%=%)

helpclean:
	@echo ""
	@echo "    List of cleaning-targets:"
	@echo ""
	@echo "        clean               - executes all steps"
	@echo ""
	@echo "        clean_debug         - cleans debug-build (object and target files)"
	@echo ""
	@echo "        clean_release       - cleans release-build (object and target files)"
	@echo ""


.PHONY: clean clean_debug clean_release helpclean
