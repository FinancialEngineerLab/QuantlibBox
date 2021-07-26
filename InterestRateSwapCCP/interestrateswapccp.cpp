
#include "pch.h"

#include <ql/quantlib.hpp>

#include <ql/indexes/ibor/krwcd.hpp>


#ifdef BOOST_MSVC

#include <ql/auto_link.hpp>

#endif


#include <iostream>

#include <iomanip>


using namespace QuantLib;

using namespace std;

using namespace ext;


#if defined(QL_ENABLE_SESSIONS)

namespace QuantLib

{

ThreadKey sessionId() { return (); }

}

#endif


int main()

{

try

{

std::cout << std::endl;


/* Inputting 1. Conventions */

Calendar calendar = SouthKorea();

DayCounter dayCounter = Actual365Fixed();

Date todaysDate(11, May, 2021);

Settings::instance().evaluationDate() = todaysDate;

todaysDate = Settings::instance().evaluationDate();


Integer fixingDays = 1;

Date settlementDate = calendar.advance(todaysDate, fixingDays, Days);

std::cout << "Today:1 " << settlementDate.weekday() << ", " << settlementDate << std::endl;


settlementDate = calendar.adjust(settlementDate);

std::cout << "Today: 2" << settlementDate.weekday() << ", " << settlementDate << std::endl;


std::cout << "Today: 3" << todaysDate.weekday() << ", " << todaysDate << std::endl;

std::cout << "Today:4 " << settlementDate.weekday() << ", " << settlementDate << std::endl;


RelinkableHandle<YieldTermStructure> discountingTermStucture;


/* Inputting 2. Quotes, Helpers */

ext::shared_ptr<IborIndex> krwCD(new KrwCD3M());


//ext::shared_ptr<IborIndex> krwCD(new KrwCD3M());


ext::shared_ptr<Quote> krw01dCl(new SimpleQuote(0.004917948098));

ext::shared_ptr<Quote> krw03mCD(new SimpleQuote(0.0069));

ext::shared_ptr<Quote> krw06mSW(new SimpleQuote(0.0069));

ext::shared_ptr<Quote> krw09mSW(new SimpleQuote(0.007175));

ext::shared_ptr<Quote> krw01ySW(new SimpleQuote(0.00775));

ext::shared_ptr<Quote> krw18mSW(new SimpleQuote(0.009089285714));

ext::shared_ptr<Quote> krw02ySW(new SimpleQuote(0.010214285714));

ext::shared_ptr<Quote> krw03ySW(new SimpleQuote(0.012275));

ext::shared_ptr<Quote> krw04ySW(new SimpleQuote(0.013835714286));

ext::shared_ptr<Quote> krw05ySW(new SimpleQuote(0.014860714286));

ext::shared_ptr<Quote> krw06ySW(new SimpleQuote(0.015539285714));

ext::shared_ptr<Quote> krw07ySW(new SimpleQuote(0.016117857143));

ext::shared_ptr<Quote> krw08ySW(new SimpleQuote(0.016485714286));

ext::shared_ptr<Quote> krw09ySW(new SimpleQuote(0.016939285714));

ext::shared_ptr<Quote> krw10ySW(new SimpleQuote(0.01725));

ext::shared_ptr<Quote> krw12ySW(new SimpleQuote(0.017682142857));

ext::shared_ptr<Quote> krw15ySW(new SimpleQuote(0.017521428571));

ext::shared_ptr<Quote> krw20ySW(new SimpleQuote(0.01755));

  
// Helpers & Instruments //

krwCD->addFixing(settlementDate, krw03mCD->value(),true);

Frequency swFixedLegFrequency = Quarterly;

BusinessDayConvention swFixedLegConvention = ModifiedFollowing;

DayCounter swFixedLegDayCounter = Actual365Fixed();


ext::shared_ptr<RateHelper> dp01d(new DepositRateHelper(Handle<Quote>(krw01dCl), Period(1 * Days), krwCD->fixingDays(), calendar, ModifiedFollowing, false, Actual365Fixed()));

ext::shared_ptr<RateHelper> dp03m(new DepositRateHelper(Handle<Quote>(krw03mCD), Period(3 * Months), krwCD->fixingDays(), calendar, ModifiedFollowing, false, Actual365Fixed()));

ext::shared_ptr<RateHelper> sw06m(new SwapRateHelper(Handle<Quote>(krw06mSW), Period(6 * Months ), calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));

ext::shared_ptr<RateHelper> sw09m(new SwapRateHelper(Handle<Quote>(krw09mSW), Period(9 * Months) , calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));

ext::shared_ptr<RateHelper> sw01y(new SwapRateHelper(Handle<Quote>(krw01ySW), Period(1 * Years ), calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));

ext::shared_ptr<RateHelper> sw18m(new SwapRateHelper(Handle<Quote>(krw18mSW), Period(18 * Months) , calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter, krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));

ext::shared_ptr<RateHelper> sw02y(new SwapRateHelper(Handle<Quote>(krw02ySW), Period(2 * Years ), calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter,krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));

ext::shared_ptr<RateHelper> sw03y(new SwapRateHelper(Handle<Quote>(krw03ySW), Period(3 * Years ), calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter,krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));

ext::shared_ptr<RateHelper> sw04y(new SwapRateHelper(Handle<Quote>(krw04ySW), Period(4 * Years ), calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter,krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));

ext::shared_ptr<RateHelper> sw05y(new SwapRateHelper(Handle<Quote>(krw05ySW), Period(5 * Years ), calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter,krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));

ext::shared_ptr<RateHelper> sw06y(new SwapRateHelper(Handle<Quote>(krw06ySW), Period(6 * Years ), calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter,krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));

ext::shared_ptr<RateHelper> sw07y(new SwapRateHelper(Handle<Quote>(krw07ySW), Period(7 * Years ), calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter,krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));

ext::shared_ptr<RateHelper> sw08y(new SwapRateHelper(Handle<Quote>(krw08ySW), Period(8 * Years ), calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter,krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));

ext::shared_ptr<RateHelper> sw09y(new SwapRateHelper(Handle<Quote>(krw09ySW), Period(9 * Years ), calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter,krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));

ext::shared_ptr<RateHelper> sw10y(new SwapRateHelper(Handle<Quote>(krw10ySW), Period(10 * Years ), calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter,krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));

ext::shared_ptr<RateHelper> sw12y(new SwapRateHelper(Handle<Quote>(krw12ySW), Period(12 * Years ), calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter,krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));

ext::shared_ptr<RateHelper> sw15y(new SwapRateHelper(Handle<Quote>(krw15ySW), Period(15 * Years ), calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter,krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));

ext::shared_ptr<RateHelper> sw20y(new SwapRateHelper(Handle<Quote>(krw20ySW), Period(20 * Years ), calendar, swFixedLegFrequency, swFixedLegConvention, swFixedLegDayCounter,krwCD));//, krwCD, Handle<Quote>(), 0 * Days, discountingTermStucture));



std::vector<ext::shared_ptr<RateHelper>> ccpInstruments;

ccpInstruments.push_back(dp01d);

ccpInstruments.push_back(dp03m);

ccpInstruments.push_back(sw06m);

ccpInstruments.push_back(sw09m);

ccpInstruments.push_back(sw01y);

ccpInstruments.push_back(sw18m);

ccpInstruments.push_back(sw02y);

ccpInstruments.push_back(sw03y);

ccpInstruments.push_back(sw04y);

ccpInstruments.push_back(sw05y);

ccpInstruments.push_back(sw06y);

ccpInstruments.push_back(sw07y);

ccpInstruments.push_back(sw08y);

ccpInstruments.push_back(sw09y);

ccpInstruments.push_back(sw10y);

ccpInstruments.push_back(sw12y);

ccpInstruments.push_back(sw15y);

ccpInstruments.push_back(sw20y);



std::cout << ccpInstruments.size() << std::endl;



double tolerance = 1.0e-12;

//auto ccpTermStructure = boost::make_shared<PiecewiseYieldCurve<Discount, Linear>>(0, krwCD->fixingCalendar(), ccpInstruments, krwCD->dayCounter());


//ext::shared_ptr<YieldTermStructure> ccpTermStructure(new PiecewiseYieldCurve<Discount, Linear>(settlementDate, ccpInstruments, Actual365Fixed(),

// PiecewiseYieldCurve<Discount, Linear>::bootstrap_type(tolerance)));


ext::shared_ptr<YieldTermStructure> ccpTermStructure(new PiecewiseYieldCurve<ZeroYield, Linear>(settlementDate, ccpInstruments, Actual365Fixed(),tolerance));

ccpTermStructure->enableExtrapolation(true);

discountingTermStucture.linkTo(ccpTermStructure);


//std::cout << discountingTermStucture->settlementDays() << std::endl;


std::cout << discountingTermStucture->zeroRate(Date(12, May, 2021), dayCounter,Continuous, Quarterly, true) << std::endl;

std::cout << discountingTermStucture->discount(Date(12, May, 2021),true) << std::endl;


// Swap //

Rate fixedRate = 0.0077;

Integer lengthInYears = 1;

VanillaSwap::Type swapType = VanillaSwap::Payer;

Date maturity = settlementDate + Period(lengthInYears * Years);

Real nominal = 1000000000;

Spread spread = 0.0;


// Fixed Leg//

Frequency fixedLegFrequency = Quarterly;

BusinessDayConvention dayConvention = ModifiedFollowing;

//DayCounter dayCounter = Actual365Fixed();

Schedule schedule(settlementDate, maturity, Period(fixedLegFrequency), calendar, dayConvention, dayConvention, DateGeneration::Forward,true);



ext::shared_ptr<IborIndex> krwCDsw(new KrwCD3M(discountingTermStucture));

krwCDsw->addFixing(settlementDate, krw03mCD->value(), true);


Real NPV;

Rate fairRate;

VanillaSwap Swap(swapType, nominal, schedule, fixedRate, dayCounter, schedule, krwCDsw, spread, dayCounter);


ext::shared_ptr<PricingEngine> swapEngine(new DiscountingSwapEngine(discountingTermStucture));

Swap.setPricingEngine(swapEngine);

NPV = Swap.NPV();



std::cout << NPV << std::endl;
// 398,179




}

catch (int e)

{

std::cout << e << std::endl;

}



}
