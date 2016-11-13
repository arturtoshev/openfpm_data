#ifndef GRID_UNIT_TEST_HPP
#define GRID_UNIT_TEST_HPP

#include "config.h"
#include "map_grid.hpp"
#include "Point_test.hpp"
#include "Space/Shape/HyperCube.hpp"
#include "timer.hpp"
#include "grid_util_test.hpp"

#ifdef TEST_COVERAGE_MODE
#define GS_SIZE 8
#else
#define GS_SIZE 128
#endif

template<unsigned int dim, typename g> void test_layout_gridNd(g & c3, size_t sz);
template<unsigned int dim, typename g> void test_layout_gridObjNd(g & c3, size_t sz);
template<typename g> void test_layout_grid3d(g & c3, size_t sz);

/*! \brief Test all grid with dimensionality dim and size sz on all dimensions
 *
 * Test all grid with dimensionality dim and size sz on all dimensions
 *
 */

template<unsigned int dim> void test_all_grid(size_t sz)
{
	size_t szz[dim];

	for (size_t i = 0 ; i < dim ; i++)
	{szz[i] = sz;}

	{grid_cpu<dim, Point_test<float> > c3(szz);
	c3.setMemory();
	test_layout_gridNd<dim>(c3,sz);}

	{grid_cpu<dim, Point_test<float> > c3(szz);
	c3.setMemory();
	test_layout_gridObjNd<dim>(c3,sz);}

#ifdef CUDA_GPU

	{grid_gpu<dim, Point_test<float> > c3(szz);
	c3.template setMemory();
	test_layout_gridNd<dim>(c3,sz);}

	{grid_gpu<dim, Point_test<float> > c3(szz);
	c3.template setMemory();
	test_layout_gridObjNd<dim>(c3,sz);}

#endif

	{grid_cpu<dim, Point_test<float> > c3(szz);
	c3.setMemory();
	test_layout_gridObjNd<dim>(c3,sz);}

	{grid_cpu<dim, Point_test<float> > c3(szz);
	c3.setMemory();
	test_layout_gridNd<dim>(c3,sz);}

#ifdef CUDA_GPU

	{grid_gpu<dim, Point_test<float> > c3(szz);
	c3.setMemory();
	test_layout_gridNd<dim>(c3,sz);}

	{grid_gpu<dim, Point_test<float> > c3(szz);
	c3.setMemory();
	test_layout_gridObjNd<dim>(c3,sz);}

#endif
}



