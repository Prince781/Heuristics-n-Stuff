/********************
 * titration_ph - analyzes stepwise titration according to a pH curve
 * 2013 Princeton Ferro
*********************/

#include <stdio.h>
#include <math.h>

double titration_wasb(double K_a, double M1, double M2, double C1, double TA) {
	//for weak acids/strong bases
	/*****
	 * K_a = acid dissociation constant (equilibrium)
	 * M1 = molarity (mol/L) of first acid
	 * C1 = initial concentration (mL) of first acid
	 * M2 = molarity (mol/L) of added solution
	 * TA = titration amount (mL), or concentration of added solution
	 *****/
	double V; //volume
	double mmol1, mmol2; //mmols
	mmol1 = C1 * M1; //mol^-3 / mL
	mmol2 = TA * M2; //mol^-3 / mL

	V = C1 + TA;

	mmol1 -= mmol2; //mmol2 is limiting
	
	double H = sqrt(K_a * ((mmol1/V)/(mmol2 > 0.0 ? (mmol2/V) : 1.0))); //is also base amount

	return -log10(H);
}

int main(int arg_count, char *arg_values[]) {
	double Ka = 1.8e-5;
	double M1 = 0.1;
	double M2 = 0.1;
	double C1 = 50.0;
	double TA = 10.0;
	printf("Final pH (with %f mL titrant): %f\n", TA, titration_wasb(Ka, M1, M2, C1, TA));
	return (0);
}