#include <hermit/syscall.h>
#include <hermit/logging.h>
#include <asm/page.h>
#include <asm/uhyve.h>
#include <hermit/minifs.h>

typedef struct {
	int fd;
    off_t length;
	int ret;
} __attribute__((packed)) uhyve_ftruncate_t;

int sys_ftruncate(int fd, off_t length) {
	uhyve_ftruncate_t arg;

	//if(minifs_enabled)
	//	return minifs_ftruncate(fd, length);

	arg.fd = fd;
	arg.length = length;
	arg.ret = -1;

	uhyve_send(UHYVE_PORT_FTRUNCATE, (unsigned)virt_to_phys((size_t)&arg));  // fct qui permet de faire l'hypercall vers l'hyperviser

	return arg.ret;
}

//***********
// AIDE PROF
//***********

// chercher dans le code de hermitux où on fait appel a sys_creat pour savoir où est le handler de syscall

// on a un pointeur en paramètre de sys_creat -> un peu délicat, il faut utiliser virt_to_phys

// dans tools/uhyve.c, contient majeure partie du code du VMM
// cf vcpu_loop : fct dans laquelle hyperviseur donne controle a la VM
// s'execute un peu comme while(true) { lancer VM(); gerer le trap;} ie tant qu'il n'y a pas de trap la VM s'execute
// cf UHYVE_PORT_CREAT et faire la meme chose