#include <ql/termstructures/yield/flatforward.hpp>

#include <ql/processes/hybridblackscholeshullwhiteprocess.hpp>

#include <iostream>

namespace QuantLib {


 

        HybridBlackScholesHullWhiteProcess::HybridBlackScholesHullWhiteProcess(

               const ext::shared_ptr<BlackScholesProcess> & blackScholesProcess,

               const ext::shared_ptr<HullWhiteProcess> & hullWhiteProcess,

               Real corrEquityShortRate,

               HybridBlackScholesHullWhiteProcess::Discretization discretization)

               : blackScholesProcess_(blackScholesProcess),

               hullWhiteProcess_(hullWhiteProcess),

               hullWhiteModel_(new HullWhite(blackScholesProcess->riskFreeRate(),

                       hullWhiteProcess->a(),

                       hullWhiteProcess->sigma())),

               corrEquityShortRate_(corrEquityShortRate),

               discretization_(discretization),

               //maxRho_(std::sqrt(1 - blackScholesProcess->rho()*blackScholesProcess->rho())

               //      - std::sqrt(QL_EPSILON) /* reserve for rounding errors */),


 

               //T_(hullWhiteProcess->getForwardMeasureTime()),

               T_(0.0),

               endDiscount_(blackScholesProcess->riskFreeRate()->discount(T_)) {


 

               QL_REQUIRE(corrEquityShortRate*corrEquityShortRate <= 1.0,

                       //+ blackScholesProcess->rho()*blackScholesProcess->rho() <= 1.0,

                       "correlation matrix is not positive definite");


 

               QL_REQUIRE(hullWhiteProcess->sigma() > 0.0,

                       "positive vol of Hull White process is required");

        }

       

        Size HybridBlackScholesHullWhiteProcess::size() const {

               return 2;

        }


 

        Disposable<Array> HybridBlackScholesHullWhiteProcess::initialValues() const {

               Array retVal(2);

               retVal[0] = blackScholesProcess_->x0();

               retVal[1] = hullWhiteProcess_->x0();


 

               return retVal;

        }


 

        Disposable<Array>

               HybridBlackScholesHullWhiteProcess::drift(Time t, const Array& x) const {

               Array retVal(2);


 

               retVal[0] = blackScholesProcess_->drift(t, x[0]);

               retVal[1] = hullWhiteProcess_->drift(t, x[1]);


 

               return retVal;

        }


 

        Disposable<Array>

               HybridBlackScholesHullWhiteProcess::apply(const Array& x0, const Array& dx) const {

               Array retVal(2);

              

               retVal[0] = blackScholesProcess_->apply(x0[0], dx[1]);

               retVal[1] = hullWhiteProcess_->apply(x0[1], dx[1]);


 

               return retVal;

        }


 

        Disposable<Matrix>

               HybridBlackScholesHullWhiteProcess::diffusion(Time t, const Array& x) const {

               Matrix retVal(2, 2);


 

               retVal[0][0] = blackScholesProcess_->diffusion(t, x[0]);

               retVal[0][1] = 0.0;

              

               const Real sigma = hullWhiteProcess_->sigma();

               retVal[1][0] = corrEquityShortRate_ * sigma;

               retVal[1][1] = std::sqrt(sigma*sigma - retVal[1][0] * retVal[1][0]);


 

               return retVal;

        }


 

        Disposable<Array>                     //여기부터 evole

               HybridBlackScholesHullWhiteProcess::evolve(Time t0, const Array& x0, Time dt, const Array& dw) const {

              

               const Rate r = x0[1];

               const Real a = hullWhiteProcess_->a();

               const Real sigma = hullWhiteProcess_->sigma();

               const Real rho = corrEquityShortRate_;

              

               blackScholesProcess_->localVolatility(); // trigger update

               const Volatility eta = blackScholesProcess_->stdDeviation(t0, x0[0], dt) / std::sqrt(dt);

              

               const Time s = t0;

               const Time t = t0 + dt;

               const Time T = T_;

               const Rate dy = blackScholesProcess_->dividendYield()->forwardRate(s, t, Continuous, NoFrequency);

               const Real df = std::log(blackScholesProcess_->riskFreeRate()->discount(t) / blackScholesProcess_->riskFreeRate()->discount(s));


 

               const Real eaT = std::exp(-a * T);

               const Real eat = std::exp(-a * t);

               const Real eas = std::exp(-a * s);

               const Real iat = 1.0 / eat;

               const Real ias = 1.0 / eas;


 

               const Real m1 = -(dy + 0.5*eta*eta)*dt - df;


 

               /*const Real m2 = -rho * sigma*eta / a * (dt - 1 / a * eaT*(iat - ias));

              

               const Real m3 = (r - hullWhiteProcess_->alpha(s))

                       *hullWhiteProcess_->B(s, t);


 

               const Real m4 = sigma * sigma / (2 * a*a)

                       *(dt + 2 / a * (eat - eas) - 1 / (2 * a)*(eat*eat - eas * eas));


 

 
