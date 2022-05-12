# SortingAlgorithmComparisonUEFI
Comparing Bubble Sort, Insertion Sort and Quicksort through efi shell.


# Requirements
The program requires gnu-efi, make and GCC compiler installed.
To run the program, execute it through efi shell. Openinig it through bios or a bootloader that understands .efi files works, however the program ends as soon as it is done, resulting in instantly going back, which renders the results unreadable.  
# How to run  
make all  
copy resulting "out.efi" file into your ESP  
launch efi shell  
type "fs0:" to navigate to the ESP  
type "out.efi"
