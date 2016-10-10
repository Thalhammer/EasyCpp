SRC = $(shell find . -name '*.cpp') $(shell find . -name '*.c')
EXCLUDE_SRC = ./external/lua/luac.c ./external/lua/lua.c
FSRC = $(filter-out $(EXCLUDE_SRC), $(SRC))
OBJ = $(FSRC:=.o)

DEP_DIR = .deps

FLAGS = -fPIC -Wall -Wno-unknown-pragmas -Iexternal -O2 -march=native
CXXFLAGS = -std=c++14
CFLAGS = 
LINKFLAGS = -lcurl -lssl

OUTFILE = libEasyCpp.so

ARCH := $(shell getconf LONG_BIT)
DEBVERSION = "0.0."`git rev-list HEAD --count`
DEBFOLDER = libeasycpp_$(DEBVERSION)

.PHONY: clean debug release package

release: $(OUTFILE)

debug: FLAGS += -g
debug: $(OUTFILE)

$(OUTFILE): $(OBJ)
	@echo Generating shared library
	@$(CXX) -shared -o $@ $^ $(LINKFLAGS)
	@echo Build done

%.cpp.o: %.cpp
	@echo Building $<
	@$(CXX) -c $(FLAGS) $(CXXFLAGS) $< -o $@
	@mkdir -p `dirname $(DEP_DIR)/$@.d`
	@$(CXX) -c $(FLAGS) $(CXXFLAGS) -MT '$@' -MM $< > $(DEP_DIR)/$@.d

%.c.o: %.c
	@echo Building $<
	@$(CC) -c $(FLAGS) $(CFLAGS) $< -o $@
	@mkdir -p `dirname $(DEP_DIR)/$@.d`
	@$(CC) -c $(FLAGS) $(CFLAGS) -MT '$@' -MM $< > $(DEP_DIR)/$@.d

clean:
	@echo Removing shared library
	@rm -f $(OUTFILE)
	@echo Removing objects
	@rm -f $(OBJ)
	@echo Removing dependency files
	@rm -rf $(DEP_DIR)

package: release
	@rm -r -f $(DEBFOLDER)
	@echo Creating package
	@mkdir -p $(DEBFOLDER)/DEBIAN
	@mkdir -p $(DEBFOLDER)/usr/include/EasyCpp/
	@mkdir -p $(DEBFOLDER)/usr/lib/
	@echo "Package: libeasycpp" >> $(DEBFOLDER)/DEBIAN/control
	@echo "Version: $(DEBVERSION)" >> $(DEBFOLDER)/DEBIAN/control
	@echo "Section: devel" >> $(DEBFOLDER)/DEBIAN/control
	@echo "Priority: optional" >> $(DEBFOLDER)/DEBIAN/control
ifeq ($(ARCH),64)
	@echo "Architecture: amd64" >> $(DEBFOLDER)/DEBIAN/control
else
	@echo "Architecture: i386" >> $(DEBFOLDER)/DEBIAN/control
endif
	@echo "Depends: libcurl3, libssl1.0.0, libstdc++6, libgcc1" >> $(DEBFOLDER)/DEBIAN/control
	@echo "Maintainer: Dominik Thalhammer <dominik@thalhammer.it>" >> $(DEBFOLDER)/DEBIAN/control
	@echo "Description: A library to make creating C++ projects easier." >> $(DEBFOLDER)/DEBIAN/control
	@find . -name $(DEBFOLDER) -prune -o -name '*.h' -exec cp --parents \{\} $(DEBFOLDER)/usr/include/EasyCpp/ \;
	@cp $(OUTFILE) $(DEBFOLDER)/usr/lib/libeasycpp.so.$(DEBVERSION)
	@ln -s libeasycpp.so.$(DEBVERSION) $(DEBFOLDER)/usr/lib/libeasycpp.so
	@fakeroot dpkg-deb --build $(DEBFOLDER)

-include $(OBJ:%=$(DEP_DIR)/%.d)
