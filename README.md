# Operating-System-Course-Project

## Multiprogramming Operating System(MOS) Phase 1

Currently the Phase 1 instructions are completed and we are working on making them more error free.

Everyone, please make sure that you work on separate branches so we can later merge them after reviewing changes.

ASSUMPTIONS:
- Jobs entered without error in input file
- No physical separation between jobs
- Job outputs separated in output file by 2 blank lines
- Program loaded in memory starting at location 00 
- No multiprogramming, load and run one program at a time
- SI interrupt for service request

NOTATION: 
- M: memory; IR: Instruction Register (4 bytes) 
- IR [1, 2]: Bytes 1, 2 of IR/Operation Code 
- IR [3, 4]: Bytes 3, 4 of IR/Operand Address
- M[&]: Content of memory location & 
- IC: Instruction Counter Register (2 bytes)
- R: General Purpose Register (4 bytes) 
- C: Toggle (1 byte) 
- : Loaded/stored/placed into 

## Multiprogramming Operating System(MOS) Phase 2

ASSUMPTIONS:
- Jobs may have program errors
- PI interrupt for program errors introduced
- No physical separation between jobs
- Job outputs separated in output file by 2 blank lines
- Paging introduced, page table stored in real memory
- Program pages allocated one of 30 memory block using random number generator 
- Load and run one program at a time
- Time limit, line limit, out-of-data errors introduced
- TI interrupt for time-out error introduced
- 2-line messages printed at termination

NOTATION:
- M: memory
- IR: Instruction Register (4 bytes)
- IR [1, 2]: Bytes 1, 2 of IR/Operation Code
- IR [3, 4]: Bytes 3, 4 of IR/Operand Address 
- M[&]: Content of memory location &
- IC: Instruction Counter Register (2 bytes)
- R: General Purpose Register (4 bytes)
- C: Toggle (1 byte)
- PTR: Page Table Register (4 bytes)
- PCB: Process Control Block (data structure)
- VA: Virtual Address
- RA: Real Address
- TTC: Total Time Counter
- LLC: Line Limit Counter
- TTL: Total Time Limit
- TLL: Total Line Limit
- EM: Error Message

## Multiprogramming Operating System(MOS) Phase 3