template<typename g> void test_layout_grid3d(g & c3, size_t sz)
{

#ifdef VERBOSE_TEST
	std::cout << "3D Array with grid_key (without redundant dimension): " << "\n";
#endif

	//! [Access a grid c3 of size sz on each direction]
	typedef Point_test<float> P;

	timer t;
	t.start();

	grid_key_dx<3> kk;

	for (size_t i = 0 ; i < sz ; i++)
	{
		for (size_t j = 0 ; j < sz ; j++)
		{
			for (size_t k = 0 ; k < sz ; k++)
			{

				kk.set(i,j,k);

				c3.template get<P::x>(kk) = 1.1f;
				c3.template get<P::y>(kk) = 1.2f;
				c3.template get<P::z>(kk) = 1.3f;
				c3.template get<P::s>(kk) = 1.0f;

				c3.template get<P::v>(kk)[0] = 1.0f;
				c3.template get<P::v>(kk)[1] = 2.0f;
				c3.template get<P::v>(kk)[2] = 3.0f;

				c3.template get<P::t>(kk)[0][0] = 1.0f;
				c3.template get<P::t>(kk)[0][1] = 2.0f;
				c3.template get<P::t>(kk)[0][2] = 3.0f;
				c3.template get<P::t>(kk)[1][0] = 4.0f;
				c3.template get<P::t>(kk)[1][1] = 5.0f;
				c3.template get<P::t>(kk)[1][2] = 6.0f;
				c3.template get<P::t>(kk)[2][0] = 7.0f;
				c3.template get<P::t>(kk)[2][1] = 8.0f;
				c3.template get<P::t>(kk)[2][2] = 9.0f;

			}
		}
	}

	//! [Access a grid c3 of size sz on each direction]

#ifdef VERBOSE_TEST
	t.stop();

	std::cout << "End : " << sz*sz*sz*16*4 << " Byte " << "  Bandwidth: " << sz*sz*sz*16*4/1024/1024/t.getwct() << " MB/s  ";
#endif

	/////////////////////////////////// MEM CHECK ////////////////////////////////////////////////////////

	bool passed = true;

	for (size_t i = 0 ; i < sz ; i++)
	{
		for (size_t j = 0 ; j < sz ; j++)
		{
			for (size_t k = 0 ; k < sz ; k++)
			{
				kk.set(i,j,k);

				c3.template get<P::x>(kk) = i;
				c3.template get<P::y>(kk) = j;
				c3.template get<P::z>(kk) = k;
				c3.template get<P::s>(kk) = i+j+k;

				c3.template get<P::v>(kk)[0] = i;
				c3.template get<P::v>(kk)[1] = j;
				c3.template get<P::v>(kk)[2] = k;

				c3.template get<P::t>(kk)[0][0] = i+i;
				c3.template get<P::t>(kk)[0][1] = i+j;
				c3.template get<P::t>(kk)[0][2] = i+k;
				c3.template get<P::t>(kk)[1][0] = j+i;
				c3.template get<P::t>(kk)[1][1] = j+j;
				c3.template get<P::t>(kk)[1][2] = j+k;
				c3.template get<P::t>(kk)[2][0] = k+i;
				c3.template get<P::t>(kk)[2][1] = k+j;
				c3.template get<P::t>(kk)[2][2] = k+k;
			}
		}
	}

	for (size_t i = 0 ; i < sz ; i++)
	{
		for (size_t j = 0 ; j < sz ; j++)
		{
			for (size_t k = 0 ; k < sz ; k++)
			{
				kk.set(i,j,k);

				if (c3.template get<P::x>(kk) != i) passed = false;
				if (c3.template get<P::y>(kk) != j) passed = false;
				if (c3.template get<P::z>(kk) != k) passed = false;
				if (c3.template get<P::s>(kk) != i+j+k) passed = false;

				if (c3.template get<P::v>(kk)[0] != i) passed = false;
				if (c3.template get<P::v>(kk)[1] != j) passed = false;
				if (c3.template get<P::v>(kk)[2] != k) passed = false;

				if (c3.template get<P::t>(kk)[0][0] != i+i) passed = false;
				if (c3.template get<P::t>(kk)[0][1] != i+j) passed = false;
				if (c3.template get<P::t>(kk)[0][2] != i+k) passed = false;
				if (c3.template get<P::t>(kk)[1][0] != j+i) passed = false;
				if (c3.template get<P::t>(kk)[1][1] != j+j) passed = false;
				if (c3.template get<P::t>(kk)[1][2] != j+k) passed = false;
				if (c3.template get<P::t>(kk)[2][0] != k+i) passed = false;
				if (c3.template get<P::t>(kk)[2][1] != k+j) passed = false;
				if (c3.template get<P::t>(kk)[2][2] != k+k) passed = false;
			}
		}
	}

	BOOST_REQUIRE_EQUAL(passed,true);
}

