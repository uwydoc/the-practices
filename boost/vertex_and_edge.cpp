// vertex_and_edge.cpp
//
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/phoenix/phoenix.hpp>
#include <boost/type_traits/remove_reference.hpp>

BOOST_PHOENIX_ADAPT_FUNCTION(
    typename boost::remove_reference<A1>::type::vertex_descriptor
  , boost_target
  , boost::target
  , 2
)

int main(int argc, char* argv[])
{
    enum { kTopLeft = 0, kTopRight, kBottomRight, kBottomLeft };

    std::vector<std::pair<int, int> > edges;
    edges.push_back(std::make_pair<int, int>(kTopLeft, kTopRight));
    edges.push_back(std::make_pair<int, int>(kTopRight, kBottomRight));
    edges.push_back(std::make_pair<int, int>(kBottomRight, kTopLeft));
    edges.push_back(std::make_pair<int, int>(kTopRight, kBottomLeft));
    edges.push_back(std::make_pair<int, int>(kBottomLeft, kTopLeft));

    typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS> graph;
    typedef graph::vertex_descriptor vertex;
    typedef graph::vertex_iterator vertex_iterator;
    typedef graph::edge_descriptor edge;
    typedef graph::edge_iterator edge_iterator;

    graph g(edges.begin(), edges.end(), 4);

    std::cout << "--- vertices: " << boost::num_vertices(g) << " ---" << '\n';
    std::pair<vertex_iterator, vertex_iterator> vp = boost::vertices(g);
    std::copy(vp.first, vp.second, std::ostream_iterator<vertex>(std::cout, "\n"));

    std::cout << "--- edges: " << boost::num_edges(g) << " ---" << '\n';
    std::pair<edge_iterator, edge_iterator> ep = boost::edges(g);
    std::copy(ep.first, ep.second, std::ostream_iterator<edge>(std::cout, "\n"));

    graph::adjacency_iterator vit, vend;
    boost::tie(vit, vend) = boost::adjacent_vertices(kTopLeft, g);
    std::cout << "--- vertices adjacent to vertex #" << kTopLeft << " ---" << '\n';
    std::copy(vit, vend, std::ostream_iterator<vertex>(std::cout, "\n"));

    graph::out_edge_iterator eit, eend;
    boost::tie(eit, eend) = boost::out_edges(kTopLeft, g);
    std::cout << "--- edges starting from vertex #" << kTopLeft << " ---" << '\n';
    using boost::phoenix::placeholders::arg1;
    using boost::phoenix::static_cast_;
    std::transform(eit, eend, std::ostream_iterator<vertex>(std::cout, "\n"),
            //[&g](graph::edge_descriptor e) { return boost::target(e, g); }
            boost_target(arg1, boost::ref(g))
            );
}
