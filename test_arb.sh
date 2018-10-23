#!/bin/bash

make

# Standard
diff ./tests/input1_ARB.expected  <(./memsim ./tests/input1.trace 4096 8 ARB 2)
diff ./tests/input2_ARB.expected  <(./memsim ./tests/input2.trace 4096 8 ARB 1)
diff ./tests/input3_ARB.expected  <(./memsim ./tests/input3.trace 4096 12 ARB 4)
diff ./tests/input4_ARB.expected  <(./memsim ./tests/input4.trace 1024 128 ARB 16)

# Detailed debug
# diff ./tests/input1_ARB.detailed  <(./memsim ./tests/input1.trace 4096 8 ARB 2)
# diff ./tests/input2_ARB.detailed  <(./memsim ./tests/input2.trace 4096 8 ARB 1)
