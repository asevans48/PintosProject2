//
//  17.15 p.q format.
// Created by aevans on 4/17/16.
//
#define f  (1 << 14)
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
    return ((int64_t) i * f) / fp;
}

int div_fp_x_y(int x, int y){
    return x/y;
}

int mul_fp_int(int fp,int i){
    return ((int64_t) i) * (fp / f);
}

int mul_fp_x_y(int x, int y){
    return x * y;
}

int subtract_fp_int(int fp, int i){
    return (fp - i * f);
}

int add_fp_int(int fp,int i){
    return (fp + i * f);
}

int subtract_fp_x_y(int x,int y){
    return x - y;
}


int add_fp_x_y(int x,int y){
    return x + y;
}


int convert_fp_to_int_toward_zero(int fp){
  return fp / f;
}

int convert_fp_to_nearest_int(int fp){
    if(fp >= 0) {
        return (fp + f / 2);
    }else{
        return (fp - f / 2);
    }
}


int convert_int_to_fp(int a){
    return a * f;
}
#endif //PINTOS_ANON_FP_ARITHMETIC_H
