#ifndef EDGE_H
#define EDGE_H

#include <glm/glm.hpp>

namespace fe { namespace collision {

	class SupportPoint;


	/**
	 * Struct Edge
	 */
	struct Edge
	{
		/** The first point of the Edge */
		SupportPoint* p1;

		/** The second point of the Edge */
		SupportPoint* p2;

		/** Compares the current Edge with the given one
		 *
		 * @param	other the Edge to compare with the current one
		 * @return	true if the Edges are the same, false otherwise */
		bool operator==(const Edge& other) const
		{ return (p1 == other.p1) && (p2 == other.p2); }
	};


	/** Calculates the minimum distance from a point to the given edge
	 *
	 * @param	p the point whose distance to the edge we want to know
	 * @param	e1 the first point of the edge
	 * @param	e2 the second point of the edge
	 * @return	the minimum distance of p to the edge between e1 and e2 */
	float distancePointEdge(
		const glm::vec3& p,
		const glm::vec3& e1, const glm::vec3& e2
	);

}}

#endif		// EDGE_H