template<unsigned int dim, typename g> void test_layout_gridObjNd(g & c3, size_t sz)
{
#ifdef VERBOSE_TEST
	std::cout << dim << "D Array with grid_key (without redundant dimension): " << "\n";

	timer t;
	t.start();
#endif

	//! [Get the object in an N-dimensional grid with an iterator]

	typedef Point_test<float> P;

	grid_key_dx_iterator<dim> key_it = c3.getIterator();

	while (key_it.isNext())
	{
		grid_key_dx<dim> kk = key_it.get();

		// Here we get a reference to the object, in reality we get an encapsulated object reference encapc
		auto v = c3.get_o(kk);

		// An encapsulated object can be accessed like that
		// (this will change the value in the grid)
		v.template get<P::x>() = 1.1f;
		v.template get<P::y>() = 1.2f;
		v.template get<P::z>() = 1.3f;
		v.template get<P::s>() = 1.0f;

		v.template get<P::v>()[0] = 1.0f;
		v.template get<P::v>()[1] = 2.0f;
		v.template get<P::v>()[2] = 3.0f;

		v.template get<P::t>()[0][0] = 1.0f;
		v.template get<P::t>()[0][1] = 2.0f;
		v.template get<P::t>()[0][2] = 3.0f;
		v.template get<P::t>()[1][0] = 4.0f;
		v.template get<P::t>()[1][1] = 5.0f;
		v.template get<P::t>()[1][2] = 6.0f;
		v.template get<P::t>()[2][0] = 7.0f;
		v.template get<P::t>()[2][1] = 8.0f;
		v.template get<P::t>()[2][2] = 9.0f;

		// From an encapsulated reference object you can create
		// an object
		Point_test<float> obj = c3.get_o(kk);

		// And do some operation
		obj.fill();

		// Note change obj does not change the grid

		++key_it;

	}

	//! [Get the object in an N-dimensional grid with an iterator]

#ifdef VERBOSE_TEST
	t.stop();

	std::cout << "End : " << pow(sz,dim)*16*4/1024/1024 << " MB " << "  Bandwidth: " << pow(sz,dim)*16*4/1024/1024/t.getect() << " MB/s  " << "\n";
#endif

	/////////////////////////////////// MEM CHECK ////////////////////////////////////////////////////////

	bool passed = true;

	//! Fill the grid with some data
	fill_grid<dim>(c3);

	key_it = c3.getIterator();

	while (key_it.isNext())
	{
		grid_key_dx<dim> kk = key_it.get();

		if (c3.template get<P::x>(kk) != c3.getGrid().LinId(kk)) passed = false;
		if (c3.template get<P::y>(kk) != c3.getGrid().LinId(kk)+1) passed = false;
		if (c3.template get<P::z>(kk) != c3.getGrid().LinId(kk)+2) passed = false;
		if (c3.template get<P::s>(kk) != c3.getGrid().LinId(kk)+3) passed = false;

		if (c3.template get<P::v>(kk)[0] != c3.getGrid().LinId(kk)+123) passed = false;
		if (c3.template get<P::v>(kk)[1] != c3.getGrid().LinId(kk)+124) passed = false;
		if (c3.template get<P::v>(kk)[2] != c3.getGrid().LinId(kk)+125) passed = false;

		if (c3.template get<P::t>(kk)[0][0] != c3.getGrid().LinId(kk)+567) passed = false;
		if (c3.template get<P::t>(kk)[0][1] != c3.getGrid().LinId(kk)+568) passed = false;
		if (c3.template get<P::t>(kk)[0][2] != c3.getGrid().LinId(kk)+569) passed = false;
		if (c3.template get<P::t>(kk)[1][0] != c3.getGrid().LinId(kk)+570) passed = false;
		if (c3.template get<P::t>(kk)[1][1] != c3.getGrid().LinId(kk)+571) passed = false;
		if (c3.template get<P::t>(kk)[1][2] != c3.getGrid().LinId(kk)+572) passed = false;
		if (c3.template get<P::t>(kk)[2][0] != c3.getGrid().LinId(kk)+573) passed = false;
		if (c3.template get<P::t>(kk)[2][1] != c3.getGrid().LinId(kk)+574) passed = false;
		if (c3.template get<P::t>(kk)[2][2] != c3.getGrid().LinId(kk)+575) passed = false;

		++key_it;
	}

	BOOST_REQUIRE_EQUAL(passed,true);
}

