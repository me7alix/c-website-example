#define HTMPL_IMPLEMENTATION
#include "../thirdparty/htmpl.h"

int main(void) {
	HTMPL_StringBuilder tb = {0};
	tmpls_builder_compile_template(&tb, "./files/index.htmpl");
	tmpls_builder_write(&tb, "./src/impls.c");
	tmpls_builder_destroy(&tb);
	return 0;
}
