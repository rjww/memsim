#!/bin/bash

make

# Second chance
diff ./tests/input1_SC.expected   <(./memsim ./tests/input1.trace 4096 8 SC)
diff ./tests/input2_SC.expected   <(./memsim ./tests/input2.trace 4096 8 SC)
diff ./tests/input3_SC.expected   <(./memsim ./tests/input3.trace 4096 12 SC)
diff ./tests/input4_SC.expected   <(./memsim ./tests/input4.trace 1024 128 SC)

# Enhanced second chance
diff ./tests/input1_ESC.expected  <(./memsim ./tests/input1.trace 4096 8 ESC)
diff ./tests/input2_ESC.expected  <(./memsim ./tests/input2.trace 4096 8 ESC)
diff ./tests/input3_ESC.expected  <(./memsim ./tests/input3.trace 4096 12 ESC)
diff ./tests/input4_ESC.expected  <(./memsim ./tests/input4.trace 1024 128 ESC)

# Additional reference bits
diff ./tests/input1_ARB.expected  <(./memsim ./tests/input1.trace 4096 8 ARB 2)
diff ./tests/input2_ARB.expected  <(./memsim ./tests/input2.trace 4096 8 ARB 1)
diff ./tests/input3_ARB.expected  <(./memsim ./tests/input3.trace 4096 12 ARB 4)
diff ./tests/input4_ARB.expected  <(./memsim ./tests/input4.trace 1024 128 ARB 16)

# Enhanced additional reference bits
diff ./tests/input1_EARB.expected <(./memsim ./tests/input1.trace 4096 8 EARB 2)
diff ./tests/input2_EARB.expected <(./memsim ./tests/input2.trace 4096 8 EARB 1)
diff ./tests/input3_EARB.expected <(./memsim ./tests/input3.trace 4096 12 EARB 4)
diff ./tests/input4_EARB.expected <(./memsim ./tests/input4.trace 1024 128 EARB 16)
