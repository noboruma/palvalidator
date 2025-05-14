// Copyright (C) MKC Associates, LLC - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Michael K. Collison <collison956@gmail.com>, July 2016
//

#ifndef __ROBUSTNESS_TESTER_H
#define __ROBUSTNESS_TESTER_H 1

#include <exception>
#include <string>
#include <list>
#include <unordered_map>
#include <boost/date_time.hpp>
#include "number.h"
#include "DecimalConstants.h"
#include "PalStrategy.h"
#include "BackTester.h"
#include "PalAst.h"
#include "RobustnessTest.h"


#include "runner.hpp"

namespace mkc_timeseries
{
  using boost::gregorian::date;
  using std::make_shared;
  using std::unordered_map;
  typedef unsigned long long HashKey;

  class RobustnessTesterException : public std::runtime_error
  {
  public:
    RobustnessTesterException(const std::string msg) 
      : std::runtime_error(msg)
    {}
    
    ~RobustnessTesterException()
    {}
    
  };

  //
  // class RobustnessTester
  //
  // Performs a robustness test of a group PriceActionLab patterns
  //
  template <class Decimal> class PalRobustnessTester
  {
  public:
    typedef typename std::list<shared_ptr<PalStrategy<Decimal>>>::const_iterator SurvivingStrategiesIterator;
    typedef typename std::list<shared_ptr<PalStrategy<Decimal>>>::const_iterator RejectedStrategiesIterator;
    typedef typename unordered_map<HashKey, shared_ptr<RobustnessCalculator<Decimal>>>::const_iterator RobustnessResultsIterator;

  public:
    PalRobustnessTester(shared_ptr<BackTester<Decimal>> aBackTester,
			shared_ptr<RobustnessPermutationAttributes> permutationAttributes,
			const PatternRobustnessCriteria<Decimal>& robustnessCriteria)
      : mBacktesterPrototype (aBackTester),
	mPermutationAttributes(permutationAttributes),
	mRobustnessCriteria(robustnessCriteria),
	mAstFactory(make_shared<AstFactory>()),
	mStrategiesToBeTested(),
	mSurvivingStrategies(),
	mRejectedStrategies(),
	mFailedRobustnessResults(),
	mPassedRobustnessResults()
    {}

    PalRobustnessTester (const PalRobustnessTester& rhs)
    : mBacktesterPrototype (rhs.mBacktesterPrototype),
      mPermutationAttributes(rhs.mPermutationAttributes),
      mRobustnessCriteria(rhs.mRobustnessCriteria),
      mAstFactory(rhs.mAstFactory),
      mStrategiesToBeTested(rhs.mStrategiesToBeTested),
      mSurvivingStrategies(rhs.mSurvivingStrategies),
      mRejectedStrategies(rhs.mRejectedStrategies),
      mFailedRobustnessResults(rhs.mFailedRobustnessResults),
      mPassedRobustnessResults(rhs.mPassedRobustnessResults)
    {}

    PalRobustnessTester&
    operator=(const PalRobustnessTester& rhs)
    {
      if (this == &rhs)
	return *this;

      mBacktesterPrototype = rhs.mBacktesterPrototype;
      mPermutationAttributes = rhs.mPermutationAttributes;
      mRobustnessCriteria = rhs.mRobustnessCriteria;
      mAstFactory = rhs.mAstFactory;
      mStrategiesToBeTested = rhs.mStrategiesToBeTested;
      mSurvivingStrategies = rhs.mSurvivingStrategies;
      mRejectedStrategies = rhs.mRejectedStrategies;
      mFailedRobustnessResults = rhs.mFailedRobustnessResults;
      mPassedRobustnessResults = rhs.mPassedRobustnessResults;

      return *this;
    }

    virtual ~PalRobustnessTester() = 0;

    //original code, for reference
    void runRobustnessTests()
    {
      typedef typename std::list<shared_ptr<PalStrategy<Decimal>>>::const_iterator CandiateStrategiesIterator;

      CandiateStrategiesIterator it = mStrategiesToBeTested.begin();
      shared_ptr<PalStrategy<Decimal>> aStrategy;
      shared_ptr<RobustnessCalculator<Decimal>> aRobustnessResult;
      unsigned long long aHashKey;
      bool isRobust = false;

      std::cout << "PalRobustnessTester::runRobustnessTests using dates: " << mBacktesterPrototype->getStartDate() << " - ";
      std::cout << mBacktesterPrototype->getEndDate() << std::endl << std::endl;

      for (; it != mStrategiesToBeTested.end(); it++)
	{
	  aStrategy = (*it);
	  RobustnessTestMonteCarlo<Decimal> aTest(mBacktesterPrototype, aStrategy, 
				     mPermutationAttributes, mRobustnessCriteria,
				     mAstFactory);

	  isRobust = aTest.runRobustnessTest();
	  aHashKey = aStrategy->getPalPattern()->hashCode();
	  aRobustnessResult = 
	    make_shared<RobustnessCalculator<Decimal>> (aTest.getRobustnessCalculator());

	  std::cout << "Run robustness test on ";
	  if (aStrategy->getPalPattern()->isLongPattern())
	    std::cout << "Long pattern with ";
	  else
	    std::cout << "Short pattern with ";

	  std::cout << "Index: " << aStrategy->getPalPattern()->getpatternIndex();
	  std::cout << ", Index date: " << aStrategy->getPalPattern()->getIndexDate();
	  std::cout << std::endl << std::endl;

	  if (isRobust)
	    {          
          std::cout << "runRobustnessTests: found robust pattern" << std::endl;
	      mSurvivingStrategies.push_back(aStrategy);
	      insertSurvivingRobustResult (aHashKey, aRobustnessResult);
	    }
	  else
	    {
	      mRejectedStrategies.push_back(aStrategy);
	      insertFailedRobustResult (aHashKey, aRobustnessResult);
	    }
	}
    }


    void insertSurvivingRobustResult(unsigned long long aHashKey,
			    shared_ptr<RobustnessCalculator<Decimal>> aRobustnessResult)
    {
      if  (findSurvivingRobustnessResults(aHashKey) == endSurvivingRobustnessResults())
	mPassedRobustnessResults.insert(make_pair(aHashKey, aRobustnessResult));
      else
	throw RobustnessTesterException("insertSurvivingRobustResult: duplicate strategies with same hashkey found");
    }

    void insertFailedRobustResult(unsigned long long aHashKey,
			    shared_ptr<RobustnessCalculator<Decimal>> aRobustnessResult)
    {
      if  (findFailedRobustnessResults(aHashKey) == endFailedRobustnessResults())
	mFailedRobustnessResults.insert(make_pair(aHashKey, aRobustnessResult));
      else
	throw RobustnessTesterException("insertFailedRobustResult: duplicate strategies with same hashkey found");
    }

    void addStrategy(shared_ptr<PalStrategy<Decimal>> aStrategy)
    {
      shared_ptr<PalStrategy<Decimal>> clonedStrategy = 
	std::dynamic_pointer_cast<PalStrategy<Decimal>> (aStrategy->cloneForBackTesting());

      mStrategiesToBeTested.push_back(clonedStrategy);
    }

    unsigned long getNumSurvivingStrategies() const
    {
      return mSurvivingStrategies.size();
    }

    unsigned long getNumRejectedStrategies() const
    {
      return mRejectedStrategies.size();
    }

    unsigned long getNumStrategiesToTest() const
    {
      return mStrategiesToBeTested.size();
    }

    SurvivingStrategiesIterator beginSurvivingStrategies() const
    {
      return mSurvivingStrategies.begin();
    }

    SurvivingStrategiesIterator endSurvivingStrategies() const
    {
      return mSurvivingStrategies.end();
    }

    RejectedStrategiesIterator beginRejectedStrategies() const
    {
      return mRejectedStrategies.begin();
    }

    RejectedStrategiesIterator endRejectedStrategies() const
    {
      return mRejectedStrategies.end();
    }

    RobustnessResultsIterator findFailedRobustnessResults(shared_ptr<PalStrategy<Decimal>> aStrategy) const
    {
      return mFailedRobustnessResults.find (aStrategy->getPalPattern()->hashCode());
    }

    RobustnessResultsIterator findFailedRobustnessResults(unsigned long long aHashCode) const
    {
      return mFailedRobustnessResults.find (aHashCode);
    }

    RobustnessResultsIterator findSurvivingRobustnessResults(shared_ptr<PalStrategy<Decimal>> aStrategy) const
    {
      return mPassedRobustnessResults.find (aStrategy->getPalPattern()->hashCode());
    }

RobustnessResultsIterator findSurvivingRobustnessResults(unsigned long long aHashCode) const
    {
      return mPassedRobustnessResults.find (aHashCode);
    }

    RobustnessResultsIterator endFailedRobustnessResults() const
    {
      return  mFailedRobustnessResults.end();
    }

    RobustnessResultsIterator endSurvivingRobustnessResults() const
    {
      return  mPassedRobustnessResults.end();
    }

  private:

    shared_ptr<BackTester<Decimal>> mBacktesterPrototype;
    shared_ptr<RobustnessPermutationAttributes> mPermutationAttributes;
    PatternRobustnessCriteria<Decimal> mRobustnessCriteria;
    shared_ptr<AstFactory> mAstFactory;
    std::list<std::shared_ptr<PalStrategy<Decimal>>> mStrategiesToBeTested;
    std::list<std::shared_ptr<PalStrategy<Decimal>>> mSurvivingStrategies;
    std::list<std::shared_ptr<PalStrategy<Decimal>>> mRejectedStrategies;
    unordered_map<unsigned long long, 
		  std::shared_ptr<RobustnessCalculator<Decimal>>> mFailedRobustnessResults;
    unordered_map<unsigned long long, 
		  std::shared_ptr<RobustnessCalculator<Decimal>>> mPassedRobustnessResults;
  };



