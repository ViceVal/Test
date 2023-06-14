#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10

typedef struct {
    char* key;
    int value;
} Entry;

typedef struct {
    int size;
    Entry* table;
} HashTable;

int hash_function(const char* key, int table_size) {
    int i;
	int hash_value = 0;
    int length = strlen(key);
    for (i = 0; i < length; i++) {
        hash_value += key[i];
    }
    return hash_value % table_size;
}

HashTable* create_table(int size) {
    int i;
	HashTable* hash_table = (HashTable*)malloc(sizeof(HashTable));
    hash_table->size = size;
    hash_table->table = (Entry*)malloc(sizeof(Entry) * size);
    for (i = 0; i < size; i++) {
        hash_table->table[i].key = NULL;
    }
    return hash_table;
}

void insert(HashTable* hash_table, const char* key, int value) {
    int index = hash_function(key, hash_table->size);

    // Verificar si la ranura está vacía
    if (hash_table->table[index].key == NULL) {
        hash_table->table[index].key = strdup(key);
        hash_table->table[index].value = value;
    } else {
        // Resolver colisión utilizando el método de división
        int next_index = (index + 1) % hash_table->size;
        while (next_index != index && hash_table->table[next_index].key != NULL) {
            next_index = (next_index + 1) % hash_table->size;
        }

        // Insertar en la siguiente ranura disponible
        if (next_index == index) {
            printf("La tabla hash está llena. No se puede insertar el elemento.\n");
        } else {
            hash_table->table[next_index].key = strdup(key);
            hash_table->table[next_index].value = value;
        }
    }
}

int search(HashTable* hash_table, const char* key) {
    int index = hash_function(key, hash_table->size);

    // Verificar si la ranura inicial contiene el elemento buscado
    if (hash_table->table[index].key != NULL && strcmp(hash_table->table[index].key, key) == 0) {
        return hash_table->table[index].value;
    } else {
        // Buscar en las siguientes ranuras hasta encontrar el elemento o determinar que no está presente
        int next_index = (index + 1) % hash_table->size;
        while (next_index != index) {
            if (hash_table->table[next_index].key != NULL && strcmp(hash_table->table[next_index].key, key) == 0) {
                return hash_table->table[next_index].value;
            }
            next_index = (next_index + 1) % hash_table->size;
        }
    }

    // Elemento no encontrado
    return -1;
}

void free_table(HashTable* hash_table) {
    int i;
	for (i = 0; i < hash_table->size; i++) {
        if (hash_table->table[i].key != NULL) {
            free(hash_table->table[i].key);
        }
    }
    free(hash_table->table);
    free(hash_table);
}

int main() {
    HashTable* hash_table = create_table(TABLE_SIZE);

    // Insertar elementos
    insert(hash_table, "Juan", 25);
    insert(hash_table, "Bob", 32);
    insert(hash_table, "Charlie", 19);
    insert(hash_table, "David", 41);

    // Buscar elementos
    printf("%d\n", search(hash_table, "Juan"));    // Resultado: 25
    printf("%d\n", search(hash_table, "Bob"));      // Resultado: 32
    printf("%d\n", search(hash_table, "Charlie"));  // Resultado: 19
    printf("%d\n", search(hash_table, "David"));      // Resultado: -1 (no encontrado)

    free_table(hash_table);

    return 0;
}

