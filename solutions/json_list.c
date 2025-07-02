#include <stdlib.h>
#include <string.h>
#include "json_list.h"


JObject* parse_json(const char *filename) {
	JObject *head = NULL, *tail = NULL;
	json_error_t error;
	json_t *root = json_load_file(filename, 0, &error);
	if (!root) {
		fprintf(stderr, "Error JSON-File: %s\n", error.text);
		return NULL;
	}
	if (!json_is_array(root)) {
		fprintf(stderr, "No Array in Json\n");
		json_decref(root);
		return NULL;
	}

	for (size_t i = 0; i < json_array_size(root); i++) {
		json_t *obj = json_array_get(root, i);
		if (!json_is_object(obj)) continue;

		json_t *name = json_object_get(obj, "name");
		json_t *age = json_object_get(obj, "age");
		if (!json_is_string(name) || !json_is_integer(age)) continue;

		JObject *new_node = malloc(sizeof(JObject));
		if (!new_node) continue;
		new_node->name = strdup(json_string_value(name));
		new_node->age = json_integer_value(age);
		new_node->next = NULL;

		if (!head) {
			head = tail = new_node;
		} else {
			tail->next = new_node;
			tail = new_node;
		}
	}
	json_decref(root);
	return head;
}

void print_list(JObject *head) {
	for (JObject *p = head; p; p = p->next) {
		printf("Name: %s, Age: %d\n", p->name, p->age);
	}
}
void free_list(JObject *head) {
	JObject *current = head;
	while (current) {
		JObject *next = current->next;
		free(current->name); 
		free(current);       
		current = next;
}
}
