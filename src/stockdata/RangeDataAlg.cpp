
#include "stockdata/RangeDataAlg.h"
#include "autil/Context.h"

namespace alch {

  namespace RangeDataAlg {

  bool findCommon(
    const RangeData& d1, const RangeData& d2,
    RangeData& f1, RangeData& f2)
  {
    // our final data
    f1.clear();
    f2.clear();

    RangeData::const_iterator i1 = d1.begin();
    RangeData::const_iterator e1 = d1.end();
    RangeData::const_iterator i2 = d2.begin();
    RangeData::const_iterator e2 = d2.end();

    while ((i1 != e1) && (i2 != e2))
    {
      // point in d1 is before d2, so we bump up i1
      if (i1->tradeTime < i2->tradeTime)
      {
        ++i1;
      }
      // point in d2 is before d1, so we bump up i2
      else if (i1->tradeTime > i2->tradeTime)
      {
        ++i2;
      }
      // same point in both!! add it to our final list
      else
      {
        f1.add(*i1);
        f2.add(*i2);
        ++i1;
        ++i2;
      }
    }

    // do final checking and set up our return values
    bool ret = true;
    Context ctx;

    if (f1.empty())
    {
      ctx << Context::PRIORITY_error
        << "Unable to find any matching points in d1"
        << Context::endl;
      ret = false;
    }
    
    if (f2.empty())
    {
      ctx << Context::PRIORITY_error
        << "Unable to find any matching points in d2"
        << Context::endl;
      ret = false;
    }
    
    if (f1.size() != f2.size())
    {
      ctx << Context::PRIORITY_error
        << "Mismatch in matching points between d1 and d2"
        << Context::endl;
      ret = false;
    }

    return ret;
  }

  } // namespace RangeDataAlg
} // namespace alch
