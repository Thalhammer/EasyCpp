SRC = $(shell find . -name '*.cpp') $(shell find . -name '*.c')
EXCLUDE_SRC = 
FSRC = $(filter-out $(EXCLUDE_SRC), $(SRC))
OBJ = $(FSRC:=.o)

DEP_DIR = .deps

FLAGS = -fPIC -Wall -Wno-unknown-pragmas -I../EasyCpp -I/usr/include/mysql -O2 -march=native
CXXFLAGS = -std=c++14
CFLAGS = 
LINKFLAGS = -lmysqlclient

OUTFILE = libEasyCpp_MySQL.so

ARCH := $(shell getconf LONG_BIT)
DEBVERSION = "0.0."`git rev-list HEAD --count`
DEBFOLDER = libeasycpp-mysql_$(DEBVERSION)

.PHONY: clean debug release

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
	@mkdir -p $(DEBFOLDER)/usr/lib/
	@echo "Package: libeasycpp-mysql" >> $(DEBFOLDER)/DEBIAN/control
	@echo "Version: $(DEBVERSION)" >> $(DEBFOLDER)/DEBIAN/control
	@echo "Section: devel" >> $(DEBFOLDER)/DEBIAN/control
	@echo "Priority: optional" >> $(DEBFOLDER)/DEBIAN/control
ifeq ($(ARCH),64)
	@echo "Architecture: amd64" >> $(DEBFOLDER)/DEBIAN/control
else
	@echo "Architecture: i386" >> $(DEBFOLDER)/DEBIAN/control
endif
	@echo "Depends: libeasycpp, libmysqlclient20" >> $(DEBFOLDER)/DEBIAN/control
	@echo "Maintainer: Dominik Thalhammer <dominik@thalhammer.it>" >> $(DEBFOLDER)/DEBIAN/control
	@echo "Description: MySQL Database driver for EasyCpp." >> $(DEBFOLDER)/DEBIAN/control
	@cp $(OUTFILE) $(DEBFOLDER)/usr/lib/libeasycpp-mysql.so.$(DEBVERSION)
	@ln -s libeasycpp-mysql.so.$(DEBVERSION) $(DEBFOLDER)/usr/lib/libeasycpp-mysql.so
	@fakeroot dpkg-deb --build $(DEBFOLDER)

-include $(OBJ:%=$(DEP_DIR)/%.d)
