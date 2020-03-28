# Encoder schematic

MC14013B truth table

Inputs                              Outputs
C(lk)   D(ata)   R(eset)    S(et)   Q   !Q
_/      0        0          0       0   1
_/      1        0          0       1   0
\_      x        0          0       Q   !Q
x       x        1          0       0   1
x       x        0          1       1   0
x       x        1          1       1   1

          