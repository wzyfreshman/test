#ifndef GETNY_H
#define GETNY_H


#define NY1(i,j) { p4 = (j==nCols-1)?0:src.at<uchar>(i,j+1);\
         p5 = (i==nRows-1 || j==nCols-1)?0:src.at<uchar>(i+1,j+1);\
         p6 = (i==nRows-1)?0:src.at<uchar>(i+1,j);\
         p7 = (i==nRows-1 || j==0)?0:src.at<uchar>(i+1,j-1);\
         p8 = (j==0)?0:src.at<uchar>(i,j-1);}


#define NY2(i,j) { p2 = (i==0)?0:src.at<uchar>(i-1,j);\
         p3 = (i==0 || j==nCols-1)?0:src.at<uchar>(i-1,j+1);\
         p9 = (i==0 || j==0)?0:src.at<uchar>(i-1,j-1); }

#define NY3(i,j) {p2 = (i==0)?0:src.at<uchar>(i-1,j);\
         p3 = (i==0 || j==nCols-1)?0:src.at<uchar>(i-1,j+1);\
         p4 = (j==nCols-1)?0:src.at<uchar>(i,j+1);\
         p5 = (i==nRows-1 || j==nCols-1)?0:src.at<uchar>(i+1,j+1);\
         p9 = (i==0 || j==0)?0:src.at<uchar>(i-1,j-1);}

#define NY4(i,j) { p3 = (i==0 || j==nCols-1)?0:src.at<uchar>(i-1,j+1);\
         p4 = (j==nCols-1)?0:src.at<uchar>(i,j+1);\
         p5 = (i==nRows-1 || j==nCols-1)?0:src.at<uchar>(i+1,j+1);}

#define NY5(i,j) {p3 = (i==0 || j==nCols-1)?0:src.at<uchar>(i-1,j+1);\
         p4 = (j==nCols-1)?0:src.at<uchar>(i,j+1);\
         p5 = (i==nRows-1 || j==nCols-1)?0:src.at<uchar>(i+1,j+1);\
         p6 = (i==nRows-1)?0:src.at<uchar>(i+1,j);\
         p7 = (i==nRows-1 || j==0)?0:src.at<uchar>(i+1,j-1);}

#define NY6(i,j) {p5 = (i==nRows-1 || j==nCols-1)?0:src.at<uchar>(i+1,j+1);\
         p6 = (i==nRows-1)?0:src.at<uchar>(i+1,j);\
         p7 = (i==nRows-1 || j==0)?0:src.at<uchar>(i+1,j-1);}

#define NY7(i,j) {p5 = (i==nRows-1 || j==nCols-1)?0:src.at<uchar>(i+1,j+1);\
         p6 = (i==nRows-1)?0:src.at<uchar>(i+1,j);\
         p7 = (i==nRows-1 || j==0)?0:src.at<uchar>(i+1,j-1);\
         p8 = (j==0)?0:src.at<uchar>(i,j-1);\
         p9 = (i==0 || j==0)?0:src.at<uchar>(i-1,j-1);}

#define NY8(i,j) {p7 = (i==nRows-1 || j==0)?0:src.at<uchar>(i+1,j-1);\
         p8 = (j==0)?0:src.at<uchar>(i,j-1);\
         p9 = (i==0 || j==0)?0:src.at<uchar>(i-1,j-1);}

#define NY9(i,j) {p2 = (i==0)?0:src.at<uchar>(i-1,j);\
         p3 = (i==0 || j==nCols-1)?0:src.at<uchar>(i-1,j+1);\
         p7 = (i==nRows-1 || j==0)?0:src.at<uchar>(i+1,j-1);\
         p8 = (j==0)?0:src.at<uchar>(i,j-1);\
         p9 = (i==0 || j==0)?0:src.at<uchar>(i-1,j-1);}




#define GETNBY1(a,b) {a=i;b=j;}
#define GETNBY2(a,b) {a=i-1;b=j;}
#define GETNBY3(a,b) {a=i-1;b=j+1;}
#define GETNBY4(a,b) {a=i;b=j+1;}
#define GETNBY5(a,b) {a=i+1;b=j+1;}
#define GETNBY6(a,b) {a=i+1;b=j;}
#define GETNBY7(a,b) {a=i+1;b=j-1;}
#define GETNBY8(a,b) {a=i;b=j-1;}
#define GETNBY9(a,b) {a=i-1;b=j-1;}

#define GOTO(i) switch(i) { \
    case 2: \
     goto LABEL2; \
    case 3: \
     goto LABEL3; \
    case 4: \
     goto LABEL4; \
    case 5: \
     goto LABEL5; \
    case 6: \
     goto LABEL6; \
    case 7: \
     goto LABEL7; \
    case 8: \
     goto LABEL8; \
    case 9: \
     goto LABEL9; \
    default: \
     continue; \
                }


#define TONBY1(i,j) {}
#define TONBY2(i,j) {i--;}
#define TONBY3(i,j) {i--;j++;}
#define TONBY4(i,j) {j++;}
#define TONBY5(i,j) {i++;j++;}
#define TONBY6(i,j) {i++;}
#define TONBY7(i,j) {i++;j--;}
#define TONBY8(i,j) {j--;}
#define TONBY9(i,j) {i--;j--;}


#define TWOPOINTTOLINE(x1,y1,x2,y2) {a = (y2)-(y1); \
                                     b = (x1)-(x2); \
                          c = (y1)*((x2)-(x1)) - (x1)*((y2)-(y1));}


#define DISTANCETOLINE(x0,y0) { tmpu = fabs(a*x0 + b*y0 +c); \
                                tmpd = sqrt(a*a + b*b); \
                                distance = tmpu/tmpd; }


#endif // GETNY_H
