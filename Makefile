KERNEL=hermitux-kernel/prefix/x86_64-hermit/extra/tests/hermitux
MUSL=musl/prefix/lib/libc.a
LOMP=libiomp/build/runtime/src/libomp.a

ifeq (, $(shell which /opt/hermit/bin/x86_64-hermit-gcc))
$(error "Hermit toolchain not found, please follow these instructions: \
https://github.com/ssrg-vt/hermitux#prerequisites")
endif


all: $(KERNEL) $(MUSL) $(LOMP)
	@echo ""
	@echo "--------------------------------------------------------"
	@echo "HermiTux was successfully compiled, don't forget to edit"
	@echo "tools/Makefile.template by editing the HERMITUX_BASE "
	@echo "variable as follows:"
	@echo "HERMITUX_BASE=$(PWD)"
	@echo "--------------------------------------------------------"

submodules: hermitux-kernel/ musl/ libiomp/
	git submodule init
	git submodule update

.PHONY: $(KERNEL)
$(KERNEL): hermitux-kernel/build/
	make -C hermitux-kernel/build install

hermitux-kernel/build/: submodules
	cd hermitux-kernel && git submodule init && git submodule update && \
		mkdir -p build/ && mkdir -p prefix && cd build && \
		cmake -DCMAKE_INSTALL_PREFIX=../prefix ..

.PHONY: $(MUSL)
$(MUSL): musl/prefix/
	make -C musl install

musl/prefix/: submodules
	cd musl && mkdir -p prefix && ./configure --prefix=$(PWD)/musl/prefix

.PHONY: $(LOMP)
$(LOMP): libiomp/build/
	make -C libiomp/build

libiomp/build/: submodules
	cd libiomp && mkdir -p build && cd build && \
		cmake -DLIBOMP_ENABLE_SHARED=OFF ..

clean:
	make -C hermitux-kernel/build clean
	make -C musl clean
	make -C libiomp/build clean