template<unsigned int dim, typename g> void test_layout_gridNd(g & c3, size_t sz)
{
#ifdef VERBOSE_TEST
	std::cout << dim << "D Array with grid_key (without redundant dimension): " << "\n";

	timer t;
	t.start();
#endif

	//! [Access to an N-dimensional grid with an iterator]
	typedef Point_test<float> P;

	grid_key_dx_iterator<dim> key_it = c3.getIterator();

	while (key_it.isNext())
	{
		grid_key_dx<dim> kk = key_it.get();

		c3.template get<P::x>(kk) = 1.1f;
		c3.template get<P::y>(kk) = 1.2f;
		c3.template get<P::z>(kk) = 1.3f;
		c3.template get<P::s>(kk) = 1.0f;

		c3.template get<P::v>(kk)[0] = 1.0f;
		c3.template get<P::v>(kk)[1] = 2.0f;
		c3.template get<P::v>(kk)[2] = 3.0f;

		c3.template get<P::t>(kk)[0][0] = 1.0f;
		c3.template get<P::t>(kk)[0][1] = 2.0f;
		c3.template get<P::t>(kk)[0][2] = 3.0f;
		c3.template get<P::t>(kk)[1][0] = 4.0f;
		c3.template get<P::t>(kk)[1][1] = 5.0f;
		c3.template get<P::t>(kk)[1][2] = 6.0f;
		c3.template get<P::t>(kk)[2][0] = 7.0f;
		c3.template get<P::t>(kk)[2][1] = 8.0f;
		c3.template get<P::t>(kk)[2][2] = 9.0f;

		++key_it;

	}
	//! [Access to an N-dimensional grid with an iterator]

#ifdef VERBOSE_TEST
	t.stop();

	std::cout << "End : " << pow(sz,dim)*16*4/1024/1024 << " MB " << "  Bandwidth: " << pow(sz,dim)*16*4/1024/1024/t.getwct() << " MB/s  " << "\n";
#endif

	/////////////////////////////////// MEM CHECK ////////////////////////////////////////////////////////

	bool passed = true;

	key_it = c3.getIterator();

	while (key_it.isNext())
	{
		grid_key_dx<dim> kk = key_it.get();

		c3.template get<P::x>(kk) = c3.getGrid().LinId(kk);
		c3.template get<P::y>(kk) = c3.getGrid().LinId(kk)+1;
		c3.template get<P::z>(kk) = c3.getGrid().LinId(kk)+2;
		c3.template get<P::s>(kk) = c3.getGrid().LinId(kk)+3;

		c3.template get<P::v>(kk)[0] = c3.getGrid().LinId(kk)+123;
		c3.template get<P::v>(kk)[1] = c3.getGrid().LinId(kk)+124;
		c3.template get<P::v>(kk)[2] = c3.getGrid().LinId(kk)+125;

		c3.template get<P::t>(kk)[0][0] = c3.getGrid().LinId(kk)+567;
		c3.template get<P::t>(kk)[0][1] = c3.getGrid().LinId(kk)+568;
		c3.template get<P::t>(kk)[0][2] = c3.getGrid().LinId(kk)+569;
		c3.template get<P::t>(kk)[1][0] = c3.getGrid().LinId(kk)+570;
		c3.template get<P::t>(kk)[1][1] = c3.getGrid().LinId(kk)+571;
		c3.template get<P::t>(kk)[1][2] = c3.getGrid().LinId(kk)+572;
		c3.template get<P::t>(kk)[2][0] = c3.getGrid().LinId(kk)+573;
		c3.template get<P::t>(kk)[2][1] = c3.getGrid().LinId(kk)+574;
		c3.template get<P::t>(kk)[2][2] = c3.getGrid().LinId(kk)+575;

		++key_it;
	}


	key_it = c3.getIterator();

	while (key_it.isNext())
	{
		grid_key_dx<dim> kk = key_it.get();

		if (c3.template get<P::x>(kk) != c3.getGrid().LinId(kk)) passed = false;
		if (c3.template get<P::y>(kk) != c3.getGrid().LinId(kk)+1) passed = false;
		if (c3.template get<P::z>(kk) != c3.getGrid().LinId(kk)+2) passed = false;
		if (c3.template get<P::s>(kk) != c3.getGrid().LinId(kk)+3) passed = false;

		if (c3.template get<P::v>(kk)[0] != c3.getGrid().LinId(kk)+123) passed = false;
		if (c3.template get<P::v>(kk)[1] != c3.getGrid().LinId(kk)+124) passed = false;
		if (c3.template get<P::v>(kk)[2] != c3.getGrid().LinId(kk)+125) passed = false;

		if (c3.template get<P::t>(kk)[0][0] != c3.getGrid().LinId(kk)+567) passed = false;
		if (c3.template get<P::t>(kk)[0][1] != c3.getGrid().LinId(kk)+568) passed = false;
		if (c3.template get<P::t>(kk)[0][2] != c3.getGrid().LinId(kk)+569) passed = false;
		if (c3.template get<P::t>(kk)[1][0] != c3.getGrid().LinId(kk)+570) passed = false;
		if (c3.template get<P::t>(kk)[1][1] != c3.getGrid().LinId(kk)+571) passed = false;
		if (c3.template get<P::t>(kk)[1][2] != c3.getGrid().LinId(kk)+572) passed = false;
		if (c3.template get<P::t>(kk)[2][0] != c3.getGrid().LinId(kk)+573) passed = false;
		if (c3.template get<P::t>(kk)[2][1] != c3.getGrid().LinId(kk)+574) passed = false;
		if (c3.template get<P::t>(kk)[2][2] != c3.getGrid().LinId(kk)+575) passed = false;

		++key_it;
	}

	BOOST_REQUIRE_EQUAL(passed,true);

	// Check sub iterator

	/*
	 * Basically we first fill the interior part of the grid than the borders
	 * creating sub iterator always of smaller dimension
	 *
	 * Example:
	 *
	 * 2D
	 *
	 * if we have a square grid 16x16 we first will with 1 the interior 14x14 grid
	 *
	 * than the 4 line borders 14x1 with one
	 * than the 4 point borders 1x1
	 *
	 * We check that
	 *
	 * 1) The number of points for each sub-grid correspond
	 * 2) No point is filled more than one time
	 * 3) All point are filled
	 *
	 * we use the property x of c3
	 *
	 */

	// Erase the property x

	key_it = c3.getIterator();

	while (key_it.isNext())
	{
		grid_key_dx<dim> kk = key_it.get();

		c3.template get<P::x>(kk) = 0.0;

		++key_it;
	}

	for(size_t i = 0 ; i <= dim ; i++)
	{
		// get the combination of dimension dim-i
		std::vector<comb<dim>> combs = HyperCube<dim>::getCombinations_R(dim-i);

		// For each combination create a sub iterator

		for (size_t j = 0 ; j < combs.size() ; j++)
		{
			// Grid key of the sub-iterator

			grid_key_dx<dim> start;
			grid_key_dx<dim> stop;

			// sub iterator

			for (size_t k = 0 ; k < dim ; k++)
			{
				// if combination is 0 the hyper-cube

				if (combs[j].c[k] == -1)
				{
					start.set_d(k,0);
					stop.set_d(k,0);
				}
				else if (combs[j].c[k] == 1)
				{
					start.set_d(k,c3.getGrid().size(k)-1);
					stop.set_d(k,c3.getGrid().size(k)-1);
				}
				else
				{
					start.set_d(k,1);
					stop.set_d(k,c3.getGrid().size(k)-2);
				}
			}

			auto key_it = c3.getSubIterator(start,stop);

			while (key_it.isNext())
			{
				grid_key_dx<dim> kk = key_it.get();

				BOOST_REQUIRE_EQUAL(c3.template get<P::x>(kk),0.0);

				c3.template get<P::x>(kk) = 1.0;

				++key_it;
			}
		}
	}

	// Check that everything is 1.0

	key_it = c3.getIterator();

	while (key_it.isNext())
	{
		grid_key_dx<dim> kk = key_it.get();

		BOOST_REQUIRE_EQUAL(c3.template get<P::x>(kk),1.0);

		++key_it;
	}
}

