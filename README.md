# virtual-stack

This project emulates a simple virtual machine by storing and referencing variables (in stack frame contexts) and then reading in a set of pseudo-assembly instructions which dictate what operations should be performed on the stack. 

We emulate an 8-bit architecture. That is, our RAM contains 2<sup>8</sup> addresses of 1 byte each and the register bank contains 8 registers of 1 byte each. No information about the program's execution state is stored outside of the RAM and register bank.

The virtual machine operates on a pseudo-assembly language called ``x2017``. The virtual machine runs on binaries in this language. There are two executables: ``objdump_x2017`` takes a ``x2017`` binary and produces it in human readable assembly format, and ``vm_x2017`` takes a ``x2017`` binary and performs its operations. The work done by ``objdump_x2017`` in producing the binary in a human readable form is used in ``vm_x2017``.

Information on ``x2017``, the operations it supports and the implementation of the stack within the RAM can be found in the code comments. 
