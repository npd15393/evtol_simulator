#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <random>
#include <iostream>
#include <string>

namespace evtol_sim {

/// \struct vehicle_stats
/// \brief contains all the required statistics to keep track of.
struct vehicle_stats {
    vehicle_stats():
        num_flights(0), num_charge_sessions(0), num_faults(0),
        total_flight_distance_miles(0.0), total_flight_time_sec(0.0), total_charge_time_sec(0.0),
        passenger_miles(0.0)
    {}

    int num_flights;
    int num_charge_sessions;
    int num_faults;
    float total_flight_distance_miles;
    float total_flight_time_sec;
    float total_charge_time_sec;
    float passenger_miles;

    friend std::ostream & operator<< (std::ostream& os, const vehicle_stats & vstats) {
        float avg_time = vstats.num_flights > 0 ? (vstats.total_flight_time_sec / (vstats.num_flights * 3600)) : 0.0;
        float avg_dist = vstats.num_flights > 0 ? (vstats.total_flight_distance_miles / vstats.num_flights) : 0.0;
        float avg_crg_t = vstats.num_charge_sessions > 0 ? (vstats.total_charge_time_sec / (vstats.num_charge_sessions * 3600)) : 0.0;
        os << "Average Time per flight (hrs): " << avg_time << std::endl;
        os << "Average Distance traveled per flight (miles): " << avg_dist  << std::endl;
        os << "Average Time charging per charge session (hrs): " << avg_crg_t << std::endl;
        os << "Total faults: " << vstats.num_faults << std::endl;
        os << "Passenger miles: " << vstats.passenger_miles << std::endl;
        return os;
    }

    friend vehicle_stats operator+(vehicle_stats const & v1, vehicle_stats const & v2) {
        vehicle_stats ans;
        // add counts
        ans.num_flights = v1.num_flights + v2.num_flights;
        ans.num_charge_sessions = v1.num_charge_sessions + v2.num_charge_sessions;
        ans.num_faults = v1.num_faults + v2.num_faults;

        // add timings
        ans.total_flight_distance_miles = v1.total_flight_distance_miles + v2.total_flight_distance_miles;
        ans.total_flight_time_sec = v1.total_flight_time_sec + v2.total_flight_time_sec;
        ans.total_charge_time_sec = v1.total_charge_time_sec + v2.total_charge_time_sec;

        // add misc
        ans.passenger_miles = v1.passenger_miles + v2.passenger_miles;
        return ans;
    }
};

/// \class vehicle
/// \brief class vehicle manages vehicle internal state and statistics..
class vehicle {
    public:
        /// \brief defines all possible states of a vehicle
        enum class STATE: int {
            FLYING = 0,
            CHARGING,
            WAITING,
            BATTERY_EMPTY,
            BATTERY_FULL
        };

        /// \brief Constructs vehicle object.
        /// \param[in] company_nm duration in hours to run the simulation.
        /// \param[in] vehicle_id number of vehicles to simulate.
        /// \param[in] cruise_speed number of chargers available.
        /// \param[in] battery json containing vehicle type attributes.
        /// \param[in] charge_time duration in hours to run the simulation.
        /// \param[in] crg_per_mile number of vehicles to simulate.
        /// \param[in] seats number of chargers available.
        /// \param[in] p_fault json containing vehicle type attributes.
        vehicle(
            std::string company_nm,
            int vehicle_id,
            float cruise_speed,
            float battery,
            float charge_time,
            float crg_per_mile,
            int seats,
            float p_fault);

        /// \brief Set internal vehicle id.
        /// \param[in] id value to set.
        void set_id(int const id);

        /// \brief Get internal vehicle id.
        /// \return vehicle id.
        int get_id() const;

        /// \brief Get internal vehicle state.
        /// \return vehicle state.
        STATE get_state() const;

        /// \brief Change vehicle state when docker/undocked.
        /// \param[in] charging true when plugged-in, false when plugged out.
        void dock(bool charging);

        /// \brief Change vehicle state when waiting in charge queue.
        void wait();

        /// \brief Update vehicle state.
        /// \param[in] t current time point.
        STATE update_state(int const t);

        /// \brief Get vehicle stats.
        /// \return vehicle stats.
        evtol_sim::vehicle_stats get_summarized_stats();

    private:
        /// \brief Samples a float from a uniform distribution.
        /// \return sampled probability.
        float sample_probability();

        /// \brief Get fault probability at a certain time given exponential distribution.
        /// \param[in] time_since_last_fault_hr time duration since last fault.
        /// \return fault probability.
        float fault_probability(float time_since_last_fault_hr);

        std::string m_company;
        float m_cruise_speed_mps;
        float m_battery_capacity;
        float m_charge_time;
        float m_energy_per_mile;
        float m_fault_probability_rate;
        float m_recharge_rate_kwh_per_s;
        float m_charge_used_kwh_per_s;
        float m_current_charge_kwh;
        int m_seats;
        int m_vehicle_id;
        int m_last_t;
        int m_last_fault_t;
        vehicle_stats m_stats;
        STATE m_current_state;
        float k_margin = 0.0001;
};

} // namespace evtol_sim

#endif // VEHICLE_HPP