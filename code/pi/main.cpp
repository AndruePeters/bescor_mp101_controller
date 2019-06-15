/*
  main.cpp
  5/1/19
  Andrue Peters

  This is the main file for the program.
  Better description later.
*/
#include <iostream>
#include <iterator>

#include <RF24/RF24.h>
#include <wiringPi.h>


// specific files projectc++c
#include <node.h>
#include <packet.h>

#include <unistd.h> // for sleep

#include <main.h>
// global radio instance
// using wiringPi, so wiringPi pin scheme is also used
// this is equivalent to radio(22, 0) in the generic gettingstarted.cpp
RF24 radio(3, 10);
node_list_t node_list;

int main()
{
  wiringPiSetup();
  rf24_init();



  while (0) {
    // First, stop listening so we can talk.
    radio.stopListening();

    // Take the time, and send it.  This will block until complete

    printf("Now sending...\n");
    unsigned long time = millis();

    bool ok = radio.write( &time, sizeof(unsigned long) );

    if (!ok){
      printf("failed.\n");
    }
    // Now, continue listening
    radio.startListening();

    // Wait here until we get a response, or timeout (250ms)
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout ) {
      if (millis() - started_waiting_at > 200 )
        timeout = true;
    }


    // Describe the results
    if ( timeout )
    {
      printf("Failed, response timed out.\n");
    }
    else
    {
      // Grab the response, compare, and send to debugging spew
      unsigned long got_time;
      radio.read( &got_time, sizeof(unsigned long) );

      // Spew it
      printf("Got response %lu, round-trip delay: %lu\n",got_time,millis()-got_time);
    }
    sleep(1);
  }
  std::cout << "Ran successfully.\n";
  return 0;
}


void rf24_init()
{
  radio.begin();
  radio.openWritingPipe(ADDRESSES[2]);
  radio.openReadingPipe(1, ADDRESSES[0]);
}

/*
 * Configures radio to match node n's properties.
 */
void match_node_radio(const nrf2401_prop &n)
{
  radio.setPALevel(n.power_level);
  radio.setDataRate(n.data_rate);
  radio.setCRCLength(n.crc_length);
  radio.setChannel(n.channel);
  set_rf24_write_addr(n.listening_addr);
}


/*
 *
 */
void send_packet(const nrf2401_prop &n, const packet &p)
{
  match_node_radio(n);
  radio.write(&p, sizeof(p));
}


/*
 *  Sets the raspberry pi write address to the arduino node listening address
 */
 void set_rf24_write_addr(const address_e listening_addr)
 {
   radio.openWritingPipe(ADDRESSES[listening_addr]);
   radio.openReadingPipe(1, ADDRESSES[0]);
 }
