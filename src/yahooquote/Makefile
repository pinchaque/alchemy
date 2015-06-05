ROOT = ../..

include $(ROOT)/mk/common.mk

BINS = \
	$(BINDIR)/yahooquote.pl \
	$(BINDIR)/yahoolist.pl \
	$(BINDIR)/yahooinfo.pl \

all package:
	$(MAKE) dirs
	$(MAKE) $(BINS)

clean:
	$(RM) $(BINS)


$(BINDIR)/%: ./%
	$(CP) $< $@

dirs:
	mkdir -p $(BINDIR)
