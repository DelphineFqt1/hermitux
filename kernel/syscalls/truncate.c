#include <hermit/syscall.h>
#include <hermit/logging.h>
#include <asm/page.h>
#include <asm/uhyve.h>
#include <hermit/minifs.h>

typedef struct {
	const char* path;
    off_t length;
	int ret;
} __attribute__((packed)) uhyve_truncate_t;

int sys_truncate(const char* path, off_t length) {
	uhyve_truncate_t arg;

	//if(minifs_enabled)
	//	return minifs_truncate(path, length);

	arg.path = (char *)virt_to_phys((size_t)path);
	arg.length = length;
	arg.ret = -1;

	uhyve_send(UHYVE_PORT_TRUNCATE, (unsigned)virt_to_phys((size_t)&arg));  // fct qui permet de faire l'hypercall vers l'hyperviser

	return arg.ret;
}
