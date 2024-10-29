#include <vehicle.hpp>
#include <iostream>

namespace evtol_sim {

vehicle::vehicle(
    std::string company_nm,
    int vehicle_id,
    float cruise_speed,
    float battery,
    float charge_time,
    float crg_per_mile,
    int seats,
    float p_fault) :
    m_company(company_nm), m_cruise_speed_mps(cruise_speed / 3600.0), m_battery_capacity(battery),
    m_charge_time(charge_time), m_energy_per_mile(crg_per_mile),
    m_seats(seats), m_fault_probability_rate(p_fault), m_vehicle_id(vehicle_id), m_last_t(0), m_last_fault_t(0),
    m_current_charge_kwh(m_battery_capacity), m_current_state(STATE::FLYING)
{
    m_recharge_rate_kwh_per_s = m_battery_capacity / (m_charge_time * 3600);
    m_charge_used_kwh_per_s = m_energy_per_mile * m_cruise_speed_mps;
}

void vehicle::set_id(int const id) {
    m_vehicle_id = id;
}

int vehicle::get_id() const {
    return m_vehicle_id;
}

vehicle::STATE vehicle::get_state() const {
    return m_current_state;
}

void vehicle::dock(bool charging) {
    if(charging) {
        m_current_state = vehicle::STATE::CHARGING;
    } else {
        m_current_state = vehicle::STATE::FLYING;
    }
}

evtol_sim::vehicle_stats vehicle::get_summarized_stats() {
    if(vehicle::STATE::FLYING == m_current_state) {
        m_stats.num_flights = m_stats.num_flights + 1;
    } else if(vehicle::STATE::CHARGING == m_current_state) {
        m_stats.num_charge_sessions = m_stats.num_charge_sessions + 1;
    }
    return m_stats;
}

void vehicle::wait() {
    m_current_state = vehicle::STATE::WAITING;
}

vehicle::STATE vehicle::update_state(int t) {
    float delta = (t - m_last_t);
    vehicle::STATE ret = m_current_state;
    float f_prob;
    float sampled_prob;
    switch(m_current_state){
        case vehicle::STATE::FLYING:
            // update flying stats
            m_current_charge_kwh = m_current_charge_kwh - m_charge_used_kwh_per_s * delta;
            m_stats.total_flight_distance_miles = m_stats.total_flight_distance_miles + m_cruise_speed_mps * delta;
            m_stats.total_flight_time_sec = m_stats.total_flight_time_sec + delta;
            m_stats.passenger_miles = m_stats.passenger_miles + m_seats * m_cruise_speed_mps * delta;

            // if battery empty, update state and stats
            if(m_current_charge_kwh <= 0) {
                ret = vehicle::STATE::BATTERY_EMPTY;
                m_current_state = vehicle::STATE::BATTERY_EMPTY;
                m_current_charge_kwh = 0;
                m_stats.num_flights = m_stats.num_flights + 1;
            }

            // fault occurrence using exponential distribution
            f_prob = fault_probability((t - m_last_fault_t) / 3600);
            sampled_prob = sample_probability();
            if(sampled_prob < f_prob) {
                ++m_stats.num_faults;
                m_last_fault_t = t;
            }
            break;
        case vehicle::STATE::WAITING:
            break;
        case vehicle::STATE::CHARGING:
            // update charging stats
            m_current_charge_kwh = m_current_charge_kwh + m_recharge_rate_kwh_per_s * delta;
            m_stats.total_charge_time_sec = m_stats.total_charge_time_sec + delta;

            // if battery full, update state and stats
            if(fabs(m_current_charge_kwh - m_battery_capacity) <= k_margin) {
                ret = vehicle::STATE::BATTERY_FULL;
                m_current_state = vehicle::STATE::BATTERY_FULL;
                m_current_charge_kwh = m_battery_capacity;
                m_stats.num_charge_sessions = m_stats.num_charge_sessions + 1;
            }

            // fault occurrence using exponential distribution
            f_prob = fault_probability((t - m_last_fault_t) / 3600);
            sampled_prob = sample_probability();
            if(sampled_prob < f_prob) {
                ++m_stats.num_faults;
                m_last_fault_t = t;
            }
            break;
        default:
            break;
    };
    m_last_t = t;
    return ret;
}

float vehicle::sample_probability() {
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define the range for the distribution (0 to 1 for probability)
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // Sample a random probability
    return dis(gen);
}

float vehicle::fault_probability(float time_since_last_fault_hr) {
    return (1 - std::exp( -1 * m_fault_probability_rate * time_since_last_fault_hr));
}

}