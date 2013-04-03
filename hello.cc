#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <v8.h>
extern "C" {
  #include "bcm2835.h"
  #include "unistd.h"
}

using namespace v8;

char arr[] = {0x00, 0xFF};

Handle<Value> Test(const Arguments& args) {
    HandleScope scope;
    if (!bcm2835_init())
        return scope.Close(String::New("could not init"));

    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536); // The default
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default

    // Send a some bytes to the slave and simultaneously read
    // some bytes back from the slave
    // Most SPI devices expect one or 2 bytes of command, after which they will send back
    // some data. In such a case you will have the command bytes first in the buffer,
    // followed by as many 0 bytes as you expect returned data bytes. After the transfer, you
    // Can the read the reply bytes from the buffer.
    // If you tie MISO to MOSI, you should read back what was sent.

    char buf[] = { 0x01, 0x02, 0x11, 0x33 }; // Data to send
    bcm2835_spi_transfern(buf, sizeof(buf));
    // buf will now be filled with the data that was read from the slave
    printf("Read from SPI: %02X  %02X  %02X  %02X \n", buf[0], buf[1], buf[2], buf[3]);

    bcm2835_spi_end();
    bcm2835_close();
    return scope.Close(String::New("success"));
}

Handle<Value> CreateFunction(const Arguments& args) {
    HandleScope scope;

    Local<FunctionTemplate> tpl = FunctionTemplate::New(Test);
    Local<Function> fn = tpl->GetFunction();
    fn->SetName(String::NewSymbol("theFunction")); // omit this to make it anonymous

    return scope.Close(fn);
}

void Init(Handle<Object> target) {
    target->Set(String::NewSymbol("hello"),
        FunctionTemplate::New(CreateFunction)->GetFunction());
}

NODE_MODULE(hello, Init)
