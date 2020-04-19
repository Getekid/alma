/* Factorial implementation, Exercise 13.5.8 */

sumr = lambda sum:Nat->Nat->Nat. lambda n:Nat. lambda m:Nat. if iszero m then n else sum (succ n) (pred m);
sum = fix sumr;

times = lambda n:Nat. lambda m:Nat.
  let p = ref (lambda mm:Nat. 0) in
    (lambda x:Unit. !p m) (p := (lambda mm:Nat. if iszero mm then 0 else sum (!p (pred mm)) n));

fact = lambda n:Nat.
  let f = ref (lambda m:Nat. 0) in
    (lambda x:Unit. !f n) (f := (lambda m:Nat. if iszero m then 1 else times (!f (pred m)) m));
/*fact 3;*/

/* Failed attempt to define a generic fix combinator. */
/*let r = ref (lambda z:T1. f (lambda y:T1->T2. f y z)) in !r x;
f = lambda f:(T1->T2)->(T1->T2). lambda x:T1.
  let r = ref (lambda y:T1->T2. f y z) in
    (lambda u:Unit. !r x) (r := (lambda z:T1. f (!r) z));*/

fixBool = lambda f:(T1->Bool)->(T1->Bool). lambda x:T1.
  let r = ref (lambda y:T1. true) in
    (lambda u:Unit. !r x) (r := (lambda y:T1. f (!r) y));

/* The following has the same type as fixBool but does pass down any variable so it's not functioning. */
fixBoolAlt = lambda f:(T1->Bool)->(T1->Bool).
  let r = ref (lambda x:T1. f (lambda y:T1. true) x) in
    (lambda u:Unit. !r) (r := f (!r));


/* Exer02-2.3 OCaml implementation. */

fixNatBool = lambda f:(Nat->Bool)->(Nat->Bool). lambda x:Nat.
  let r = ref (lambda y:Nat. true) in
    (lambda u:Unit. !r x) (r := (lambda y:Nat. f (!r) y));

evenr = lambda ie:Nat->Bool. lambda n:Nat.
  if iszero n then true
    else if iszero (pred n) then false
    else ie (pred (pred n));

even = fixNatBool evenr;

even 12;
even 9;
even 1;
even 23;
even 42;
