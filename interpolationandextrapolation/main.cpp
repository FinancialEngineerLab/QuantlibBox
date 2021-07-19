
#include "alternativeUFR.hpp"

 

using namespace QuantLib;

using namespace boost::unit_test_framework;

 

namespace Alternative_forward_term_structure

{

           struct Datum

           {

                     Integer n;

                     TimeUnit units;

                     Rate rate;

           };

 

           struct LLFRWeight

           {

                     Time ttm;

                     Real weight;

           };

 

           struct CommonVars

           {

                     Date today,settlement;

                     Calendar calendar;

                     Natural settlementDays;

                     Currency ccy;

                     BusinessDayConvention businessConvention;

                     DayCounter dayCount;

                     Frequency fixedFrequency;

                     Period floatingTenor;

                    

                     ext::shared_ptr<IborIndex> index;

                     ext::shared_ptr<YieldTermStructure> ftkCurve;

                     RelinkableHandle<YieldTermStructure> ftkCurveHandle;

                     RelinkableHandle<YieldTermStructure> ftkSpreadedCurveHandle;

                     ext::shared_ptr<Quote> ufrRate;

                     Period fsp = Period(240, Months);

                     Real alpha; // for Beta

                     Rate zeroFSP;

                     Rate discountFactorFSP;

                     SavedSettings backup;; // for cleanup

 

                     /* Input Utilities */

                     CommonVars()

                     {

                                settlementDays = 1;

                                businessConvention = ModifiedFollowing;

                                dayCount = Actual365Fixed();

                                calendar = SouthKorea();

                                ccy = KRWCurrency();

                                fixedFrequency = Monthly;

                                floatingTenor = 1 * Months;

                                alpha = 0.1;

 

                                ext::shared_ptr<SimpleQuote> LP = ext::make_shared<SimpleQuote>(0.00456);

                                Handle<Quote> LP_spread(LP);

 

                                /* Time Data */

                                today = calendar.adjust(Date(19, Mar, 2021));

                                Settings::instance().evaluationDate() = today;

                                settlement = calendar.advance(today, settlementDays, Days);

 

                                /* Interest Rate Data and Index*/

                                Datum swapData[] =

                                {

                                          {3, Months, 0.007500000000000}

                                          ,{1, Years, 0.008317857143000}

                                          ,{2, Years, 0.010475000000000}

                                          ,{3, Years, 0.012228571429000}

                                          ,{4, Years, 0.013700000000000}

                                          ,{5, Years, 0.014628571429000}

                                          ,{7, Years, 0.015710714286000}

                                          ,{10, Years,0.016628571429000}

                                          ,{12, Years,0.016992857143000}

                                          ,{15, Years,0.016714285714000}

                                          ,{20, Years,0.016592857143000}

                                };

                                InterestRate ufr(0.05069310344377, dayCount, Continuous, Annual); // Compounding ?

                                ufrRate = ext::shared_ptr<Quote>(new SimpleQuote(ufr.equivalentRate(Continuous, Annual, 1.0)));

                                ext::shared_ptr<IborIndex> index(new KrwCD3M);

                                index->addFixing(settlement, swapData[1].rate, true);

                                // Approaching member variable in the swapData Structure Array

 

                                /* Helpers */

                                Size nInstruments = LENGTH(swapData);

                                std::vector<ext::shared_ptr<RateHelper>> instruments(nInstruments);

                                for (Size i = 0; i < nInstruments; i++)

                                {

                                          instruments[i] = ext::shared_ptr<RateHelper>(new SwapRateHelper(

                                                     swapData[i].rate, Period(swapData[i].n, swapData[i].units), calendar,

                                                     fixedFrequency, businessConvention, dayCount, index));

                                }

                                ext::shared_ptr<YieldTermStructure>

                                          ftkCurve(new PiecewiseYieldCurve<Discount, LogLinear>(settlement, instruments, dayCount));

                                ext::shared_ptr<YieldTermStructure>

                                          spreaded(new ZeroSpreadedTermStructure(Handle<YieldTermStructure>(ftkCurve), LP_spread));

                                ftkCurve->enableExtrapolation();

                                spreaded->enableExtrapolation();

                                ftkCurveHandle.linkTo(ftkCurve);

                                ftkSpreadedCurveHandle.linkTo(spreaded);// for Obeserver and Observable

 

                                Rate zeroFSP = spreaded->zeroRate(ftkSpreadedCurveHandle->referenceDate() + fsp, dayCount, Continuous, Annual, false);

                                Rate discountFactorFSP = spreaded->discount(ftkSpreadedCurveHandle->referenceDate() + fsp, false);

                     }

           };

 

