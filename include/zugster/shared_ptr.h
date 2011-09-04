#pragma once

// It's annoying to still have to do this in mid-2011, but there's no
// implementation of std::make_shared in XCode 4, so we have to use
// the boost version. But you can't mix boost::make_shared with
// std::shared_ptr. I'd rather use std:: for everything, but Apple
// lags a couple of years behind Microsoft in the area of C++
// standardization. Ironic, no?

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/pointer_cast.hpp>

using boost::shared_ptr;
using boost::make_shared;
using boost::dynamic_pointer_cast;
