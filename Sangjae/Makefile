# project name (generate executable with this name)
TARGET 		= test.out


CC 			= g++
# compiling flags here
CFLAGS 		= -g -std=c++11 -pthread -W -Wall


LINKER 		= g++
# linking flags here
LFLAGS 		=  -g -std=c++11 -pthread -W -Wall


# change these to proper directories where each file should be
SRCDIR 		= src
SUBDIR 		= $(SRCDIR)/fault $(SRCDIR)/mem_global $(SRCDIR)/mem_local $(SRCDIR)/utils
OBJDIR 		= obj
OUTDIR 		= bin

SOURCES 		= $(wildcard $(SRCDIR)/*.cc $(foreach fd, $(SUBDIR), $(fd)/*.cc))
INCLUDES 		= $(wildcard $(SRCDIR)/*.cc $(foreach fd, $(SUBDIR), $(fd)/*.hh))
NOTDIR_SOURCES 	= $(notdir $(SOURCES))

OBJECTS 		= $(addprefix $(OBJDIR)/, $(SOURCES:cc=o))
INC_DIRS 		= $(addprefix -I, $(SUBDIR))


$(TARGET): $(OBJECTS)
	$(LINKER) $(LFLAGS) -o $@ $(OBJECTS) 
	@echo "Linking Complete!!"

$(OBJDIR)/%.o: %.cc $(INCLUDE)
	@mkdir -p $(@D)
	$(CC) -o $@ -c $< $(CFLAGS) $(INC_DIRS)
	@echo "Compiled "$<" Succesfully!!"

.PHOYNY: clean
clean:
	rm -rf $(OBJDIR)/* *.out
	@echo "Cleanup Complete!!"

echoes:
	@echo "INCLUDE files: $(INCLUDES)"
	@echo "SOURCE files: $(SOURCES)"
	@echo "OBJECT files: $(OBJECTS)" 
