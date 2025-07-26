# Protocol buffer

## 1. Overview

- Are a language-neutral, platform-neutral extensible mechanism for serializing structured data.

- Protobuf vs JSON, Smaller and faster because we just transfer bytes, meanwhile json transfer human-readable data. That saves network bandwidth. And also the CPU to handle JSON messages.

- Define an interface for all applications. Each applications generate that interface to corresponding code and use it. That means each application will have their own adapter to grpc to communicate with others.

```text
|App 1|<->|gRPC adapter| <------------------->|gRPC adapter|<->|App 2|
```

### 1.1. Who use Protobuf

- gRPC.
- Google Cloud.
- Envoy.

### 1.2. How do Protobuf work?

1. Create `.proto` file to define data structure.
2. Generate code using the `protoc` compiler.
3. Compile PB code with your project code.
4. Use PB classes to serialize, share, and deserialize data.

### 1.3. Revision

- proto2.
- proto3.
- edition 2023.

### 1.4. Keep in mind

- It can sometimes be difficult, or even impossible, to change field names after they've been used in production.
- Field names contain dashes.
- Use pluralized names for repeated fields.

### 1.5. Field number

- Each field in a message has a unique numbered tag. These tags are used to identify your fields  in the message binary format, and should not be changed once in use.

## 2. Syntax

## 3. Generating client and server code

This command will generate 2 files `.grpc.pb.h` and `.grpc.pb.cc` for service classes.

```bash
protoc --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` sensor.proto
```

This command will generate 2 files `.pb.h` and `.pb.cc` for message classes.

```bash
protoc --cpp_out=. sensor.proto
```

Message classes are used to present data structure to transfer, and service classes presents how to use services to send/recv those messages.

### 3.1. Message classes

```text
message ReadRequest {
  int32 sensor_type = 2;
}
```

The message `ReadRequest` will be generated into a class:

```cpp
class ReadRequest final : public ::google::protobuf::Message
{
    // Default methods ...
    ReadRequest* PROTOBUF_NONNULL New(::google::protobuf::Arena* PROTOBUF_NULLABLE arena = nullptr);
    void CopyFrom(const ReadRequest& from);
    void MergeFrom(const ReadRequest& from);
    void Swap(ReadRequest* PROTOBUF_NONNULL other);
    bool IsInitialized() const;
    ABSL_ATTRIBUTE_REINITIALIZES void Clear() PROTOBUF_FINAL;
    // And more ...

    // accessors -------------------------------------------------------
    enum : int {
       kSensorTypeFieldNumber = 2,
    };

    // int32 sensor_type = 2;
    bool has_sensor_type() const;
    void clear_sensor_type() ;
    ::int32_t sensor_type() const;
    void set_sensor_type(::int32_t value);
}
```

#### 3.1.1. Enum

```text
enum Level
{
    SENSOR_LEVEL_LOW = 0;
    SENSOR_LEVEL_MEDIUM = 2;
    SENSOR_LEVEL_HIGH = 4;
}
```

become enum with some helper functions:

```cpp
enum Level : int {
  SENSOR_LEVEL_LOW = 0,
  SENSOR_LEVEL_MEDIUM = 2,
  SENSOR_LEVEL_HIGH = 4,
  Level_INT_MIN_SENTINEL_DO_NOT_USE_ =
      ::std::numeric_limits<::int32_t>::min(),
  Level_INT_MAX_SENTINEL_DO_NOT_USE_ =
      ::std::numeric_limits<::int32_t>::max(),
};

// Along with some methods...

inline bool Level_IsValid(int value) {
  return 0 <= value && value <= 4 && ((21u >> value) & 1) != 0;
}

template <>
inline const ::std::string& Level_Name(Level value) {
  return ::google::protobuf::internal::NameOfDenseEnum<Level_descriptor, 0, 4>(
      static_cast<int>(value));
}
```

#### 3.1.2. Nested messages

For messages that use other messages as its field, for example:

```text
message HomeSensorData {
    int32 temperature = 1;
    int32 light = 2;
}

message ReadReply {
    HomeSensorData home_data = 1;
    Level level = 2;
}
```

The `home_data` become a member of `ReadReply` class, and can access through some methods:

```cpp
class HomeSensorData final : public ::google::protobuf::Message
{
    // accessors -------------------------------------------------------
    enum : int {
        kTemperatureFieldNumber = 1,
        kLightFieldNumber = 2,
    };

    // int32 temperature = 1;
    bool has_temperature() const;
    void clear_temperature() ;
    ::int32_t temperature() const;
    void set_temperature(::int32_t value);

    public:
    // int32 light = 2;
    bool has_light() const;
    void clear_light() ;
    ::int32_t light() const;
    void set_light(::int32_t value);
}

class ReadReply final : public ::google::protobuf::Message
{
  // accessors -------------------------------------------------------
  enum : int {
    kHomeDataFieldNumber = 1,
    kLevelFieldNumber = 2,
  };

  bool has_home_data() const;
  void clear_home_data() ;
  const ::HomeSensorData& home_data() const;
  [[nodiscard]] ::HomeSensorData* PROTOBUF_NULLABLE release_home_data();
  ::HomeSensorData* PROTOBUF_NONNULL mutable_home_data();
  void set_allocated_home_data(::HomeSensorData* PROTOBUF_NULLABLE value);
}
```

If you want to change value, use `mutable_home_data()` method to get the pointer, if you want to read-only, get constant reference `home_data()`. You also can free and allocate new one.

### 3.2. Service classes

One class is generated for each service, inside this class there are few sub-classes that are used for both client and server sides.

```cpp
class Sensor final {

    // Client side code.
    class StubInterface { ... };
    class Stub final : public StubInterface { ... };

    static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

    // Server side code.
    class Service : public ::grpc::Service { ... };

    class WithAsyncMethod_ReadSensor : public BaseClass {...};
    typedef WithAsyncMethod_ReadSensor<Service> AsyncService;

    class WithCallbackMethod_ReadSensor : public BaseClass {...};
    typedef WithCallbackMethod_ReadSensor<Service > CallbackService;
    typedef CallbackService ExperimentalCallbackService;
    // ...
}
```

#### 3.2.1. API Implementations

GRPC can be implemented using synchronous, asynchronous, or asynchronous with callback approaches.

- Synchronous: blocking APIs, simplicity, clients wait for server responses.
- Asynchronous:
  - Non-blocking.
  - binding RPC with a completion queue. Do read/write operations with a tag. And then using completion queue's Next method to wait for the next ready response.
  - Take one thread to handle multiple RPC response, caller thread can still do its tasks, CompletionQueue will handle Completion responses and able to notice to outside. Like event-driven architecture. One thread to handle completion responses for all RPCs.
- Callback:
  - Wrapper of Asynchronous.
  - No need to use CompletionQueue and make a thread for that (By default gRPC will launch a thread for that automatically - this is also called a reactor and you can wrap it too.)
  - Just pass the callback - function pointer to that, and it will be called directly, you can using some basic notify mechanism like future-promise, condition variable to notify to outside.

#### 3.2.2. Client side

#### 3.2.3. Server side

