/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#pragma once

/**
 * @file NaiveParametricCurveDigitizer3D.h
 * @author Kacper Pluta (\c kacper.pluta@esiee.fr )
 * Laboratoire d'Informatique Gaspard-Monge - LIGM, A3SI, France
 *
 * @date 2018/08/06
 *
 * Header file for module NaiveParametricCurveDigitizer3D
 *
 * This file is part of the DGtal library.
 */

#if defined(CurveDigitizer_RECURSES)
#error Recursive header files inclusion detected in CurveDigitizer.h
#else // defined(CurveDigitizer_RECURSES)
/** Prevents recursive inclusion of headers. */
#define CurveDigitizer_RECURSES

#if !defined CurveDigitizer_h
/** Prevents repeated inclusion of headers. */
#define CurveDigitizer_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/base/Common.h"
#include <vector>
#include <map>
#include <iterator>
#include "DGtal/geometry/curves/parametric/C3DParametricCurve.h"
//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

/////////////////////////////////////////////////////////////////////////////
// class NaiveParametricCurveDigitizer3D
/**
 * Description of class 'NaiveParametricCurveDigitizer3D' <p>
 * \brief Aim: Digitization of 3D parametric curves.
 * This method produces, for good parameters step and k_next, a 26-connected
 * digital curves obtained from a digitization process of 3D parametric curves.
 */
template <typename TParametricCurve>
class NaiveParametricCurveDigitizer3D
{
    BOOST_CONCEPT_ASSERT(( concepts::C3DParametricCurve < TParametricCurve > ));
    // ----------------------- Standard services ------------------------------
public:
    typedef typename TParametricCurve::Space::Point Point;
    typedef typename TParametricCurve::Space::RealPoint RealPoint;
    typedef std::vector<Point> DigitalCurve;
    typedef std::vector< std::pair < long double, unsigned int > > MetaData;

    NaiveParametricCurveDigitizer3D();

    /**
     * Destructor.
     */
    ~NaiveParametricCurveDigitizer3D() = default;


    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    NaiveParametricCurveDigitizer3D ( const NaiveParametricCurveDigitizer3D & other ) = delete;


    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    NaiveParametricCurveDigitizer3D & operator= ( const NaiveParametricCurveDigitizer3D & other ) = delete;

    // ----------------------- Interface --------------------------------------
public:
    void attach ( const TParametricCurve * t_curve );
    void init ( long double tmin, long double tmax, long double timeStep );
    unsigned int setKNext ( unsigned int knext );

    void digitize ( std::back_insert_iterator < DigitalCurve > );
    void digitize ( std::back_insert_iterator < DigitalCurve >, std::back_insert_iterator < MetaData > );

    /**
     * Writes/Displays the object on an output stream.
     * @param out the output stream where the object is written.
     */
    void selfDisplay ( std::ostream & out ) const;

    /**
     * Checks the validity/consistency of the object.
     * @return 'true' if the object is valid, 'false' otherwise.
     */
    bool isValid() const;

    // ------------------------- Private Datas --------------------------------
private:
    typedef DigitalCurve Buffer;
    typedef std::map < Point, std::pair < long double, unsigned int > > DataInfo;
    typedef typename Buffer::const_iterator ConstIterator;

    unsigned int K_NEXT;
    unsigned int BUFFER_SIZE;
    // ------------------------- Protected Datas ------------------------------
protected:
    DigitalCurve digitalCurve;
    MetaData metaDataContainter;
    bool metaData;
    const TParametricCurve * curve;
    long double step;
    long double timeMin;
    long double timeMax;
    bool initOK;
    // ------------------------- Hidden services ------------------------------
protected:
private:

    // This is defined here due the the problems with MS co
    struct KConstIter { typename DigitalCurve::const_iterator jt; unsigned int k; };
    struct KIter { typename DigitalCurve::iterator jt; unsigned int k; };

    bool is26Connected ( const Point &x, const Point &y );
    void syncData ( ConstIterator, ConstIterator, DataInfo & );
    void flashBuffers ( Buffer &, DataInfo & );
    void updateMetaDate ( const Point &, const RealPoint &, DataInfo &, long double);
    void cleanCurve ( );
    void cleanClosedPart ( );



    // ------------------------- Internals ------------------------------------

}; // end of class CurveDigitizer


/**
 * Overloads 'operator<<' for displaying objects of class 'CurveDigitizer'.
 * @param out the output stream where the object is written.
 * @param object the object of class 'CurveDigitizer' to write.
 * @return the output stream after the writing.
 */
template <typename T>
std::ostream&
operator<< ( std::ostream & out, const NaiveParametricCurveDigitizer3D<T> & object );


} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#if !defined(BUILD_INLINE)
#include "DGtal/geometry/curves/parametric/NaiveParametricCurveDigitizer3D.ih"
#endif


//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined CurveDigitizer_h

#undef CurveDigitizer_RECURSES
#endif // else defined(CurveDigitizer_RECURSES)
