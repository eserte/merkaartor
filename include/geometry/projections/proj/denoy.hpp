#ifndef _PROJECTIONS_DENOY_HPP
#define _PROJECTIONS_DENOY_HPP

// Generic Geometry Library - projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright Barend Gehrels (1995-2009), Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande (2008-2009)
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Geometry Library by Barend Gehrels (Geodan, Amsterdam)

// Original copyright notice:

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <geometry/projections/impl/base_static.hpp>
#include <geometry/projections/impl/base_dynamic.hpp>
#include <geometry/projections/impl/projects.hpp>
#include <geometry/projections/impl/factory_entry.hpp>

namespace projection
{
	#ifndef DOXYGEN_NO_IMPL
	namespace impl
	{
		namespace denoy
		{
			static const double C0 = 0.95;
			static const double C1 = -.08333333333333333333;
			static const double C3 = .00166666666666666666;
			static const double D1 = 0.9;
			static const double D5 = 0.03;


			// template class, using CRTP to implement forward/inverse
			template <typename LL, typename XY, typename PAR>
			struct base_denoy_spheroid : public base_t_f<base_denoy_spheroid<LL, XY, PAR>, LL, XY, PAR>
			{

				typedef typename base_t_f<base_denoy_spheroid<LL, XY, PAR>, LL, XY, PAR>::LL_T LL_T;
				typedef typename base_t_f<base_denoy_spheroid<LL, XY, PAR>, LL, XY, PAR>::XY_T XY_T;


				inline base_denoy_spheroid(const PAR& par)
					: base_t_f<base_denoy_spheroid<LL, XY, PAR>, LL, XY, PAR>(*this, par) {}

				inline void fwd(LL_T& lp_lon, LL_T& lp_lat, XY_T& xy_x, XY_T& xy_y) const
				{
					xy_y = lp_lat;
					xy_x = lp_lon;
					lp_lon = fabs(lp_lon);
					xy_x *= cos((C0 + lp_lon * (C1 + lp_lon * lp_lon * C3)) *
						(lp_lat * (D1 + D5 * lp_lat * lp_lat * lp_lat * lp_lat)));
				}
			};

			// Denoyer Semi-Elliptical
			template <typename PAR>
			void setup_denoy(PAR& par)
			{
				par.es = 0.;
				// par.fwd = s_forward;
			}

		} // namespace denoy
	} //namespaces impl
	#endif // doxygen

	/*!
		\brief Denoyer Semi-Elliptical projection
		\ingroup projections
		\tparam LL latlong point type
		\tparam XY xy point type
		\tparam PAR parameter type
		\par Projection characteristics
		 - Pseudocylindrical
		 - no inverse
		 - Spheroid
		\par Example
		\image html ex_denoy.gif
	*/
	template <typename LL, typename XY, typename PAR = parameters>
	struct denoy_spheroid : public impl::denoy::base_denoy_spheroid<LL, XY, PAR>
	{
		inline denoy_spheroid(const PAR& par) : impl::denoy::base_denoy_spheroid<LL, XY, PAR>(par)
		{
			impl::denoy::setup_denoy(this->m_par);
		}
	};

	#ifndef DOXYGEN_NO_IMPL
	namespace impl
	{

		// Factory entry(s)
		template <typename LL, typename XY, typename PAR>
		class denoy_entry : public impl::factory_entry<LL, XY, PAR>
		{
			public :
				virtual projection<LL, XY>* create_new(const PAR& par) const
				{
					return new base_v_f<denoy_spheroid<LL, XY, PAR>, LL, XY, PAR>(par);
				}
		};

		template <typename LL, typename XY, typename PAR>
		inline void denoy_init(impl::base_factory<LL, XY, PAR>& factory)
		{
			factory.add_to_factory("denoy", new denoy_entry<LL, XY, PAR>);
		}

	} // namespace impl
	#endif // doxygen

}

#endif // _PROJECTIONS_DENOY_HPP
