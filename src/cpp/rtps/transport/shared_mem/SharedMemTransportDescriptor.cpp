// Copyright 2020 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cstdint>

#include <fastdds/rtps/transport/shared_mem/SharedMemTransportDescriptor.h>

#include <fastdds/rtps/transport/PortBasedTransportDescriptor.hpp>
#include <fastdds/rtps/transport/TransportInterface.h>

using namespace eprosima::fastdds::rtps;

namespace eprosima {
namespace fastdds {
namespace rtps {

static constexpr uint32_t shm_default_segment_size = 0;
static constexpr uint32_t shm_default_port_queue_capacity = 512;
static constexpr uint32_t shm_default_healthy_check_timeout_ms = 1000;

} // rtps
} // fastdds
} // eprosima

//*********************************************************
// SharedMemTransportDescriptor
//*********************************************************
SharedMemTransportDescriptor::SharedMemTransportDescriptor()
    : PortBasedTransportDescriptor(shm_default_segment_size, s_maximumInitialPeersRange)
    , segment_size_(shm_default_segment_size)
    , port_queue_capacity_(shm_default_port_queue_capacity)
    , healthy_check_timeout_ms_(shm_default_healthy_check_timeout_ms)
    , rtps_dump_file_("")
{
    maxMessageSize = s_maximumMessageSize;
    std::cout << "TEBD: starting object create init inside shared mem send\n";
    m_poObjectCreate = new void*;
    object_create_init(m_poObjectCreate);
    std::cout << "TEBD: finished object create init inside shared mem send\n";
    const char** args;
    args = new const char*[2];
    args[0] = "/testsharedmeminit";
    args[1] = "This is my test file message!\nIt was made after initializing\n";
    object_create(m_poObjectCreate, 2, args);
}

bool SharedMemTransportDescriptor::operator ==(
        const SharedMemTransportDescriptor& t) const
{
    return (this->segment_size_ == t.segment_size() &&
           this->port_queue_capacity_ == t.port_queue_capacity() &&
           this->healthy_check_timeout_ms_ == t.healthy_check_timeout_ms() &&
           this->rtps_dump_file_ == t.rtps_dump_file() &&
           this->dump_thread_ == t.dump_thread() &&
           PortBasedTransportDescriptor::operator ==(t));
}

//! Set the size of the shared memory segment
RTPS_DllAPI void SharedMemTransportDescriptor::segment_size(uint32_t segment_size)
    {
    std::cout << "TEBD: starting test object create call inside shared mem send\n";
    const char** args;
    args = new const char*[2];
    args[0] = "/testsharedmem";
    args[1] = "This is my test file message!\nIt should be from the shared memory\n";
    object_create(m_poObjectCreate, 2, args);
    std::cout << "TEBD: finished test object create call inside shared mem send\n";

        segment_size_ = segment_size;
    }

#ifdef FASTDDS_SHM_TRANSPORT_DISABLED
TransportInterface* SharedMemTransportDescriptor::create_transport() const
{
    return nullptr;
}

#endif // ifdef FASTDDS_SHM_TRANSPORT_DISABLED
