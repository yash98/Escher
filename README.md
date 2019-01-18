# Escher

## Matrix Library Executable Usage
all numbering is done according to arguments received in `char* argv[]`

Format of any command:
`./matrix functionName arguments`

0. executable
1. function name
2. arguments to functions as stated below

### Function Names
- #### convolution   
    **Ex:** `./matrix convolution input.txt numRowsOfInput kernel.txt numRowsOfKernel pad matrixMult`  

    *arguments:*   
    2. input file   
    3. number of rows in input image    
    4. kernel file  
    5. number of rows in kernel    
    6. write `pad` - for applying enough padding to keep image size maintained. `noPad` - to apply no padding     
    7. method of convolutions: simpleConvol, matrixMult 

- #### pooling
    **Ex:** `./matrix pooling input.txt numRowsofInput maxpool poolingSize`

    *arguments:*    
    2. input file   
    3. number of rows in input file     
    4. type of pooling. `avgpool` - to apply average pooling `maxpool` - to apply max pooling   
    5. size of pooling 

- #### nonLinearActivation
    **Ex:** `./matrix nonLinearActivation input.txt numRowsofInput `relu`

    *arguments:*    
    2. input file   
    3. number of rows in input file     
    4. type of activation function  

## Compiling
For simple compiling: `make`   
For debugging: `make DEBUG=1` 
For cleaning object files and executables: `make clean`

## Input files format
Space separated floats. No space before end of line.
Column Major order
```
0.0 0.3 6.0  
0.1 0.4 0.7
2.0 5.0 0.8
```