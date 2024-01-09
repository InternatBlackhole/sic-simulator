#!/bin/bash

ASM=${1?Error: No assembley file given}

# Run the assembler
java -cp ./sictools.jar sic.Asm $ASM
