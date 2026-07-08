#pragma once

#include <memory>
#include <variant>

namespace view3d {

template<typename T>
class HostBuffer {
  public:
    HostBuffer() = default;

    explicit HostBuffer(size_t size)
        : data_(std::make_unique<T[]>(size)), size_(size)
    {
    }

    T* data() noexcept
    {
        return data_.get();
    }

    const T* data() const noexcept
    {
        return data_.get();
    }

    inline size_t size() const noexcept
    {
        return size_;
    }

    inline T& operator[](size_t index) noexcept
    {
        return data_[index];
    }

    inline const T& operator[](size_t index) const noexcept
    {
        return data_[index];
    }

    auto begin(this const auto& self) noexcept
    {
        return &self.data_[0];
    }

    auto end(this const auto& self) noexcept
    {
        return &self.data_[self.size_];
    }

  private:
    std::unique_ptr<T[]> data_;
    size_t size_;
};

unsigned int createRawDeviceBuffer(size_t raw_size);
void deleteRawDeviceBuffer(size_t raw_size) noexcept;

void copyToDeviceBuffer(unsigned int id, const void* data, size_t raw_size);
void copyFromDeviceBuffer(unsigned int id, void* data, size_t raw_size);
void copyFromDeviceToDeviceBuffer(unsigned int src_id, unsigned int dst_id,
                                  size_t raw_size);

template<typename T>
class DeviceBuffer {
  public:
    DeviceBuffer() = default;

    explicit DeviceBuffer(size_t size)
        : id(createRawDeviceBuffer(size * sizeof(T))), size_(size)
    {
    }

    ~DeviceBuffer()
    {
        deleteRawDeviceBuffer(id);
    }

    DeviceBuffer(const DeviceBuffer&) = delete;
    DeviceBuffer& operator=(const DeviceBuffer&) = delete;

    DeviceBuffer(DeviceBuffer&& other) noexcept
    {
        swap(other);
    }

    DeviceBuffer& operator=(DeviceBuffer&& other) noexcept
    {
        swap(other);
        return *this;
    }

    void swap(DeviceBuffer<T>& other) noexcept
    {
        std::swap(id, other.id);
        std::swap(size_, other.size_);
    }

    unsigned int getID() const noexcept
    {
        return id;
    }

    size_t size() const noexcept
    {
        return size_;
    }

    void upload(const T* data)
    {
        copyToDeviceBuffer(id, data, size_ * sizeof(T));
    }

    void upload(const HostBuffer<T>& host_buffer)
    {
        upload(host_buffer.data());
    }

    void download(T* data) const
    {
        copyFromDeviceBuffer(id, data, size_ * sizeof(T));
    }

    HostBuffer<T> download() const
    {
        HostBuffer<T> host_buffer(size_);
        download(host_buffer.data());
        return std::move(host_buffer);
    }

    void copyFrom(const DeviceBuffer<T>& other)
    {
        size_t min_size = std::min(size_, other.size_);
        copyFromDeviceToDeviceBuffer(other.id, id, min_size * sizeof(T));
    }

    void copyTo(DeviceBuffer<T>& other) const
    {
        size_t min_size = std::min(size_, other.size_);
        copyFromDeviceToDeviceBuffer(id, other.id, min_size * sizeof(T));
    }

  private:
    unsigned int id = 0;
    size_t size_ = 0;
};

template<typename T>
class Buffer {
  public:
    Buffer() : buffer(HostBuffer<T>())
    {
    }

    // NOLINTNEXTLINE(google-explicit-constructor)
    Buffer(HostBuffer<T>&& host_buffer) : buffer(std::move(host_buffer))
    {
    }

    // NOLINTNEXTLINE(google-explicit-constructor)
    Buffer(DeviceBuffer<T>&& device_buffer) : buffer(std::move(device_buffer))
    {
    }

    Buffer& operator=(HostBuffer<T>&& host_buffer)
    {
        buffer = std::move(host_buffer);
        return *this;
    }

    Buffer& operator=(DeviceBuffer<T>&& device_buffer)
    {
        buffer = std::move(device_buffer);
        return *this;
    }

    bool inHost() const noexcept
    {
        return std::holds_alternative<HostBuffer<T>>(buffer);
    }

    bool inDevice() const noexcept
    {
        return std::holds_alternative<DeviceBuffer<T>>(buffer);
    }

    size_t size() const noexcept
    {
        return std::visit([](const auto& buf) { return buf.size(); }, buffer);
    }

    decltype(auto) visit(this auto&& self, auto&& visitor)
    {
        return std::visit(visitor, self.buffer);
    }

    decltype(auto) asHost(this auto&& self)
    {
        return std::get<HostBuffer<T>>(self.buffer);
    }

    decltype(auto) asDevice(this auto&& self)
    {
        return std::get<DeviceBuffer<T>>(self.buffer);
    }

  private:
    std::variant<HostBuffer<T>, DeviceBuffer<T>> buffer;
};

} // namespace view3d
