#!/bin/bash

make

# Standard
diff ./tests/input1_ESC.expected  <(./memsim ./tests/input1.trace 4096 8 ESC)
diff ./tests/input2_ESC.expected  <(./memsim ./tests/input2.trace 4096 8 ESC)
diff ./tests/input3_ESC.expected  <(./memsim ./tests/input3.trace 4096 12 ESC)
diff ./tests/input4_ESC.expected  <(./memsim ./tests/input4.trace 1024 128 ESC)

# Detailed debug
# diff ./tests/input1_ESC.detailed  <(./memsim ./tests/input1.trace 4096 8 ESC)
# diff ./tests/input2_ESC.detailed  <(./memsim ./tests/input2.trace 4096 8 ESC)
