#include <vehicle_type.hpp>
#include <gtest/gtest.h>

TEST(VehicleTypeTest, CreateObj) {
    EXPECT_NO_THROW(evol_sim::vehicle_type_c v = evol_sim::vehicle_type_c(1.0,2.0,3.0,4.0,5,0.5));
}
