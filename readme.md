# HttpClientMod for Particle devices

This is a fork of [nmattisson](https://github.com/nmattisson/HttpClient)'s Http Client for Particle Photon devices (and possibly others but I haven't tested on them).  This fork allows me to change the way the HttpClient's internal buffer is handled, which allows the end user to change the buffer size, use a dynamic buffer or even process the character stream as it's received without modifying the library code.

### Example usage

```cpp
#include "application.h"

/**
* Declaring the variables.
*/
unsigned int nextTime = 0;    // Next time to contact the server

StaticBuffer<1024> buffer;
HttpClientMod http(&buffer);

// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
    //  { "Content-Type", "application/json" },
    //  { "Accept" , "application/json" },
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

http_request_t request;
http_response_t response;

void setup() {
    Serial.begin(9600);
}

void loop() {
    if (nextTime > millis()) {
        return;
    }

    Serial.println();
    Serial.println("Application>\tStart of Loop.");
    // Request path and body can be set at runtime or at setup.
    request.hostname = "www.timeapi.org";
    request.port = 80;
    request.path = "/utc/now";

    // The library also supports sending a body with your request:
    //request.body = "{\"key\":\"value\"}";

    // Get request
    http.get(request, response, headers);
    Serial.print("Application>\tResponse status: ");
    Serial.println(response.status);

    Serial.print("Application>\tHTTP Response Body: ");
    Serial.println(response.body);

    nextTime = millis() + 10000;
}


```

### Using a Static Buffer

A static buffer is the safest way of defining a buffer for the HttpClientMod, theres no chance of you running out of memory (once the buffer object is created) and if you create it as in the example above, you shouldn't have any memory problems (at least with HttpClientMod code) if the application compiles.  The downside is that once that buffer is full, any additional data sent from the Http server will be discarded.

The figures in angle brackets specify the size of the buffer to create in characters, in the case of the example above we create a buffer of 1024 characters.

### Using a Dynamic Buffer

A dynamic buffer resizes (within set limits) to accommidate variable size data and is prefect for situations where you might usually only get a small amount of data back but might occassionally get something larger, but it has the risk that you might run out of memory while using it.

Note that a dynamic buffer may use up to twice it's current size when being resized.

The constructor parameters are (in order):
* Initial Buffer Size
* Increment Size
* Maximum Buffer Size

The buffer starts by allocating (using malloc) the amount of memory specified by the Initial Buffer Size, once that is full the size buffer is increased (using realloc) by the specified Increment Size.  This repeats
until the buffer size would exceed Maximum Buffer Size, whereupon the buffer is null terminated and further data is discarded.

```cpp

DynamicBuffer dynamicBuffer(1024, 256, 2048);
HttpClientMod http(&dynamicBuffer);

```

### Processing the character stream as it's received.

You could also processes the stream as it arrives by using something like the following.

Be aware that if you do not properly honor clear() and getBuffer(), the values returned from HttpClient in the response object will not be valid.

```cpp

class StreamProcessor : public Buffer {
    public:
        bool append(const char c) {
            // Process the character here...
            return true;
        };
        
        void clear() {};
        bool isFull() { return false; };
		const char* const getBuffer() const { return NULL; };
}

StreamProcessor streamProcessor;
HttpClientMod http(&streamProcessor);

```
