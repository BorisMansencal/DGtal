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

/**
 * @file
 * @ingroup Examples
 * @author Jacques-Olivier Lachaud (\c jacques-olivier.lachaud@univ-savoie.fr )
 * Laboratory of Mathematics (CNRS, UMR 5127), University of Savoie, France
 *
 * @date 2018/06/26
 *
 * An example file named shortcuts.
 *
 * This file is part of the DGtal library.
 */

///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ConfigExamples.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/helpers/Shortcuts.h"
#include "DGtal/base/Common.h"
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;

///////////////////////////////////////////////////////////////////////////////

int main( int argc, char** argv )
{
  // 3d tests
  {
    typedef Shortcuts< Z3i::KSpace > SH3;
    trace.beginBlock ( "Setting parameters" );
    auto params = SH3::defaultParameters();
    // Set your own parameters with operator().
    params( "polynomial", "3*x^2+2*y^2+z^2-90" )
      ( "gridstep", 0.5 )
      ( "noise",    0.2 )
      ( "surfaceComponents", "All" )
      ( "surfelAdjacency",   0 );
    params( "faceSubdivision", "Centroid" );
    std::cout << params << std::endl;
    trace.endBlock();
    trace.beginBlock ( "Making implicit shape" );
    auto implicit_shape = SH3::makeImplicitShape3D( params );
    std::cout << *implicit_shape << std::endl;
    trace.endBlock();
    trace.beginBlock ( "Making Khalimsky space" );
    auto K = SH3::getKSpace( params );
    std::cout << K << std::endl;
    trace.endBlock();
    trace.beginBlock ( "Making implicit digital shape" );
    auto digital_shape = SH3::makeDigitizedImplicitShape3D( implicit_shape, params );
    std::cout << *digital_shape << std::endl;
    trace.endBlock();
    trace.beginBlock ( "Making binary image from implicit digital shape" );
    auto binary_image = SH3::makeBinaryImage( digital_shape, params );
    std::cout << *binary_image << std::endl;
    trace.endBlock();
    trace.beginBlock ( "Save binary image into file" );
    auto ok = SH3::saveBinaryImage( binary_image, "dummy.vol" );
    std::cout << ( ok ? "dummy.vol OK" : "dummy.vol ERROR" ) << std::endl;
    trace.endBlock();
    trace.beginBlock ( "Making binary image from vol file" );
    auto al_capone = SH3::makeBinaryImage( examplesPath + "samples/Al.100.vol", params );
    std::cout << *al_capone << std::endl;
    auto ok2 = SH3::saveBinaryImage( al_capone, "dummy2.vol" );
    std::cout << ( ok ? "dummy2.vol OK" : "dummy2.vol ERROR" ) << std::endl;
    trace.endBlock();
    trace.beginBlock ( "Making digital surface" );
    auto Kal         = SH3::getKSpace( al_capone, params );
    auto light_surf = SH3::makeLightDigitalSurface( al_capone, Kal, params );
    std::cout << "#surfels = " << light_surf->size() << std::endl;
    std::vector< std::string > traversals { "Default", "DepthFirst", "BreadthFirst" };
    for ( auto&& mode : traversals ) {
      auto surfels = SH3::getSurfelRange( light_surf, params( "surfaceTraversal", mode ) );
      double distance  = 0.0;
      for ( unsigned int i = 1; i < surfels.size(); ++i )
  	distance += ( K.sCoords( surfels[ i-1 ] ) - K.sCoords( surfels[ i ] ) ).norm();
      std::cout << "avg " << mode << " distance = " << distance / (surfels.size()-1.0) << std::endl;
    }
    trace.endBlock();
    trace.beginBlock ( "Making all light digital surfaces" );
    auto vec_surfs   = SH3::makeLightDigitalSurfaces( al_capone, Kal, params );
    std::cout << "#connected components        = " << vec_surfs.size() << std::endl;
    unsigned int nb_small = 0;
    unsigned int nb_big = 0;
    for ( auto&& surf : vec_surfs )
      {
  	unsigned int n = surf->size();
  	nb_small += n <  100 ? 1 : 0;
  	nb_big   += n >= 100 ? 1 : 0;
      }
    std::cout << "#connected components <  100 = " << nb_small << std::endl;
    std::cout << "#connected components >= 100 = " << nb_big << std::endl;
    trace.endBlock();
    trace.beginBlock ( "Make triangulated surface from digital surface" );
    {
      SH3::Surfel2Index s2i;
      auto trisurf = SH3::makeTriangulatedSurface( s2i, light_surf );
      std::cout << "trisurf=" << *trisurf << std::endl;
    }
    trace.endBlock();
    trace.beginBlock ( "Load a vol file as a float image" );
    {
      auto gimage   = SH3::makeFloatImage( examplesPath + "samples/lobster.vol" );
      auto m        = * std::min_element( gimage->begin(), gimage->end() );
      auto M        = * std::max_element( gimage->begin(), gimage->end() );
      float     avg = 0.0;
      int        nb = 0;
      std::for_each( gimage->begin(), gimage->end(),
		     [&avg,&nb] (float x) { avg += x; nb += 1; } );
      avg          /= nb;
      std::cout << "min=" << m << " avg=" << avg << " max=" << M << std::endl;
    }
    trace.endBlock();
    trace.beginBlock ( "Make marching-cubes triangulated surface from implicit shape 3D" );
    {
      auto params   = SH3::defaultParameters  ();
      params( "polynomial", "goursat" )( "gridstep", 0.25 )
	( "thresholdMin", 128 );
      auto ishape   = SH3::makeImplicitShape3D( params );
      auto fimage   = SH3::makeFloatImage     ( ishape, params );
      auto gimage   = SH3::makeGrayScaleImage ( fimage, params );
      auto trisurf  = SH3::makeTriangulatedSurface( gimage, params );
      bool ok       = SH3::saveOBJ            ( trisurf, "goursat.obj" );
      std::cout << *ishape << std::endl;
      std::cout << "trisurf =" << *trisurf << std::endl;
      auto K        = SH3::getKSpace( gimage, params );
      auto bimage   = SH3::makeBinaryImage ( gimage, params );
      auto idx_surf = SH3::makeIdxDigitalSurface( bimage, K,
						  params( "surfaceComponents", "All" ) );
      auto primal   = SH3::makePrimalPolygonalSurface( idx_surf );
      bool ok2      = SH3::saveOBJ            ( primal, "goursat-primal.obj" );
    }
    trace.endBlock();

    trace.beginBlock ( "View marching-cubes surface from gray-scale image" );
    {
      auto gimage   = SH3::makeGrayScaleImage( examplesPath + "samples/lobster.vol" );
      auto params   = SH3::defaultParameters();
      params( "faceSubdivision", "Centroid" )( "surfelAdjacency", 1);
      auto polysurf = SH3::makePolygonalSurface( gimage, params( "thresholdMin", 40 ) );
      std::cout << "polysurf=" << *polysurf << std::endl;
      bool ok       = SH3::saveOBJ( polysurf, "lobster-40.obj" );
      auto trisurf  = SH3::makeTriangulatedSurface( gimage, params( "thresholdMin", 20 ) );
      std::cout << "trisurf =" << *trisurf << std::endl;
      bool ok2      = SH3::saveOBJ( trisurf, "lobster-20-tri.obj" );
      auto trisurf2 = SH3::makeTriangulatedSurface( polysurf, params );
      std::cout << "trisurf2=" << *trisurf2 << std::endl;
      bool ok3      = SH3::saveOBJ( trisurf2, "lobster-40-tri.obj" );
    }
    trace.endBlock();

    trace.beginBlock ( "Save digital surface as .obj file" );
    {
      ofstream objfile( "primal-al.obj" );
      bool ok = SH3::outputPrimalDigitalSurfaceAsObj( objfile, light_surf );
      std::cout << "- saving as primal-al.obj: " << ( ok ? "OK" : "ERROR" ) << std::endl;
    }
    {
      ofstream objfile( "dual-al.obj" );
      bool ok = SH3::outputDualDigitalSurfaceAsObj
	( objfile, light_surf, params );
      std::cout << "- saving as dual-al.obj: " << ( ok ? "OK" : "ERROR" ) << std::endl;
    }
    trace.endBlock();
    trace.beginBlock ( "Making indexed digital surface" );
    {
      auto idx_surf    = SH3::makeIdxDigitalSurface
	( al_capone, Kal, params( "surfaceComponents", "All" ) );
      auto positions   = idx_surf->positions();
      std::cout << "#surfels = " << idx_surf->size() << std::endl;
      for ( auto&& mode : traversals ) {
	auto surfels = SH3::getIdxSurfelRange( idx_surf, params( "surfaceTraversal", mode ) );
	double distance  = 0.0;
	for ( unsigned int i = 1; i < surfels.size(); ++i ) 
	  distance += ( positions[ surfels[ i-1 ] ] - positions[ surfels[ i ] ] ).norm();
	std::cout << "avg " << mode << " distance = " << distance / (surfels.size()-1.0) << std::endl;
      }
      auto poly_surf   = SH3::makeDualPolygonalSurface( idx_surf );
      std::cout << "polysurf = " << *poly_surf << std::endl;
      bool      okd    = SH3::saveOBJ( poly_surf, "al-idx-dual.obj" );
    }
    trace.endBlock();
    trace.beginBlock ( "Save indexed-digital surface as .obj file" );
    auto new_idx_surf = SH3::makeIdxDigitalSurface( vec_surfs );
    {
      ofstream objfile( "primal-idx-al.obj" );
      bool ok = SH3::outputPrimalIdxDigitalSurfaceAsObj( objfile, new_idx_surf );
      std::cout << "- saving as primal-idx-al.obj: " << ( ok ? "OK" : "ERROR" ) << std::endl;
    }
    {
      ofstream objfile( "dual-idx-al.obj" );
      bool ok = SH3::outputDualIdxDigitalSurfaceAsObj( objfile, new_idx_surf, params );
      std::cout << "- saving as dual-idx-al.obj: " << ( ok ? "OK" : "ERROR" ) << std::endl;
    }
    trace.endBlock();

    // trace.beginBlock ( "Compute true geometry" );
    // {
    //   params( "surfaceTraversal", "DepthFirst" )( "verbose", 0 );
    //   auto K           = SH3::getKSpace( params );
    //   auto surface     = SH3::makeLightDigitalSurface( binary_image, K, params );
    //   auto surfels     = SH3::getSurfelRange( surface, params );
    //   auto positions   = SH3::getPositions( implicit_shape, K, surfels, params ); 
    //   auto normals     = SH3::getNormalVectors( implicit_shape, K, surfels, params ); 
    //   auto mean_curv   = SH3::getMeanCurvatures( implicit_shape, K, surfels, params ); 
    //   auto gauss_curv  = SH3::getGaussianCurvatures( implicit_shape, K, surfels, params ); 
    //   SH3::RealPoint p = positions.front();
    //   SH3::RealPoint q = p;
    //   for ( auto&& r : positions ) {
    // 	p = p.inf( r ); q = q.sup( r );
    //   }
    //   SH3::Scalar   h0 =  1000.0;
    //   SH3::Scalar   h1 = -1000.0;
    //   for ( auto&& h : mean_curv ) {
    // 	h0 = std::min( h0, h );	h1 = std::max( h1, h );
    //   }
    //   SH3::Scalar   g0 =  1000.0;
    //   SH3::Scalar   g1 = -1000.0;
    //   for ( auto&& g : gauss_curv ) {
    // 	g0 = std::min( g0, g );	g1 = std::max( g1, g );
    //   }
    //   std::cout << "#position = " << positions.size()
    // 		<< " p=" << p << " q=" << q << std::endl;
    //   std::cout << "#normals = " << normals.size() << std::endl;
    //   std::cout << "H_min = " << h0 << " H_max = " << h1;
    //   std::cout << " expected: H_min = 0.0912870 H_max = 0.263523" << std::endl;
    //   std::cout << "G_min = " << g0 << " G_max = " << g1;
    //   std::cout << " expected: G_min = 0.0074074 G_max = 0.0666666" << std::endl;

    //   auto     t_normals = SH3::getTrivialNormalVectors( K, surfels );
    //   auto    ct_normals = SH3::getCTrivialNormalVectors( surface, surfels, params );
    //   auto   vcm_normals = SH3::getVCMNormalVectors( surface, surfels, params );
    //   auto    ii_normals = SH3::getIINormalVectors( binary_image, surfels, params );
    //   // Need to reorient II normals with CTrivial (otherwise unstable orientation).
    //   SH3::orientVectors( ii_normals, ct_normals );
    //   auto   t_angle_dev = SH3::getVectorsAngleDeviation( normals, t_normals );
    //   auto  ct_angle_dev = SH3::getVectorsAngleDeviation( normals, ct_normals );
    //   auto vcm_angle_dev = SH3::getVectorsAngleDeviation( normals, vcm_normals );
    //   auto  ii_angle_dev = SH3::getVectorsAngleDeviation( normals, ii_normals );
    //   std::cout << "Trivial  angle_dev  mean="
    // 		<< t_angle_dev.mean() << " max=" << t_angle_dev.max() << std::endl;
    //   std::cout << "CTrivial angle_dev  mean="
    // 		<< ct_angle_dev.mean() << " max=" << ct_angle_dev.max() << std::endl;
    //   std::cout << "VCM      angle_dev  mean="
    // 		<< vcm_angle_dev.mean() << " max=" << vcm_angle_dev.max() << std::endl;
    //   std::cout << "II       angle_dev  mean="
    // 		<< ii_angle_dev.mean() << " max=" << ii_angle_dev.max() << std::endl;
    // }
    // trace.endBlock();

  }
  // 2d tests
  {
    typedef Shortcuts< Z2i::KSpace > SH2;
    auto params = SH2::defaultParameters();
    trace.beginBlock ( "Load and threshold gray-scale image" );
    auto gl_image = SH2::makeGrayScaleImage( examplesPath + "samples/contourS.pgm" );
    auto b_image  = SH2::makeBinaryImage( gl_image, params( "thresholdMin", 128 ) );
    auto ok       = SH2::saveBinaryImage( b_image, "dummy3.pgm" );
    std::cout << *gl_image << std::endl;
    trace.endBlock();
  } 
  // debug
  {
    using namespace Z3i;
    typedef Shortcuts< KSpace > SH3;
    trace.beginBlock ( "Setting parameters" );
    auto params = SH3::defaultParameters();
    params( "faceSubdivision", "Centroid" );
    Domain domain( Point::diagonal(-1), Point::diagonal(2) );
    auto b_image = SH3::makeBinaryImage( domain );
    auto K       = SH3::getKSpace( b_image, params );
    b_image->setValue( Point( 0, 0, 0 ), true );
    b_image->setValue( Point( 0, 0, 1 ), true );
    b_image->setValue( Point( 0, 1, 1 ), true );
    b_image->setValue( Point( 1, 1, 1 ), true );
    auto light_surf = SH3::makeLightDigitalSurfaces( b_image, K, params )[ 0 ];
    std::cout << light_surf << std::endl;
    {
      ofstream objfile( "primal-test.obj" );
      bool ok = SH3::outputPrimalDigitalSurfaceAsObj( objfile, light_surf );
      std::cout << "- saving as primal-test.obj: " << ( ok ? "OK" : "ERROR" ) << std::endl;
    }
    {
      ofstream objfile( "dual-test.obj" );
      bool ok = SH3::outputDualDigitalSurfaceAsObj( objfile, light_surf, params );
      std::cout << "- saving as dual-test.obj: " << ( ok ? "OK" : "ERROR" ) << std::endl;
    }
    trace.endBlock();
  }
  return 0;
}
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