           ext::shared_ptr<Quote> calculateLLFR(const Handle<YieldTermStructure>& ts, const Period& fsp)

           {

                     DayCounter dc = ts->dayCounter();

                     Time cutOff = ts->timeFromReference(ts->referenceDate()+ 15 * Years);

                     Time long1 = ts->timeFromReference(ts->referenceDate() + 20 * Years);

                     Time long2 = ts->timeFromReference(ts->referenceDate() + 25 * Years);

                     Time long3 = ts->timeFromReference(ts->referenceDate() + 30 * Years);

                     Time long4 = ts->timeFromReference(ts->referenceDate() + 35 * Years);

                     Time long5 = ts->timeFromReference(ts->referenceDate() + 40 * Years);

 

                     // referenceDate: discountfactor = 1

 

                     LLFRWeight llfrWeights[] = {

                                { long1, 1.0}, {long2, 0.0}, {long3, 0.0}

                                //{35.0, 0.0}, {40.0, 0.0},{45.0, 0.0}, {50.0, 0.0}

                     };

                     Size nWeights = LENGTH(llfrWeights);

                     Rate llfr = 0.0;

                     for (Size j = 0; j< nWeights; j++)

                     {

                                LLFRWeight w = llfrWeights[j];

                                llfr += w.weight * ts->forwardRate(cutOff, w.ttm, Continuous, NoFrequency, true);

                                cutOff = ts->timeFromReference(ts->referenceDate() + fsp + 5 * Years * j);

                     }

                     return ext::shared_ptr<Quote>(new SimpleQuote(llfr));

           }

 

           Rate calculateExtrapolationForward(Time t, Time fsp, Rate llfr, Rate ufr, Real alpha)

           {

                     Time deltaT = t - fsp;

                     Real beta = (1.0 - std::exp(-alpha * deltaT)) / (alpha*deltaT);

                     return ufr + (llfr - ufr)*beta;

           }

          

}

 

void AlternativeUltimateForwardTermStructure::InterpolatedCurve()

{

           using namespace Alternative_forward_term_structure;

           CommonVars vars;

 

           Period tenors_intp[239] = { 1 * Months };

           for (Size i = 1; i <= 240; ++i)

           {

                     tenors_intp[i-1] = i * Months;

           }

           Size nTenors_intp = LENGTH(tenors_intp);

 

           //1 year forward rate and Zero Rate //

           for (Size i = 1; i <= nTenors_intp; ++i)

           {

                     Date mat0 = vars.ftkSpreadedCurveHandle->referenceDate() + tenors_intp[i - 1];

                     Date mati = vars.ftkSpreadedCurveHandle->referenceDate() + tenors_intp[i];

                     Time t0 = vars.ftkSpreadedCurveHandle->timeFromReference(mat0);

                     Time ti = vars.ftkSpreadedCurveHandle->timeFromReference(mati);

 

                     Rate zero = vars.ftkSpreadedCurveHandle->zeroRate(ti, Continuous, NoFrequency, true);

                     Rate forward = vars.ftkSpreadedCurveHandle->forwardRate(t0, ti, Continuous, NoFrequency, true).rate();

                     std::cout << "Tenor :" << tenors_intp[i] << "ZeroSpot+zero LP interpolated before FSP : " << zero << std::endl;

                     std::cout << "Tenor :" << tenors_intp[i] << "Forward+zero LP interpolated before FSP : " << forward << std::endl;

           }

};

 

 

