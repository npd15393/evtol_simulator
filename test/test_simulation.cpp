#include <vehicle.hpp>
#include <simulation.hpp>
#include <gtest/gtest.h>


TEST(SimulationTest, CreateInitializeObj) {
    evtol_sim::simulation sim;
    std::string json_str_valid = R"([
        {
            "company_name": "Alpha Company",
            "cruise_speed_mph": 120,
            "battery_capacity_kwh": 320,
            "time_to_charge_hrs": 0.6,
            "energy_used_cruise_kwh_per_mile": 1.6,
            "passenger_count": 4,
            "fault_probability_per_hr": 0.25
        },
        {
            "company_name": "Bravo Company",
            "cruise_speed_mph": 100,
            "battery_capacity_kwh": 100,
            "time_to_charge_hrs": 0.2,
            "energy_used_cruise_kwh_per_mile": 1.5,
            "passenger_count": 5,
            "fault_probability_per_hr": 0.1
        }
    ])";

    // Parse the JSON string
    nlohmann::json json_data = nlohmann::json::parse(json_str_valid);

    EXPECT_TRUE(sim.initialize(3.0, 10, 3, json_data));

    std::string json_str_invalid = R"([
        {
            "company_name": "A Company",
            "cruise_speed_mph": -2,
            "battery_capacity_kwh": 320,
            "time_to_charge_hrs": 0.6,
            "energy_used_cruise_kwh_per_mile": 1.6,
            "passenger_count": 4,
            "fault_probability_per_hr": 1.25
        },
        {
            "company_name": "B Company",
            "cruise_speed_mph": 100,
            "battery_capacity_kwh": 100,
            "time_to_charge_hrs": 0.2,
            "energy_used_cruise_kwh_per_mile": 1.5,
            "passenger_count": 5,
            "fault_probability_per_hr": 0.1
        }
    ])";

    json_data = nlohmann::json::parse(json_str_invalid);

    EXPECT_FALSE(sim.initialize(3.0, 10, 3, json_data));
}
