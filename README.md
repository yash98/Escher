# Escher

## Matrix Library Executable Usage
all numbering is done according to arguments received in `char* argv[]`

Format of any command:
`./mat functionName arguments`

0. executable
1. function name
2. arguments to functions as stated below

### Function Names
- #### convolution   
    **Ex:** `./mat convolution input.txt numRowsOfInput kernel.txt numRowsOfKernel pad matrixMult`  

    *arguments:*   
    2. input file   
    3. number of rows in input image    
    4. kernel file  
    5. number of rows in kernel    
    6. write `pad` - for applying enough padding to keep image size maintained. `noPad` - to apply no padding     
    7. method of convolutions: simpleConvol, matrixMult 

- #### pooling
    **Ex:** `./mat pooling input.txt numRowsofInput maxpool poolingSize`

    *arguments:*    
    2. input file   
    3. number of rows in input file     
    4. type of pooling. `avgpool` - to apply average pooling `maxpool` - to apply max pooling   
    5. size of pooling 

- #### nonLinearActivation
    **Ex:** `./mat nonLinearActivation input.txt numRowsofInput `relu`

    *arguments:*    
    2. input file   
    3. number of rows in input file     
    4. type of activation function  


For compiling: first run `make` command     
Arguments 