BOOST_AUTO_TEST_SUITE( grid_test )

////////// Test function ///////////

#ifdef SE_CLASS2

grid_cpu<3,scalar<float>> & test_error()
{
	size_t sz[] = {16,16,16};

	grid_cpu<3,scalar<float>> g(sz);

	return g;
}

#endif

/////////////////////////////////////

BOOST_AUTO_TEST_CASE( grid_safety_check )
{
#if defined(SE_CLASS1) && defined (THROW_ON_ERROR)

	bool error = false;

	typedef Point_test<float> p;

	size_t sz[] = {16,16,16};

	// Create a grid

	grid_cpu<3,Point_test<float>> g(sz);
	grid_cpu<3,Point_test<float>> g2(sz);

	// try to access uninitialized grid
	grid_key_dx<3> keyOut(23,1,1);
	grid_key_dx<3> keyGood(15,1,1);
	grid_key_dx<3> keyNeg(-1,0,0);

	error = false;
	try
	{g.get<p::x>(keyOut);}
	catch (size_t e)
	{
		error = true;
		BOOST_REQUIRE_EQUAL(e,GRID_ERROR);
		BOOST_REQUIRE_EQUAL(g.getLastError(),1001ul);
	}
	BOOST_REQUIRE_EQUAL(error,true);

	error = false;
	g.setMemory();
	try
	{g.get<p::x>(keyOut);}
	catch (size_t e)
	{
		error = true;
		BOOST_REQUIRE_EQUAL(e,GRID_ERROR);
		BOOST_REQUIRE_EQUAL(g.getLastError(),1002ul);
	}
	BOOST_REQUIRE_EQUAL(error,true);

	error = false;
	Point_test<float> t;
	try
	{g.set(keyOut,t);}
	catch (size_t e)
	{
		error = true;
		BOOST_REQUIRE_EQUAL(e,GRID_ERROR);
		BOOST_REQUIRE_EQUAL(g.getLastError(),1002ul);
	}
	BOOST_REQUIRE_EQUAL(error,true);

	error = false;
	try
	{g.set(keyGood,g2,keyOut);}
	catch (size_t e)
	{
		error = true;
		BOOST_REQUIRE_EQUAL(e,GRID_ERROR);
		BOOST_REQUIRE_EQUAL(g.getLastError(),1004ul);
	}
	BOOST_REQUIRE_EQUAL(error,true);

	//// Negative key

	error = false;
	try
	{g.get<p::x>(keyNeg);}
	catch (size_t e)
	{
		error = true;
		BOOST_REQUIRE_EQUAL(e,GRID_ERROR);
		BOOST_REQUIRE_EQUAL(g.getLastError(),1003ul);
	}
	BOOST_REQUIRE_EQUAL(error,true);

	error = false;
	Point_test<float> t2;
	try
	{g.set(keyNeg,t2);}
	catch (size_t e)
	{
		error = true;
		BOOST_REQUIRE_EQUAL(e,GRID_ERROR);
		BOOST_REQUIRE_EQUAL(g.getLastError(),1003ul);
	}
	BOOST_REQUIRE_EQUAL(error,true);

	error = false;
	try
	{g.set(keyGood,g2,keyNeg);}
	catch (size_t e)
	{
		error = true;
		BOOST_REQUIRE_EQUAL(e,GRID_ERROR);
		BOOST_REQUIRE_EQUAL(g.getLastError(),1005ul);
	}
	BOOST_REQUIRE_EQUAL(error,true);

	#if defined(SE_CLASS2) && defined (THROW_ON_ERROR)

	error = false;

	// Create a grid

	grid_cpu<3,scalar<float>> * gp = new grid_cpu<3,scalar<float>>(sz);
	delete gp;

	// Try to access the class

	try
	{gp->size();}
	catch (size_t e)
	{
		error = true;
		BOOST_REQUIRE_EQUAL(e,MEM_ERROR);
	}
	BOOST_REQUIRE_EQUAL(error,true);

	error = false;
	try
	{grid_cpu<3,scalar<float>> gr = test_error();}
	catch (size_t e)
	{
		error = true;
		BOOST_REQUIRE_EQUAL(e,MEM_ERROR);
	}
	BOOST_REQUIRE_EQUAL(error,true);

	#endif

#endif
}

