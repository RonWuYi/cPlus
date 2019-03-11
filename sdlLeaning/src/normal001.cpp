#include <iostream>
#include <iomanip>
#include <random>

//std::default_random_engine engine(static_cast<unsigned int>( time(nullptr)) );
//std::uniform_int_distribution<unsigned int> randomInt(1, 6);
//
//int getNumber();

template <typename Distribution>
typename Distribution::result_type getNumber(
        std::default_random_engine& random_engine,
        Distribution& distribution
        )
{
    return distribution( random_engine );
}
int main(){


    std::default_random_engine engine(static_cast<unsigned int>( time(nullptr)) );
    std::uniform_int_distribution<unsigned int> randomInt(1, 6);

    for (unsigned int counter = 1; counter <= 10 ; ++counter) {
        std::cout << std::setw( 10 ) <<randomInt( engine );
        if ( counter % 5 == 0 )
            std::cout<<std::endl;
    }
    std::cout<< getNumber( engine, randomInt ) <<std::endl;
    return 0;

}

//int getNumber(){
//    return randomInt( engine );
//}
