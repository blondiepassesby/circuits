
// DAC8412FPZ

#define ADDR_BIT0   7   // Control pin got LSB of address bus
#define ADDR_BIT1   6   // Control pin got MSB of address bus
#define DATA_BIT0   30  // Control pin for LSB of data bus

#define RW      5       // Pin for read-write command : High -> read, Low -> write
#define CSB     4       // Pin for chip select : High -> Off, Low -> On. Chip ignores commands unless this is low
#define RSTB    3       // Pin to Reset all channels to midscale :  High -> do nothing, Low -> do reset
#define LDACB   2       // Ping to control "Load DAC"

#define analogPin  A0
static uint32_t count = 0;

// Prep the address bus to talk to one DAC
static inline void setAddrBus(
    int dacId
)
{
    digitalWrite(ADDR_BIT0, (dacId  & 0x1) ? HIGH : LOW);
    digitalWrite(ADDR_BIT1, (dacId >> 0x1) ? HIGH : LOW);
}

// Prep the data bus with a given value
static inline void setDataBus(
    int16_t value
)
{
    for(int i=0; i<12; ++i) {
        digitalWrite(
            i + DATA_BIT0,
            (value & (1<<i)) ? HIGH : LOW
        );
    }
}

// Manipulate the "chip select" signal
static inline void ioState(
    bool on
)
{
    digitalWrite(CSB, on ? LOW : HIGH);
}

// Load a value on all 4 DACs
static inline void loadValue(
    int16_t value
)
{
    for(int i=0; i<4; ++i) {
        ioState(false);        delay(10);
            setAddrBus(i);     delay(10);
            setDataBus(value); delay(10);
        ioState(true);         delay(10);
        ioState(false);        delay(10);
    }
}

// Read a value from A0, output on serial
static inline void output()
{
    uint16_t value = analogRead(analogPin);
    Serial.print("count:");
    Serial.print(count);
    Serial.print(" millis:");
    Serial.print(millis());
    Serial.print(" value:");
    Serial.println(value);
}

void setup()
{
    // Prep control pins
    pinMode(       RW, OUTPUT);
    pinMode(      CSB, OUTPUT);
    pinMode(     RSTB, OUTPUT);
    pinMode(    LDACB, OUTPUT);
    pinMode(ADDR_BIT0, OUTPUT);
    pinMode(ADDR_BIT1, OUTPUT);
    for(int i=0; i<12; ++i) {
        pinMode(i + DATA_BIT0, OUTPUT);
    }

    // Reset DAC
    digitalWrite(RSTB,  LOW); delay(10);  // Turn reset on
    digitalWrite(RSTB, HIGH); delay(10);  // Turn reset off

    // Prep serial for output
    Serial.begin(230400);
}

void loop()
{
    loadValue(count*10);
    output();
    ++count;
}