BOOST_AUTO_TEST_CASE( grid_use)
{
	/*  tensor<int,3,3,3> c;
	  tensor<tensor<int,3,3,3>,3,3,3> c2;*/

	std::cout << "Grid unit test start" << "\n";

	size_t sz[3] = {GS_SIZE,GS_SIZE,GS_SIZE};

	// test the grid from dimensionality 1 to 8 with several size non multiple of two
	// Dimension 8-1

	// With test coverage reduce the test size

#ifndef TEST_COVERAGE_MODE
	test_all_grid<8>(4);
	test_all_grid<7>(8);
	test_all_grid<6>(9);
	test_all_grid<5>(18);
	test_all_grid<4>(37);
	test_all_grid<3>(126);
	test_all_grid<2>(1414);
	test_all_grid<1>(2000000);


	// Special case grid of size 0
	test_all_grid<8>(0);
	test_all_grid<7>(0);
	test_all_grid<6>(0);
	test_all_grid<5>(0);
	test_all_grid<4>(0);
	test_all_grid<3>(0);
	test_all_grid<2>(0);
	test_all_grid<1>(0);
#else
	test_all_grid<4>(4);
	test_all_grid<3>(8);
	test_all_grid<2>(16);
	test_all_grid<1>(256);
#endif

	// Test the 3d gpu grid with CudaMemory and HeapMemory with different size

	for (int i = 0 ; i <= GS_SIZE ; i++)
	{
#ifdef CUDA_GPU

		{
		//! [Definition and allocation of a 3D grid on GPU memory]
		grid_gpu<3, Point_test<float> > c3(sz);
		c3.setMemory();
		//! [Definition and allocation of a 3D grid on GPU memory]
		test_layout_grid3d(c3,i);
		}

#endif

		{grid_cpu<3, Point_test<float> > c3(sz);
		c3.setMemory();
		test_layout_grid3d(c3,i);}

		{
		//! [Definition and allocation of a 3D grid on CPU memory]
		grid_cpu<3, Point_test<float> > c3(sz);
		c3.setMemory();
		//! [Definition and allocation of a 3D grid on CPU memory]
		test_layout_grid3d(c3,i);}

		// Test the 3d cpu grid with Cudamemory and HeapMemory

		{grid_cpu<3, Point_test<float> > c3(sz);
		c3.setMemory();
		test_layout_grid3d(c3,i);}

	}

	std::cout << "Grid unit test end" << "\n";
}

