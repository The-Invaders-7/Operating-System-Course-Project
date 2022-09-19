# Operating-System-Course-Project

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
