// two_dim_rand.cpp
//
#include <iostream>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random.hpp>

double on_circle_prob(double x, double y)
{
    static const double k_delta = 0.01;
    if (std::abs(x*x + y*y - 1) <= k_delta)
        return 1.0;
    else
        return 0;
}

int main(int argc, char* argv[])
{
    using boost::lexical_cast;
    using boost::random::uniform_real_distribution;
    using boost::random::discrete_distribution;

    std::size_t samples = argc > 1 ? lexical_cast<std::size_t>(argv[1]) : 100;
    std::size_t n = argc > 2 ? lexical_cast<std::size_t>(argv[2]) : 100;
    double xmin = -1.0;
    double xmax = 1.0;
    double ymin = -1.0;
    double ymax = 1.0;

    boost::mt19937 gen;
    uniform_real_distribution<> x_dist(xmin, xmax);
    for (int i = 0; i < samples; ) {
        double x = x_dist(gen);
        discrete_distribution<> y_dist(n, ymin, ymax, boost::bind(on_circle_prob, x, _1));
        int rand_n = y_dist(gen);
        double y = (rand_n * 1.0 / n) * (ymax - ymin) + ymin;
        if (true || on_circle_prob(x, y) != 0) {
            std::cout << x << ' ' << y << std::endl;
            i++;
        }
    }
}