/* \brief This is an ordinary test simple 3D with plain C array
 *
 * This is an ordinary test simple 3D with plain C array
 *
 */

BOOST_AUTO_TEST_CASE( C_array_test )
{
	// Testing the grids

#ifdef VERBOSE_TEST
	std::cout << "Grid size known at runtime" << "\n";
	std::cout << "1D Array with index calculation: " << "\n";
#endif

	Point_orig<float> * pA = new Point_orig<float>[GS_SIZE*GS_SIZE*GS_SIZE];

	int gs_sq = GS_SIZE*GS_SIZE;
	int gs = GS_SIZE;

#ifdef VERBOSE_TEST
	timer t;
	t.start();
#endif

	for (int i = 0 ; i < GS_SIZE ; i++)
	{
		for (int j = 0 ; j < GS_SIZE ; j++)
		{
			for (int k = 0 ; k < GS_SIZE ; k++)
			{
				pA[i*gs_sq+j*gs+k].x = 1.1f;
				pA[i*gs_sq+j*gs+k].y = 1.2f;
				pA[i*gs_sq+j*gs+k].z = 1.3f;
				pA[i*gs_sq+j*gs+k].s = 1.0f;

				pA[i*gs_sq+j*gs+k].v[0] = 1.0f;
				pA[i*gs_sq+j*gs+k].v[1] = 2.0f;
				pA[i*gs_sq+j*gs+k].v[2] = 3.0f;

				pA[i*gs_sq+j*gs+k].t[0][0] = 1.0f;
				pA[i*gs_sq+j*gs+k].t[0][1] = 2.0f;
				pA[i*gs_sq+j*gs+k].t[0][2] = 3.0f;
				pA[i*gs_sq+j*gs+k].t[1][0] = 4.0f;
				pA[i*gs_sq+j*gs+k].t[1][1] = 5.0f;
				pA[i*gs_sq+j*gs+k].t[1][2] = 6.0f;
				pA[i*gs_sq+j*gs+k].t[2][0] = 7.0f;
				pA[i*gs_sq+j*gs+k].t[2][1] = 8.0f;
				pA[i*gs_sq+j*gs+k].t[2][2] = 9.0f;
			}
		}
	}

	delete [] pA;

#ifdef VERBOSE_TEST
	t.stop();

	std::cout << "End : " << GS_SIZE*GS_SIZE*GS_SIZE*16*4/1024/1024 << " MB " << "  Bandwidth: " << GS_SIZE*GS_SIZE*GS_SIZE*16*4/1024/1024/t.getcputime() << " MB/s  \n";
#endif
}

