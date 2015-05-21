/*
 * CellDecomposer.hpp
 *
 *  Created on: Apr 1, 2015
 *      Author: Pietro Incardona
 */

#ifndef CELLDECOMPOSER_HPP_
#define CELLDECOMPOSER_HPP_

#include "Space/SpaceBox.hpp"

/*! \brief Decompose a cell into space
 *
 * It is a convenient class for cell cell decomposition and index linearization
 *
 */

template<unsigned int dim,typename T>
class CellDecomposer_sm
{
protected:

	// Total number of cell
	size_t tot_n_cell;

	// Domain of the cell list
	SpaceBox<dim,T> box;

	// Unit box of the Cell list
	SpaceBox<dim,T> box_unit;

	// Grid structure of the Cell list
	grid_sm<dim,void> gr_cell;

	// Cell padding
	size_t padding;

	/*! \brief Initialize
	 *
	 */
	void Initialize()
	{
		tot_n_cell = 1;

		// Total number of cells and calculate the unit cell size

		for (size_t i = 0 ; i < dim ; i++)
		{
			tot_n_cell *= gr_cell.size(i);

			// Cell are padded by 1
			box_unit.setHigh(i,box.getHigh(i) / (gr_cell.size(i)-2));
		}

		size_t off[dim];
		for (size_t i = 0; i < dim ; i++)
			off[i] = 1;

		padding = gr_cell.LinId(off);
	}

public:

	/*! \brief Return the underlying grid information of the cell list
	 *
	 * \return the grid infos
	 *
	 */
	grid_sm<dim,void> & getGrid()
	{
		return gr_cell;
	}

	/*! \brief Get the cell-id
	 *
	 * Convert the point coordinates into the cell id
	 *
	 * \param pos Point position
	 *
	 * \return the cell-id
	 *
	 */
	size_t getCell(const T (& pos)[dim])
	{
		size_t cell_id = pos[0] / box_unit.getHigh(0);

		for (size_t s = 1 ; s < dim ; s++)
		{
			cell_id += gr_cell.size(s) * ((size_t)(pos[s] / box_unit.getHigh(s)));
		}

		return cell_id + padding;
	}

	/*! \brief Get the cell-id
	 *
	 * Convert the point coordinates into the cell id
	 *
	 * \param pos Point position
	 *
	 * \return the cell-id
	 *
	 */
	size_t getCell(const Point<dim,T> & pos)
	{
		size_t cell_id = pos.get(0) / box_unit.getHigh(0);

		for (size_t s = 1 ; s < dim ; s++)
		{
			cell_id += gr_cell.size_s(s-1) * ((size_t)(pos.get(s) / box_unit.getHigh(s)));
		}

		return cell_id;
	}

	/*! \brief Get the cell-id
	 *
	 * Convert the point coordinates into the cell id
	 *
	 * \param pos Point position
	 *
	 * \return the cell-id
	 *
	 */
	template<typename Mem> size_t getCell(const encapc<1,Point<dim,T>,Mem> & pos, const size_t pad = 0)
	{
		typedef Point<dim,T> p;

		size_t cell_id = pos.template get<p::x>()[0] / box_unit.getHigh(0);

		for (size_t s = 1 ; s < dim ; s++)
		{
			cell_id += gr_cell.size_s(s-1) * (size_t)(pos.template get<p::x>()[s] / box_unit.getHigh(s) + pad);
		}

		return cell_id;
	}

	/*! \brief Set the domain to decompose
	 *
	 * \param box Domain to decompose
	 * \param div array with the number of cells on each dimensions
	 *
	 */
	void setDimensions(SpaceBox<dim,T> & box, const size_t (&div)[dim])
	{
		this->box = box;
		this->gr_cell.setDimensions(div);
		Initialize();
	}

	/*! \brief Set the domain to decompose
	 *
	 * \param box Domain to decompose
	 * \param div array with the number of cells on each dimensions
	 *
	 */
	void setDimensions(Box<dim,T> & box, const size_t (&div)[dim])
	{
		this->box = box;
		this->gr_cell.setDimensions(div);
		Initialize();
	}

	/*! \brief Constructor
	 *
	 * \param box Space where is defined the cell list (it is assumed p1 = {0, .... 0})
	 * \param div Reference array to the number of divisions on each dimensions
	 * \pad cell padding
	 *
	 *  Example for div = {7,7} and pad = 1
	 *
	 * +-----------------------+
     * |p |p |p |p |p |p |p |p |
     * +-----------------------+
     * |p |  |  |  |  |  |  |p |
     * +-----------------------+
     * |p |  |  |  |  |  |  |p |
     * +-----------------------+
     * |p |  |  |  |  |  |  |p |
     * +-----------------------+
     * |p |9 |  |  |  |  |  |p |
     * +-----------------------+
     * |p |p |p |p |p |p |p |p |
     * +-----------------------+
	 *
	 * Cell with p are padding cell cell that are around but external the box, the cell number 9 that
	 * is at the origin of the box is identified with 9
	 *
	 */
	CellDecomposer_sm(SpaceBox<dim,T> & box, size_t (&div)[dim])
	:box(box),gr_cell(div)
	{
		Initialize();
	}


	//! default constructor
	CellDecomposer_sm()
	{

	}
};


#endif /* CELLDECOMPOSER_HPP_ */
