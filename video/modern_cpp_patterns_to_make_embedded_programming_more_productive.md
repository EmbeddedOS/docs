# Modern CPP Patterns to make embedded programming more productive

## 1. Declarative GPIO

### 1.1. Old way

```c
config.Mode = GPIO_MODE_PUSH_PULL;
config.Pull = GPIO_PULLUPDOWN_NONE;

GPIO_Configure(GPIOA, &config);
GPIO_writePin(GPIOA, 1, GPIO_LOGIC_HIGH);

config.PinNumber = 4;
config.AlternateFunc = GPIO_AF_2;
```

### 1.2. Better way: Strong types in an iterable container

```cpp
using GPIO_DEF = \
struct IODefStruct {
    GPIO_TypeDef* GPIO;
    uint32_t PinNumber;
    enum IO_FUNCTION:uint32_t {INPUT=0, OUTPUT=1, ALT=2, ANALOG=3} Function;
    uint32_t AltFunc;
    enum IO_TYPE:uint32_t {NORMAL=0, OPENDRAIN=1} Type;
    enum IO_SPEED:uint32_t {LOW=0, MEDIUM=1, HIGH=2, VERYHIGH=3} Speed;
    enum IO_PULL:uint32_t {NONE=0, PULLUP=1, PULLDOWN=2} Bias;
    enum IO_STATE:uint32_t {LOGIC_LOW, LOGIC_HIGH, DONT_CARE} InitialState;
};
```

And make an array to select GPIO configurations:

```cpp
static const std::array<GPIO_DEF, 2> gpio_defs = {{
    // Green LED.
    {GPIOB, 0,
    GPIO_DEF::IO_FUNCTION::OUTPUT, 1,
    GPIO_DEF::IO_TYPE::NORMAL,
    GPIO_DEF::IO_SPEED::LOW,
    GPIO_DEF::IO_PULL::NONE,
    GPIO_DEF::IO_STATE::LOGIC_LOW},
    // Red LED.
    {GPIOB, 2,
    GPIO_DEF::IO_FUNCTION::OUTPUT, 1,
    GPIO_DEF::IO_TYPE::NORMAL,
    GPIO_DEF::IO_SPEED::LOW,
    GPIO_DEF::IO_PULL::NONE,
    GPIO_DEF::IO_STATE::LOGIC_LOW},
}}
```

### 1.3. Declarative GPIO

- Weak enums are used because they convert implicitly to their underlying type.
  - But they are constrained to tight scope minimize errors.

- Syntax is expressive - and easy to maintain.
- Easy to consolidate an entire project's IO configuration in one place.

### 1.4. Configuring

Configuring: write one hardware-specific function.

```cpp
bool Configure(const GPIO_DEF& def)
{
    // ... Chip-specific register settings ...
}
```

Configuring: the iterators way

```cpp
template <typename Iter>
bool Configure(Iter begin, Iter end)
{
    while( begin != end ) Configure(begin++);
}
```

Configure: the ranges way

```cpp
template<typename T> requires std::ranges::range<T>
bool Configure(const T& definitions)
{
    for (auto&def : definitions) Configure(def);
}
```

### 1.5. Passing GPIO references around

Let get an example, `GPIO` is passing around to SPI control struct.

```cpp
struct SPIBus
{
    SPIBus(SPI_TypeDef* peripheral) {}
};

struct SPIProtocol
{
    enum class SPIMode {Mode1, Mode2, Mode3, Mode4};
    SPIProtocol(SPIMode mode, uint32_t speed) {}
};

struct SPIConnection
{
    SPIConnection(const SPIBus& bus,
                  const SPIProtocol& conn,
                  const GPIO_DEF& chip_select) {}
};
```

### 1.6. Asserting

```cpp
class AssertType {};
class AssertTypeLogicHigh: public AssertType { static constexpr bool ValueWhenAsserted = true; };

class AssertTypeLogicLow: public AssertType { static constexpr bool ValueWhenAsserted = false; };

template <typename Assert> requires std::devired_from<Assert, AssertType>
class GPIOAssertFunctor
{
    const GPIO_DEF& mGPIO;
    public:
    constexpr GPIOAssertFunctor(const GPIO_DEF& io): mGPIO(io) {}
    void operator()(bool enable)
    {
        if (enable == Assert::ValueWhenAsserted)
        {
            mGPIO.GPIO->ODR |= (0b1 << mGPIO.PinNumber);
        } else
        {
            mGPIO.GPIO->ODR &= ~(0b1 << mGPIO.PinNumber);
        }
    }
}
```

The SPI Connection, we can implement:

```cpp
template<typename Assert>
struct SPIConnection
{
    const GPIOAssertFunctor<Assert>& mEnableFunction;
    SPIConnection(const SPIBus& bus,
                  const SPIProtocol& conn,
                  const GPIOAssertFunctor<Assert>& enable)
                : mEnableFunction(enable) {}

    bool ReadWrite(std::span<char> outdata,
                   std::span<char> indata,
                   size_t length)
    {
        mEnableFunction(true);
        // Write to the bus ...
        mEnableFunction(false);
    }
};
```

### 1.7. Example use case