void AlternativeUltimateForwardTermStructure::ExtrapolatedLPForward()

{

           using namespace Alternative_forward_term_structure;

           CommonVars vars;

           Period tenors[101] = { 20  * Years };

           Natural h[101] = { 1 };

           Rate forward = 0;

 

           ext::shared_ptr<Quote> llfr = calculateLLFR(vars.ftkSpreadedCurveHandle, vars.fsp);

           ext::shared_ptr<YieldTermStructure> ufrTs(

                     new UltimateForwardTermStructure(vars.ftkSpreadedCurveHandle, Handle<Quote>(llfr),

                                Handle<Quote>(vars.ufrRate), vars.fsp, vars.alpha));

           Time cutOff = ufrTs->timeFromReference(ufrTs->referenceDate() + vars.fsp);

 

           for (int i = 20; i <= 120; ++i)

           {

                     tenors[i - 20] = (i) *Years;

                     h[i - 20] = i-20;

           }

 

           Size nTenors = LENGTH(tenors);

           for (Size i = 1; i <= nTenors; ++i)

           {

                     Date mat0 = vars.settlement + tenors[i - 1];

                     Date mati = vars.settlement + tenors[i];

                     Time t0 = ufrTs->timeFromReference(mat0);

                     Time ti = ufrTs->timeFromReference(mati);

                     Rate discount_T0;

                     Rate discount_Ti;

 

 

                     //Rate actual = ufrTs->forwardRate(ti, t0, Continuous, NoFrequency, true).rate();

                    

                     Rate expected_T0 = calculateExtrapolationForward(t0, cutOff, llfr->value(), vars.ufrRate->value(), vars.alpha);

                     Rate expected_Ti = calculateExtrapolationForward(ti, cutOff, llfr->value(), vars.ufrRate->value(), vars.alpha);

 

                     Rate zero_T0 = std::log(exp(((20 * vars.zeroFSP + (h[i - 1] * expected_T0)) / (20 + h[i - 1]))));

                     Rate zero_Ti = std::log(exp(((20 * vars.zeroFSP + h[i] * expected_Ti)) / (20 + h[i])));

 

                     Rate forward = -(zero_T0 * (20 + h[i - 1])) + (zero_Ti * (20 + h[i]));

 

 

                     std::cout <<std::setprecision(10) << "Tenor: " << tenors[i] << " h=" <<h[i]

                                << " 20+h = " << 20+h[i] <<" zero_Ti= " << zero_Ti << ", Zero_T0= : " << zero_T0 << " Expected_Ti = " << expected_Ti << " Expected 20Y~ Extrapolated : " << forward << std::endl;

 

                     //Real tolerance = 1.0e-15;

                     //if (std::abs(actual - forward) > tolerance)

                     //         BOOST_ERROR("Error! \n" << std::setprecision(5) << "Calc : " << actual << "\n" << " expected : " << forward << "\n" << "tenor:" << tenors[i] << "\n");

                     //std::cout << "Tenor:" << tenors[i] << " Expected 20Y~ Extrapolated : " << forward << "  Actual 20Y~ Extrapolated : " << actual << std::endl;

           }

 

 

}

 

void AlternativeUltimateForwardTermStructure::ExtrapolatedForward()

