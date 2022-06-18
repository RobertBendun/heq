#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Max(A, B) ((A) >= (B) ? (A) : (B))

enum Value_Type
{
	Symbol,
	Number,
	Quotation
};

typedef struct Deque {
	struct Value *values;
	size_t count;
	size_t capacity;
} Deque;

typedef struct Value {
	enum Value_Type type;
	union
	{
		int number;
		char symbol[Max(16, sizeof(Deque))];
		Deque deque;
	};
} Value;

Value* read(FILE *f)
{
	Value *result;

	char format[16];
	char input[sizeof(result->symbol) + 1] = {};
	int input_count, read_count;

	// sizeof(input) - 1 since fscanf expects to read at most this number,
	// and it always put zero after it
	snprintf(format, 16, "%%%zus%%n", sizeof(input) - 1);

	if (fscanf(f, format, input, &read_count) == 0 || read_count == 0 || !input[0])
		return NULL;

	if ((input_count = strlen(input)) == sizeof(result->symbol)) {
		fprintf(stderr, "ERRROR: Word cannot have size greater then %zu\n", sizeof(result->symbol));
		exit(0);
	}

	result = malloc(sizeof(Value));
	assert(result);
	sscanf(input, "%d%n", &result->number, &read_count);
	if (read_count == input_count) {
		result->type = Number;
	} else {
		result->type = Symbol;
		memcpy(result->symbol, input, input_count);
	}
	return result;
}

void dump(Value *const value)
{
	switch (value->type)
	{
	case Number: printf("Number(%d)", value->number); break;
	case Symbol: printf("Symbol(%s)", value->symbol); break;
	case Quotation:
		assert(0 && "unimplemented");
	}
}

int main()
{
	Value *v = NULL;
	while ((v = read(stdin))) {
		dump(v);
		puts("");
	}
}