```cpp
static const std::array<GPIO_DEF, 12> IOPins = { /* */ };
bool success = Configure(IOPins);
auto bus = SPIBus(SPI1);

auto protocol = SPIProtocol(SPIProtocol::SPIMode::Mode1, 1'000'000);
auto chip_select = GPIOAssertFunctor<AssertTypeLogicLow>(IOPins[4]);
auto connection = SPIConnection(bus, protocol, chip_select);
```

### 1.8. Summarize

- Centralize your IO definitions.
- Abstract IO functions - with little or no cost.

## 2. Create Lookup tables using compile-time expressions

### 2.1. Compiler-driven lookup table generation: the old way

```cpp
const float TemperatureFromThermistor[] = { 25.0, 26.2, 27.5, ... };
//                                  ADC = 0, 1, 2, ..., 255.
```

- Generate via spreadsheet and copy and paste.
- Generate via script and copy and paste.
- Generate via script and incorporate into build system.

### 2.2. Compiler-driven lookup table generation: some considerations

- Constants in embedded system must be carefully crafted to end up (usually) in the `.rodata` section.
  - This results in constants being placed in flash - and not copied to RAM.

- It is common for hardware and firmware to have to evolve together.
  - Example: changing resistor values to tune the performance of the system.

### 2.3. Compiler-driven lookup table generation

Incorporate governing equations into the code, and let the compiler generate constant tables:

- Analog Conversion: Vx = code * (Vn/2^n)
- Resistor divider: R = Vx * R1 / (V0 - Vx)

Implement in CPP using `constexpr`:

```cpp
constexpr float VoltageFromCode(const float Vref, const size_t n, const unit16_t code)
{
    float V = Vref*code/std::pow(2, n);

    return V;
}

constexpr float ResistanceFromDivider(const float V0, const float V, const float R0)
{
    float R = R0*V0*(V0-V);
    return R;
}
```

### 2.4. Sum up

- Lookup tables can be very fast or space-saving.
  - Sometimes critical for embedded, real-time applications.
- We can place the design support in the source code.
  - No external processes to cause mistakes.
  - No complexities in the build process.
- Tables reside in non-volatile memory rather than precious RAM.

## 3. Code with number structures humans can actually read and edit

### 3.1. Address-like structures

- Many embedded applications use long numeric structures.
  - Frequently known at compile time for storage in nonvolatile memory.
- For example:
  - bluetooth:
    - 128-bit UUIDs.
    - 48-bit MAC address.
  - Internet Protocol:
    - 32-bit IPv4 addresses.
    - 128-bit IPv6 addresses.
  - USB:
    - Unicode encoded string (descriptors).

### 3.2. The old way

```cpp
// Canonical UUID.
// {01234567-89AB-CDEF-01233-456789ABCDEF}
static constexpr uint8_t uuid1[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, /*... */};
```

### 3.3. Address-like structures in modern way

```cpp
class UUID
{
private:
    std::array<uint8_t, 16> mData {0};
public:
    UUID() = default;
    friend constexpr UUID operator"" _uuid(const char *text, size_t length);
};
```

```cpp
constexpr UUID operator ""_uuid(const char *text, size_t length)
{
    UUID result;
    size_t index = 0;
    size_t count = 0;
    while (index < length)
    {
       // Build UUID from text ...
    }

    if (count != 32) throw std::invalid_argument("Invalid UUID string");

    return result;
}
```

And using string conversion:

```cpp
static const UUID uuid = "{ 01234567-89AB-CDEF-01233-456789ABCDEF }"_uuid;
static const MACAddr mac = "12:34:56:78:80:AB"_mac_addr;
// ...
```

## 4. Use Stream-based IO, but skip the library

### 4.1. Lean Stream-based IO

```cpp
int main(int, char**)
{
    std::cout << "Hello, world" << "\r\n";
}
```

### 4.2. Create a Lightweight Filestream Object

```cpp
namespace mcu
{
    class FileStream
    {
    public:
        using Radix_t = enum class RadixEnum {Binary=2, Octal=8, Decimal=10, Hex=16};
    protected:
        size_t mFileDescriptor;
        Radix_t mRadixSetting;
    public:
        FileStream(size_t fd): mFileDescriptor(fd), mRadixSetting(RadixEnum::Hex) {}

    public:
        FileStream& operator <<(const char *string)
        {
            const size_t length = std::strlen(string);
            _write(mFileDescriptor, string, length);
            return *this;
        }
    }

    template<typename U> requires std::integral<U>
    FileStream& operator <<(const U value) { /* ... */ }

    template<typename U> requires std::floating_point<U>
    FileStream& operator <<(const U value) { /* ... */ }
}
```

### 4.3. Instantiate Filestream for each output

```cpp
namespace mcu
{
    FileStream debug(1);
    FileStream swo(2);
}
```

### 4.4. Some plumbing for output modes

```cpp
extern "C" int _write(int fd, const void*data, size_t length)
{
    switch (fd)
    {
        case 1:
            SWOWrite((const char*)data, length);
            break;
        case 2:
            UARTWrite((const char*)data, length);
            break;
        case 3:
            UDPWrite(connection, (const char*)data, length);
        /* ... */
    }
}
```

And use them:

```cpp
int main()
{
    mcu::swo << "Hello, Serial Wire Debug!\r\n";
    mcu::debug << "Hello, UART Debug!\r\n";
}
```

## Dare to use the heap
