#include <simulation.hpp>

namespace evtol_sim {

bool simulation::initialize(float duration_hrs, int fleet_count, int num_chargers, json const & data) {
    bool ret = true;
    std::set<std::string> cmpnys;
    int remaining_count = fleet_count;
    m_sim_duration = duration_hrs * 3600;
    m_num_chargers = num_chargers;
    // check for json validity and add at least one vehicle of each type
    for (const auto& row : data) {
        ret = ret && is_json_block_valid(row, cmpnys);
        if(ret) {
            std::string company_nm = row["company_name"];
            float cruise_spd = row["cruise_speed_mph"];
            float battery = row["battery_capacity_kwh"];
            float charge_time = row["time_to_charge_hrs"];
            float energy_used_cruising = row["energy_used_cruise_kwh_per_mile"];
            int seats = row["passenger_count"];
            float p_fault = row["fault_probability_per_hr"]; 

            cmpnys.emplace(company_nm);
            std::vector<std::shared_ptr<evtol_sim::vehicle>> temp_v;
            temp_v.emplace_back(std::make_shared<evtol_sim::vehicle>(company_nm, 0, cruise_spd, battery, charge_time, energy_used_cruising, seats, p_fault));
            m_vehicles[company_nm] = temp_v;
            --remaining_count;
        } else {
            break;
        }
    }

    if(ret) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        std::string type_nm;
        for(auto & type : m_vehicles) {
            type_nm = type.first;
            int random_num = rand() % remaining_count;

            for(int i = 0; i < random_num; i++) {
                // copy from first vehicle ptr object
                type.second.push_back(std::make_shared<evtol_sim::vehicle>(*type.second[0]));
                // assign a new id to the copied vehicle ptr obj
                type.second[type.second.size() - 1]->set_id(i+1);
            }
            remaining_count = remaining_count - random_num;
        }

        // for the last vehicle type, use all remaining counts
        for(int i = 0; i < remaining_count; i++) {
                m_vehicles[type_nm].push_back(std::make_shared<evtol_sim::vehicle>(*(m_vehicles[type_nm][0])));
        }
    }
    return ret;
}

void simulation::run() {
    // use LCM to come up with max time step
    int time_step = 1;
    int total_steps_count = m_sim_duration / time_step;
    for(int t_step = 0; t_step < total_steps_count; t_step++) {
        for(auto & vehicles_per_type : m_vehicles) {
            for(auto unit : vehicles_per_type.second) {
                auto state = unit->update_state(time_step * t_step);
                // manage battery full/empty events
                if(state == evtol_sim::vehicle::STATE::BATTERY_EMPTY) {
                    // if battery empty, try to charge
                    if(!add_to_charger(unit)) {
                        // if all chargers occupied, add to queue
                        unit->wait();
                        m_charge_queue.push(unit);
                    }
                } else if(state == evtol_sim::vehicle::STATE::BATTERY_FULL) {
                    // if charge full, undock from charger
                    remove_from_charger(unit);
                    if(!m_charge_queue.empty()) {
                        // charge next vehicle in queue 
                        add_to_charger(m_charge_queue.front());
                        m_charge_queue.pop();
                    }
                } else {}
            }
        }
    }    
}

bool simulation::is_json_block_valid(json const & vehicle_type, std::set<std::string> const & cmpnys) {
    bool ret_val;
    bool is_unique_type = (0 == cmpnys.count(vehicle_type["company_name"]));
    if(!is_unique_type) {
        std::cerr << "Error in JSON block:" << vehicle_type["company_name"] <<". Duplicate entry." << std::endl;
    }
    bool values_positive =
        (0 < vehicle_type["cruise_speed_mph"]) && (0 < vehicle_type["battery_capacity_kwh"]) 
        && (0 < vehicle_type["time_to_charge_hrs"]) && (0 < vehicle_type["energy_used_cruise_kwh_per_mile"])
        && (0 < vehicle_type["passenger_count"]) && (0 < vehicle_type["fault_probability_per_hr"]);
    if(!values_positive) {
        std::cerr << "Error in JSON block:" << vehicle_type["company_name"] <<". Value not positive." << std::endl;
    }
    bool is_int = vehicle_type["passenger_count"].is_number_integer();
    if(!is_int) {
        std::cerr << "Error in JSON block:" << vehicle_type["company_name"] <<". Count is not an Integer." << std::endl;
    }
    bool valid_probability = (1 >= vehicle_type["fault_probability_per_hr"]);
    if(!valid_probability) {
        std::cerr << "Error in JSON block:" << vehicle_type["company_name"] <<". Probability invalid." << std::endl;
    }
    ret_val = is_unique_type && values_positive && is_int && valid_probability;
    return ret_val;
}

bool simulation::add_to_charger(std::shared_ptr<vehicle> v_ptr) {
    bool ret = false;
    if(m_chargers.size() < m_num_chargers) {
        m_chargers.insert(v_ptr);
        v_ptr->dock(true);
        ret = true;
    }
    return ret;
}

void simulation::remove_from_charger(std::shared_ptr<vehicle> v_ptr) {
    m_chargers.erase(v_ptr);
    v_ptr->dock(false);
}

void simulation::print_summary() {
    for(auto & vehicles_per_type : m_vehicles) {
        evtol_sim::vehicle_stats summary_per_vehicle;
        // add stats of each vehicle of same type
        for(auto vehicle_ptr : vehicles_per_type.second) {
            // get summarized stats for each vehicle (different from m_stats if vehicle was in flying/charging state when sim session ended)
            summary_per_vehicle = summary_per_vehicle + vehicle_ptr->get_summarized_stats();
        }

        std::cout << vehicles_per_type.first <<std::endl;
        std::cout << summary_per_vehicle << std::endl;
        std::cout << "---------------------" << std::endl;
    }
}

}