#!/bin/bash

make

# Standard
diff ./tests/input1_SC.expected   <(./memsim ./tests/input1.trace 4096 8 SC)
diff ./tests/input2_SC.expected   <(./memsim ./tests/input2.trace 4096 8 SC)
diff ./tests/input3_SC.expected   <(./memsim ./tests/input3.trace 4096 12 SC)
diff ./tests/input4_SC.expected   <(./memsim ./tests/input4.trace 1024 128 SC)

# Detailed debug
# diff ./tests/input1_SC.detailed   <(./memsim ./tests/input1.trace 4096 8 SC)
# diff ./tests/input2_SC.detailed   <(./memsim ./tests/input2.trace 4096 8 SC)