BOOST_AUTO_TEST_CASE(grid_operator_equal)
{
	//! [Create a grid g1 and copy into another g2]

	size_t sz[] = {16,16};

	typedef Box<2,float> b;

	grid_cpu<2,Box<2,float>> g1(sz);
	g1.setMemory();

	auto it = g1.getIterator();

	while (it.isNext())
	{
		auto key = it.get();

		g1.get<b::p1>(key)[0] = key.get(0);
		g1.get<b::p2>(key)[1] = key.get(1);

		++it;
	}

	grid_cpu<2,Box<2,float>> g2;
	g2 = g1;

	//! [Create a grid g1 and copy into another g2]

	bool ret = (g2 == g1);

	BOOST_REQUIRE_EQUAL(ret,true);
}

/*! \brief Fill twi grid of boxes with data
 *
 * \param g1 Grid1
 * \param g2 Grid2
 *
 */

void fill_2_grid_data(grid_cpu<2,Box<2,float>> & g1, grid_cpu<2,Box<2,float>> & g2)
{
	typedef Box<2,float> b;

	auto it1 = g1.getIterator();

	while (it1.isNext())
	{
		auto key = it1.get();

		g1.get<b::p1>(key)[0] = key.get(0);
		g1.get<b::p2>(key)[1] = key.get(1);

		++it1;
	}

	auto it2 = g2.getIterator();

	while (it2.isNext())
	{
		auto key = it2.get();

		g2.get<b::p1>(key)[0] = key.get(0);
		g2.get<b::p2>(key)[1] = key.get(1);

		++it2;
	}


}

BOOST_AUTO_TEST_CASE(grid_operator_swap)
{
	size_t sz1[] = {16,16};
	size_t sz2[] = {5,5};

	grid_cpu<2,Box<2,float>> g1_old(sz1);
	g1_old.setMemory();
	grid_cpu<2,Box<2,float>> g2_old(sz2);
	g2_old.setMemory();
	grid_cpu<2,Box<2,float>> g1(sz1);
	g1.setMemory();
	grid_cpu<2,Box<2,float>> g2(sz2);
	g2.setMemory();

	fill_2_grid_data(g1_old,g2_old);
	fill_2_grid_data(g1,g2);

	//! [swap the memory of two grids]

	g2.swap(g1);

	//! [swap the memory of two grids]

	bool ret = (g2 == g1_old);
	BOOST_REQUIRE_EQUAL(ret,true);
	ret = (g1 == g2_old);
	BOOST_REQUIRE_EQUAL(ret,true);
}

BOOST_AUTO_TEST_CASE(grid_resize_less)
{
	size_t sz1[] = {256,256};
	size_t sz2[] = {5,5};

	grid_cpu<2,Box<2,float>> g1(sz1);
	g1.setMemory();

	fill_2_grid_data(g1,g1);


	g1.resize(sz2);

	BOOST_REQUIRE_EQUAL(g1.size(),25ul);
}

BOOST_AUTO_TEST_SUITE_END()

#endif
