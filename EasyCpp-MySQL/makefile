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

-include $(OBJ:%=$(DEP_DIR)/%.d)
