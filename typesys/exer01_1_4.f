/* Exer01-1.4 OCAml implementation.  */

fix = lambda f.(lambda x.f (lambda y.x x y)) (lambda x.f (lambda y.x x y));

/* Recurcive function for subtracting two numerals. */
subr = lambda sub.lambda m.lambda n.if iszero n then m else sub (pred m) (pred n);
sub = fix subr;

/* Recurcive function MOD of two numerals. */
modr = lambda mod.lambda m.lambda n.if iszero (sub m n) then (if iszero (sub n m) then 0 else m) else mod (sub m n) n;
mod = fix modr;

/* The GCD function! */
gcdr = lambda gcd.lambda m.lambda n.if iszero n then m else gcd n (mod m n);
gcd = fix gcdr;

/* Print the examples. */
gcd 24 42;
gcd 42 17;
gcd 714 630;
gcd 85 204;
