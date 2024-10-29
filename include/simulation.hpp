#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <nlohmann-json/v3.11.3/json.hpp>
#include <vehicle.hpp>

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <set>
#include <map>
#include <queue>

#include <cstdlib>
#include <ctime>

using json = nlohmann::json;

namespace evtol_sim {

/// \class simulation
/// \brief class simulation manages vehicle objects and their interaction with sim environment.
class simulation {
    public:
        simulation() = default;

        /// \brief Initialize a simulation session.
        /// \param[in] duration_hrs duration in hours to run the simulation.
        /// \param[in] fleet_count number of vehicles to simulate.
        /// \param[in] num_chargers number of chargers available.
        /// \param[in] data json containing vehicle type attributes.
        /// \return bool if initialization was successful.
        bool initialize(float duration_hrs, int fleet_count, int num_chargers, json const & data);

        /// \brief Run the simulation.
        void run();

        /// \brief Print a summary of vehicle stats.
        void print_summary();

    private:
        /// \brief Initialize a simulation session.
        /// \param[in] vehicle_type json block corresponding to a vehicle type.
        /// \param[in] cmpnys set of company strings.
        /// \return bool if json has valid values.
        bool is_json_block_valid(json const & vehicle_type, std::set<std::string> const & cmpnys);
        
        /// \brief Adds a vehicle to a charger if available.
        /// \param[in] v_ptr shared_ptr to vehicle.
        /// \return bool if adding was a success.
        bool add_to_charger(std::shared_ptr<vehicle> v_ptr);

        /// \brief Remove a vehicle from the charger.
        /// \param[in] v_ptr shared_ptr to vehicle.
        void remove_from_charger(std::shared_ptr<vehicle> v_ptr);

        std::unordered_map<std::string, std::vector<std::shared_ptr<vehicle> > > m_vehicles;
        float m_sim_duration;
        int m_num_chargers;
        std::queue<std::shared_ptr<vehicle> > m_charge_queue;
        std::set<std::shared_ptr<vehicle> > m_chargers;
};


} // namespace evtol_sim

#endif // SIMULATION_HPP