
#include "stockalg/Returns.h"
#include <cmath>

namespace alch {

namespace Returns {

  boost::posix_time::time_duration duration(RangeDataPtr data)
  {
    RangeData::size_type size = data->size();
    if (!size)
    {
      return boost::posix_time::time_duration(0, 0, 0, 0);
    }
    else
    {
      RangeData::Point start = data->get(0);
      RangeData::Point end = data->get(size - 1);
      return (end.tradeTime - start.tradeTime);
    }
  }
  
  // AR = ((1 + TR) ^ (1/YR)) - 1
  // AR: annualized return
  // TR: total return
  // YR: #years
  double annualized(RangeDataPtr data)
  {
    const double seconds_per_year = 86400 * 365.242375;
    double tr = Returns::total(data);
    boost::posix_time::time_duration dur = Returns::duration(data);
    double yr = (double)dur.total_seconds() / seconds_per_year;
    if (yr == 0.0)
    {
      return 0.0;
    }
    else
    {
      return ::pow(1.0 + tr, (1.0 / yr)) - 1.0;
    }
  }

  double total(RangeDataPtr data)
  {
    RangeData::size_type size = data->size();
    if (!size) { return 0.0; }
    RangeData::Point start = data->get(0);
    RangeData::Point end = data->get(size - 1);
    if (start.close == 0.0) { return 0.0; }
    return ((end.close - start.close) / start.close);
  }

} // namespace Returns

} // namespace alch
