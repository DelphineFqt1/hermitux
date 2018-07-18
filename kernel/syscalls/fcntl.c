#include <hermit/syscall.h>
#include <hermit/logging.h>
#include <asm/uhyve.h>
#include <asm/page.h>

typedef struct {
	unsigned int fd;
	unsigned int cmd;
	unsigned long arg;
	int ret;
} __attribute__ ((packed)) uhyve_fcntl_t;

int sys_fcntl(unsigned int fd, unsigned int cmd, unsigned long arg) {

	/* uhyve: TODO -> when arg is a pointer or not, and
	 * which size ..... */

	if(likely(is_uhyve())) {
		uhyve_fcntl_t uhyve_args = { fd, cmd, arg };
		uhyve_send(UHYVE_PORT_FCNTL, (unsigned)virt_to_phys((size_t)&uhyve_args));
		LOG_WARNING("fcntl: currently unsupported\n");
		return -ENOSYS;
		//return uhyve_args.ret;
	}

	/* qemu: TODO */
	LOG_ERROR("fcntl: not supported with qemu isle\n");
	return -ENOSYS;
}
