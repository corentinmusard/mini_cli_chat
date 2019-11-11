TOPTARGETS := all clean
SUBDIRS := src

$(TOPTARGETS): $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

clean: ${SUBDIRS}
	rmdir bin

.PHONY: $(TOPTARGETS) $(SUBDIRS)
