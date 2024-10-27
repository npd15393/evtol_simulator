#ifndef VEHICLE_TYPE_HPP
#define VEHICLE_TYPE_HPP

namespace evol_sim {

class vehicle_type_c{
    public:
        enum class STATE: int {
            FLYING = 0,
            CHARGING,
            WAITING
        };

        vehicle_type_c(
            float cruise_speed,
            float battery,
            float charge_time,
            float crg_per_mile,
            int seats,
            float p_fault);

        void tick(int const t);

    private:
        float m_cruise_speed;
        float m_battery_capacity;
        float m_charge_time;
        float m_energy_per_mile;
        int m_seats;
        float m_fault_probability;
        int last_t;
};

} // namespace evol_sim

#endif // VEHICLE_TYPE_HPP