  template <class Decimal>
    inline PalRobustnessTester<Decimal>::~PalRobustnessTester()
    {}

  template <class Decimal> class PalStandardRobustnessTester : public PalRobustnessTester<Decimal>
    {
    public:
    PalStandardRobustnessTester(shared_ptr<BackTester<Decimal>> aBackTester)
      : PalRobustnessTester<Decimal>(aBackTester,
				  make_shared<PALRobustnessPermutationAttributes>(),
				  PatternRobustnessCriteria<Decimal> (createADecimal<Decimal>("70.0"), 
								   createADecimal<Decimal>("2.0"), 
								   createAPercentNumber<Decimal>("2.0"),
								   createADecimal<Decimal>("0.9")))
	{}

      PalStandardRobustnessTester (const PalStandardRobustnessTester& rhs)
	: PalRobustnessTester<Decimal>(rhs)
      {}

      PalStandardRobustnessTester&
      operator=(const PalStandardRobustnessTester& rhs)
      {
	if (this == &rhs)
	  return *this;

	PalRobustnessTester<Decimal>::operator=(rhs);

	return *this;
      }

      ~PalStandardRobustnessTester()
      {}
    };

  template <class Decimal> class StatisticallySignificantRobustnessTester : public PalRobustnessTester<Decimal>
    {
    public:
    StatisticallySignificantRobustnessTester(shared_ptr<BackTester<Decimal>> aBackTester)
      : PalRobustnessTester<Decimal>(aBackTester,
				  make_shared<StatSignificantAttributes>(),
				  PatternRobustnessCriteria<Decimal> (createADecimal<Decimal>("70.0"), 
								   createADecimal<Decimal>("2.0"), 
								   createAPercentNumber<Decimal>("2.0"),
								   createADecimal<Decimal>("0.9")))
	{}

      StatisticallySignificantRobustnessTester (const StatisticallySignificantRobustnessTester& rhs)
	: PalRobustnessTester<Decimal>(rhs)
      {}

      StatisticallySignificantRobustnessTester&
      operator=(const StatisticallySignificantRobustnessTester& rhs)
      {
	if (this == &rhs)
	  return *this;

	PalRobustnessTester<Decimal>::operator=(rhs);

	return *this;
      }

      ~StatisticallySignificantRobustnessTester()
	{}
    };
}

#endif
