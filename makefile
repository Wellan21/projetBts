# Compilateur
CXX = g++

# Options de compilation
CXXFLAGS = -Wall -g

# Options de l'éditeur de liens
LDFLAGS = -lwiringPi -lsqlite3

# Fichiers source
SRCS = clientSocket.cpp cmodelisateur.cpp main.cpp

# Fichiers d'en-tête
HDRS = clientSocket.h cmodelisateur.h

# Fichiers objets
OBJS = $(SRCS:.cpp=.o)

# Nom de l'exécutable
EXEC = demonstrateur

# Cible par défaut
all: $(EXEC)

# Lier les fichiers objets pour créer l'exécutable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compiler les fichiers source en fichiers objets
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cible de nettoyage pour supprimer les fichiers compilés
clean:
	rm -f $(OBJS) $(EXEC)

# Cibles fictives (phony) pour éviter les conflits avec des fichiers du même nom
.PHONY: all clean
