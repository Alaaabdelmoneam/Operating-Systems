#include <pthread.h>

struct station {
    int number_of_empty_seats;
    int number_of_waiting_passengers;
    int number_of_passengers_walking_onboard;
    pthread_mutex_t mutex;
    /** use condition variables*/
    pthread_cond_t passenger_seats_available;
    pthread_cond_t train_can_leave;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
