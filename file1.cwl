NAME _PROGRAM ;
BODY DATA INTEGER16 _VALUEIN , _RESULTO , _CYCLEAC ;
  GET ( _VALUEIN )
  1 >> _RESULTO
  FOR  0 >> _CYCLEAC TO 32767 DO
    IF _VALUEIN != 0 THEN ; ELSE GOTO endcyclea ;
    _RESULTO * _VALUEIN >> _RESULTO
    _VALUEIN - 1 >> _VALUEIN
  ;
  endcyclea :
  PUT ( _RESULTO )
END