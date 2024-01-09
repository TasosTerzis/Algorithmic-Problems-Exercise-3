# folder paths
INCLUDE = ./include
INPUT = ./input
MAIN = ./main
MODULES = ./modules
OBJ = obj/
GRAPH_SRC = $(MODULES)/graph
LSH_SRC = $(MODULES)/lsh
CUBE_SRC = $(MODULES)/cube
COMMON_SRC = $(MODULES)/common
UTILITIES_SRC = $(MODULES)/utilities
OUTPUT = ./output

# compiler g++
CC = g++

# files .o
OBJS_LSH = $(OBJ)mainLsh.o $(OBJ)hashLSH.o $(OBJ)helpers.o \
		$(OBJ)utilitiesLsh.o $(OBJ)funcG.o $(OBJ)funcH.o \
		$(OBJ)dataset.o $(OBJ)image.o
		
OBJS_HYPERCUBE = $(OBJ)mainCube.o $(OBJ)hypercube.o $(OBJ)funcH.o \
		$(OBJ)dataset.o $(OBJ)hashtable.o $(OBJ)helpers.o \
		$(OBJ)utilitiesCube.o $(OBJ)image.o

OBJS_GRAPH = $(OBJ)mainGraph.o  $(OBJ)dataset.o $(OBJ)graph.o\
		$(OBJ)helpers.o $(OBJ)utilitiesGraph.o $(OBJ)utilitiesLsh.o \
		$(OBJ)funcG.o $(OBJ)funcH.o $(OBJ)hashLSH.o $(OBJ)image.o

OBJS_NET = $(OBJ)mainNet.o  $(OBJ)dataset.o $(OBJ)graph.o\
		$(OBJ)helpers.o $(OBJ)utilitiesGraph.o $(OBJ)utilitiesLsh.o \
		$(OBJ)funcG.o $(OBJ)funcH.o $(OBJ)hashLSH.o $(OBJ)image.o

# executables
EXEC_LSH = mainLsh
EXEC_HYPERCUBE = mainCube
EXEC_GRAPH = mainGraph
EXEC_NET = mainNet

# arguments 
ARGS_LSH = -d $(INPUT)/10k_images.dat -q $(INPUT)/100from60k.dat -k 4 -L 7 -o $(OUTPUT)/lsh.log -N 1 -R 2000
ARGS_CUBE = -d $(INPUT)/10k_images.dat -q $(INPUT)/100from60k.dat -k 11 -M 120 -probes 140 -o $(OUTPUT)/cube.log -N 1 -R 2500
ARGS_GRAPH = -d $(INPUT)/10k_images.dat -q $(INPUT)/100from60k.dat -k 50 -E 35 -R 6 -N 1 -l 200 -m 2 -o $(OUTPUT)/graph.log
ARGS_NET = -d $(INPUT)/10k_images.dat -q $(INPUT)/100from60k.dat -k 50 -E 35 -R 6 -N 3 -l 300 -m 1 -o $(OUTPUT)/graph.log 		 -rd $(INPUT)/10k_images_REDUCED.dat -rq $(INPUT)/100from60k_REDUCED.dat

# Create rules to build object files in the obj/ directory
$(OBJ)%.o: $(MAIN)/%.cpp
	$(CC) -c $< -o $@
$(OBJ)%.o: $(LSH_SRC)/%.cpp
	$(CC) -c $< -o $@
$(OBJ)%.o: $(CUBE_SRC)/%.cpp
	$(CC) -c $< -o $@
$(OBJ)%.o: $(GRAPH_SRC)/%.cpp
	$(CC) -c $< -o $@
$(OBJ)%.o: $(COMMON_SRC)/%.cpp
	$(CC) -c $< -o $@
$(OBJ)%.o: $(UTILITIES_SRC)/%.cpp
	$(CC) -c $< -o $@

# dependencies
$(EXEC_LSH): $(OBJS_LSH)
	$(CC) $^ -o $@ $(LDFLAGS)
$(EXEC_HYPERCUBE): $(OBJS_HYPERCUBE)
	$(CC) $^ -o $@ $(LDFLAGS)
$(EXEC_GRAPH): $(OBJS_GRAPH)
	$(CC) $^ -o $@ $(LDFLAGS)
$(EXEC_NET): $(OBJS_NET)
	$(CC) $^ -o $@ $(LDFLAGS)

# compile & run
runLsh: $(EXEC_LSH)
	time ./$(EXEC_LSH) $(ARGS_LSH)
runCube: $(EXEC_HYPERCUBE)
	time ./$(EXEC_HYPERCUBE) $(ARGS_CUBE)  
runGraph: $(EXEC_GRAPH)
	time ./$(EXEC_GRAPH) $(ARGS_GRAPH)
runNet: $(EXEC_NET)
	time ./$(EXEC_NET) $(ARGS_NET)

# clean all .o, executables, logs
clean:
	rm -f $(OBJ)*.o $(EXEC_LSH)  $(EXEC_HYPERCUBE) $(EXEC_GRAPH) $(EXEC_NET) $(OUTPUT)/*.log

# run to check memory leaks
valgrindLsh: $(EXEC_LSH)
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all ./$(EXEC_LSH) $(ARGS_LSH)
valgrindCube: $(EXEC_HYPERCUBE)
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all ./$(EXEC_HYPERCUBE) $(ARGS_CUBE)	
valgrindGraph: $(EXEC_GRAPH)
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all ./$(EXEC_GRAPH) $(ARGS_GRAPH)
valgrindNet: $(EXEC_NET)
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all ./$(EXEC_NET) $(ARGS_NET)