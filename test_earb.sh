#!/bin/bash

make

# Standard
# diff ./tests/input1_EARB.expected <(./memsim ./tests/input1.trace 4096 8 EARB 2)
# diff ./tests/input2_EARB.expected <(./memsim ./tests/input2.trace 4096 8 EARB 1)
# diff ./tests/input3_EARB.expected <(./memsim ./tests/input3.trace 4096 12 EARB 4)
# diff ./tests/input4_EARB.expected <(./memsim ./tests/input4.trace 1024 128 EARB 16)

# Detailed debug
diff ./tests/input1_EARB.detailed <(./memsim ./tests/input1.trace 4096 8 EARB 2)
# diff ./tests/input2_EARB.detailed <(./memsim ./tests/input2.trace 4096 8 EARB 1)
