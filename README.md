# Software-Design_Term-Project
The VC1620 is a decimal computer with 100,000 words of memory. Each word consists of 12 decimal digits. (Note: the IBM 1620 used decimal numbers.)  
Output For Assembler
 
Example of a test program. (Yes, this program does not do much.)  This should be in a file.
;this is a test
        org    100
hi     read    x;comment immediately after statement       
hay  copy   y, x ; This is the another comment.
        WRITE    x; to show that operation codes are case insensitive
          bp      hi y; shows we don't have to have everything line up.
        halt

;test
x      dc      5
y      ds      1
z      ds      99
b      dc      555
a      dc      100
        end
 
Output from the test program.  For ease of debugging, the output will be written to the screen.
Symbol Table:
Symbol #     Symbol     Location
   0                 a              207
   1                 b              206
   2                 hay           101
   3                 hi              100
   4                 x               105
   5                 y               106
   6                 z               107
__________________________________________________________
 

Press Enter to continue...

Translation of Program:

Location    Contents    Original Statement
                                          ;this is a test
    0                                             org      100
  100          070010500000  hi      read     x;comment immediately after statement
  101          050010600105  hay   copy     y, x   ; Note that commas are an optional separator.
  102          080010500000           WRITE    x; to show case insensitive for op codes.
  103          120010000106           bp       hi y
  104          130000000000           halt

                                            ;test
  105          000000000005   x       dc       5
  106                                     y       ds      1
  107                                     z       ds       99
  206          000000000555   b       dc       555
  207          000000000100   a       dc       100
                                             end

___________________________________________________________

Press Enter to continue...

Results from emulating program:

? 10
10
? 144
144
?-7
-7

End of emulation


 
