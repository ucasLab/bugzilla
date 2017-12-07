#define FADD(a, b) ((a)+(b))
#define FSUB(a, b) ((a)-(b))
#define FMUL(a, b, q) (((a)*(b))>>(q))
#define FDIV(a, b, q) (((a)<<(q))/(b))

#define FADDI(a, b, q) ((a)+((b)<<(q)))
#define FSUBI(a, b, q) ((a)-((b)<<(q)))
#define FMULI(a, b) ((a)*(b))
#define FDIVI(a, b) ((a)/(b))

#define FCONV(a, q1, q2) (((q2)>(q1)) ? (a)<<((q2)-(q1)) : (a)>>((q1)-(q2)))

#define FADDG(a, b, q1, q2, q3) (FCONV(a,q1,q3)+FCONV(b,q2,q3))
#define FSUBG(a, b, q1, q2, q3) (FCONV(a,q1,q3)-FCONV(b,q2,q3))
#define FMULG(a, b, q1, q2, q3) FCONV((a)*(b), (q1)+(q2), q3)
#define FDIVG(a, b, q1, q2, q3) (FCONV(a, q1, (q2)+(q3))/(b))

#define TOFIX(d, q) ((int)( (d)*(double)(1<<(q)) ))
#define TOFLT(a, q) ( (double)(a) / (double)(1<<(q)) )



