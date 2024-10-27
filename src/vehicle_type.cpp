#include <vehicle_type.hpp>
#include <iostream>

namespace evol_sim {

vehicle_type_c::vehicle_type_c(
    float cruise_speed,
    float battery,
    float charge_time,
    float crg_per_mile,
    int seats,
    float p_fault) :
    m_cruise_speed(cruise_speed), m_battery_capacity(battery),
    m_charge_time(charge_time), m_energy_per_mile(crg_per_mile),
    m_seats(seats), m_fault_probability(p_fault), last_t(0)
{}

void vehicle_type_c::tick(int t) {
    std::cout<<(t-last_t)<<std::endl;
}

}