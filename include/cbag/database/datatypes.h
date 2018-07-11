/** \file datatypes.h
 *  \brief This file defines datatypes used by the database.
 *
 *  \author Eric Chang
 *  \date   2018/07/10
 */


#ifndef CBAG_DATABASE_COMMON_H
#define CBAG_DATABASE_COMMON_H

#include <cstdint>

#include <boost/variant.hpp>

#include <enum.h>

namespace cbag {
    typedef int32_t coord_t;
    typedef int32_t lay_t;
    typedef int32_t purp_t;
    typedef uint32_t dist_t;

    /** Define the orientation enum.
     */
    BETTER_ENUM(Orientation, uint32_t, R0, R90, R180, R270, MY, MYR90, MX, MXR90)

    /** Location and Orientation of an instance.
     */
    struct Transform {
        Transform()
                : x(0), y(0), orient(Orientation::R0) {}

        Transform(coord_t x, coord_t y)
                : x(x), y(y), orient(Orientation::R0) {}

        Transform(coord_t x, coord_t y, Orientation orient)
                : x(x), y(y), orient(orient) {}

        coord_t x, y;
        Orientation orient;
    };

    /** A custom struct representing time data.
     *
     *  This struct is used to distinguish time from int, long, or double, so that boost::variant
     *  will not be confused.
     */
    struct Time {
        Time() = default;

        explicit Time(time_t time_val) : time_val(time_val) {}

        time_t time_val;
    };

    /** Type definition for a parameter value type.
     */
    typedef boost::variant<int32_t, double, std::string, Time, std::vector<unsigned char> > value_t;

    /** Type definition for a parameter dictonary.
     */
    typedef std::map<std::string, value_t> ParamMap;

}

#endif //CBAG_DATABASE_COMMON_H
