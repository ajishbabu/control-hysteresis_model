/*
 * =====================================================================================
 *
 *       Filename:  bouc_wen_model.cpp
 *
 *    Description:  Class implementing the hysteresis model
 *
 *        Version:  1.0
 *        Created:  10/16/09 15:36:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ajish Babu (), ajish.babu@dfki.de
 *        Company:  DFKI
 *
 * =====================================================================================
 */

#include "BoucWenBaberNooriModel.hpp"

using namespace hysteresis_model;

BoucWenBaberNooriModel::BoucWenBaberNooriModel(double _sampling_period, int _simulation_per_cycle,
	double _initial_time, double *_initial_state) 
: RK4_SIM(1, 1, (_sampling_period/((double)_simulation_per_cycle)),
	_initial_time, _initial_state)
{
    // Initialize parameters
    sampling_period = _sampling_period;
    simulation_per_cycle = _simulation_per_cycle;

    setParameters(0.0, 0.0, 0.0,0.0,1.0, 0.0, 0.0, 0.0, 0.0);
}

inline void BoucWenBaberNooriModel::DERIV(const double t, const double *x, 
	const double *u, double *xdot)
{
    // The actual Bouc-Wen-Baber-Noori model which calculates the 
    // hysteretic defection using this intergral
    xdot[0] = h / eta * 
	( A * u[0]
	  - nu * (
	      beta * fabs(u[0]) * pow(fabs(x[0]), n-1) * x[0]
	      + gamma* u[0] 	    * pow(fabs(x[0]), n) 
	      )
	);
}

double BoucWenBaberNooriModel::getStress(double strain, double strainVel)
{

    ctrl_input[0] = strainVel;
    for (int ii=0; ii < simulation_per_cycle; ii++)
    {
	solve();
    }

    // Returns the stress which is weighted sum of the actual strain part and
    // the hysteretic strain part
    return a*ki*strain + (1-a)*ki*plant_state[0];
}

void BoucWenBaberNooriModel::getParameters(double *p) const
{
    p[0] = A;  		
    p[1] = beta;
    p[2] = gamma;
    p[3] = n;
    p[4] = a;
    p[5] = ki;
    p[6] = nu;	
    p[7] = eta;	
    p[8] = h;	
}

void BoucWenBaberNooriModel::setParameters(double* const p)
{
    setParameters(p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],p[8]);	

    if(gamma < -beta)
	gamma = -beta;
    if(gamma > beta)
	gamma = beta;
}


void BoucWenBaberNooriModel::setParameters(
	double _A, 
	double _beta,
	double _gamma,
	double _n,
	double _a,
	double _ki,
	double _nu,
	double _eta,
	double _h)
{
    A	 = A;  		
    beta = beta;
    gamma= gamma;
    n    = n;
    a    = a;
    ki   = ki;
    nu	 = nu;	
    eta	 = eta;	
    h	 = h;	
}
