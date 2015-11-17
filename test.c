#include <string.h>
#include <glib.h>
#include <stdlib.h>
#include <stdio.h>

int main() {

	GHashTable *ht = g_hash_table_new(g_int_hash, g_int_equal);
	int a = 10;
	int b = 100;
	int a1 = 10;

	gint *key = g_new(gint, 1);
	gint *value = g_new(gint, 1);
	*value = 1000;
	*key = 10;
	g_hash_table_insert(ht, key, value);
	gint *value1 = g_new(gint, 1);
	if (g_hash_table_lookup(ht, &a1) != NULL) {
		value1 = g_hash_table_lookup(ht, key);
	}

	printf("Key:%d and value %d", *key, *value1);
}
