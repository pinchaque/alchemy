########################################################################
# Assumes ROOT is set to the source tree root

ifndef ROOT
$(error Variable ROOT is not defined)
endif

include $(ROOT)/mk/common.mk

########################################################################
# Intermediate Files
OBJECTS = $(addprefix $(OBJDIR)/,$(subst .cpp,.o,$(SOURCES)))

TEST_TARGET = $(OBJDIR)/unittest
TEST_OBJECTS = \
	$(addprefix $(OBJDIR)/,$(subst .cpp,.o,$(TEST_SOURCES))) \
	$(OBJDIR)/TestMain.o


########################################################################
# Public Targets

.PHONY: all clean package test testclean

all:
	$(MAKE) package
	$(MAKE) test


package:
	$(MAKE) $(DIRS)
	$(MAKE) $(TARGET)


clean: packageclean testclean


packageclean:
	$(RM) $(OBJECTS) $(TARGET)


test:
ifdef TEST_SOURCES
	$(MAKE) $(TEST_TARGET)
	LD_LIBRARY_PATH=$(LIBDIR):$$LD_LIBRARY_PATH $(TEST_TARGET)
else
	@echo "No unit tests available in package $(PACKAGE)"
endif

testclean:
ifdef TEST_SOURCES
	$(RM) $(TEST_OBJECTS) $(TEST_TARGET)
else
	@echo "No unit tests available in package $(PACKAGE)"
endif


########################################################################
# Dependency Handling

DF = $(DEPDIR)/$(*F)
MAKEDEPEND = touch $(DF).d && makedepend -Y -p$(OBJDIR)/ $(CPPFLAGS) $(INCLUDES) -f $(DF).d $< 2>/dev/null
BUILDDEP = $(MAKEDEPEND); \
cp $(DF).d $(DF).P; \
sed -e 's/\#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
    -e '/^$$/ d' -e 's/$$/ :/' < $(DF).d >> $(DF).P; \
    rm -f $(DF).d $(DF).d.bak

DEPS = $(addprefix $(DEPDIR)/, $(addsuffix .P, $(basename $(SOURCES))))

ifdef TEST_SOURCES
DEPS += $(addprefix $(DEPDIR)/, $(addsuffix .P, $(basename $(TEST_SOURCES))))
DEPS += $(DEPDIR)/TestMain.P
endif

-include $(DEPS)


########################################################################
# Private Targets

$(LIBDIR)/lib$(PACKAGE).so: $(OBJECTS)
	$(LD) $(LDSOFLAGS) $+ -o $@

$(BINDIR)/$(PACKAGE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $+ -o $@

$(OBJDIR)/%.o: %.cpp
	@$(BUILDDEP)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(DIRS):
	mkdir -p $@

$(TEST_TARGET): $(TEST_OBJECTS) $(OBJECTS)
	$(CXX) $(LDFLAGS) $+ -lcppunit -ldl -o $@