{

           using namespace Alternative_forward_term_structure;

           CommonVars vars;

 

           ext::shared_ptr<Quote> llfr = calculateLLFR(vars.ftkCurveHandle, vars.fsp);

           ext::shared_ptr<YieldTermStructure> ufrTs(

                     new UltimateForwardTermStructure(vars.ftkCurveHandle, Handle<Quote>(llfr),

                                Handle<Quote>(vars.ufrRate), vars.fsp, vars.alpha));

           Time cutOff = ufrTs->timeFromReference(ufrTs->referenceDate() + vars.fsp);

 

 

           Period tenors_intp[240] = { 1 * Months };

           for (int i = 0; i <= 240; ++i)

           {

                     tenors_intp[i] = i * Months;

           }

 

           Period tenors[5000] = { 241 * Months };

           for (int i = 242; i <= 1200; ++i)

           {

                     tenors[i - 241] = i * Months;

           }

 

           Size nTenors_intp = LENGTH(tenors_intp);

           Size nTenors = LENGTH(tenors);

 

           for (Size i = 21; i <= nTenors; ++i)

           {

                     Date mat0 = vars.settlement + tenors[i - 1];

                     Date mati = vars.settlement + tenors[i];

                     Time t0 = ufrTs->timeFromReference(mat0);

                     Time ti = ufrTs->timeFromReference(mati);

 

                     Rate actual = ufrTs->forwardRate(cutOff, t0, Continuous, NoFrequency, true).rate();

                     Rate expected = calculateExtrapolationForward(t0, cutOff, llfr->value(), vars.ufrRate->value(), vars.alpha);

 

                     Real tolerance = 1.0e-15;

                     if (std::abs(actual - expected) > tolerance)

                                BOOST_ERROR("Error! \n" << std::setprecision(5) << "Calc : " << actual << "\n" << " expected : " << expected << "\n" << "tenor:" << tenors[i] << "\n");

                     std::cout << "Tenor:" << tenors[i] << " Expected 20Y~ Extrapolated : " << expected << "  Actual 20Y~ Extrapolated : " << actual << std::endl;

           }

 

 

           // 1 year forward rate and Zero Rate //

           for (Size i = 1; i <= nTenors_intp; ++i)

           {

 

                     Date mat0 = vars.ftkCurveHandle->referenceDate() + tenors_intp[i - 1];

                     Date mati = vars.ftkCurveHandle->referenceDate() + tenors_intp[i];

                     Time t0 = vars.ftkCurveHandle->timeFromReference(mat0);

                     Time ti = vars.ftkCurveHandle->timeFromReference(mati);

 

                     Rate zero = vars.ftkCurveHandle->zeroRate(ti, Continuous, NoFrequency, false);

                     Rate actual = vars.ftkCurveHandle->forwardRate(t0, ti, Continuous, NoFrequency, false).rate();

                     //Rate forward_intp = vars.f`tkCurve->forwardRate(vars.settlement, t, Continuous, NoFrequency, true).rate();

                     std::cout << "Tenor Date:" << tenors_intp[i] << "Zero Spot interpolated : " << zero << std::endl;

                     std::cout << "Tenor Date:" << tenors_intp[i] << "Forward interpolated : " << actual << std::endl;

 

           }

 

           for (Size i = 1; i <= nTenors; ++i)

           {

 

                     Date mat0 = vars.ftkCurveHandle->referenceDate() + tenors[i - 1];

                     Date mati = vars.ftkCurveHandle->referenceDate() + tenors[i];

                     Time t0 = ufrTs->timeFromReference(mat0);

                     Time ti = ufrTs->timeFromReference(mati);

 

 

                     Rate expected_b = calculateExtrapolationForward(t0, cutOff, llfr->value(), vars.ufrRate->value(), vars.alpha);

                     Rate expected_c = calculateExtrapolationForward(ti, cutOff, llfr->value(), vars.ufrRate->value(), vars.alpha);

                     Rate expected_forward = std::log(expected_b / expected_c) / (ti - t0);

                     Rate actual = vars.ftkCurveHandle->forwardRate(t0, ti, Continuous, NoFrequency, true).rate();

 

                     Real tolerance = 1.0e-15;

                     if (std::abs(actual - expected_forward) > tolerance)

                                BOOST_ERROR("Error! \n" << std::setprecision(5) << "Calc : " << actual << "\n" << " expected : " << expected_forward << "\n" << "tenor:" << tenors[i] << "\n");

                     std::cout << "Tenor:" << tenors[i] << "  Alternative Extrapolated : " << expected_forward << "  Actual Extrapolated : " << actual << std::endl;

           }

 

}

 

int main()

{

           AlternativeUltimateForwardTermStructure::InterpolatedCurve();

           AlternativeUltimateForwardTermStructure::ExtrapolatedLPForward();

           system("pause");

           return 0;

}

 
