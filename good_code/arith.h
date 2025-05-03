// This library fixes implementation defined stuffs in C.
extern int Arith_max(int x, int y);
extern int Arith_min(int x, int y);
/*
In C division rounds toward 0, diff from python where it just floor.
In C it's flor if both input are positive, else ceiling.
Also in C<99 if x,y have diff sign,behaviour is implementation defined.
*/
extern int Arith_div(int x, int y);
extern int Arith_mod(int x, int y);
extern int Arith_ceiling(int x, int y);
extern int Arith_floor(int x, int y);
