#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* constants */


// base-Tone #101214 / rgb(016,018,020)
const double bToneH = 210;
const double bToneS = 11.1;
const double bToneL = 7.1;


/* implementations */


// (hybrid log-gamma) electro-optical transfer function
double* EOTF(double* E, double Lw, double Lb) { // E := {R,G,B} with range(0:1), Lw := max White level, Lb := max Black level
  // as defined by BT.2390-9
  const double gamma = 1.2 * pow(1.1111,log2(Lw/1000));
  const double beta = sqrt(3 * pow(Lb/Lw,1/gamma));
  const double a = 0.17883277;
  const double b = 1 - 4 * a;
  const double c = 0.5 - a * log(4*a);
  const double alphaYs = pow(Lw*(0.2627*0.5 * 0.6780*0.5 + 0.0593*0.5),gamma-1); // 0.5 represent normalized linear scene light for R,G,B

  for (int i = 0; i < 3; i++) {
    E[i] = (1.0-beta)*E[i]+beta;
    if (E[i] >= 0.0) {
      if (E[i] <= 0.5) {
        E[i] = pow(E[i],2)/3;
      } else {
        E[i] = (exp((E[i]-c)/a)+b)/12;
      }
    } else {
      E[i] = 0.0;
    }
    E[i] = E[i] * alphaYs * 255;
  }

  return E;
}

// generates hlength amount of hues for given Saturation S and Lightness L
double** genHues(int hlength, double S, double L, double Lw, double Lb) {

  double** colr = malloc(sizeof(double*)*hlength);

  for (int i = hlength; i > 0; i--) {
    double H = fmod((i/hlength)*360+bToneH,360); //start from base-Tone and rotate

    // HSL to RGB converstion
    double C = (1-abs(2*L-1))*S;
    double X = C * (1-abs(fmod(H/60,2)-1));
    double m = L-C/2;

    if (0 <= H && H < 60) {
      colr[i][0] = C+m;
      colr[i][1] = X+m;
      colr[i][2] = m;
    }
    if (60 <= H && H < 120) {
      colr[i][0] = X+m;
      colr[i][1] = C+m;
      colr[i][2] = m;
    }
    if (120 <= H && H < 180) {
      colr[i][0] = m;
      colr[i][1] = C+m;
      colr[i][2] = X+m;
    }
    if (180 <= H && H < 240) {
      colr[i][0] = m;
      colr[i][1] = X+m;
      colr[i][2] = C+m;
    }
    if (240 <= H && H < 300) {
      colr[i][0] = X+m;
      colr[i][1] = m;
      colr[i][2] = C+m;
    }
    if (300 <= H && H < 360) {
      colr[i][0] = C+m;
      colr[i][1] = m;
      colr[i][2] = X+m;
    }

    // adjust theoretical color to perceived color
    EOTF(colr[i], Lw, Lb);
  }
  return colr;
}

// generates steplength amount of hue-arrays from genHues()
double*** genSteps(int steplength, int hlength, double Lw, double Lb) {

  double*** colr = malloc(sizeof(double**)*steplength);

  for (int i = 0; i < steplength; i++) {
    // TODO: generates random color sets, needs to always include intended colors
    double S = i/steplength*2;
    if (S > 1) {S = 1-(S-1);}
    double L = i/steplength;
    genHues(hlength, S, L, Lw, Lb);
  }
  return colr;
}

int main(int argc, char** argv) {

  // very wacky input validation
  if (argv[1] == NULL) {
    argv[1] = "-h";
  }
  if (argv[1][0] != '-' || strlen(argv[1]) != 2 ) {
    argv[1] = "- ";
  }

  switch (argv[1][1]) {
    case 'h':
      printf("usage:\n");
      printf("  %s -h                                                           display this help and exit\n", argv[0]);
      printf("  %s -H {#HueSteps} {Saturation} {Lightness} {Lw=1} {Lb=0}        generate 1 * Hue-Steps\n", argv[0]);
      printf("  %s -L {#LightnessSteps} {HuesSteps} {Lw=1} {Lb=0}               generate Lightness-Steps * Hue-Steps\n", argv[0]);
      printf("Print MCOLR_751 color for given Hue-Steps, additionally Lightness-Steps can be generated.\nMaximum White/Black Levels (Lw/Lb) can be modified if needed.\n");
      printf("\n");
      printf("Github repo <https://github.com/sirpiksel/MCOLR_751>\n");
      break;
    case 'H':
      break;
    case 'L':
      break;
    default:
      printf("Error: INVALID ARGUMENTS\n");
  }
  return 0;
}
