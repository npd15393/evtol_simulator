#include <vehicle.hpp>
#include <gtest/gtest.h>

TEST(VehicleTest, CreateObj) {
    EXPECT_NO_THROW(evtol_sim::vehicle v = evtol_sim::vehicle("abc", 0, 3600.0, 2.0, 3.0, 4.0, 5, 0.5));
}

TEST(VehicleTest, StateUpdates) {
    evtol_sim::vehicle v = evtol_sim::vehicle("abc", 0, 3600, 1.0, 1.0 / 1800.0, 0.5, 5, 0.5);
    // Plug in
    v.dock(true);
    EXPECT_EQ(evtol_sim::vehicle::STATE::CHARGING, v.get_state());
    // Plug out
    v.dock(false);
    EXPECT_EQ(evtol_sim::vehicle::STATE::FLYING, v.get_state());
    // Add to charging queue
    v.wait();
    EXPECT_EQ(evtol_sim::vehicle::STATE::WAITING, v.get_state());
    // When flying
    v.dock(false);
    EXPECT_EQ(evtol_sim::vehicle::STATE::FLYING, v.get_state());
    v.update_state(1);
    EXPECT_EQ(evtol_sim::vehicle::STATE::FLYING, v.get_state());
    v.update_state(2);
    EXPECT_EQ(evtol_sim::vehicle::STATE::BATTERY_EMPTY, v.get_state());
    // When charging
    v.dock(true);
    EXPECT_EQ(evtol_sim::vehicle::STATE::CHARGING, v.get_state());
    v.update_state(3);
    EXPECT_EQ(evtol_sim::vehicle::STATE::CHARGING, v.get_state());
    v.update_state(4);
    EXPECT_EQ(evtol_sim::vehicle::STATE::BATTERY_FULL, v.get_state());
}

TEST(VehicleTest, GetStats) {
    evtol_sim::vehicle v = evtol_sim::vehicle("abc", 0, 3600, 1.0, 1.0 / 1800.0, 0.5, 5, 0.5);
    EXPECT_NO_THROW(v.get_summarized_stats());
}

