/*
  main.cpp
  5/1/19
  Andrue Peters

  This is the main file for the program.
  Better description later.
*/
#include <list>
#include <iostream>
#include <iterator>

#include <RF24.h>
#include <wiringPi.h>


// specific files projectc++c
#include <node.h>
#include <packet.h>

#include <unistd.h>
// global radio instance
// using wiringPi, so wiringPi pin scheme is also used
// this is equivalent to radio(22, 0) in the generic gettingstarted.cpp
RF24 radio(3, 10);

int main()
{
  wiringPiSetup();
  radio.begin();
  radio.openWritingPipe(ADDRESSES[2]);
  radio.openReadingPipe(1, ADDRESSES[0]);

  while (1) {
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
