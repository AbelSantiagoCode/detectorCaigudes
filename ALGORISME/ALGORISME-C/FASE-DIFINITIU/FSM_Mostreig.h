#ifndef FSM_MOSTREIG
#define FSM_MOSTREIG


void init_mostreig();
bool first_threshold(const int x, const int y, const int z);

void init_timer_out(const int len_finestra);


void check_zone_threshold();

void classificar_rangX(const int x);


void classificar_rangY(const int y);


void classificar_rangZ(const int z);


void reset_buffer_2();

void run_FSM();


#endif
