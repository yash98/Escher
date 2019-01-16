# Escher

## Matrix Library Executable Usage
all numbering is done according to arguments recieved in `char* argv[]`

Format of any command:
`./mat functionName arguments`

0. executable
1. function name
2. arguments to functions as stated below

### Function Names
- #### convolution   
    **Ex:** `./mat convolution input.txt numRowsOfInput kernel.txt numRowsOfKernel pad matrixMult`  

    *arguments:*   
    2. input image file   
    3. number of rows in input image    
    4. kernel file  
    5. number of rows in kernel    
    6. write `pad` - for applying enough padding to keep image size maintained. `noPad` - to apply no padding     
    7. method of convolutions: simpleConvol, matrixMult 