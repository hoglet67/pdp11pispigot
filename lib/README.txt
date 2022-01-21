These files came from:
https://www.tuhs.org/cgi-bin/utree.pl?file=V10/cmd/PDP11/crt/

All constants in the original file were terminated by decimal point
"." indicating they are decimal. This was removed, as this syntax is
mot understood by GNU Assembler, where constants default to decimal
unless prefixed by 0x (hex) or 0 (octal).
