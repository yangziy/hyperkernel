# Hyperkernel

See recent updates and contact information at:
https://unsat.cs.washington.edu/projects/hyperkernel/

This is a playground with kernel designs and ideas for formal
verification.  Don't use it for production.

## How to run hv6

We have tested running hv6 with the following setup:

- Linux Ubuntu 17.10
- Binutils 2.29.1
- GCC 7.2.0
- QEMU 2.10.1

Install these packages before proceeding.  Other platforms or
versions may not work.

To compile:

	make

To run in QEMU:

	make qemu

Try a few commands in shell:

	ls
	ps
	pstree
	readylist
	wttr

By default a web server starts on boot.  Point your browser to
http://localhost:10080/sh.html.

To run on real hardware, make sure your machine has VT-x (VMX) and
VT-d (IOMMU) support and compatible devices (for E1000 we tested
using Intel I217-LM).  To get an ISO:

	make iso

You can use it for PXE booting or creating a bootable USB.

## How to verify hv6

We have tested verification with the following setup:

- Linux Ubuntu 17.10
- LLVM 5.0.0
- Z3 4.5.0
- Python 2.7.10

`make hv6-verify`:
    Runs the verification scripts for the hv6 kernel. This includes building the kernel
    into LLVM IR, translating the kernel to Python using Irpy, and invoking `hv6/spec/main.py`.
    Individual tests can be run, for example, to run just the `sys_set_runnable` test, invoke:
    `make hv6-verify -- -v --failfast HV6.test_sys_set_runnable`

`make irpy/test`:
    Runs the Irpy test suite, which compares symbolic execution results to running the C code
    directly.

The proof guarantees that the system call handlers are a refinement
of our state-machine specification in `hv6/spec/`.  It also shows
that the state-machine specification satisfies certain high-level
properties, such as process isolation or the correctness of reference
counters.

This does not mean that the kernel is guaranteed to have zero bugs.
There can be bugs in unverified code (e.g., initialization and glue
code), the specification (or things not modeled in the specification),
or the verification toolchain including `irpy/` and Z3.

The current verification time is roughly 30 min on our test machine
(quad-core i7-7700K).  This is longer than the number reported in
the paper, as we have added more lemmas since then, and several
lemmas are unnecessarily proved more than once (the verifier is not
smart enough to cache the results).

## Directory structure

`hv6/`:
    Contains the implementation of the hv6 kernel

`hv6/spec/`:
    Contains the specification for the hv6 kernel

`irpy/compiler/`:
    Contains the implementation of the IR -> Python compiler used for
    symbolic execution

`irpy/libirpy/`:
    Contains the Python library and resources for performing symbolic
    execution over Python generated by the Irpy compiler

A few quick pointers:

- syscalls: `hv6/syscall.c`
- state-machine specification: `hv6/spec/kernel/spec/specs.py`
- declarative specification: `hv6/spec/kernel/spec/top.py`
- kernel configuration: `kernel/config.h`

## Detailed directory structure (Added by Zion 11/16/2021)
Feel free to modify or add details.

```
├── Makefile
├── README.md                               
├── boot                                    Bootloader?
├── drivers                                 QEMU driver?
├── hv6                                     
│   ├── Makefrag                            
│   ├── arch                                ?
│   ├── spec
│   │   ├── base.py                         ?
│   │   ├── kernel                          Specifications of the hv6 kernel
│   │   │   ├── datatypes.py               
│   │   │   ├── main.py                     unittest Runner
│   │   │   ├── spec                        
│   │   │   │   ├── __init__.py
│   │   │   │   └── equiv.py                Equivalence relation between the state-transition specification and the LLVM-IR-Z3 (defined below).
│   │   │   │   ├── helpers.py              ?
│   │   │   │   ├── invariants.py           ?
│   │   │   │   ├── specs.py                State-transition specification
│   │   │   │   └── top.py                  Declarative specification
│   │   │   └── syscall_spec.py
│   │   └── user                            
│   │       ├── datatypes.py
│   │       ├── main.py
│   │       └── spec.py                     State-transition specification
|   |
│   ├── user                                C source of the user space applications
|   └── ...                                 C source of the kernel
├── include
│   └── uapi                                Something about QEMU hardware?
├── irpy
│   ├── Makefrag
│   ├── compiler                            LLVM-IR to Z3 description compiler. We call the output LLVM-IR-Z3. It's Z3 data structure embedded in Python.
│   ├── libirpy                             
|   |   ├── ctx.py                          Export a high-level Python interface for the manipulation of LLVM-IR-Z3
|   |   ├── datatypes.py                    Types used in the state-transition specification
|   |   ├── itypes.py                       Types used in the LLVM-IR-Z3
|   |   ├── server.py                       Proxy of the main Z3 solver process
|   |       solver_utils.py 
|   |       solver.py 
|   |   ├── util.py 
|   |   ├── z3eval.py                       (?) Evaluator of the LLVM-IR-Z3?
|   |   └── eval.py                         Base class of the file above
│   ├── test                                
│   └── o.test                              
├── kernel                                  (?) Unverified hardware specific part of the kernel
├── lib                                     Standard C library
├── o.x86_64                                
│   ├── hv6                                 Built directory of the Hyperkernel OS. 
        └── hv6py/kernel/impl.py            LLVM-IR-Z3 of the verified part of the kernel.
│   └── irpy                                Built directory of irpy/libirpy/compiler. 
│       └── compiler                        
├── scripts                                 (?) At what cases will those scripts be useful?
├── user                                    (?)
└── web                                     Files associated with the web server
```

## License

Code borrowed from other sources keeps the original copyright and license.

Files we created are licensed under the Apache License, Version 2.0,
viewable at http://www.apache.org/licenses/LICENSE-2.0, and are
marked as such.

In the copyright notices where we refer to "Hyperkernel Authors",
we mean Luke Nelson, Helgi Sigurbjarnarson, Kaiyuan Zhang, Dylan Johnson,
James Bornholt, Emina Torlak, and Xi Wang.

## Acknowledgments

hv6 borrows code from the following sources:

- xv6: many of the source files in the hv6/ directory

- sv6
  - hv6/user/ns.c
  - kernel/tsc.c

- FreeBSD
  - include/uapi/assym.h
  - kernel/intel_iommu.c
  - kernel/intel_iommu.h
  - kernel/svm.c
  - kernel/svm.h
  - kernel/vmx.c
  - kernel/vmx.h
  - lib/stdlib.c
  - lib/string.c
  - scripts/genassym.sh

- Linux
  - include/uapi/machine/trap_support.h

- NetBSD
  - hv6/user/fs/nvmedisk.c
  - include/uapi/nvme.h
  - include/uapi/pcireg.h
  - include/uapi/queue.h

- QEMU
  - include/uapi/e1000.h

- lwIP: files under hv6/user/lwip/ (and user/lwip/)

- ISOLINUX: binaries under boot/isolinux/

- Linux binaries statically linked with uClibc
  - Dune's bench: hv6/user/linux/bench_linux
  - sparse's compile: hv6/user/linux/compile
  - gzip: hv6/user/linux/gzip
  - lua (patched with Linenoise): hv6/user/linux/lua
  - sha1sum: hv6/user/linux/sha1sum
  - tcc: hv6/user/linux/tcc

- Terminus font: hv6/user/linux/ter-x16n.psf

- JavaScript libraries
  - D3.js: web/d3.v4.min.js
  - jQuery: web/jquery.min.js
