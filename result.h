#include <stdbool.h>

struct Result {
	void * value;
	bool is_error;
};


#define ERROR(msg) (struct Result) {msg, true }
#define SOME(data) (struct Result) {data, false }
