#include <pthread.h>
#include "caltrain.h"


/* the station object is initiated by this function */

void station_init(struct station *station){

    station->number_of_empty_seats = 0;
    station -> number_of_waiting_passengers = 0;
    station -> number_of_passengers_walking_onboard = 0;
    pthread_mutex_init(&station->mutex, NULL);
    pthread_cond_init(&station->passenger_seats_available, NULL);
    pthread_cond_init(&station->train_can_leave, NULL);
}

/* when a train arrives, this function is called */

void station_load_train(struct station *station, int count) // count is the number of empty seats
{
    // mutex start
    pthread_mutex_lock(&(station->mutex));
    station -> number_of_empty_seats = count;
    while(station -> number_of_empty_seats > 0 && station -> number_of_waiting_passengers > 0)
    {
        pthread_cond_broadcast(&(station -> passenger_seats_available));
        pthread_cond_wait(&(station -> train_can_leave), &(station -> mutex));
    }
    station->number_of_empty_seats =0;
    pthread_mutex_unlock(&station->mutex);

    // mutex end
}

/* passenger entered the station */
void station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&station->mutex); // true mutex 1
    station -> number_of_waiting_passengers ++;
    /* wait until at least one empty seat exists and number of walking onboard is less than the number of available seats*/
    // pthread_mutex_lock(&(station->mutex)); // bagarab : matgarabsh tany
    while (station->number_of_empty_seats == 0){
        pthread_cond_wait(&station->passenger_seats_available , &station->mutex);
    }
    // pthread_mutex_lock(&station->mutex); // true mutex 2 faster
    station -> number_of_waiting_passengers --;
    station -> number_of_empty_seats --;
    station -> number_of_passengers_walking_onboard ++;
    pthread_mutex_unlock(&(station->mutex)); // true 1 or 2

}
/* someone has taken a seat */
/* no synchronization for setting on seats but mutex is used to signal the train to leave*/
void station_on_board(struct station *station)
{
    // wait for broadcast of signal
    // pthread_mutex_lock(&(station -> mutex)); // true working mutex1
    station -> number_of_passengers_walking_onboard--;
    if ((station -> number_of_empty_seats == 0 || station -> number_of_waiting_passengers == 0) && (station -> number_of_passengers_walking_onboard == 0)) // you may need to make it just check for the walking people
    {
        pthread_mutex_lock(&(station -> mutex)); // true working mutex2- faster
        pthread_cond_signal(&station -> train_can_leave);
        pthread_mutex_unlock(&(station -> mutex)); // true working mutex2- faster
        //signal
    }
    // pthread_mutex_unlock(&station->mutex);  // true working mutex1
    /* if the train is full, let the train move */
}
