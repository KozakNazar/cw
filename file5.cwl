NAME _PROGRAM ;
BODY DATA INTEGER16 _AAAAAAA ;
  GET ( _AAAAAAA )
  DO 
    _AAAAAAA - 1 >> _AAAAAAA
    PUT ( _AAAAAAA )
  WHILE ( _AAAAAAA >= 1 )
END