########################################################################
# Top-Level Makefile

# This Makefile will build everything!

PACKAGES = \
	yahooquote \
	autil \
	stockdata \
	afwk \
	nnet \
	stockalg \
	stockplot \
	stocknnet \
	alchemyportfolio \
	alchemyrisk \
	alchemydata \
	alchemylist \
	alchemyplot \
	alchemygendata \
	alchemytrain \
	alchemyprofile \
	alchemycreateprofile \

all:
	@for i in $(PACKAGES); do (cd src/$$i && $(MAKE) all) || exit 1; done

clean:
	@for i in $(PACKAGES); do (cd src/$$i && $(MAKE) clean testclean) \
	  || exit 1; done
