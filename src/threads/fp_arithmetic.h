//
//  17.15 p.q format.
// Created by aevans on 4/17/16.
//
#define f  (1 << 14)
#define INT_MAX ((1<<31) -1)
#define INT_MIN (-(1<<31))
#ifndef PINTOS_ANON_FP_ARITHMETIC_H
#define PINTOS_ANON_FP_ARITHMETIC_H

int convert_fp_to_int_toward_zero(int fp);
int convert_fp_to_nearest_int(int fp);
int convert_int_to_fp(int fp);
int add_fp_x_y(int x, int y);
int subtract_fp_x_y(int x, int y);
int add_fp_int(int fp, int i);
int subtract_fp_int(int fp, int i);
int mul_fp_x_y(int x,int y);
int mul_fp_int(int fp, int i);
int div_fp_x_y(int x,int y);
int div_fp_int(int fp,int i);

int div_fp_int(int fp,int i){
    return fp / i;
}

int div_fp_x_y(int x, int y){
    return ((int64_t) x) * f / y;
}

int mul_fp_int(int fp,int i){
    return fp * i;
}

int mul_fp_x_y(int x, int y){
    return ((int64_t) x) * y / f;
}

int subtract_fp_int(int fp, int i){
    return fp - convert_int_to_fp(i);
}


int subtract_fp_x_y(int x,int y){
    return x - y;
}

int add_fp_int(int fp,int i){
    return fp + convert_int_to_fp(i) ;
}

int add_fp_x_y(int x,int y){
    return x + y;
}


int convert_fp_to_int_toward_zero(int fp){
  return fp / f;
}

int convert_fp_to_nearest_int(int fp){
    if(fp >= 0) {
        return (fp + f / 2) / f;
    }else{
        return (fp - f / 2) / f;
    }
}


int convert_int_to_fp(int a){
    return a * f;
}
#endif //PINTOS_ANON_FP_ARITHMETIC_H
