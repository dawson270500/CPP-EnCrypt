# CPP-EnCrypt
A basic encryption program that can be used from the command line.

# Features
 - Encrypt files into .enc files
 - Decrypt files back, current stable version does not know the original file name so it must be specified 

# Usage

CPP-EnCrypt.exe -f#\<file\> -p#\<password\> [-encrypt / -decrypt] {-o#\<output file\>}  
Arguements: 
  - -f#\<file\>: The file to work on, replace <file> with path to file
  - -p#\<password\>: The password to encrypt or decrypt the file with
  - -encrypt: Encrypt the file, cannot be used when -decrypt is used Output file cannot be set when encrypting
  - -decrypt: Decrypt the file, cannot be used when -encrypt it used. Output file is required for decrypting
  - -o#\<output file\>: The file to output to, cannot be set when encrypting, is required when decrypting
